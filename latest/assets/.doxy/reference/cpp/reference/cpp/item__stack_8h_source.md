

# File item\_stack.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**inventory**](dir_d1e84b530b14f41e8b6f5ec1b5dee76c.md) **>** [**item\_stack.h**](item__stack_8h.md)

[Go to the documentation of this file](item__stack_8h.md)


```C++
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

#include <memory>
#include <string>

#include "endstone/inventory/item_type.h"
#include "endstone/inventory/meta/item_meta.h"
#include "endstone/nbt/tag.h"

namespace endstone {

namespace core {
class EndstoneItemStack;
}

class ItemStack {
public:
    explicit ItemStack(ItemTypeId type, int amount = 1, int data = 0)
    {
        auto *item_type = ItemType::get(type);
        if (!item_type) {
            throw std::invalid_argument(fmt::format("Unknown item type: {}", type));
        }
        *this = item_type->createItemStack(amount);
        impl_->setData(data);
    }
    ItemStack(const ItemStack &other) : impl_(other.impl_->clone()) {}
    ItemStack(ItemStack &&other) noexcept = default;
    ItemStack &operator=(const ItemStack &other)
    {
        if (this != &other) {
            impl_ = other.impl_->clone();
        }
        return *this;
    }
    ItemStack &operator=(ItemStack &&other) noexcept = default;
    ~ItemStack() = default;

    [[nodiscard]] const ItemType &getType() const { return impl_->getType(); }

    void setType(ItemTypeId type) { impl_->setType(type); }

    [[nodiscard]] int getAmount() const { return impl_->getAmount(); }

    void setAmount(int amount) { impl_->setAmount(amount); }

    [[nodiscard]] int getData() const { return impl_->getData(); }

    void setData(int data) { impl_->setData(data); }

    [[nodiscard]] std::string getTranslationKey() const { return impl_->getTranslationKey(); }

    [[nodiscard]] int getMaxStackSize() const { return impl_->getMaxStackSize(); }

    [[nodiscard]] CompoundTag getNbt() const { return impl_->getNbt(); }

    void setNbt(const CompoundTag &nbt) { impl_->setNbt(nbt); }

    bool operator==(const ItemStack &other) const
    {
        if (&other == this) {
            return true;
        }
        return getAmount() == other.getAmount() && isSimilar(other);
    }

    bool operator!=(const ItemStack &other) const { return !(*this == other); }

    [[nodiscard]] bool isSimilar(const ItemStack &other) const
    {
        if (&other == this) {
            return true;
        }
        return impl_->isSimilar(*other.impl_);
    }

    [[nodiscard]] std::unique_ptr<ItemMeta> getItemMeta() const { return impl_->getItemMeta(); }

    [[nodiscard]] bool hasItemMeta() const { return impl_->hasItemMeta(); }

    bool setItemMeta(ItemMeta *meta) { return impl_->setItemMeta(meta); }

private:
    friend class core::EndstoneItemStack;

    class Impl {
    public:
        virtual ~Impl() = default;
        [[nodiscard]] virtual std::unique_ptr<Impl> clone() const = 0;
        [[nodiscard]] virtual const ItemType &getType() const = 0;
        virtual void setType(ItemTypeId type) = 0;
        [[nodiscard]] virtual int getAmount() const = 0;
        virtual void setAmount(int amount) = 0;
        [[nodiscard]] virtual int getData() const = 0;
        virtual void setData(int data) = 0;
        [[nodiscard]] virtual std::string getTranslationKey() const = 0;
        [[nodiscard]] virtual int getMaxStackSize() const = 0;
        [[nodiscard]] virtual bool isSimilar(const Impl &other) const = 0;
        [[nodiscard]] virtual std::unique_ptr<ItemMeta> getItemMeta() const = 0;
        [[nodiscard]] virtual bool hasItemMeta() const = 0;
        virtual bool setItemMeta(const ItemMeta *meta) = 0;
        [[nodiscard]] virtual CompoundTag getNbt() const = 0;
        virtual void setNbt(const CompoundTag &nbt) = 0;
    };

    explicit ItemStack(std::unique_ptr<Impl> impl) : impl_(std::move(impl)) {}
    std::unique_ptr<Impl> impl_;
};
}  // namespace endstone

template <>
struct fmt::formatter<endstone::ItemStack> : formatter<string_view> {
    using Type = endstone::ItemStack;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "ItemStack({} x {})", val.getType(), val.getAmount());
    }
};  // namespace fmt
```


