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

#include "endstone/runtime/bedrock_hooks/actor_interaction.h"

#include <cstddef>
#include <optional>
#include <utility>

#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/inventory/transaction/item_use_on_actor_inventory_transaction.h"
#include "bedrock/world/item/item_stack.h"
#include "endstone/runtime/hook.h"
#include "endstone/runtime/bedrock_hooks/shear.h"

namespace {
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

thread_local std::optional<endstone::runtime::ActorInteractionContext> actor_interaction_context;
}  // namespace

void ItemUseOnActorInventoryTransaction::executeInteraction(void *context)
{
    const auto *callback = static_cast<const InteractionCallbackContext *>(context);
    auto previous_context = std::move(actor_interaction_context);
    actor_interaction_context.reset();

    if (callback->transaction && callback->player && callback->target &&
        callback->transaction->action_type_ == ActionType::Interact) {
        actor_interaction_context = endstone::runtime::ActorInteractionContext{
            callback->transaction,
            callback->player,
            callback->target,
        };
        endstone::runtime::installShearHooks(*callback->target, *callback->player);
    }

    ENDSTONE_HOOK_CALL_ORIGINAL(&ItemUseOnActorInventoryTransaction::executeInteraction, context);
    actor_interaction_context = std::move(previous_context);
}

namespace endstone::runtime {

const ActorInteractionContext *getActorInteractionContext()
{
    return actor_interaction_context ? &*actor_interaction_context : nullptr;
}

EquipmentSlot getInteractionHand(const ::Player &player, const ::ItemStack &item)
{
    const auto matches_main_hand = item == player.getCarriedItem();
    const auto matches_off_hand = item == player.getOffhandSlot();
    return matches_off_hand && !matches_main_hand ? EquipmentSlot::OffHand : EquipmentSlot::Hand;
}

}  // namespace endstone::runtime
