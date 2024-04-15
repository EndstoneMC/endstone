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

#include "bedrock/actor/actor.h"

#include "bedrock/actor/components/actor_identifier.h"
#include "bedrock/actor/components/actor_owner.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/level.h"
#include "endstone/detail/server.h"
#include "endstone/event/actor/actor_remove_event.h"

using endstone::detail::EndstoneLevel;
using endstone::detail::EndstoneServer;

void Actor::remove()
{
    auto &server = entt::locator<EndstoneServer>::value();
    auto &bedrock_level = getLevel();
    auto *level = static_cast<EndstoneLevel *>(server.getLevel(bedrock_level.getLevelId()));
    if (!level) {
        throw std::runtime_error("Unable to find the level associated with the actor.");
    }

    if (auto *actor = level->getActor(getRuntimeID().id)) {
        endstone::ActorRemoveEvent e{*actor};
        server.getPluginManager().callEvent(e);
        level->removeActor(actor->getRuntimeId());
    }

#if _WIN32
    ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&Actor::remove, __FUNCDNAME__, this);
#else
    ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&Actor::remove, "_ZN5Actor6removeEv", this);
#endif
}

Level &Actor::getLevel() const
{
    return *level_;
}

ActorRuntimeID Actor::getRuntimeID() const
{
    return tryGetComponent<RuntimeIDComponent>()->id;
}

Actor *Actor::tryGetFromEntity(EntityContext const &ctx, bool include_removed)
{
    auto *component = ctx.tryGetComponent<ActorOwnerComponent>();
    if (!component) {
        return nullptr;
    }

    auto *actor = component->owner;
    // TODO(fixme): this should be !actor->isRemoved() instead of actor->isAlive() though they are equivalent for now
    if (actor->isAlive() || include_removed) {
        return actor;
    }
    return nullptr;
}
