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

#include "bedrock/bedrock.h"
#include "bedrock/network/raknet/rak_peer_interface.h"
#include "bedrock/network/raknet/types.h"

struct ConnectionDefinition {
    std::uint16_t port;                      // +0
    std::uint16_t port_v6;                   // +2
    std::uint32_t unk1;                      // +4
    std::uint32_t max_incoming_connections;  // +8
    std::uint32_t max_connections;           // +12
    bool unk3;                               // +16
};

class RakPeerHelper {
public:
    enum class PeerPurpose;

    BEDROCK_API RakNet::StartupResult peerStartup(RakNet::RakPeerInterface *, ConnectionDefinition const &,
                                                  RakPeerHelper::PeerPurpose);
};
