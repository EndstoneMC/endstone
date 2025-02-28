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
#include "bedrock/deps/raknet/rak_peer_interface.h"
#include "bedrock/deps/raknet/raknet_types.h"
#include "bedrock/network/connection_definition.h"

class RakPeerHelper {
public:
    class IPSupportInterface {
    public:
        virtual ~IPSupportInterface() = default;
        [[nodiscard]] virtual bool useIPv4Only() const = 0;
        [[nodiscard]] virtual bool useIPv6Only() const = 0;
        [[nodiscard]] virtual std::uint16_t getDefaultGamePort() const = 0;
        [[nodiscard]] virtual std::uint16_t getDefaultGamePortv6() const = 0;
    };

    enum class PeerPurpose : int {
        Gameplay = 0,
        LanDiscovery = 1,
        Count = 2
    };

    ENDSTONE_HOOK RakNet::StartupResult peerStartup(RakNet::RakPeerInterface *, ConnectionDefinition const &,
                                                    PeerPurpose);
};
