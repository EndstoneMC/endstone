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
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/registry/item_registry_manager.h"
#include "endstone/core/enchantments/enchantment.h"
#include "endstone/core/inventory/item_type.h"

namespace endstone::core {

template <>
std::vector<Identifier<Enchantment>> EndstoneRegistry<Enchantment, Enchant>::identifiers() const
{
    std::vector<Identifier<Enchantment>> keys;
    for (const auto &enchant : Enchant::getEnchants()) {
        keys.emplace_back(EnchantmentId::minecraft(enchant->getStringId().getString()));
    }
    return keys;
}

template <>
const Enchant *EndstoneRegistry<Enchantment, Enchant>::getMinecraft(Identifier<Enchantment> id) const
{
    return Enchant::getEnchantFromName(std::string(id.getKey()));
}

template <>
std::unique_ptr<Registry<Enchantment>> EndstoneRegistry<Enchantment, Enchant>::create()
{
    return std::make_unique<EndstoneRegistry>(
        [](const auto &key, const auto &handle) { return std::make_unique<EndstoneEnchantment>(key, handle); });
}

template <>
std::vector<Identifier<ItemType>> EndstoneRegistry<ItemType, ::Item>::identifiers() const
{
    std::vector<Identifier<ItemType>> keys;
    for (const auto &name : ItemRegistryManager::getItemRegistry().getNameToItemMap() | std::views::keys) {
        keys.emplace_back(name.getString());
    }
    return keys;
}

template <>
const ::Item *EndstoneRegistry<ItemType, ::Item>::getMinecraft(Identifier<ItemType> id) const
{
    const auto item = ItemRegistryManager::getItemRegistry().getItem(std::string(id));
    return item.get();
}

template <>
std::unique_ptr<Registry<ItemType>> EndstoneRegistry<ItemType, ::Item>::create()
{
    return std::make_unique<EndstoneRegistry>(
        [](auto _, const auto &handle) { return std::make_unique<EndstoneItemType>(handle); });
}

}  // namespace endstone::core
