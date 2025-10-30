// ©2013-2020 Cameron Desrochers.
// Distributed under the simplified BSD license (see the license file that
// should have come with this header).

#pragma once

#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>

#include "atomicops.h"

#ifndef MOODYCAMEL_CACHE_LINE_SIZE
#define MOODYCAMEL_CACHE_LINE_SIZE 64
#endif

// NOLINTBEGIN(readability-identifier-naming)
template <typename T, size_t MAX_BLOCK_SIZE = 512>
class SPSCQueue {
public:
    explicit SPSCQueue(size_t size = 15)
    {
        assert(MAX_BLOCK_SIZE == ceilToPow2(MAX_BLOCK_SIZE) && "MAX_BLOCK_SIZE must be a power of 2");
        assert(MAX_BLOCK_SIZE >= 2 && "MAX_BLOCK_SIZE must be at least 2");

        Block *firstBlock = nullptr;

        largestBlockSize = ceilToPow2(size + 1);  // We need a spare slot to fit size elements in the block
        if (largestBlockSize > MAX_BLOCK_SIZE * 2) {
            // We need a spare block in case the producer is writing to a different block the consumer is reading from,
            // and wants to enqueue the maximum number of elements. We also need a spare element in each block to avoid
            // the ambiguity between front == tail meaning "empty" and "full". So the effective number of slots that are
            // guaranteed to be usable at any time is the block size - 1 times the number of blocks - 1. Solving for
            // size and applying a ceiling to the division gives us (after simplifying):
            size_t initialBlockCount = (size + MAX_BLOCK_SIZE * 2 - 3) / (MAX_BLOCK_SIZE - 1);
            largestBlockSize = MAX_BLOCK_SIZE;
            Block *lastBlock = nullptr;
            for (size_t i = 0; i != initialBlockCount; ++i) {
                auto block = make_block(largestBlockSize);
                if (block == nullptr) {
                    break;
                }
                if (firstBlock == nullptr) {
                    firstBlock = block;
                }
                else {
                    lastBlock->next = block;
                }
                lastBlock = block;
                block->next = firstBlock;
            }
        }
        else {
            firstBlock = make_block(largestBlockSize);
            if (firstBlock == nullptr) {
                return;
            }
            firstBlock->next = firstBlock;
        }
        frontBlock = firstBlock;
        tailBlock = firstBlock;

        // Make sure the reader/writer threads will have the initialized memory setup above:
        Lockless::fence(Lockless::MemoryOrder::Sync);
    }

    SPSCQueue(SPSCQueue &&other) noexcept
        : frontBlock(other.frontBlock.load()), tailBlock(other.tailBlock.load()),
          largestBlockSize(other.largestBlockSize)
    {
        other.largestBlockSize = 32;
        Block *b = make_block(other.largestBlockSize);
        if (b == nullptr) {
            return;
        }
        b->next = b;
        other.frontBlock = b;
        other.tailBlock = b;
    }

    // Note: The queue should not be accessed concurrently while it's
    // being moved. It's up to the user to synchronize this.
    SPSCQueue &operator=(SPSCQueue &&other) noexcept
    {
        Block *b = frontBlock.load();
        frontBlock = other.frontBlock.load();
        other.frontBlock = b;
        b = tailBlock.load();
        tailBlock = other.tailBlock.load();
        other.tailBlock = b;
        std::swap(largestBlockSize, other.largestBlockSize);
        return *this;
    }

    ~SPSCQueue()
    {
        // Make sure we get the latest version of all variables from other CPUs:
        Lockless::fence(Lockless::MemoryOrder::Sync);

        // Destroy any remaining objects in queue and free memory
        Block *frontBlock_ = frontBlock;
        Block *block = frontBlock_;
        do {
            Block *nextBlock = block->next;
            size_t blockFront = block->front;
            size_t blockTail = block->tail;

            for (size_t i = blockFront; i != blockTail; i = (i + 1) & block->sizeMask) {
                auto element = reinterpret_cast<T *>(block->data + (i * sizeof(T)));
                element->~T();
                (void)element;
            }

            auto rawBlock = block->rawThis;
            block->~Block();
            std::free(rawBlock);
            block = nextBlock;
        } while (block != frontBlock_);
    }

    // Enqueues a copy of element if there is room in the queue.
    // Returns true if the element was enqueued, false otherwise.
    // Does not allocate memory.
    bool try_enqueue(T const &element)
    {
        return inner_enqueue<CannotAlloc>(element);
    }

