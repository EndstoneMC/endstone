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

#include "bedrock/bedrock.h"
#include "bedrock/world/item/crafting/recipe_ingredient.h"

class RecipeUnlockingRequirement {
public:
    enum class UnlockingContext : int {
        None = 0,
        AlwaysUnlocked = 1,
        PlayerInWater = 2,
        PlayerHasManyItems = 3,
    };

    RecipeUnlockingRequirement() = default;
    explicit RecipeUnlockingRequirement(UnlockingContext context) : context_(context) {}
    explicit RecipeUnlockingRequirement(std::vector<RecipeIngredient> unlocking_ingredients)
        : ingredients_(std::move(unlocking_ingredients))
    {
    }

    [[nodiscard]] bool isUnlockable() const
    {
        return context_ != UnlockingContext::None || !ingredients_.empty();
    }
    [[nodiscard]] bool isUnlockedByContext() const { return context_ != UnlockingContext::None; }
    [[nodiscard]] bool isUnlockedByIngredients() const { return !ingredients_.empty(); }
    [[nodiscard]] bool canBeUnlockedByContext(UnlockingContext context) const { return context_ == context; }
    [[nodiscard]] bool canBeUnlockedByIngredient(const RecipeIngredient &ingredient) const;
    [[nodiscard]] UnlockingContext getUnlockingContext() const { return context_; }
    [[nodiscard]] const std::vector<RecipeIngredient> &getUnlockingIngredients() const { return ingredients_; }
    static UnlockingContext unlockingContextFromString(const std::string &context);

private:
    UnlockingContext context_{UnlockingContext::None};  // +0
    std::vector<RecipeIngredient> ingredients_;         // +8
};
BEDROCK_STATIC_ASSERT_SIZE(RecipeUnlockingRequirement, 32, 32);
