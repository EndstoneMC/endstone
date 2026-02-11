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

#include "bedrock/network/disconnection_request_info.h"
#include "bedrock/network/network_identifier.h"
#include "bedrock/world/actor/player/player.h"

enum class IncomingPacketFilterResult : int {
    Allowed = 0,
    RejectedSilently = 1,
    RejectedWithDisconnect = 2,
};

enum class OutgoingPacketFilterResult : int {
    Allowed = 0,
    Reject = 1,
};

class NetEventCallback : public Bedrock::EnableNonOwnerReferences {
public:
    virtual void onPlayerReady(Player &) = 0;
    ~NetEventCallback() override = default;
    virtual void onConnect(const NetworkIdentifier &) = 0;
    virtual void onUnableToConnect(Connection::DisconnectFailReason, const std::string &, const std::string &) = 0;
    virtual void onTick() = 0;
    virtual void onOutgoingPacket(const NetworkIdentifier &, MinecraftPacketIds, SubClientId, SubClientId) = 0;
    virtual void onValidPacketReceived(const NetworkIdentifier &, MinecraftPacketIds, SubClientId, SubClientId) = 0;
    virtual void onStoreOfferReceive(ShowStoreOfferRedirectType, const std::string &) = 0;
    virtual void onDisconnect(const NetworkIdentifier &, const Connection::DisconnectFailReason,
                              const Connection::DisconnectionStage, const std::string &message_from_server,
                              const std::string &message_body_override, bool skip_message,
                              const std::string &telemetryOverride) = 0;
    virtual IncomingPacketFilterResult allowIncomingPacketId(const NetworkIdentifierWithSubId &, MinecraftPacketIds,
                                                             size_t) = 0;
    virtual OutgoingPacketFilterResult allowOutgoingPacket(const std::vector<NetworkIdentifierWithSubId> &,
                                                           const Packet &) = 0;
    // ...
};
