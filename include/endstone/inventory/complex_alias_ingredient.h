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

#include "endstone/inventory/recipe_ingredient.h"

namespace endstone {
/**
 * Represents an ingredient that matches any item an id stands for.
 */
class ComplexAliasIngredient : public RecipeIngredient {
public:
    ~ComplexAliasIngredient() override = default;

    /**
     * Gets the alias that this ingredient will match.
     *
     * The id predates the item flattening, such as `minecraft:planks`, and stands for every item it was split into.
     * It is not an item tag, and the items are not reported.
     *
     * @return the alias
     */
    [[nodiscard]] virtual const std::string &getAlias() const = 0;
};
}  // namespace endstone
