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

#include "endstone/runtime/bedrock_hooks/armor_stand.h"

#include <cstddef>
#include <optional>

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/armor_slot.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/inventory/transaction/item_use_on_actor_inventory_transaction.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/item_stack.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_armor_stand_manipulate_event.h"
#include "endstone/runtime/bedrock_hooks/actor_interaction.h"

namespace {
struct ArmorStandSlot {
    std::optional<ArmorSlot> minecraft_slot;
    endstone::EquipmentSlot endstone_slot;
};

struct HumanoidArmorItemView {
    std::byte item[sizeof(::Item)];
    ArmorSlot slot;
};

bool isArmorStand(const ::Actor &actor)
{
    return actor.getActorIdentifier().getCanonicalName() == "minecraft:armor_stand";
}

ArmorStandSlot getHeldItemSlot(const ::ItemStack &item)
{
    if (item.isNull() || !item.getItem()) {
        return {std::nullopt, endstone::EquipmentSlot::Hand};
    }

    // Ordinary items such as bows and arrows use the main hand.
    if (!item.getItem()->isHumanoidArmor()) {
        return {std::nullopt, endstone::EquipmentSlot::Hand};
    }

    const auto slot = reinterpret_cast<const HumanoidArmorItemView *>(item.getItem())->slot;
    switch (slot) {
    case ArmorSlot::Feet:
        return {ArmorSlot::Feet, endstone::EquipmentSlot::Feet};
    case ArmorSlot::Legs:
        return {ArmorSlot::Legs, endstone::EquipmentSlot::Legs};
    case ArmorSlot::Torso:
        return {ArmorSlot::Torso, endstone::EquipmentSlot::Chest};
    case ArmorSlot::Head:
        return {ArmorSlot::Head, endstone::EquipmentSlot::Head};
    default:
        return {std::nullopt, endstone::EquipmentSlot::Hand};
    }
}

std::optional<ArmorStandSlot> getArmorStandSlot(const ::Actor &armor_stand, const Vec3 &location,
                                                const ::ItemStack &player_item)
{
    // ItemUseOnActorInventoryTransaction::hit_pos_ is a world position.
    const auto y = location.y - armor_stand.getPosition().y;

    // Match ArmorStand::getInteraction's vertical hit ranges before selecting the
    // equipment slot from the held item.
    std::optional<ArmorStandSlot> slot;
    if (y >= 0.1F && y < 0.55F) {
        slot = ArmorStandSlot{ArmorSlot::Feet, endstone::EquipmentSlot::Feet};
    }
    else if (y >= 0.9F && y < 1.6F) {
        if (!armor_stand.getCarriedItem().isNull()) {
            slot = ArmorStandSlot{std::nullopt, endstone::EquipmentSlot::Hand};
        }
        else {
            slot = ArmorStandSlot{ArmorSlot::Torso, endstone::EquipmentSlot::Chest};
        }
    }
    else if (y >= 0.4F && y < 1.2F) {
        slot = ArmorStandSlot{ArmorSlot::Legs, endstone::EquipmentSlot::Legs};
    }
    else if (y >= 1.6F) {
        slot = ArmorStandSlot{ArmorSlot::Head, endstone::EquipmentSlot::Head};
    }

    if (slot && !player_item.isNull()) {
        return getHeldItemSlot(player_item);
    }
    return slot;
}

const ::ItemStack &getArmorStandItem(const ::Actor &armor_stand, const ArmorStandSlot &slot)
{
    if (slot.endstone_slot == endstone::EquipmentSlot::Hand) {
        return armor_stand.getCarriedItem();
    }
    return armor_stand.getArmor(*slot.minecraft_slot);
}

}  // namespace

namespace endstone::runtime {

bool fireArmorStandManipulateEvent(const ::Player &player, const ::Actor &target, const ::ItemStack &item)
{
    const auto *interaction = getActorInteractionContext();
    if (!interaction || !isArmorStand(target) || interaction->player != &player || interaction->target != &target) {
        return true;
    }

    const auto location = interaction->transaction->hit_pos_;
    const auto slot = getArmorStandSlot(target, location, item);
    if (!slot) {
        return true;
    }
    const auto &armor_stand_item = getArmorStandItem(target, *slot);

    const auto &server = endstone::core::EndstoneServer::getInstance();
    endstone::PlayerArmorStandManipulateEvent event{
        player.getEndstoneActor<endstone::core::EndstonePlayer>(),
        target.getEndstoneActor(),
        endstone::core::EndstoneItemStack::fromMinecraft(armor_stand_item),
        endstone::core::EndstoneItemStack::fromMinecraft(item),
        getInteractionHand(player, item),
        slot->endstone_slot,
    };
    server.getPluginManager().callEvent(event);
    return !event.isCancelled();
}

}  // namespace endstone::runtime
