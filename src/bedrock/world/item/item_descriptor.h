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

#include <map>
#include <optional>

#include "bedrock/core/utility/binary_stream.h"
#include "bedrock/deps/json/value.h"
#include "bedrock/nbt/compound_tag.h"
#include "bedrock/world/level/block/block.h"
#include "bedrock/world/level/block/block_type.h"

class Item;

class ItemDescriptor {
public:
    static constexpr int ANY_AUX_VALUE = 0x7FFF;
    static constexpr HashType64 EMPTY_INGREDIENT_HASH = 0xB7F87A68;
    explicit ItemDescriptor() = default;
    explicit ItemDescriptor(const Block &);
    explicit ItemDescriptor(const BlockType &);
    explicit ItemDescriptor(const Item &, int);
    ItemDescriptor(const ItemDescriptor &);
    ItemDescriptor(ItemDescriptor &&) = default;

    virtual ~ItemDescriptor() = default;
    virtual void serialize(Json::Value &) const;
    virtual void serialize(BinaryStream &) const;

    ItemDescriptor &operator=(ItemDescriptor &&) = default;
    [[nodiscard]] const Item *getItem() const;
    [[nodiscard]] const Block *getBlock() const;
    [[nodiscard]] bool isValid(bool should_resolve) const;
    [[nodiscard]] bool isNull() const;
    [[nodiscard]] std::int16_t getId() const;
    [[nodiscard]] std::int16_t getAuxValue() const;
    [[nodiscard]] std::string getFullName() const;

    struct ItemEntry {
        const Item *item;
        std::int16_t aux_value;
        ItemEntry(const Item *, std::int16_t);
        const Block *getBlock() const;
    };

    enum class InternalType : std::uint8_t {
        Invalid = 0,
        Default = 1,
        Molang = 2,
        ItemTag = 3,
        Deferred = 4,
        ComplexAlias = 5,
    };

    class BaseDescriptor {
    public:
        [[nodiscard]] virtual std::unique_ptr<BaseDescriptor> clone() const = 0;
        [[nodiscard]] virtual bool sameItems(BaseDescriptor const &, bool) const;
        [[nodiscard]] virtual bool sameItem(ItemEntry const &, bool) const = 0;
        [[nodiscard]] virtual std::string getFullName() const;
        [[nodiscard]] virtual std::string toString() const;
        [[nodiscard]] virtual ItemEntry getItem() const = 0;
        [[nodiscard]] virtual bool forEachItemUntil(std::function<bool(Item const &, std::int16_t)> func) const;
        [[nodiscard]] virtual std::map<std::string, std::string> toMap() const = 0;
        [[nodiscard]] virtual std::optional<CompoundTag> save() const = 0;
        virtual void serialize(Json::Value &val) const;
        virtual void serialize(BinaryStream &stream) const = 0;
        [[nodiscard]] virtual InternalType getType() const = 0;
        [[nodiscard]] virtual bool isValid() const;
        [[nodiscard]] virtual std::size_t getHash() const = 0;
        [[nodiscard]] virtual bool shouldResolve() const;
        [[nodiscard]] virtual std::unique_ptr<BaseDescriptor> resolve() const;
        virtual ~BaseDescriptor() = default;
    };

private:
    mutable std::unique_ptr<BaseDescriptor> impl_;
};

struct ItemTagDescriptor;
