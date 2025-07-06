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

ItemInstance Recipes::getFurnaceRecipeResult(const ItemStackBase &item, const HashedString &tag) const
{
    if (!item || furnace_recipes_.empty()) {
        return ItemInstance();
    }

    for (const auto &[key, value] : furnace_recipes_) {
        auto id = key.id >> 16;
        auto aux = static_cast<std::uint16_t>(key.id);
        if (id == item.getId() && key.tag == tag &&
            (aux == ItemDescriptor::ANY_AUX_VALUE || aux == item.getAuxValue())) {
            return value;
        }
    }

    return ItemInstance();
}
