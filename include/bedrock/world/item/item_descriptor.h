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

#include "bedrock/deps/jsoncpp/value.h"
#include "bedrock/nbt/compound_tag.h"
#include "bedrock/world/item/item_tag.h"

class Item;

class ItemDescriptor {
public:
    enum class InternalType : std::uint8_t {
        Invalid = 0,
        Default = 1,
        Molang = 2,
        ItemTag = 3,
        Deferred = 4,
        ComplexAlias = 5,
    };

    struct ItemEntry {
        const Item *item;
        std::int16_t aux_value;
    };

    class BaseDescriptor {
    public:
        [[nodiscard]] virtual std::unique_ptr<ItemDescriptor::BaseDescriptor> clone() const = 0;
        [[nodiscard]] virtual bool sameItems(ItemDescriptor::BaseDescriptor const &, bool) const = 0;
        [[nodiscard]] virtual bool sameItem(ItemDescriptor::ItemEntry const &, bool) const = 0;
        [[nodiscard]] virtual std::string const &getFullName() const = 0;
        [[nodiscard]] virtual ItemDescriptor::ItemEntry getItem() const = 0;
        [[nodiscard]] virtual bool forEachItemUntil(std::function<bool(Item const &, std::int16_t)> func) const = 0;
        [[nodiscard]] virtual std::map<std::string, std::string> toMap() const = 0;
        [[nodiscard]] virtual std::optional<CompoundTag> save() const = 0;
        virtual void serialize(Json::Value &val) const = 0;
        virtual void serialize(BinaryStream &stream) const = 0;
        [[nodiscard]] virtual ItemDescriptor::InternalType getType() const = 0;
        [[nodiscard]] virtual bool isValid() const = 0;
        [[nodiscard]] virtual std::size_t getHash() const = 0;
        [[nodiscard]] virtual bool shouldResolve() const = 0;
        [[nodiscard]] virtual std::unique_ptr<ItemDescriptor::BaseDescriptor> resolve() const = 0;
        virtual ~BaseDescriptor() = 0;
    };

    class ItemTagDescriptor : public BaseDescriptor {
    public:
        ItemTag item_tag;
    };

    ItemDescriptor() = default;
    virtual ~ItemDescriptor() = default;

    [[nodiscard]] std::string const &getFullName() const
    {
        static std::string empty;
        if (!impl) {
            return empty;
        }
        return impl->getFullName();
    }

    [[nodiscard]] const Item *getItem() const
    {
        if (!impl) {
            return nullptr;
        }
        if (impl->shouldResolve()) {
            impl = std::move(impl->resolve());
        }
        return impl->getItem().item;
    }

    [[nodiscard]] std::int16_t getAuxValue() const
    {
        if (!impl) {
            return 0x7FFF;
        }
        if (impl->shouldResolve()) {
            impl = std::move(impl->resolve());
        }
        return impl->getItem().aux_value;
    }

    mutable std::unique_ptr<ItemDescriptor::BaseDescriptor> impl;
};
