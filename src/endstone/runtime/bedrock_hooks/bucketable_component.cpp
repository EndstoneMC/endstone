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

#include "bedrock/entity/components/bucketable_component.h"

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/item/item_stack.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_bucket_actor_event.h"
#include "endstone/runtime/hook.h"

InteractionResult BucketableComponent::getInteraction(::Actor &owner, ::Player &player, ::ActorInteraction &interaction)
{
    const auto result =
        ENDSTONE_HOOK_CALL_ORIGINAL(&BucketableComponent::getInteraction, this, owner, player, interaction);
    if (!result.isSuccessful()) {
        return result;
    }

    const auto &server = endstone::core::EndstoneServer::getInstance();
    endstone::PlayerBucketActorEvent event{
        player.getEndstoneActor<endstone::core::EndstonePlayer>(),
        owner.getEndstoneActor(),
        endstone::core::EndstoneItemStack::fromMinecraft(player.getCarriedItem()),
        endstone::EquipmentSlot::Hand,
    };
    server.getPluginManager().callEvent(event);
    if (event.isCancelled()) {
        return InteractionResult::Failure();
    }
    return result;
}
