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

#include "bedrock/world/item/crafting/recipe_unlocking_requirement.h"

bool RecipeUnlockingRequirement::canBeUnlockedByIngredient(const RecipeIngredient &ingredient) const
{
    for (const auto &unlocking : ingredients_) {
        if (unlocking.sameItem(ingredient, true)) {
            return true;
        }
    }
    return false;
}

RecipeUnlockingRequirement::UnlockingContext RecipeUnlockingRequirement::unlockingContextFromString(
    const std::string &context)
{
    if (context == "AlwaysUnlocked") {
        return UnlockingContext::AlwaysUnlocked;
    }
    if (context == "PlayerInWater") {
        return UnlockingContext::PlayerInWater;
    }
    if (context == "PlayerHasManyItems") {
        return UnlockingContext::PlayerHasManyItems;
    }
    return UnlockingContext::None;
}
