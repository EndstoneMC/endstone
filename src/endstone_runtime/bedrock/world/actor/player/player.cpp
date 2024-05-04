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

#include "bedrock/network/protocol/game/available_commands_packet.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/server.h"

using endstone::detail::EndstoneServer;

void Player::setPermissions(CommandPermissionLevel level)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&Player::setPermissions, this, level);
    auto &player = getEndstonePlayer();
    player.recalculatePermissions();
    player.updateCommands();
}

const std::string &Player::getName() const
{
    return ENDSTONE_HOOK_CALL_ORIGINAL(&Player::getName, this);
}

endstone::detail::EndstonePlayer &Player::getEndstonePlayer() const
{
    return static_cast<endstone::detail::EndstonePlayer &>(Actor::getEndstoneActor());
}
