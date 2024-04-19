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

#include "bedrock/server/level/server_level.h"

#include "bedrock/world/actor/actor_initialization_method.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/server.h"
#include "endstone/event/actor/actor_spawn_event.h"

using endstone::detail::EndstoneServer;

void ServerLevel::_postReloadActorAdded(Actor &actor, ActorInitializationMethod method)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerLevel::_postReloadActorAdded, this, actor, method);

    if (actor.isPlayer()) {
        return;
    }

    auto &server = entt::locator<EndstoneServer>::value();
    endstone::ActorSpawnEvent e{actor.getEndstoneActor()};
    server.getPluginManager().callEvent(e);

    if (e.isCancelled()) {
        actor.despawn();
    }
}
