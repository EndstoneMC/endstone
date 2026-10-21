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

#include "bedrock/entity/components/interact_component.h"

#include "bedrock/entity/weak_entity_ref.h"
#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/actor_interaction.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/item/item_stack.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_shear_actor_event.h"
#include "endstone/runtime/hook.h"

Interaction InteractComponent::getInteraction(Actor &owner, Player &player)
{
    auto result = ENDSTONE_HOOK_CALL_ORIGINAL(&InteractComponent::getInteraction, this, owner, player);
    if (!result.isSuccessful()) {
        return result;
    }

    const auto interact_text = result.getInteractText();
    if (interact_text != "action.interact.shear" && interact_text != "action.interact.mooshear" &&
        interact_text != "action.interact.take_sulfur_cube") {
        return result;
    }

    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (!server.getEndstonePluginManager().isEventRegistered<endstone::PlayerShearActorEvent>()) {
        return result;
    }

    result.wrapCallback([owner_ref = WeakEntityRef(owner.getWeakEntity()),
                         player_ref = WeakEntityRef(player.getWeakEntity())](Interaction::ExecutionCallback &callback) {
        auto *owner = owner_ref.tryUnwrap<::Actor>();
        auto *player = player_ref.tryUnwrap<::Player>();
        if (owner != nullptr && player != nullptr) {
            const auto &server = endstone::core::EndstoneServer::getInstance();
            const auto event = std::make_shared<endstone::PlayerShearActorEvent>(
                player->getEndstoneActor<endstone::core::EndstonePlayer>(), owner->getEndstoneActor(),
                endstone::core::EndstoneItemStack::fromMinecraft(player->getCarriedItem()),
                endstone::EquipmentSlot::Hand);
            server.getPluginManager().callEvent(event);
            if (event->isCancelled()) {
                return;
            }
        }
        callback();
    });
    return result;
}