    // Enqueues a moved copy of element if there is room in the queue.
    // Returns true if the element was enqueued, false otherwise.
    // Does not allocate memory.
    bool try_enqueue(T &&element)
    {
        return inner_enqueue<CannotAlloc>(std::forward<T>(element));
    }

    // Like try_enqueue() but with emplace semantics (i.e. construct-in-place).
    template <typename... Args>
    bool try_emplace(Args &&...args)
    {
        return inner_enqueue<CannotAlloc>(std::forward<Args>(args)...);
    }

    // Enqueues a copy of element on the queue.
    // Allocates an additional block of memory if needed.
    // Only fails (returns false) if memory allocation fails.
    bool enqueue(T const &element)
    {
        return inner_enqueue<CanAlloc>(element);
    }

    // Enqueues a moved copy of element on the queue.
    // Allocates an additional block of memory if needed.
    // Only fails (returns false) if memory allocation fails.
    bool enqueue(T &&element)
    {
        return inner_enqueue<CanAlloc>(std::forward<T>(element));
    }

    // Like enqueue() but with emplace semantics (i.e. construct-in-place).
    template <typename... Args>
    bool emplace(Args &&...args)
    {
        return inner_enqueue<CanAlloc>(std::forward<Args>(args)...);
    }

    // Attempts to dequeue an element; if the queue is empty,
    // returns false instead. If the queue has at least one element,
    // moves front to result using operator=, then returns true.
    template <typename U>
    bool try_dequeue(U &result)
    {
#ifndef NDEBUG
        ReentrantGuard guard(this->dequeuing);
#endif

        // High-level pseudocode:
        // Remember where the tail block is
        // If the front block has an element in it, dequeue it
        // Else
        //     If front block was the tail block when we entered the function, return false
        //     Else advance to next block and dequeue the item there

        // Note that we have to use the value of the tail block from before we check if the front
        // block is full or not, in case the front block is empty and then, before we check if the
        // tail block is at the front block or not, the producer fills up the front block *and
        // moves on*, which would make us skip a filled block. Seems unlikely, but was consistently
        // reproducible in practice.
        // In order to avoid overhead in the common case, though, we do a double-checked pattern
        // where we have the fast path if the front block is not empty, then read the tail block,
        // then re-read the front block and check if it's not empty again, then check if the tail
        // block has advanced.

        Block *frontBlock_ = frontBlock.load();
        size_t blockTail = frontBlock_->localTail;
        size_t blockFront = frontBlock_->front.load();

        if (blockFront != blockTail || blockFront != (frontBlock_->localTail = frontBlock_->tail.load())) {
            Lockless::fence(Lockless::MemoryOrder::Acquire);

        non_empty_front_block:
            // Front block not empty, dequeue from here
            auto element = reinterpret_cast<T *>(frontBlock_->data + (blockFront * sizeof(T)));
            result = std::move(*element);
            element->~T();

            blockFront = (blockFront + 1) & frontBlock_->sizeMask;

            Lockless::fence(Lockless::MemoryOrder::Release);
            frontBlock_->front = blockFront;
        }
        else if (frontBlock_ != tailBlock.load()) {
            Lockless::fence(Lockless::MemoryOrder::Acquire);

            frontBlock_ = frontBlock.load();
            blockTail = frontBlock_->localTail = frontBlock_->tail.load();
            blockFront = frontBlock_->front.load();
            Lockless::fence(Lockless::MemoryOrder::Acquire);

            if (blockFront != blockTail) {
                // Oh look, the front block isn't empty after all
                goto non_empty_front_block;
            }

            // Front block is empty but there's another block ahead, advance to it
            Block *nextBlock = frontBlock_->next;
            // Don't need an acquire Lockless::fence here since next can only ever be set on the tailBlock,
            // and we're not the tailBlock, and we did an acquire earlier after reading tailBlock which
            // ensures next is up-to-date on this CPU in case we recently were at tailBlock.

            size_t nextBlockFront = nextBlock->front.load();
            size_t nextBlockTail = nextBlock->localTail = nextBlock->tail.load();
            Lockless::fence(Lockless::MemoryOrder::Acquire);

            // Since the tailBlock is only ever advanced after being written to,
            // we know there's for sure an element to dequeue on it
            assert(nextBlockFront != nextBlockTail);
            AE_UNUSED(nextBlockTail);

            // We're done with this block, let the producer use it if it needs
            Lockless::fence(Lockless::MemoryOrder::Release);  // Expose possibly pending changes to frontBlock->front from last dequeue
            frontBlock = frontBlock_ = nextBlock;

            Lockless::fence(Lockless::MemoryOrder::Release);  // Not strictly needed

            auto element = reinterpret_cast<T *>(frontBlock_->data + (nextBlockFront * sizeof(T)));

            result = std::move(*element);
            element->~T();

            nextBlockFront = (nextBlockFront + 1) & frontBlock_->sizeMask;

            Lockless::fence(Lockless::MemoryOrder::Release);
            frontBlock_->front = nextBlockFront;
        }
        else {
            // No elements in current block and no other block to advance to
            return false;
        }

        return true;
    }

