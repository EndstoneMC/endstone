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

#include "bedrock/world/actor/armor_stand.h"

#include <optional>

#include "bedrock/world/actor/actor_interaction.h"
#include "bedrock/world/actor/armor_slot.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/gamemode/interaction_result.h"
#include "bedrock/world/item/humanoid_armor_item.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/item_stack.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_armor_stand_manipulate_event.h"
#include "endstone/runtime/hook.h"

InteractionResult ArmorStand::getInteraction(Player &player, ActorInteraction &interaction, Vec3 const &location)
{
    const auto result = ENDSTONE_HOOK_CALL_ORIGINAL(&ArmorStand::getInteraction, this, player, interaction, location);
    if (interaction.getInteractText() != "action.interact.armorstand.equip" || !interaction.shouldCapture()) {
        return result;
    }

    const auto height = location.y - getPosition().y;
    std::optional<ArmorSlot> armor_slot;
    if (height >= 0.1F && height < 0.55F) {
        armor_slot = ArmorSlot::Feet;
    }
    else if (height >= 0.9F && height < 1.6F) {
        armor_slot = ArmorSlot::Torso;
    }
    else if (height >= 0.4F && height < 1.2F) {
        armor_slot = ArmorSlot::Legs;
    }
    else if (height >= 1.6F) {
        armor_slot = ArmorSlot::Head;
    }

    const auto &player_item = player.getCarriedItem();
    if (!player_item.isNull()) {
        const auto *item = player_item.getItem();
        armor_slot = item != nullptr && item->isHumanoidArmor()
                       ? std::optional{static_cast<const HumanoidArmorItem *>(item)->slot_}
                       : std::nullopt;
    }

    auto slot = endstone::EquipmentSlot::Hand;
    switch (armor_slot.value_or(ArmorSlot::_count)) {
    case ArmorSlot::Head:
        slot = endstone::EquipmentSlot::Head;
        break;
    case ArmorSlot::Torso:
        slot = endstone::EquipmentSlot::Chest;
        break;
    case ArmorSlot::Legs:
        slot = endstone::EquipmentSlot::Legs;
        break;
    case ArmorSlot::Feet:
        slot = endstone::EquipmentSlot::Feet;
        break;
    case ArmorSlot::Body:
        slot = endstone::EquipmentSlot::Body;
        break;
    default:
        break;
    }

    const auto &armor_stand_item = armor_slot.has_value() ? getArmor(*armor_slot) : getCarriedItem();
    const auto &server = endstone::core::EndstoneServer::getInstance();
    endstone::PlayerArmorStandManipulateEvent event{
        player.getEndstoneActor<endstone::core::EndstonePlayer>(),
        getEndstoneActor(),
        endstone::core::EndstoneItemStack::fromMinecraft(armor_stand_item),
        endstone::core::EndstoneItemStack::fromMinecraft(player_item),
        slot,
    };
    server.getPluginManager().callEvent(event);
    if (event.isCancelled()) {
        interaction.suppressInteraction();
        return InteractionResult::Failure();
    }
    return result;
}
