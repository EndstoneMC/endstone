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

#include "bedrock/server/network/server_network_handler.h"

#include <entt/entt.hpp>

#include "endstone/detail/hook.h"
#include "endstone/detail/server.h"
#include "endstone/event/player/player_chat_event.h"

using endstone::detail::EndstoneServer;

bool ServerNetworkHandler::_loadNewPlayer(ServerPlayer &server_player, bool flag)
{
    auto &server = entt::locator<EndstoneServer>::value();
    auto &endstone_player = server_player.getEndstonePlayer();
    // TODO(event): call PlayerLoginEvent
    return ENDSTONE_HOOK_CALL_ORIGINAL(&ServerNetworkHandler::_loadNewPlayer, this, server_player, flag);
}

void ServerNetworkHandler::_displayGameMessage(const Player &player, ChatEvent &event)
{
    auto &server = entt::locator<EndstoneServer>::value();
    endstone::PlayerChatEvent e{player.getEndstonePlayer(), event.message};
    server.getPluginManager().callEvent(e);

    if (e.isCancelled()) {
        return;
    }

    event.message = std::move(e.getMessage());
    server.getLogger().info("<{}> {}", e.getPlayer().getName(), e.getMessage());

    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerNetworkHandler::_displayGameMessage, this, player, event);
}
