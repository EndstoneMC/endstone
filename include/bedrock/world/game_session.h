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

#include "bedrock/entity/entity_context.h"
#include "bedrock/network/net_event_callback.h"
#include "bedrock/network/server_network_handler.h"

class ServerNetworkSystem;
class LoopbackPacketSender;

class GameSession {
public:
    [[nodiscard]] Bedrock::NonOwnerPointer<ServerNetworkHandler> getServerNetworkHandler() const
    {
        return Bedrock::NonOwnerPointer<ServerNetworkHandler>(*server_network_handler_);
    }

private:
    ServerNetworkSystem *network_system_;                           // +0
    int unknown1_;                                                  // +8
    OwnerStorageEntity entity_;                                     // +16
    char unknown2_[16];                                             // +48
    std::unique_ptr<ServerNetworkHandler> server_network_handler_;  // +64
    void *unknown3_;                                                // +72
    std::unique_ptr<NetEventCallback> net_event_callback_;          // +80
    LoopbackPacketSender *loopback_packet_sender_;                  // +88
    SubClientId sub_client_id_;                                     // +96
};
