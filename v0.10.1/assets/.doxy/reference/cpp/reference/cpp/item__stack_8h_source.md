

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

#include "endstone/detail/endstone.h"
#include "endstone/inventory/item_factory.h"
#include "endstone/inventory/item_type.h"
#include "endstone/inventory/meta/item_meta.h"
#include "endstone/registry.h"

namespace endstone {

namespace core {
class EndstoneItemStack;
}

class ItemStack {
public:
    explicit ItemStack(const std::string &type, const int amount = 1, const int data = 0)
    {
        if (const auto *item_type = ItemType::get(type)) {
            type_ = *item_type;
            amount_ = amount;
            data_ = data;
        }
    }

    explicit ItemStack(const ItemType &type, const int amount = 1, const int data = 0)
        : type_(type), amount_(amount), data_(data)
    {
    }

    ItemStack(const ItemStack &stack) : type_(stack.getType()), amount_(stack.getAmount()), data_(stack.getData())
    {
        if (stack.hasItemMeta()) {
            ItemStack::setItemMeta(stack.getItemMeta().get());
        }
    }

    virtual ~ItemStack() = default;

protected:
    friend class core::EndstoneItemStack;
    [[nodiscard]] virtual bool isEndstoneItemStack() const
    {
        return false;
    }

public:
    [[nodiscard]] virtual const ItemType &getType() const
    {
        return type_;
    }

    virtual Result<void> setType(const std::string &type)
    {
        const auto *item_type = ItemType::get(type);
        ENDSTONE_CHECKF(item_type != nullptr, "Unknown item type: {}", type);
        setType(*item_type);
        return {};
    }

    virtual void setType(const ItemType &type)
    {
        // TODO(item): clear item components when the type is changed
        type_ = type;
    }

    [[nodiscard]] virtual int getAmount() const
    {
        return amount_;
    }

    virtual void setAmount(const int amount)
    {
        amount_ = amount;
    }

    [[nodiscard]] virtual int getData() const
    {
        return data_;
    }

    virtual void setData(const int data)
    {
        data_ = data;
    }

    [[nodiscard]] virtual int getMaxStackSize() const
    {
        return getType().getMaxStackSize();
    }

    bool operator==(const ItemStack &other) const
    {
        if (&other == this) {
            return true;
        }
        return getAmount() == other.getAmount() && isSimilar(other);
    }

    bool operator!=(const ItemStack &other) const
    {
        return !(*this == other);
    }

    [[nodiscard]] virtual bool isSimilar(const ItemStack &other) const
    {
        if (&other == this) {
            return true;
        }
        return getType() == other.getType() && hasItemMeta() == other.hasItemMeta() &&
               (hasItemMeta()
                    ? Endstone::getServer().getItemFactory().equals(getItemMeta().get(), other.getItemMeta().get())
                    : true);
    }

    [[nodiscard]] virtual std::unique_ptr<ItemMeta> getItemMeta() const
    {
        return meta_ == nullptr ? Endstone::getServer().getItemFactory().getItemMeta(type_) : meta_->clone();
    }

    [[nodiscard]] virtual bool hasItemMeta() const
    {
        return !Endstone::getServer().getItemFactory().equals(meta_.get(), nullptr);
    }

    virtual bool setItemMeta(ItemMeta *meta)
    {
        return setItemMeta0(meta, type_);
    }

    [[nodiscard]] virtual std::unique_ptr<ItemStack> clone() const
    {
        return std::make_unique<ItemStack>(*this);
    }

    static Result<ItemStack> create(const ItemType &type, const int amount = 1, const int data = 0)
    {
        ENDSTONE_CHECKF(amount >= 1 && amount <= 255, "Item stack amount must be between 1 to 255, got {}.", amount);
        return ItemStack(type, amount, data);
    }

    static Result<ItemStack> create(const std::string &type, const int amount = 1, const int data = 0)
    {
        const auto *item_type = ItemType::get(type);
        ENDSTONE_CHECKF(item_type != nullptr, "Unknown item type: {}", type);
        return create(*item_type, amount, data);
    }

private:
    bool setItemMeta0(const ItemMeta *meta, const ItemType &type)
    {
        if (!meta) {
            meta_ = nullptr;
            return true;
        }

        const auto &item_factory = Endstone::getServer().getItemFactory();
        if (!item_factory.isApplicable(meta, type)) {
            return false;
        }

        meta_ = item_factory.asMetaFor(meta, type);
        return true;
    }

    std::reference_wrapper<const ItemType> type_ = *ItemType::get("minecraft:air");
    int amount_ = 0;
    int data_ = 0;
    std::unique_ptr<ItemMeta> meta_ = nullptr;
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


