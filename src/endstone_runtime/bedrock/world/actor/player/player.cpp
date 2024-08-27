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

#include "bedrock/world/actor/player/player.h"

#include <entt/entt.hpp>

#include "bedrock/entity/components/abilities_component.h"
#include "bedrock/entity/components/actor_game_type_component.h"
#include "bedrock/network/packet/available_commands_packet.h"
#include "bedrock/world/actor/actor_flags.h"
#include "bedrock/world/level/level.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/server.h"
#include "endstone/event/player/player_teleport_event.h"

using endstone::detail::EndstoneServer;

void Player::teleportTo(const Vec3 &pos, bool should_stop_riding, int cause, int entity_type, bool keep_velocity)
{
    Vec3 position = pos;
    auto &server = entt::locator<EndstoneServer>::value();
    auto &player = getEndstonePlayer();
    endstone::Location to{&player.getDimension(), pos.x, pos.y, pos.z, getRotation().x, getRotation().y};
    endstone::PlayerTeleportEvent e{player, player.getLocation(), to};
    server.getPluginManager().callEvent(e);

    if (e.isCancelled()) {
        return;
    }
    position = {e.getTo().getX(), e.getTo().getY(), e.getTo().getZ()};
    ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&Player::teleportTo, __FUNCDNAME__, this, position, should_stop_riding, cause,
                                     entity_type, keep_velocity);
}

Container &Player::getInventory()
{
    return ENDSTONE_HOOK_CALL_ORIGINAL(&Player::getInventory, this);
}

const std::string &Player::getName() const
{
    return ENDSTONE_HOOK_CALL_ORIGINAL(&Player::getName, this);
}

void Player::setPermissions(CommandPermissionLevel level)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&Player::setPermissions, this, level);
    auto &player = getEndstonePlayer();
    player.recalculatePermissions();
    player.updateCommands();
}

endstone::detail::EndstonePlayer &Player::getEndstonePlayer() const
{
    return static_cast<endstone::detail::EndstonePlayer &>(getEndstoneActor());
}

GameType Player::getPlayerGameType() const
{
    auto game_type = GameType::Undefined;
    if (auto *component = tryGetComponent<ActorGameTypeComponent>(); component) {
        game_type = component->game_type;
    }
    if (game_type == GameType::Default) {
        game_type = getLevel().getDefaultGameType();
    }
    return game_type;
}

LayeredAbilities &Player::getAbilities()
{
    auto component = getPersistentComponent<AbilitiesComponent>();
    return component->abilities;
}

LayeredAbilities const &Player::getAbilities() const
{
    auto component = getPersistentComponent<AbilitiesComponent>();
    return component->abilities;
}

bool Player::isEmoting() const
{
    return getStatusFlag(ActorFlags::EMOTING);
}

bool Player::isFlying() const
{
    return getAbilities().getBool(AbilitiesIndex::Flying);
}

int Player::getPlayerLevel() const
{
    return static_cast<int>(getAttribute("minecraft:player.level").getCurrentValue());
}

float Player::getLevelProgress() const
{
    return getAttribute("minecraft:player.experience").getCurrentValue();
}

int Player::getXpNeededForLevelRange(int start, int end)
{
    auto xp = 0;
    auto current = start;
    while (current < end) {
        if (current > 30) {
            xp += (9 * current - 138);
        }
        else if (current > 15) {
            xp += (5 * current - 38);
        }
        else {
            xp += (2 * current + 7);
        }
        ++current;
    }
    return xp;
}
