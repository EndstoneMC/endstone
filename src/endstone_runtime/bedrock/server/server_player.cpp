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

#include "bedrock/locale/i18n.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/server.h"
#include "endstone/event/player/player_death_event.h"
#include "endstone/event/player/player_join_event.h"
#include "endstone/event/player/player_quit_event.h"

using endstone::detail::EndstoneServer;

void ServerPlayer::die(const ActorDamageSource &source)
{
    auto death_cause_message = source.getDeathMessage(getName(), this);
    auto death_message = getI18n().get(death_cause_message.first, death_cause_message.second, nullptr);

    auto &server = entt::locator<EndstoneServer>::value();
    endstone::Player &endstone_player = getEndstonePlayer();
    endstone_player.closeForm();

    auto e = std::make_unique<endstone::PlayerDeathEvent>(endstone_player, death_message);
    server.getPluginManager().callEvent(*static_cast<endstone::PlayerEvent *>(e.get()));

    if (!e->getDeathMessage().empty()) {
        server.getLogger().info(e->getDeathMessage());
        bool dirty = (e->getDeathMessage() != death_message);
        for (const auto &player : server.getOnlinePlayers()) {
            if (player == &endstone_player || !player->hasPermission(EndstoneServer::BroadcastChannelUser)) {
                continue;
            }
            if (dirty) {
                player->sendMessage(e->getDeathMessage());
            }
            else {
                player->sendMessage({death_cause_message.first, death_cause_message.second});
            }
        }
    }

    ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&ServerPlayer::die, __FUNCDNAME__, this, source);
}

void ServerPlayer::doInitialSpawn()
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerPlayer::doInitialSpawn, this);
    auto &server = entt::locator<EndstoneServer>::value();
    auto &endstone_player = getEndstonePlayer();
    endstone::PlayerJoinEvent e{endstone_player};
    server.getPluginManager().callEvent(e);
    endstone_player.recalculatePermissions();
    endstone_player.updateCommands();
}

void ServerPlayer::disconnect()
{
    auto &server = entt::locator<EndstoneServer>::value();
    auto &endstone_player = getEndstonePlayer();
    endstone_player.disconnect();
    endstone::PlayerQuitEvent e{endstone_player};
    server.getPluginManager().callEvent(e);
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerPlayer::disconnect, this);
}
