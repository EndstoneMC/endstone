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
#include "bedrock/network/packet.h"
#include "bedrock/network/packet/death_info_packet.h"
#include "bedrock/world/level/level.h"
#include "endstone/color_format.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/message.h"
#include "endstone/detail/server.h"
#include "endstone/event/player/player_death_event.h"
#include "endstone/event/player/player_join_event.h"
#include "endstone/event/player/player_quit_event.h"
#include "endstone/translatable.h"

using endstone::detail::EndstonePlayer;
using endstone::detail::EndstoneServer;

void ServerPlayer::die(const ActorDamageSource &source)
{
    // Note: reset the packet sender in PlayerDeathManager to prevent BDS
    // from sending the death message as we will take over it
    auto *player_death_manager = getLevel()._getPlayerDeathManager();
    player_death_manager->resetPacketSender();

    ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&ServerPlayer::die, __FUNCDNAME__, this, source);

    auto &server = entt::locator<EndstoneServer>::value();
    auto &endstone_player = *getEndstoneActor<EndstonePlayer>();
    endstone_player.closeForm();

    // Do a server side translation for logging
    auto death_cause_message = source.getDeathMessage(getName(), this);
    auto death_message = getI18n().get(death_cause_message.first, death_cause_message.second, nullptr);

    // Fire player death event
    const auto e = std::make_unique<endstone::PlayerDeathEvent>(endstone_player, death_message);
    server.getPluginManager().callEvent(*static_cast<endstone::PlayerEvent *>(e.get()));
    if (e->getDeathMessage() != death_message) {
        death_cause_message.first = e->getDeathMessage();
        death_cause_message.second.clear();
    }

    // Send death info
    const auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::DeathInfo);
    const auto pk = std::static_pointer_cast<DeathInfoPacket>(packet);
    pk->death_cause_message = death_cause_message;
    sendNetworkPacket(*packet);

    // Log death message to console if not empty
    if (e->getDeathMessage().empty()) {
        return;
    }
    server.getLogger().info(e->getDeathMessage());

    // Broadcast death messages
    if (player_death_manager->getPlayerDeathManagerProxy()->shouldShowDeathMessages()) {
        endstone::Translatable tr{death_cause_message.first, death_cause_message.second};
        for (const auto &player : server.getOnlinePlayers()) {
            player->sendMessage(tr);
        }
    }
}

void ServerPlayer::setLocalPlayerAsInitialized()
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerPlayer::setLocalPlayerAsInitialized, this);

    auto &server = entt::locator<EndstoneServer>::value();
    auto &endstone_player = *getEndstoneActor<EndstonePlayer>();

    endstone::Translatable tr{endstone::ColorFormat::Yellow + "%multiplayer.player.joined",
                              {endstone_player.getName()}};
    const std::string join_message = endstone::detail::EndstoneMessage::toString(tr);

    endstone::PlayerJoinEvent e{endstone_player, join_message};
    server.getPluginManager().callEvent(e);

    if (e.getJoinMessage() != join_message) {
        tr = endstone::Translatable{e.getJoinMessage(), {}};
    }

    if (!e.getJoinMessage().empty()) {
        for (const auto &player : server.getOnlinePlayers()) {
            player->sendMessage(tr);
        }
    }

    endstone_player.recalculatePermissions();
    endstone_player.updateCommands();
}

void ServerPlayer::disconnect()
{
    auto &server = entt::locator<EndstoneServer>::value();
    auto &endstone_player = *getEndstoneActor<EndstonePlayer>();
    endstone_player.disconnect();

    endstone::Translatable tr{endstone::ColorFormat::Yellow + "%multiplayer.player.left", {endstone_player.getName()}};
    const std::string quit_message = endstone::detail::EndstoneMessage::toString(tr);

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
