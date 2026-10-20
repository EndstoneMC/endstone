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

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/armor_slot.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/container.h"
#include "bedrock/world/item/item_stack.h"

namespace endstone::runtime {

inline bool inventoryContains(::Player &player, const ::ItemStack &item)
{
    if (item.isNull()) {
        return false;
    }
    auto &inventory = player.getInventory();
    for (auto i = 0; i < inventory.getContainerSize(); ++i) {
        if (inventory.getItem(i).matchesItem(item)) {
            return true;
        }
    }
    if (player.getOffhandSlot().matchesItem(item)) {
        return true;
    }
    for (const auto slot : {ArmorSlot::Head, ArmorSlot::Torso, ArmorSlot::Legs, ArmorSlot::Feet}) {
        if (player.getArmor(slot).matchesItem(item)) {
            return true;
        }
    }
    return false;
}

}  // namespace endstone::runtime
