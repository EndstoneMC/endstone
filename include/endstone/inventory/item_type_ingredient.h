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

#include "endstone/inventory/item_type.h"
#include "endstone/inventory/recipe_ingredient.h"

namespace endstone {
/**
 * Represents an ingredient that matches an item type, whatever its data value.
 */
class ItemTypeIngredient : public RecipeIngredient {
public:
    explicit ItemTypeIngredient(const ItemType &type, int count = 1)
        : RecipeIngredient(std::make_unique<ItemTypeImpl>(type, count))
    {
    }
    ItemTypeIngredient(const ItemTypeIngredient &other) : RecipeIngredient(other) {}
    ItemTypeIngredient(ItemTypeIngredient &&other) noexcept = default;
    ItemTypeIngredient &operator=(const ItemTypeIngredient &other) = default;
    ItemTypeIngredient &operator=(ItemTypeIngredient &&other) noexcept = default;
    ~ItemTypeIngredient() override = default;

    /**
     * Gets the item type that this ingredient will match.
     *
     * @return the item type
     */
    [[nodiscard]] const ItemType &getItemType() const { return impl_->getItemType(); }

private:
    class ItemTypeImpl : public TypedImpl<ItemTypeIngredient> {
    public:
        ItemTypeImpl(const ItemType &type, int count) : type_(&type), count_(count) {}
        [[nodiscard]] std::unique_ptr<Impl> clone() const override { return std::make_unique<ItemTypeImpl>(*this); }
        [[nodiscard]] bool test(const ItemStack &item) const override { return item.getType() == *type_; }
        [[nodiscard]] int getCount() const override { return count_; }
        [[nodiscard]] const ItemType &getItemType() const override { return *type_; }

    private:
        const ItemType *type_;
        int count_;
    };

    friend class RecipeIngredient;
    friend class core::EndstoneIngredient;
    explicit ItemTypeIngredient(std::unique_ptr<Impl> impl) : RecipeIngredient(std::move(impl)) {}
};
}  // namespace endstone
