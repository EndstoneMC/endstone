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

#include "bedrock/entity/utilities/pushable_by_entity_utility.h"

#include "bedrock/world/actor/actor.h"
#include "endstone/core/plugin/plugin_manager.h"
#include "endstone/core/server.h"
#include "endstone/event/actor/actor_collide_with_actor_event.h"
#include "endstone/runtime/hook.h"

void PushableByEntityUtility::push(Actor &owner, Actor &other, bool push_self_only)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (server.getEndstonePluginManager().isEventRegistered<endstone::ActorCollideWithActorEvent>()) {
        endstone::ActorCollideWithActorEvent event{owner.getEndstoneActor<endstone::Actor>(),
                                                   other.getEndstoneActor<endstone::Actor>()};
        server.getPluginManager().callEvent(event);
        if (event.isCancelled()) {
            return;
        }
    }

    ENDSTONE_HOOK_CALL_ORIGINAL(&PushableByEntityUtility::push, owner, other, push_self_only);
}
