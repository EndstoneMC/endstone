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

#include "bedrock/world/item/crafting/recipes.h"

ItemInstance Recipes::getFurnaceRecipeResult(const ItemStackBase &item, const HashedString &) const
{
    if (!item || furnace_results_.empty()) {
        return ItemInstance();
    }

    // BDS 1.26.20 reorganised furnace recipes into a nested id -> aux -> result map.
    const auto outer = furnace_results_.find(item.getId());
    if (outer == furnace_results_.end()) {
        return ItemInstance();
    }

    if (const auto exact = outer->second.find(item.getAuxValue()); exact != outer->second.end()) {
        return exact->second;
    }
    if (const auto any = outer->second.find(ItemDescriptor::ANY_AUX_VALUE); any != outer->second.end()) {
        return any->second;
    }

    return ItemInstance();
}
