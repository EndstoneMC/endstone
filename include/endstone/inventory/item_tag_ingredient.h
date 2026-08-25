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

#include <string>
#include <utility>

#include "endstone/inventory/recipe_ingredient.h"

namespace endstone {
/**
 * Represents an ingredient that matches any item carrying a tag.
 */
class ItemTagIngredient : public RecipeIngredient {
public:
    explicit ItemTagIngredient(std::string item_tag, int count = 1)
        : RecipeIngredient(std::make_unique<ItemTagImpl>(std::move(item_tag), count))
    {
    }
    ItemTagIngredient(const ItemTagIngredient &other) : RecipeIngredient(other) {}
    ItemTagIngredient(ItemTagIngredient &&other) noexcept = default;
    ItemTagIngredient &operator=(const ItemTagIngredient &other) = default;
    ItemTagIngredient &operator=(ItemTagIngredient &&other) noexcept = default;
    ~ItemTagIngredient() override = default;

    /**
     * Gets the item tag that this ingredient will match.
     *
     * Bedrock keeps item tags server-side, so the tag is reported rather than the item types in it.
     *
     * @return the item tag
     */
    [[nodiscard]] const std::string &getItemTag() const { return impl_->getItemTag(); }

private:
    class ItemTagImpl : public TypedImpl<ItemTagIngredient> {
    public:
        ItemTagImpl(std::string item_tag, int count) : item_tag_(std::move(item_tag)), count_(count) {}
        [[nodiscard]] std::unique_ptr<Impl> clone() const override { return std::make_unique<ItemTagImpl>(*this); }
        [[nodiscard]] bool test(const ItemStack & /*item*/) const override { return false; }
        [[nodiscard]] int getCount() const override { return count_; }
        [[nodiscard]] const std::string &getItemTag() const override { return item_tag_; }

    private:
        std::string item_tag_;
        int count_;
    };

    friend class RecipeIngredient;
    friend class core::EndstoneIngredient;
    explicit ItemTagIngredient(std::unique_ptr<Impl> impl) : RecipeIngredient(std::move(impl)) {}
};
}  // namespace endstone
