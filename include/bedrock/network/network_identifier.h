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

#include "bedrock/network/raknet/types.h"

class NetworkIdentifier {
public:
    enum class Type : std::uint32_t;

    std::uint32_t nether_net_id;     // +0
    RakNet::RakNetGUID raknet_guid;  // +8
    union {                          //
        sockaddr_storage sa_stor;    //
        sockaddr_in6 addr6;          //
        sockaddr_in addr4;           //
    } address;                       // +24
    Type type;                       // +152
};
static_assert(sizeof(NetworkIdentifier) == 160);
