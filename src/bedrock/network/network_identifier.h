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

#include <cstdint>
#include <string>

#include "bedrock/bedrock.h"
#include "bedrock/common_types.h"
#include "bedrock/deps/nethernet/network_id.h"
#include "bedrock/deps/raknet/raknet_types.h"

class NetworkIdentifier {
public:
    enum class Type : std::uint32_t {
        RakNet = 0,
        Address = 1,
        Address6 = 2,
        NetherNet = 3,
        Invalid = 4,
    };

    NetherNet::NetworkID nether_net_id;  //
    RakNet::RakNetGUID guid;             //
    union {                              //
        sockaddr_storage sa_stor;        //
        sockaddr_in6 addr6;              //
        sockaddr_in addr4;               //
    } sock;                              //
    Type type;                           // +176

    [[nodiscard]] std::string getAddress() const;
    [[nodiscard]] std::uint16_t getPort() const;
    [[nodiscard]] Type getType() const;
    bool operator==(const NetworkIdentifier &other) const;
    bool operator!=(const NetworkIdentifier &other) const;
    [[nodiscard]] bool equalsTypeData(const NetworkIdentifier &other) const;
};
static_assert(sizeof(NetworkIdentifier) == 176);

struct NetworkIdentifierWithSubId {
    NetworkIdentifier id;
    SubClientId sub_client_id;
};
