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

#include <functional>

#include "bedrock/common_types.h"
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/entity/components/user_entity_identifier_component.h"
#include "bedrock/network/packet.h"

class PacketSender : public Bedrock::EnableNonOwnerReferences {
public:
    [[nodiscard]] virtual bool isInitialized() const = 0;
    virtual void send(Packet &) = 0;
    virtual void sendTo(const NetworkIdentifier &, SubClientId, Packet &) = 0;
    virtual void sendToServer(Packet &) = 0;
    virtual void sendToClient(UserEntityIdentifierComponent const *, Packet const &) = 0;
    virtual void sendToClient(NetworkIdentifier const &, Packet const &, SubClientId) = 0;
    virtual void sendToClients(std::vector<NetworkIdentifierWithSubId> const &, Packet const &) = 0;
    virtual void sendBroadcast(Packet const &) = 0;
    virtual void sendBroadcast(NetworkIdentifier const &, SubClientId, Packet const &) = 0;
    virtual void flush(NetworkIdentifier const &, std::function<void()> &&) = 0;

private:
    SubClientId sender_sub_id_{SubClientId::PrimaryClient};
};
