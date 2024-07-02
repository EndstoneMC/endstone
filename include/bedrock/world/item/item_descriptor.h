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

class Item;

class ItemDescriptor {
    enum class InternalType;

public:
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
        [[nodiscard]] virtual bool forEachItemUntil(std::function<bool(Item const &, short)> func) const = 0;
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

    ItemDescriptor() = default;
    virtual ~ItemDescriptor() = default;

    [[nodiscard]] std::string const &getFullName() const
    {
        static std::string empty;
        if (!impl_) {
            return empty;
        }
        return impl_->getFullName();
    }

    [[nodiscard]] const Item *getItem() const
    {
        if (!impl_) {
            return nullptr;
        }
        if (impl_->shouldResolve()) {
            impl_ = std::move(impl_->resolve());
        }
        return impl_->getItem().item;
    }

    [[nodiscard]] std::int16_t getAuxValue() const
    {
        if (!impl_) {
            return 0x7FFF;
        }
        if (impl_->shouldResolve()) {
            impl_ = std::move(impl_->resolve());
        }
        return impl_->getItem().aux_value;
    }

private:
    mutable std::unique_ptr<ItemDescriptor::BaseDescriptor> impl_;
};
