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
#include <utility>

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
#include "endstone/runtime/hook.h"

namespace {
struct ArmorStandSlot {
    std::optional<ArmorSlot> minecraft_slot;
    endstone::EquipmentSlot endstone_slot;
};

struct ArmorStandInteractionContext {
    const ::Player *player;
    const ::Actor *target;
    ::ItemStack item;
    std::optional<Vec3> location;
    std::optional<ArmorStandSlot> slot;
    std::optional<::ItemStack> armor_stand_item;
};

struct InteractionCallbackContext {
    void *vtable;  // std::__function::__func vtable
    const ItemUseOnActorInventoryTransaction *transaction;  // +0x08
    ::Player *player;  // +0x10
    void *result;  // +0x18
    ::Actor *target;  // +0x20
};

static_assert(offsetof(InteractionCallbackContext, transaction) == 0x8);
static_assert(offsetof(InteractionCallbackContext, player) == 0x10);
static_assert(offsetof(InteractionCallbackContext, result) == 0x18);
static_assert(offsetof(InteractionCallbackContext, target) == 0x20);

struct HumanoidArmorItemView {
    std::byte item[sizeof(::Item)];
    ArmorSlot slot;
};

// Bedrock's HumanoidArmorItem stores its armor slot immediately after Item.
thread_local std::optional<ArmorStandInteractionContext> armor_stand_interaction_context;

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
                                                const ::ItemStack *player_item = nullptr)
{
    // ItemUseOnActorInventoryTransaction::hit_pos_ is a world position.
    const auto y = location.y - armor_stand.getPosition().y;

    // Match ArmorStand::getInteraction's vertical hit ranges before selecting the
    // equipment slot from the held item.
    if (y >= 0.1F && y < 0.55F) {
        if (player_item && !player_item->isNull()) {
            return getHeldItemSlot(*player_item);
        }
        return ArmorStandSlot{ArmorSlot::Feet, endstone::EquipmentSlot::Feet};
    }
    if (y >= 0.9F && y < 1.6F) {
        if (player_item && !player_item->isNull()) {
            return getHeldItemSlot(*player_item);
        }
        if (!armor_stand.getCarriedItem().isNull()) {
            return ArmorStandSlot{std::nullopt, endstone::EquipmentSlot::Hand};
        }
        return ArmorStandSlot{ArmorSlot::Torso, endstone::EquipmentSlot::Chest};
    }
    if (y >= 0.4F && y < 1.2F) {
        if (player_item && !player_item->isNull()) {
            return getHeldItemSlot(*player_item);
        }
        return ArmorStandSlot{ArmorSlot::Legs, endstone::EquipmentSlot::Legs};
    }
    if (y >= 1.6F) {
        if (player_item && !player_item->isNull()) {
            return getHeldItemSlot(*player_item);
        }
        return ArmorStandSlot{ArmorSlot::Head, endstone::EquipmentSlot::Head};
    }
    return std::nullopt;
}

const ::ItemStack &getArmorStandItem(const ::Actor &armor_stand, const ArmorStandSlot &slot)
{
    if (slot.endstone_slot == endstone::EquipmentSlot::Hand) {
        return armor_stand.getCarriedItem();
    }
    return armor_stand.getArmor(*slot.minecraft_slot);
}

endstone::EquipmentSlot getInteractionHand(const ::Player &player, const ::ItemStack &item)
{
    const auto matches_main_hand = item == player.getCarriedItem();
    const auto matches_off_hand = item == player.getOffhandSlot();
    return matches_off_hand && !matches_main_hand ? endstone::EquipmentSlot::OffHand
                                                  : endstone::EquipmentSlot::Hand;
}

}  // namespace

void ItemUseOnActorInventoryTransaction::executeInteraction(void *context)
{
    const auto *callback = static_cast<const InteractionCallbackContext *>(context);
    auto previous_context = std::move(armor_stand_interaction_context);
    armor_stand_interaction_context.reset();

    if (callback->transaction && callback->player && callback->target &&
        callback->transaction->action_type_ == ActionType::Interact && isArmorStand(*callback->target)) {
        const auto location = callback->transaction->hit_pos_;
        auto slot = getArmorStandSlot(*callback->target, location);
        std::optional<::ItemStack> armor_stand_item;
        if (slot) {
            armor_stand_item = getArmorStandItem(*callback->target, *slot);
        }
        armor_stand_interaction_context = ArmorStandInteractionContext{
            callback->player,
            callback->target,
            {},
            location,
            std::move(slot),
            std::move(armor_stand_item),
        };
    }

    ENDSTONE_HOOK_CALL_ORIGINAL(&ItemUseOnActorInventoryTransaction::executeInteraction, context);
    armor_stand_interaction_context = std::move(previous_context);
}

namespace endstone::runtime {

void prepareArmorStandInteraction(const ::Player *player, const ::Actor *target, const ::ItemStack &item)
{
    if (!player || !target || !isArmorStand(*target)) {
        armor_stand_interaction_context.reset();
        return;
    }

    if (armor_stand_interaction_context && armor_stand_interaction_context->player == player &&
        armor_stand_interaction_context->target == target) {
        armor_stand_interaction_context->item = item;
        armor_stand_interaction_context->slot =
            armor_stand_interaction_context->location
                ? getArmorStandSlot(*target, *armor_stand_interaction_context->location, &item)
                : std::nullopt;
        armor_stand_interaction_context->armor_stand_item =
            armor_stand_interaction_context->slot
                ? std::optional<::ItemStack>{getArmorStandItem(*target, *armor_stand_interaction_context->slot)}
                : std::nullopt;
        return;
    }

    armor_stand_interaction_context =
        ArmorStandInteractionContext{player, target, item, std::nullopt, std::nullopt, std::nullopt};
}

bool fireArmorStandManipulateEvent()
{
    if (!armor_stand_interaction_context || !armor_stand_interaction_context->location ||
        !armor_stand_interaction_context->slot || !armor_stand_interaction_context->armor_stand_item) {
        return true;
    }

    const auto &context = *armor_stand_interaction_context;
    const auto &slot = *context.slot;

    const auto &server = endstone::core::EndstoneServer::getInstance();
    endstone::PlayerArmorStandManipulateEvent event{
        context.player->getEndstoneActor<endstone::core::EndstonePlayer>(),
        context.target->getEndstoneActor(),
        endstone::core::EndstoneItemStack::fromMinecraft(*context.armor_stand_item),
        endstone::core::EndstoneItemStack::fromMinecraft(context.item),
        getInteractionHand(*context.player, context.item),
        slot.endstone_slot,
    };
    server.getPluginManager().callEvent(event);
    return !event.isCancelled();
}

}  // namespace endstone::runtime
