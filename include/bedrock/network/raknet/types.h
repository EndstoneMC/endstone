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

#include <cstddef>
#include <cstdint>

#include "bedrock/bedrock.h"
#include "bedrock/network/raknet/socket_includes.h"

/**
 * https://github.com/facebookarchive/RakNet/blob/master/Source/RakNetTypes.h
 */

namespace RakNet {

using SystemIndex = std::uint16_t;

struct SystemAddress {
    union  // In6OrIn4
    {
        sockaddr_storage sa_stor;
        sockaddr_in6 addr6;
        sockaddr_in addr4;
    } address;
    std::uint16_t debug_port;

    [[nodiscard]] std::uint16_t GetPort() const;                                              // NOLINT
    BEDROCK_API void ToString(bool write_port, char *dest, char port_delimiter = '|') const;  // NOLINT
};
static_assert(sizeof(SystemAddress) == 136);

struct RakNetGUID {
    std::uint64_t g;
    SystemIndex system_index;
};
static_assert(sizeof(RakNetGUID) == 16);

}  // namespace RakNet
