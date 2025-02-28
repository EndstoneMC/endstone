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

#include <variant>

#include "bedrock/forward.h"
#include "bedrock/network/server_network_system.h"

class ClientOrServerNetworkSystemRef
    : std::variant<std::reference_wrapper<ClientNetworkSystem>, std::reference_wrapper<ServerNetworkSystem>> {
public:
    using ClientRefT = std::reference_wrapper<ClientNetworkSystem>;
    using ServerRefT = std::reference_wrapper<ServerNetworkSystem>;

    ClientOrServerNetworkSystemRef(ClientNetworkSystem &network_system)
        : std::variant<ClientRefT, ServerRefT>(network_system)
    {
    }

    ClientOrServerNetworkSystemRef(ServerNetworkSystem &network_system)
        : std::variant<ClientRefT, ServerRefT>(network_system)
    {
    }

    ClientNetworkSystem &toClientNetworkSystem()
    {
        return std::get<ClientRefT>(*this);
    }

    [[nodiscard]] const ClientNetworkSystem &toClientNetworkSystem() const
    {
        return std::get<ClientRefT>(*this);
    }

    ServerNetworkSystem &toServerNetworkSystem()
    {
        return std::get<ServerRefT>(*this);
    }

    [[nodiscard]] const ServerNetworkSystem &toServerNetworkSystem() const
    {
        return std::get<ServerRefT>(*this);
    }
};
