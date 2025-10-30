// ©2013-2016 Cameron Desrochers.
// Distributed under the simplified BSD license (see the license file that
// should have come with this header).
// Uses Jeff Preshing's semaphore implementation (under the terms of its
// separate zlib license, embedded below).

#pragma once

#include <atomic>
#include <cassert>
#include <type_traits>
#include <utility>

// AE_UNUSED
#define AE_UNUSED(x) ((void)x)

#ifndef AE_TSAN_ANNOTATE_RELEASE
#define AE_TSAN_ANNOTATE_RELEASE()
#define AE_TSAN_ANNOTATE_ACQUIRE()
#endif

namespace Lockless {
enum class MemoryOrder {
    Relaxed,
    Acquire,
    Release,
    AcqRel,
    SeqCst,
    Sync = SeqCst
};

inline void fence(MemoryOrder order)
{
    switch (order) {
    case MemoryOrder::Relaxed:
        break;
    case MemoryOrder::Acquire:
        AE_TSAN_ANNOTATE_ACQUIRE();
        std::atomic_thread_fence(std::memory_order_acquire);
        break;
    case MemoryOrder::Release:
        AE_TSAN_ANNOTATE_RELEASE();
        std::atomic_thread_fence(std::memory_order_release);
        break;
    case MemoryOrder::AcqRel:
        AE_TSAN_ANNOTATE_ACQUIRE();
        AE_TSAN_ANNOTATE_RELEASE();
        std::atomic_thread_fence(std::memory_order_acq_rel);
        break;
    case MemoryOrder::SeqCst:
        AE_TSAN_ANNOTATE_ACQUIRE();
        AE_TSAN_ANNOTATE_RELEASE();
        std::atomic_thread_fence(std::memory_order_seq_cst);
        break;
    default:
        assert(false);
    }
}

template <typename T>
class WeakAtomic {
public:
    WeakAtomic() : value() {}
    template <typename U>
    WeakAtomic(U &&x) : value(std::forward<U>(x))
    {
    }
    WeakAtomic(WeakAtomic const &other) : value(other.load()) {}
    WeakAtomic(WeakAtomic &&other) noexcept : value(std::move(other.load())) {}

    operator T() const
    {
        return load();
    }

    template <typename U>
    WeakAtomic const &operator=(U &&x)
    {
        value.store(std::forward<U>(x), std::memory_order_relaxed);
        return *this;
    }

    WeakAtomic const &operator=(WeakAtomic const &other)
    {
        value.store(other.value.load(std::memory_order_relaxed), std::memory_order_relaxed);
        return *this;
    }

    T load() const
    {
        return value.load(std::memory_order_relaxed);
    }

    T fetch_add_acquire(T increment)
    {
        return value.fetch_add(increment, std::memory_order_acquire);
    }

    T fetch_add_release(T increment)
    {
        return value.fetch_add(increment, std::memory_order_release);
    }

private:
    std::atomic<T> value;
};
}  // namespace Lockless
