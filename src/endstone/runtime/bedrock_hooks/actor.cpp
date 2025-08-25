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

#include "bedrock/world/actor/actor.h"

#include "bedrock/entity/components/actor_definition_identifier_component.h"
#include "bedrock/entity/utilities/rotation_utility.h"
#include "bedrock/world/actor/player/player.h"
#include "endstone/core/actor/actor.h"
#include "endstone/core/actor/mob.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/actor/actor_remove_event.h"
#include "endstone/event/actor/actor_teleport_event.h"
#include "endstone/runtime/hook.h"

using endstone::core::EndstoneActor;
using endstone::core::EndstoneActorComponent;
using endstone::core::EndstoneMob;
using endstone::core::EndstonePlayer;
using endstone::core::EndstoneServer;

void Actor::teleportTo(const Vec3 &pos, bool should_stop_riding, int cause, int entity_type, bool keep_velocity)
{
    Vec3 position = pos;
    if (!isPlayer()) {
        auto &server = entt::locator<EndstoneServer>::value();
        auto &actor = getEndstoneActor();
        endstone::Location to{pos.x, pos.y, pos.z, getRotation().x, getRotation().y, actor.getDimension()};
        endstone::ActorTeleportEvent e{actor, actor.getLocation(), to};
        server.getPluginManager().callEvent(e);

        if (e.isCancelled()) {
            return;
        }
        position = {e.getTo().getX(), e.getTo().getY(), e.getTo().getZ()};
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&Actor::teleportTo, this, position, should_stop_riding, cause, entity_type,
                                keep_velocity);
}
