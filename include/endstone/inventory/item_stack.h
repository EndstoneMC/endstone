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

namespace endstone {

namespace core {
class EndstoneItemStack;
}

/**
 * @brief Represents a stack of items.
 */
class ItemStack {
public:
    /**
     *@brief An item stack
     *
     * @param type the type
     * @param amount the amount in the stack
     * @param data the data value
     */
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

    /**
     * @brief Gets the type of this item
     *
     * @return Type of the items in this stack
     */
    [[nodiscard]] const ItemType &getType() const { return impl_->getType(); }

    /**
     * @brief Sets the type of this item
     *
     * @param type New type to set the items in this stack to
     */
    void setType(ItemTypeId type) { impl_->setType(type); }

    /**
     * @brief Gets the amount of items in this stack
     *
     * @return Amount of items in this stack
     */
    [[nodiscard]] int getAmount() const { return impl_->getAmount(); }

    /**
     * @brief Sets the amount of items in this stack
     *
     * @param amount New amount of items in this stack
     */
    void setAmount(int amount) { impl_->setAmount(amount); }

    /**
     * @brief Gets the data for this stack of items
     *
     * @return Data for this item
     */
    [[nodiscard]] int getData() const { return impl_->getData(); }

    /**
     * @brief Sets the data for this stack of items
     *
     * @param data New data for this item
     */
    void setData(int data) { impl_->setData(data); }

    /**
     * @brief Get the translation key, suitable for use in a translation component.
     *
     * @return the translation key
     */
    [[nodiscard]] std::string getTranslationKey() const { return impl_->getTranslationKey(); }

    /**
     * @brief Get the maximum stack size for this item.
     *
     * @return The maximum you can stack this item to.
     */
    [[nodiscard]] int getMaxStackSize() const { return impl_->getMaxStackSize(); }

    bool operator==(const ItemStack &other) const
    {
        if (&other == this) {
            return true;
        }
        return getAmount() == other.getAmount() && isSimilar(other);
    }

    bool operator!=(const ItemStack &other) const { return !(*this == other); }

    /**
     * @brief Checks if the two stacks are equal, but does not consider stack size (amount).
     *
     * @param other the item stack to compare to
     * @return true if the two stacks are equal, ignoring the amount
     */
    [[nodiscard]] bool isSimilar(const ItemStack &other) const
    {
        if (&other == this) {
            return true;
        }
        return impl_->isSimilar(*other.impl_);
    }

    /**
     * @brief Gets a copy of this ItemStack's ItemMeta.
     *
     * @return a copy of the current ItemStack's ItemMeta
     */
    [[nodiscard]] std::unique_ptr<ItemMeta> getItemMeta() const { return impl_->getItemMeta(); }

    /**
     * @brief Checks to see if any metadata has been defined.
     *
     * @return Returns true if some metadata has been set for this item
     */
    [[nodiscard]] bool hasItemMeta() const { return impl_->hasItemMeta(); }

    /**
     * @brief Set the ItemMeta of this ItemStack.
     *
     * @param meta new ItemMeta, or null to indicate meta data be cleared.
     * @return True if successfully applied ItemMeta
     */
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
