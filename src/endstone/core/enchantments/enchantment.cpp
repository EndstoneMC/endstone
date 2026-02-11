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

#include "endstone/core/enchantments/enchantment.h"

#include "bedrock/world/item/item.h"
#include "endstone/core/inventory/item_stack.h"

namespace endstone::core {
EndstoneEnchantment::EndstoneEnchantment(std::string id, const Enchant &handle) : id_(std::move(id)), handle_(handle) {}

EnchantmentId EndstoneEnchantment::getId() const
{
    return id_;
}

std::string EndstoneEnchantment::getTranslationKey() const
{
    return getHandle().getDescriptionId();
}

int EndstoneEnchantment::getMaxLevel() const
{
    return getHandle().getMaxLevel();
}

int EndstoneEnchantment::getStartLevel() const
{
    return getHandle().getMinLevel();
}

bool EndstoneEnchantment::conflictsWith(const Enchantment &other) const
{
    return !getHandle().isCompatibleWith(static_cast<const EndstoneEnchantment &>(other).getHandle().getEnchantType());
}

bool EndstoneEnchantment::canEnchantItem(const ItemStack &item) const
{
    const auto stack = EndstoneItemStack::toMinecraft(item);
    return getHandle().canEnchant(stack.getItem()->getEnchantSlot(), false);
}

const Enchant &EndstoneEnchantment::getHandle() const
{
    return handle_;
}

}  // namespace endstone::core
