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
#include "bedrock/entity/gamerefs_entity/gamerefs_entity.h"
#include "bedrock/gamerefs/owner_ptr.h"
#include "bedrock/network/client_or_server_network_system.h"
#include "bedrock/network/net_event_callback.h"
#include "bedrock/network/packet_sender.h"

class LoopbackPacketSender : public PacketSender {
public:
    LoopbackPacketSender(SubClientId, ClientNetworkSystem &);
    LoopbackPacketSender(SubClientId, ServerNetworkSystem &);
    void addLoopbackCallback(NetEventCallback &);
    void removeLoopbackCallback(NetEventCallback &);
    void setUserList(const std::vector<OwnerPtr<EntityContext>> *);
    [[nodiscard]] bool isInitialized() const override = 0;
    void send(Packet &) override = 0;
    void sendToServer(Packet &) override = 0;
    void sendToClient(const NetworkIdentifier &, const Packet &, SubClientId) override = 0;
    void sendToClient(const UserEntityIdentifierComponent *, const Packet &) override = 0;
    void sendToClients(const std::vector<NetworkIdentifierWithSubId> &, const Packet &) override = 0;
    void sendBroadcast(const Packet &) override = 0;
    void sendBroadcast(const NetworkIdentifier &, SubClientId, const Packet &) override = 0;
    void flush(const NetworkIdentifier &, std::function<void()> &&) override = 0;

private:
    ClientOrServerNetworkSystemRef network_;
    std::vector<NetEventCallback *> loopback_callbacks_;
    const std::vector<OwnerPtr<EntityContext>> *user_list_;
    std::vector<NetworkIdentifierWithSubId> temp_user_ids_;
};
