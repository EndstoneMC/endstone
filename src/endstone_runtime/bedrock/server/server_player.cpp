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
#include "bedrock/network/minecraft_packets.h"
#include "bedrock/network/packet/death_info_packet.h"
#include "bedrock/world/level/level.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/message.h"
#include "endstone/detail/server.h"
#include "endstone/event/player/player_death_event.h"
#include "endstone/event/player/player_join_event.h"
#include "endstone/event/player/player_quit_event.h"

using endstone::detail::EndstoneMessage;
using endstone::detail::EndstoneServer;

void ServerPlayer::die(const ActorDamageSource &source)
{
    // Note: reset the packet sender in PlayerDeathManager to prevent BDS
    // from sending the death message as we will take over it
    auto *player_death_manager = getLevel()._getPlayerDeathManager();
    player_death_manager->resetPacketSender();

    ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&ServerPlayer::die, __FUNCDNAME__, this, source);

    auto &server = entt::locator<EndstoneServer>::value();
    endstone::Player &endstone_player = getEndstonePlayer();
    endstone_player.closeForm();

    // Do a server side translation for logging
    auto death_cause_message = source.getDeathMessage(getName(), this);
    auto death_message = getI18n().get(death_cause_message.first, death_cause_message.second, nullptr);

    // Fire player death event
    const auto e = std::make_unique<endstone::PlayerDeathEvent>(endstone_player, death_message);
    server.getPluginManager().callEvent(*static_cast<endstone::PlayerEvent *>(e.get()));
    const auto event_death_message = EndstoneMessage::toTranslatable(e->getDeathMessage());
    death_cause_message.first = event_death_message.getTranslationKey();
    death_cause_message.second = event_death_message.getParameters();

    // Send death info
    const auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::DeathInfo);
    const auto pk = std::static_pointer_cast<DeathInfoPacket>(packet);
    pk->death_cause_message = death_cause_message;
    sendNetworkPacket(*packet);

    // Log death message to console if not empty
    if (death_cause_message.first.empty()) {
        return;
    }
    server.getLogger().info(EndstoneMessage::toString(e->getDeathMessage()));

    // Broadcast death messages
    if (!player_death_manager->getPlayerDeathManagerProxy()->shouldShowDeathMessages()) {
        return;
    }
    for (const auto &player : server.getOnlinePlayers()) {
        player->sendMessage(e->getDeathMessage());
    }
}

void ServerPlayer::setLocalPlayerAsInitialized()
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerPlayer::setLocalPlayerAsInitialized, this);
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
