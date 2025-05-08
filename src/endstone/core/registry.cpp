// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include "endstone/core/registry.h"

#include "bedrock/world/item/enchanting/enchant.h"
#include "enchantments/enchantment.h"

namespace endstone::core {

template <>
const Enchant *MinecraftRegistry<Enchant>::get(const NamespacedKey &key) const
{
    return Enchant::getEnchantFromName(key.getKey());
}

template <>
std::vector<NamespacedKey> MinecraftRegistry<Enchant>::keys() const
{
    std::vector<NamespacedKey> keys;
    for (const auto &enchant : Enchant::getEnchants()) {
        if (auto key = NamespacedKey::fromString(enchant->getStringId().getString()); key.has_value()) {
            keys.emplace_back(key.value());
        }
    }
    return keys;
}

template <>
std::unique_ptr<Registry<Enchantment>> EndstoneRegistry<Enchantment, Enchant>::createRegistry()
{
    return std::make_unique<EndstoneRegistry>(
        [](auto key, const auto &handle) { return std::make_unique<EndstoneEnchantment>(key, handle); });
}

}  // namespace endstone::core
