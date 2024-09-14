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
    ClientOrServerNetworkSystemRef network_;                           // +0
    OwnerPtr<EntityContext> level_entity_;                             // +16
    Bedrock::NonOwnerPointer<Level> level_;                            // +48
    std::unique_ptr<ServerNetworkHandler> server_network_handler_;     // +64
    std::unique_ptr<NetEventCallback> legacy_client_network_handler_;  // +72
    std::unique_ptr<NetEventCallback> client_network_handler_;         // +80
    LoopbackPacketSender *loopback_packet_sender_;                     // +88
    SubClientId client_sub_id_;                                        // +96
};
