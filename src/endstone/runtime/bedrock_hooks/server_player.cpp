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

#include "bedrock/server/server_player.h"

#include "endstone/color_format.h"
#include "endstone/core/message.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_join_event.h"

void ServerPlayer::setLocalPlayerAsInitialized()
{
    local_player_initialized_ = true;

    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    auto &endstone_player = getEndstoneActor<endstone::core::EndstonePlayer>();

    endstone::Translatable tr{endstone::ColorFormat::Yellow + "%multiplayer.player.joined",
                              {endstone_player.getName()}};
    const std::string join_message = endstone::core::EndstoneMessage::toString(tr);

    endstone::PlayerJoinEvent e{endstone_player, join_message};
    server.getPluginManager().callEvent(e);
    if (e.getJoinMessage() != join_message) {
        tr = endstone::Translatable{e.getJoinMessage(), {}};
    }

    if (!e.getJoinMessage().empty()) {
        for (const auto &online_player : server.getOnlinePlayers()) {
            online_player->sendMessage(tr);
        }
    }
    endstone_player.recalculatePermissions();
    endstone_player.updateCommands();
}