    // Returns a pointer to the front element in the queue (the one that
    // would be removed next by a call to `try_dequeue` or `pop`). If the
    // queue appears empty at the time the method is called, nullptr is
    // returned instead.
    // Must be called only from the consumer thread.
    T *peek() const
    {
#ifndef NDEBUG
        ReentrantGuard guard(this->dequeuing);
#endif
        // See try_dequeue() for reasoning

        Block *frontBlock_ = frontBlock.load();
        size_t blockTail = frontBlock_->localTail;
        size_t blockFront = frontBlock_->front.load();

        if (blockFront != blockTail || blockFront != (frontBlock_->localTail = frontBlock_->tail.load())) {
            Lockless::fence(Lockless::MemoryOrder::Acquire);
        non_empty_front_block:
            return reinterpret_cast<T *>(frontBlock_->data + (blockFront * sizeof(T)));
        }
        else if (frontBlock_ != tailBlock.load()) {
            Lockless::fence(Lockless::MemoryOrder::Acquire);
            frontBlock_ = frontBlock.load();
            blockTail = frontBlock_->localTail = frontBlock_->tail.load();
            blockFront = frontBlock_->front.load();
            Lockless::fence(Lockless::MemoryOrder::Acquire);

            if (blockFront != blockTail) {
                goto non_empty_front_block;
            }

            Block *nextBlock = frontBlock_->next;

            size_t nextBlockFront = nextBlock->front.load();
            Lockless::fence(Lockless::MemoryOrder::Acquire);

            assert(nextBlockFront != nextBlock->tail.load());
            return reinterpret_cast<T *>(nextBlock->data + (nextBlockFront * sizeof(T)));
        }

        return nullptr;
    }

    // Removes the front element from the queue, if any, without returning it.
    // Returns true on success, or false if the queue appeared empty at the time
    // `pop` was called.
    bool pop()
    {
#ifndef NDEBUG
        ReentrantGuard guard(this->dequeuing);
#endif
        // See try_dequeue() for reasoning

        Block *frontBlock_ = frontBlock.load();
        size_t blockTail = frontBlock_->localTail;
        size_t blockFront = frontBlock_->front.load();

        if (blockFront != blockTail || blockFront != (frontBlock_->localTail = frontBlock_->tail.load())) {
            Lockless::fence(Lockless::MemoryOrder::Acquire);

        non_empty_front_block:
            auto element = reinterpret_cast<T *>(frontBlock_->data + (blockFront * sizeof(T)));
            element->~T();

            blockFront = (blockFront + 1) & frontBlock_->sizeMask;

            Lockless::fence(Lockless::MemoryOrder::Release);
            frontBlock_->front = blockFront;
        }
        else if (frontBlock_ != tailBlock.load()) {
            Lockless::fence(Lockless::MemoryOrder::Acquire);
            frontBlock_ = frontBlock.load();
            blockTail = frontBlock_->localTail = frontBlock_->tail.load();
            blockFront = frontBlock_->front.load();
            Lockless::fence(Lockless::MemoryOrder::Acquire);

            if (blockFront != blockTail) {
                goto non_empty_front_block;
            }

            // Front block is empty but there's another block ahead, advance to it
            Block *nextBlock = frontBlock_->next;

            size_t nextBlockFront = nextBlock->front.load();
            size_t nextBlockTail = nextBlock->localTail = nextBlock->tail.load();
            Lockless::fence(Lockless::MemoryOrder::Acquire);

            assert(nextBlockFront != nextBlockTail);
            AE_UNUSED(nextBlockTail);

            Lockless::fence(Lockless::MemoryOrder::Release);
            frontBlock = frontBlock_ = nextBlock;

            Lockless::fence(Lockless::MemoryOrder::Release);

            auto element = reinterpret_cast<T *>(frontBlock_->data + (nextBlockFront * sizeof(T)));
            element->~T();

            nextBlockFront = (nextBlockFront + 1) & frontBlock_->sizeMask;

            Lockless::fence(Lockless::MemoryOrder::Release);
            frontBlock_->front = nextBlockFront;
        }
        else {
            // No elements in current block and no other block to advance to
            return false;
        }

        return true;
    }

