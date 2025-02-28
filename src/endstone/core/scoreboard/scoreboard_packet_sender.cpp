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

#include "endstone/core/scoreboard/scoreboard_packet_sender.h"

#include "endstone/core/server.h"
#include "endstone/core/util/uuid.h"

namespace endstone::core {

ScoreboardPacketSender::ScoreboardPacketSender(EndstoneServer &server, EndstoneScoreboard &scoreboard,
                                               PacketSender &sender)
    : server_(server), scoreboard_(scoreboard), sender_(sender)
{
}

bool ScoreboardPacketSender::isInitialized() const
{
    return sender_.isInitialized();
}

void ScoreboardPacketSender::send(::Packet &packet)
{
    throw std::runtime_error("Not supported!");
}

void ScoreboardPacketSender::sendTo(const NetworkIdentifier &, SubClientId, ::Packet &)
{
    throw std::runtime_error("Not supported!");
}

void ScoreboardPacketSender::sendToServer(::Packet &packet)
{
    throw std::runtime_error("Not supported!");
}

void ScoreboardPacketSender::sendToClient(const UserEntityIdentifierComponent *user_identifider, const ::Packet &packet)
{
    auto uuid = EndstoneUUID::fromMinecraft(user_identifider->client_uuid);
    auto *player = server_.getPlayer(uuid);
    if (!player) {
        return;
    }

    if (&player->getScoreboard() != &scoreboard_) {
        return;
    }

    sender_.sendToClient(user_identifider, packet);
}

void ScoreboardPacketSender::sendToClient(const NetworkIdentifier &network_identifier, const ::Packet &packet,
                                          SubClientId sub_id)
{
    for (const auto &item : server_.getOnlinePlayers()) {
        auto *player = static_cast<EndstonePlayer *>(item);

        if (&player->getScoreboard() != &scoreboard_) {
            continue;
        }

        auto user_identifier = player->getPlayer().getPersistentComponent<UserEntityIdentifierComponent>();
        if (user_identifier->network_id != network_identifier || user_identifier->client_sub_id != sub_id) {
            continue;
        }

        sender_.sendToClient(network_identifier, packet, sub_id);
        break;
    }
}

void ScoreboardPacketSender::sendToClients(
    const std::vector<NetworkIdentifierWithSubId> &network_identifier_with_sub_ids, const ::Packet &packet)
{
    throw std::runtime_error("Not supported!");
}

void ScoreboardPacketSender::sendBroadcast(const ::Packet &packet)
{
    for (const auto &item : server_.getOnlinePlayers()) {
        auto *player = static_cast<EndstonePlayer *>(item);

        if (&player->getScoreboard() != &scoreboard_) {
            continue;
        }

        auto user_identifier = player->getPlayer().getPersistentComponent<UserEntityIdentifierComponent>();
        sender_.sendToClient(user_identifier, packet);
    }
}

void ScoreboardPacketSender::sendBroadcast(const NetworkIdentifier &, SubClientId, const ::Packet &)
{
    throw std::runtime_error("Not supported!");
}

void ScoreboardPacketSender::flush(const NetworkIdentifier &network_identifier, std::function<void()> &&callback)
{
    sender_.flush(network_identifier, std::forward<decltype(callback)>(callback));
}

}  // namespace endstone::core
