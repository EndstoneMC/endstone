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

#include "endstone/inventory/item_stack.h"
#include "endstone/inventory/recipe_ingredient.h"

namespace endstone {
/**
 * Represents an ingredient that matches one item with one exact data value.
 */
class ExactIngredient : public RecipeIngredient {
public:
    explicit ExactIngredient(ItemStack item)
        : RecipeIngredient(std::make_unique<ExactImpl>(std::move(item)))
    {
    }
    ExactIngredient(const ExactIngredient &other) : RecipeIngredient(other) {}
    ExactIngredient(ExactIngredient &&other) noexcept = default;
    ExactIngredient &operator=(const ExactIngredient &other) = default;
    ExactIngredient &operator=(ExactIngredient &&other) noexcept = default;
    ~ExactIngredient() override = default;

    [[nodiscard]] ItemStack getItemStack() const { return impl_->getItemStack(); }

private:
    class ExactImpl : public TypedImpl<ExactIngredient> {
    public:
        explicit ExactImpl(ItemStack item) : item_(std::move(item)) {}
        [[nodiscard]] std::unique_ptr<Impl> clone() const override { return std::make_unique<ExactImpl>(*this); }
        [[nodiscard]] bool test(const ItemStack &item) const override { return item_.isSimilar(item); }
        [[nodiscard]] int getCount() const override { return item_.getAmount(); }
        [[nodiscard]] ItemStack getItemStack() const override { return item_; }

    private:
        ItemStack item_;
    };

    friend class RecipeIngredient;
    friend class core::EndstoneIngredient;
    explicit ExactIngredient(std::unique_ptr<Impl> impl) : RecipeIngredient(std::move(impl)) {}
};
}  // namespace endstone