    // Returns the approximate number of items currently in the queue.
    // Safe to call from both the producer and consumer threads.
    [[nodiscard]] size_t size_approx() const
    {
        size_t result = 0;
        Block *frontBlock_ = frontBlock.load();
        Block *block = frontBlock_;
        do {
            Lockless::fence(Lockless::MemoryOrder::Acquire);
            size_t blockFront = block->front.load();
            size_t blockTail = block->tail.load();
            result += (blockTail - blockFront) & block->sizeMask;
            block = block->next.load();
        } while (block != frontBlock_);
        return result;
    }

    // Returns the total number of items that could be enqueued without incurring
    // an allocation when this queue is empty.
    // Safe to call from both the producer and consumer threads.
    //
    // NOTE: The actual capacity during usage may be different depending on the consumer.
    //       If the consumer is removing elements concurrently, the producer cannot add to
    //       the block the consumer is removing from until it's completely empty, except in
    //       the case where the producer was writing to the same block the consumer was
    //       reading from the whole time.
    [[nodiscard]] size_t max_capacity() const
    {
        size_t result = 0;
        Block *frontBlock_ = frontBlock.load();
        Block *block = frontBlock_;
        do {
            Lockless::fence(Lockless::MemoryOrder::Acquire);
            result += block->sizeMask;
            block = block->next.load();
        } while (block != frontBlock_);
        return result;
    }

private:
    enum AllocationMode {
        CanAlloc,
        CannotAlloc
    };

    template <AllocationMode canAlloc, typename... Args>
    bool inner_enqueue(Args &&...args)
    {
        Block *tailBlock_ = tailBlock.load();
        size_t blockFront = tailBlock_->localFront;
        size_t blockTail = tailBlock_->tail.load();

        size_t nextBlockTail = (blockTail + 1) & tailBlock_->sizeMask;
        if (nextBlockTail != blockFront || nextBlockTail != (tailBlock_->localFront = tailBlock_->front.load())) {
            Lockless::fence(Lockless::MemoryOrder::Acquire);
            // This block has room for at least one more element
            char *location = tailBlock_->data + (blockTail * sizeof(T));
            new (location) T(std::forward<Args>(args)...);

            Lockless::fence(Lockless::MemoryOrder::Release);
            tailBlock_->tail = nextBlockTail;
        }
        else {
            Lockless::fence(Lockless::MemoryOrder::Acquire);
            if (tailBlock_->next.load() != frontBlock) {
                // Note that the reason we can't advance to the frontBlock and start adding new entries there
                // is because if we did, then dequeue would stay in that block, eventually reading the new values,
                // instead of advancing to the next full block (whose values were enqueued first and so should be
                // consumed first).

                Lockless::fence(Lockless::MemoryOrder::Acquire);  // Ensure we get latest writes if we got the latest frontBlock

                // tailBlock is full, but there's a free block ahead, use it
                Block *tailBlockNext = tailBlock_->next.load();
                size_t nextBlockFront = tailBlockNext->localFront = tailBlockNext->front.load();
                nextBlockTail = tailBlockNext->tail.load();
                Lockless::fence(Lockless::MemoryOrder::Acquire);

                // This block must be empty since it's not the head block and we
                // go through the blocks in a circle
                assert(nextBlockFront == nextBlockTail);
                tailBlockNext->localFront = nextBlockFront;

                char *location = tailBlockNext->data + (nextBlockTail * sizeof(T));
                new (location) T(std::forward<Args>(args)...);

                tailBlockNext->tail = (nextBlockTail + 1) & tailBlockNext->sizeMask;

                Lockless::fence(Lockless::MemoryOrder::Release);
                tailBlock = tailBlockNext;
            }
            else if (canAlloc == CanAlloc) {
                // tailBlock is full and there's no free block ahead; create a new block
                auto newBlockSize = largestBlockSize >= MAX_BLOCK_SIZE ? largestBlockSize : largestBlockSize * 2;
                auto newBlock = make_block(newBlockSize);
                if (newBlock == nullptr) {
                    // Could not allocate a block!
                    return false;
                }
                largestBlockSize = newBlockSize;

                new (newBlock->data) T(std::forward<Args>(args)...);
                assert(newBlock->front == 0);
                newBlock->tail = newBlock->localTail = 1;

                newBlock->next = tailBlock_->next.load();
                tailBlock_->next = newBlock;

                // Might be possible for the dequeue thread to see the new tailBlock->next
                // *without* seeing the new tailBlock value, but this is OK since it can't
                // advance to the next block until tailBlock is set anyway (because the only
                // case where it could try to read the next is if it's already at the tailBlock,
                // and it won't advance past tailBlock in any circumstance).

                Lockless::fence(Lockless::MemoryOrder::Release);
                tailBlock = newBlock;
            }
            else if (canAlloc == CannotAlloc) {
                // Would have had to allocate a new block to enqueue, but not allowed
                return false;
            }
            else {
                assert(false && "Should be unreachable code");
                return false;
            }
        }

        return true;
    }

public:
    // Disable copying
    SPSCQueue(SPSCQueue const &) = delete;

