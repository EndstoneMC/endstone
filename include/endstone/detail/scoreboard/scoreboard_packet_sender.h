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

#pragma once

#include "bedrock/network/packet_sender.h"

namespace endstone::detail {

class EndstoneScoreboard;
class EndstoneServer;

class ScoreboardPacketSender : public PacketSender {
public:
    ScoreboardPacketSender(EndstoneServer &server, EndstoneScoreboard &scoreboard, PacketSender &sender);
    ~ScoreboardPacketSender() override = default;
    void send(::Packet &) override;
    void sendToServer(::Packet &) override;
    void sendToClient(const UserEntityIdentifierComponent *, const ::Packet &) override;
    void sendToClient(const NetworkIdentifier &, const ::Packet &, SubClientId) override;
    void sendToClients(const std::vector<NetworkIdentifierWithSubId> &, const ::Packet &) override;
    void sendBroadcast(const ::Packet &) override;
    void sendBroadcast(const NetworkIdentifier &, SubClientId, const ::Packet &) override;
    void flush(const NetworkIdentifier &, std::function<void()> &&) override;

private:
    EndstoneServer &server_;
    EndstoneScoreboard &scoreboard_;
    PacketSender &sender_;
};

}  // namespace endstone::detail
