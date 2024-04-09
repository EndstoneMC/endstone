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

#include "bedrock/actor/server_player.h"

#include "bedrock/actor/components/user_entity_identifier.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/server.h"
#include "endstone/util/uuid.h"

using endstone::detail::EndstoneServer;

namespace {
endstone::UUID getPlayerUniqueId(ServerPlayer &player)
{
    auto *component = player.tryGetComponent<UserEntityIdentifierComponent>();
    return {component->uuid.msb, component->uuid.lsb};
}
}  // namespace

void ServerPlayer::doInitialSpawn()
{
    auto &server = entt::locator<EndstoneServer>::value();
    auto *player = server.getPlayer(getPlayerUniqueId(*this));
    // TODO(event): send PlayerJoinEvent
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerPlayer::doInitialSpawn, this);
    sendCommands();
}

void ServerPlayer::disconnect()
{
    auto &server = entt::locator<EndstoneServer>::value();
    auto *player = server.getPlayer(getPlayerUniqueId(*this));
    // TODO(event): send PlayerQuitEvent
    server.removePlayer(player->getUniqueId());
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerPlayer::disconnect, this);
}