    // Disable assignment
    SPSCQueue &operator=(SPSCQueue const &) = delete;

private:
    static size_t ceilToPow2(size_t x)
    {
        // From http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        for (size_t i = 1; i < sizeof(size_t); i <<= 1) {
            x |= x >> (i << 3);
        }
        ++x;
        return x;
    }

    template <typename U>
    static char *align_for(char *ptr)
    {
        const std::size_t alignment = std::alignment_of_v<U>;
        return ptr + ((alignment - (reinterpret_cast<std::uintptr_t>(ptr) % alignment)) % alignment);
    }

    struct Block {
        // Avoid false-sharing by putting highly contended variables on their own cache lines
        Lockless::WeakAtomic<size_t> front;  // (Atomic) Elements are read from here
        size_t localTail;                    // An uncontended shadow copy of tail, owned by the consumer

        char cachelineFiller0[MOODYCAMEL_CACHE_LINE_SIZE - sizeof(Lockless::WeakAtomic<size_t>) - sizeof(size_t)];
        Lockless::WeakAtomic<size_t> tail;  // (Atomic) Elements are enqueued here
        size_t localFront;

        char cachelineFiller1[MOODYCAMEL_CACHE_LINE_SIZE - sizeof(Lockless::WeakAtomic<size_t>) -
                              sizeof(size_t)];  // next isn't very contended, but we don't want it on the same cache
        // line as tail (which is)
        Lockless::WeakAtomic<Block *> next;  // (Atomic)

        char *data;  // Contents (on heap) are aligned to T's alignment

        const size_t sizeMask;

        // size must be a power of two (and greater than 0)
        Block(size_t const &_size, char *_rawThis, char *_data)
            : front(0UL), localTail(0), tail(0UL), localFront(0), next(nullptr), data(_data), sizeMask(_size - 1),
              rawThis(_rawThis)
        {
        }

        // C4512 - Assignment operator could not be generated
        Block &operator=(Block const &) = delete;

        char *rawThis;
    };

    static Block *make_block(size_t capacity)
    {
        // Allocate enough memory for the block itself, as well as all the elements it will contain
        auto size = sizeof(Block) + std::alignment_of_v<Block> - 1;
        size += sizeof(T) * capacity + std::alignment_of_v<T> - 1;
        auto *newBlockRaw = static_cast<char *>(std::malloc(size));
        if (newBlockRaw == nullptr) {
            return nullptr;
        }

        auto newBlockAligned = align_for<Block>(newBlockRaw);
        auto newBlockData = align_for<T>(newBlockAligned + sizeof(Block));
        return new (newBlockAligned) Block(capacity, newBlockRaw, newBlockData);
    }

    Lockless::WeakAtomic<Block *> frontBlock;  // (Atomic) Elements are dequeued from this block

    char cachelineFiller[MOODYCAMEL_CACHE_LINE_SIZE - sizeof(Lockless::WeakAtomic<Block *>)];
    Lockless::WeakAtomic<Block *> tailBlock;  // (Atomic) Elements are enqueued to this block

    size_t largestBlockSize;
};
// NOLINTEND(readability-identifier-naming)
