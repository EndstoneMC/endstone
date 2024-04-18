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

#include "bedrock/world/level/actor/actor_manager.h"

#include "bedrock/world/actor/registry/entity_context.h"
#include "endstone/detail/actor/actor.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/level.h"
#include "endstone/detail/server.h"
#include "endstone/event/actor/actor_spawn_event.h"

using endstone::detail::EndstoneActor;
using endstone::detail::EndstoneLevel;
using endstone::detail::EndstoneServer;

Actor *ActorManager::addActorEntity(IAddActorEntityProxy &proxy, OwnerPtr<EntityContext> ctx)
{
    auto *actor = ENDSTONE_HOOK_CALL_ORIGINAL(&ActorManager::addActorEntity, this, proxy, ctx);
    if (!actor || actor->isPlayer()) {
        return actor;
    }

    auto &server = entt::locator<EndstoneServer>::value();
    auto &bedrock_level = actor->getLevel();
    // TODO(fixme): find level by level and dimension
    auto *level = static_cast<EndstoneLevel *>(server.getLevel(bedrock_level.getLevelId()));
    if (!level) {
        throw std::runtime_error("Unable to find the level associated with the actor.");
    }

    auto endstone_actor = std::make_unique<EndstoneActor>(server, *actor);
    endstone::ActorSpawnEvent e{*endstone_actor};
    server.getPluginManager().callEvent(e);

    if (e.isCancelled()) {
        actor->despawn();
    }
    else {
        level->addActor(std::move(endstone_actor));
    }

    return actor;
}
