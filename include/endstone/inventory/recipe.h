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
#include <vector>

#include "endstone/inventory/item_stack.h"
#include "endstone/inventory/recipe_ingredient.h"
#include "endstone/object.h"

namespace endstone {
/**
 * Represents some type of crafting recipe.
 */
class Recipe : public Object {
public:
    ~Recipe() override = default;

    /**
     * Get the result of this recipe.
     *
     * @return The result stack
     */
    [[nodiscard]] virtual ItemStack getResult() const = 0;

    [[nodiscard]] virtual const std::vector<Nullable<RecipeIngredient>> &getIngredients() const = 0;

    /**
     * Get the identifier of this recipe, such as `minecraft:crafting_table`.
     *
     * @return the recipe identifier
     */
    [[nodiscard]] virtual const std::string &getId() const = 0;

    /**
     * Get the crafting station this recipe belongs to, such as `crafting_table` or `smithing_table`.
     *
     * @return the crafting tag
     */
    [[nodiscard]] virtual const std::string &getTag() const = 0;
};
}  // namespace endstone
