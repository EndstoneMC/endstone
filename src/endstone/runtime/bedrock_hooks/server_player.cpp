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

#include <utility>

#include "bedrock/locale/i18n.h"
#include "endstone/color_format.h"
#include "endstone/core/message.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_join_event.h"
#include "endstone/event/player/player_quit_event.h"
#include "endstone/lang/translatable.h"
#include "endstone/runtime/hook.h"

using endstone::core::EndstonePlayer;
using endstone::core::EndstoneServer;

void ServerPlayer::disconnect()
{
    auto &server = entt::locator<EndstoneServer>::value();
    auto &endstone_player = getEndstoneActor<EndstonePlayer>();
    endstone_player.disconnect();

    endstone::Translatable tr{endstone::ColorFormat::Yellow + "%multiplayer.player.left", {endstone_player.getName()}};
    const std::string quit_message = endstone::core::EndstoneMessage::toString(tr);

    endstone::PlayerQuitEvent e{endstone_player, quit_message};
    server.getPluginManager().callEvent(e);

    if (e.getQuitMessage() != quit_message) {
        tr = endstone::Translatable{e.getQuitMessage(), {}};
    }

    if (!e.getQuitMessage().empty()) {
        for (const auto &player : server.getOnlinePlayers()) {
            player->sendMessage(tr);
        }
    }

    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerPlayer::disconnect, this);
}
