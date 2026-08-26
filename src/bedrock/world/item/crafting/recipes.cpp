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

#include <cstdint>

#include "bedrock/world/item/item.h"

ItemInstance Recipes::getFurnaceRecipeResult(const ItemStackBase &item, const HashedString &tag) const
{
    if (!item) {
        return ItemInstance();
    }

    const auto outer = furnace_results_.find(static_cast<int>(tag.getHash()));
    if (outer == furnace_results_.end()) {
        return ItemInstance();
    }
    const auto &results = outer->second;

    if (const auto exact = results.find(item.getIdAux()); exact != results.end()) {
        return exact->second;
    }

    const auto any_id_aux =
        (static_cast<int>(static_cast<std::uint16_t>(item.getItem()->getId())) << 16) | ItemDescriptor::ANY_AUX_VALUE;
    if (const auto any = results.find(any_id_aux); any != results.end()) {
        return any->second;
    }

    return ItemInstance();
}
