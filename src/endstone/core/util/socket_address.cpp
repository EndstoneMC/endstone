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

#include "endstone/core/util/socket_address.h"

#include "endstone/core/server.h"

namespace endstone::core {
SocketAddress EndstoneSocketAddress::fromSystemAddress(const RakNet::SystemAddress &address)
{
    char buffer[INET6_ADDRSTRLEN + 5 + 1] = {};
    address.ToString(false, buffer);
    return {buffer, address.GetPort()};
}

SocketAddress EndstoneSocketAddress::fromNetworkIdentifier(const NetworkIdentifier &network_id)
{
    switch (network_id.getType()) {
    case NetworkIdentifier::Type::RakNet: {
        const auto *peer = EndstoneServer::getInstance().getRakNetConnector().getPeer();
        const auto addr = peer->GetSystemAddressFromGuid(network_id.guid);
        return fromSystemAddress(addr);
    }
    case NetworkIdentifier::Type::Address:
    case NetworkIdentifier::Type::Address6: {
        return {network_id.getAddress(), network_id.getPort()};
    }
    case NetworkIdentifier::Type::NetherNet:
    case NetworkIdentifier::Type::Invalid:
    default:
        return {};
    }
}
}  // namespace endstone::core
