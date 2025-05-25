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
#include "endstone/inventory/meta/item_meta.h"

namespace endstone {

namespace core {
class EndstoneItemStack;
}

/**
 * @brief Represents a stack of items.
 */
class ItemStack {
protected:
    ItemStack() = default;

public:
    explicit ItemStack(std::string type, const int amount = 1) : type_(std::move(type)), amount_(amount) {}
    ItemStack(const ItemStack &stack)
    {
        type_ = stack.getType();
        amount_ = stack.getAmount();
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
    /**
     * @brief Gets the type of this item
     *
     * @return Type of the items in this stack
     */
    [[nodiscard]] virtual std::string getType() const
    {
        return type_;
    }

    /**
     * @brief Sets the type of this item
     *
     * @param type New type to set the items in this stack to
     */
    virtual void setType(std::string type)
    {
        // TODO(item): clear item components when the type is changed
        type_ = std::move(type);
    }

    /**
     * @brief Gets the amount of items in this stack
     *
     * @return Amount of items in this stack
     */
    [[nodiscard]] virtual int getAmount() const
    {
        return amount_;
    }

    /**
     * @brief Sets the amount of items in this stack
     *
     * @param amount New amount of items in this stack
     */
    virtual void setAmount(int amount)
    {
        amount_ = amount;
    }

    /**
     * @brief Gets a copy of this ItemStack's ItemMeta.
     *
     * @return a copy of the current ItemStack's ItemMeta
     */
    [[nodiscard]] virtual std::unique_ptr<ItemMeta> getItemMeta() const
    {
        return meta_ == nullptr ? Endstone::getServer().getItemFactory().getItemMeta(type_) : meta_->clone();
    }

    /**
     * @brief Checks to see if any metadata has been defined.
     *
     * @return Returns true if some metadata has been set for this item
     */
    [[nodiscard]] virtual bool hasItemMeta() const
    {
        return !Endstone::getServer().getItemFactory().equals(meta_.get(), nullptr);
    }

    /**
     * @brief Set the ItemMeta of this ItemStack.
     *
     * @param meta new ItemMeta, or null to indicate meta data be cleared.
     * @return True if successfully applied ItemMeta
     */
    virtual bool setItemMeta(ItemMeta *meta)
    {
        return setItemMeta0(meta, type_);
    }

    [[nodiscard]] virtual std::unique_ptr<ItemStack> clone() const
    {
        return std::make_unique<ItemStack>(*this);
    }

private:
    bool setItemMeta0(ItemMeta *meta, const std::string_view type)
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

    std::string type_ = "minecraft:air";
    int amount_ = 0;
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
