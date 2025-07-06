// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <functional>

#include "bedrock/common_types.h"
#include "bedrock/core/string/string_hash.h"
#include "bedrock/nbt/nbt_io.h"
#include "bedrock/symbol.h"

class BlockState {
protected:
    BlockState(size_t, const HashedString &, size_t);

public:
    virtual ~BlockState() = default;
    [[nodiscard]] size_t getID() const
    {
        return id_;
    }
    [[nodiscard]] size_t getVariationCount() const;
    [[nodiscard]] const HashedString &getName() const
    {
        return name_;
    }
    virtual void toNBT(CompoundTag &, int) const = 0;
    virtual bool fromNBT(const CompoundTag &, int &) const = 0;
    static void forEachState(std::function<bool(const BlockState &)> callback)
    {
        const auto *node = BEDROCK_VAR(StateListNode *, "BlockState::StateListNode::head");
        while (node != nullptr) {
            if (!callback(*node->stat)) {
                break;
            }
            node = node->next;
        }
    }

protected:
    const size_t id_;
    const size_t variation_count_;
    const HashedString name_;
    struct StateListNode {
        static StateListNode *head;
        StateListNode *next;
        StateListNode *prev;
        BlockState *stat;
        StateListNode(BlockState *);
        ~StateListNode();
    };
    static_assert(sizeof(StateListNode) == 24);
    StateListNode node_;
};

template <typename T>
class BlockStateVariant;

template <>
class BlockStateVariant<bool> : public BlockState {
public:
    BlockStateVariant(size_t, const HashedString &, size_t);
    BlockStateVariant(size_t, const HashedString &, std::initializer_list<bool>);
    void toNBT(CompoundTag &, int) const override;
    [[nodiscard]] bool fromNBT(const CompoundTag &, int &) const override;
};

template <>
class BlockStateVariant<int> : public BlockState {
public:
    BlockStateVariant(size_t, const HashedString &, size_t);
    BlockStateVariant(size_t, const HashedString &, std::initializer_list<int>);
    void toNBT(CompoundTag &, int) const override;
    [[nodiscard]] bool fromNBT(const CompoundTag &, int &) const override;
};

class Block;
class BlockStateInstance {
public:
    template <typename T>
    const Block *trySet(DataID data, const T &val, const std::vector<std::unique_ptr<Block>> &permutations) const
    {
        auto v = static_cast<std::uint32_t>(val);
        if (v >= variation_count_) {
            return nullptr;
        }
        auto index = (data & ~mask_) | ((v << (end_bit_ - num_bits_ + 1)) & mask_);
        if (index >= permutations.size()) {
            return nullptr;
        }
        return permutations[index].get();
    }

    template <typename T>
    T get(const DataID &data) const
    {
        return static_cast<T>((0xFFFF >> (MAX_STATE_BITS - num_bits_)) & (data >> (end_bit_ - num_bits_ + 1)));
    }

    [[nodiscard]] std::uint32_t getMask() const
    {
        return mask_;
    }

    [[nodiscard]] std::uint32_t getNumBits() const
    {
        return num_bits_;
    }

    [[nodiscard]] bool isValidData(std::uint32_t id) const
    {
        return (mask_ & id) >> (end_bit_ - num_bits_ + 1) < variation_count_;
    }

    [[nodiscard]] const BlockState &getState() const
    {
        return *state_;
    }

private:
    static constexpr std::uint32_t MAX_STATE_BITS = 16;

    std::uint32_t end_bit_;          // +0
    std::uint32_t num_bits_;         // +4
    std::uint32_t variation_count_;  // +8
    std::uint32_t mask_;             // +12
    bool initialised_;               // +16
    BlockState *state_;              // +24
};
static_assert(sizeof(BlockStateInstance) == 32);
