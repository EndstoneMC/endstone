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

#include "bedrock/network/network_handler.h"

#include <entt/entt.hpp>

#include "bedrock/actor/components/user_entity_identifier.h"
#include "endstone/detail/actor/player.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/server.h"

bool ServerNetworkHandler::_loadNewPlayer(ServerPlayer &server_player, bool flag)
{
    using endstone::detail::EndstonePlayer;
    using endstone::detail::EndstoneServer;

    auto &server = entt::locator<EndstoneServer>::value();
    auto new_player = std::make_unique<EndstonePlayer>(server, server_player);
    server.addPlayer(std::move(new_player));
    auto *component = server_player.tryGetComponent<UserEntityIdentifierComponent>();
    endstone::UUID uuid = {component->uuid.msb, component->uuid.lsb};
    auto *player = server.getPlayer(uuid);
    // TODO(event): call PlayerLoginEvent
    return ENDSTONE_HOOK_CALL_ORIGINAL(&ServerNetworkHandler::_loadNewPlayer, this, server_player, flag);
}
