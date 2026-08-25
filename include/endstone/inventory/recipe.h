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

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <optional>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "endstone/inventory/item_stack.h"
#include "endstone/inventory/recipe_ingredient.h"
#include "endstone/object.h"

namespace endstone {

namespace core {
class EndstoneRecipe;
}

/**
 * Represents some type of crafting recipe.
 */
class Recipe : public Object {
public:
    Recipe(const Recipe &other) : impl_(other.impl_->clone()) {}
    Recipe(Recipe &&other) noexcept = default;
    Recipe &operator=(const Recipe &other)
    {
        if (this != &other) {
            impl_ = other.impl_->clone();
        }
        return *this;
    }
    Recipe &operator=(Recipe &&other) noexcept = default;
    ~Recipe() override = default;

    [[nodiscard]] ClassInfo getClassInfo() const override { return impl_->getClassInfo(); }
    [[nodiscard]] bool isInstanceOf(ClassInfo target) const override { return impl_->isInstanceOf(target); }

    /**
     * Get the result of this recipe.
     *
     * @return The result stack
     */
    [[nodiscard]] ItemStack getResult() const { return impl_->getResult(); }

    [[nodiscard]] const std::vector<std::optional<RecipeIngredient>> &getIngredients() const
    {
        return impl_->getIngredients();
    }

    [[nodiscard]] const std::string &getRecipeId() const { return impl_->getRecipeId(); }

    /**
     * Get the crafting station this recipe belongs to, such as `crafting_table` or `smithing_table`.
     *
     * @return the crafting tag
     */
    [[nodiscard]] const std::string &getTag() const { return impl_->getTag(); }

    /**
     * Attempts to copy this recipe as the given type T.
     *
     * @tparam T Target type to copy as (must derive from Recipe)
     * @return A copy of this recipe as T, or `std::nullopt` if this is not an instance of T
     */
    template <typename T>
        requires std::is_base_of_v<Recipe, T>
    [[nodiscard]] std::optional<T> as() const
    {
        if (!is<T>()) {
            return std::nullopt;
        }
        return T(impl_->clone());
    }

protected:
    class Impl {
    public:
        virtual ~Impl() = default;
        [[nodiscard]] virtual std::unique_ptr<Impl> clone() const = 0;
        [[nodiscard]] virtual ClassInfo getClassInfo() const = 0;
        [[nodiscard]] virtual bool isInstanceOf(ClassInfo target) const = 0;
        [[nodiscard]] virtual ItemStack getResult() const = 0;
        [[nodiscard]] virtual const std::vector<std::optional<RecipeIngredient>> &getIngredients() const = 0;
        [[nodiscard]] virtual const std::string &getRecipeId() const = 0;
        [[nodiscard]] virtual const std::string &getTag() const = 0;
        [[nodiscard]] virtual int getWidth() const { return 0; }
        [[nodiscard]] virtual int getHeight() const { return 0; }
        [[nodiscard]] virtual std::optional<RecipeIngredient> getSmithingIngredient(std::size_t /*index*/) const
        {
            return std::nullopt;
        }
    };

    class SimpleImpl : public Impl {
    public:
        SimpleImpl(std::initializer_list<ClassInfo> types, std::string recipe_id, std::string tag, ItemStack result,
                   std::vector<std::optional<RecipeIngredient>> ingredients, int width = 0, int height = 0)
            : types_(types), recipe_id_(std::move(recipe_id)), tag_(std::move(tag)), result_(std::move(result)),
              ingredients_(std::move(ingredients)), width_(width), height_(height)
        {
        }

        [[nodiscard]] std::unique_ptr<Impl> clone() const override { return std::make_unique<SimpleImpl>(*this); }
        [[nodiscard]] ClassInfo getClassInfo() const override { return types_.front(); }
        [[nodiscard]] bool isInstanceOf(ClassInfo target) const override
        {
            for (const auto &type : types_) {
                if (type == target) {
                    return true;
                }
            }
            return false;
        }
        [[nodiscard]] ItemStack getResult() const override { return result_; }
        [[nodiscard]] const std::vector<std::optional<RecipeIngredient>> &getIngredients() const override
        {
            return ingredients_;
        }
        [[nodiscard]] const std::string &getRecipeId() const override { return recipe_id_; }
        [[nodiscard]] const std::string &getTag() const override { return tag_; }
        [[nodiscard]] int getWidth() const override { return width_; }
        [[nodiscard]] int getHeight() const override { return height_; }
        [[nodiscard]] std::optional<RecipeIngredient> getSmithingIngredient(std::size_t index) const override
        {
            return index < ingredients_.size() ? ingredients_[index] : std::nullopt;
        }

    private:
        std::vector<ClassInfo> types_;
        std::string recipe_id_;
        std::string tag_;
        ItemStack result_;
        std::vector<std::optional<RecipeIngredient>> ingredients_;
        int width_;
        int height_;
    };

    explicit Recipe(std::unique_ptr<Impl> impl) : impl_(std::move(impl)) {}

    friend class core::EndstoneRecipe;
    std::unique_ptr<Impl> impl_;
};

}  // namespace endstone
