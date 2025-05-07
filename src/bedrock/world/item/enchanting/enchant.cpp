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

#include "bedrock/world/item/enchanting/enchant.h"

#include "bedrock/symbol.h"

bool Enchant::canEnchant(int slot, bool allow_non_vanilla) const
{
    // If we’re allowing “non-vanilla” enchants, everything’s permitted
    if (allow_non_vanilla) {
        return true;
    }
    // Otherwise, vanilla rules only: item must declare at least one slot
    if (slot == NONE) {
        return false;
    }

    if (slot & primary_slots_ || slot & secondary_slots_) {
        return true;
    }
    return false;
}

const Enchant *Enchant::getEnchant(const Type &type)
{
    if (type > Type::NumEnchantments) {
        return nullptr;
    }
    return getEnchants().at(static_cast<int>(type)).get();
}

const Enchant *Enchant::getEnchantFromName(const HashedString &name)
{
    for (const auto &enchant : getEnchants()) {
        if (enchant->getStringId() == name) {
            return enchant.get();
        }
    }
    return nullptr;
}

std::vector<std::unique_ptr<Enchant>> Enchant::getEnchants()
{
    static auto &enchants = *BEDROCK_VAR(std::vector<std::unique_ptr<Enchant>> *, "Enchant::mEnchants");
    return enchants;
}
