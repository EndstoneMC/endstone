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

#include <algorithm>
#include <cstddef>
#include <cstdint>

#include "bedrock/bedrock.h"
#include "bedrock/deps/raknet/socket_includes.h"

/**
 * https://github.com/facebookarchive/RakNet/blob/master/Source/RakNetTypes.h
 */

namespace RakNet {

using Time = std::uint64_t;
using TimeMS = std::uint32_t;
using TimeUS = std::uint64_t;

enum class StartupResult {
    // NOLINTBEGIN
    RAKNET_STARTED,
    RAKNET_ALREADY_STARTED,
    INVALID_SOCKET_DESCRIPTORS,
    INVALID_MAX_CONNECTIONS,
    SOCKET_FAMILY_NOT_SUPPORTED,
    SOCKET_PORT_ALREADY_IN_USE,
    SOCKET_FAILED_TO_BIND,
    SOCKET_FAILED_TEST_SEND,
    PORT_CANNOT_BE_ZERO,
    FAILED_TO_CREATE_NETWORK_THREAD,
    COULD_NOT_GENERATE_GUID,
    STARTUP_OTHER_FAILURE
    // NOLINTEND
};

using SystemIndex = std::uint16_t;

struct SystemAddress {
    SystemAddress()
    {
        address.addr4.sin_family = AF_INET;
        // used for operator ==
        memset(&address, 0, sizeof(address));
        address.addr4.sin_family = AF_INET;
        system_index = -1;
        debug_port = 0;
    }
    [[nodiscard]] std::uint16_t GetPort() const;                                                // NOLINT
    ENDSTONE_HOOK void ToString(bool write_port, char *dest, char port_delimiter = '|') const;  // NOLINT

    union  // In6OrIn4
    {
        sockaddr_storage sa_stor;
        sockaddr_in6 addr6;
        sockaddr_in addr4;
    } address;
    std::uint16_t debug_port;
    SystemIndex system_index;
};
BEDROCK_STATIC_ASSERT_SIZE(SystemAddress, 136, 136);

struct RakNetGUID {
    RakNetGUID() : g(-1), system_index(-1) {}
    explicit RakNetGUID(uint64_t g) : g(g), system_index(-1) {}

    std::uint64_t g;
    SystemIndex system_index;
};
BEDROCK_STATIC_ASSERT_SIZE(RakNetGUID, 16, 16);

const RakNetGUID UNASSIGNED_RAKNET_GUID(static_cast<uint64_t>(-1));
const SystemAddress UNASSIGNED_SYSTEM_ADDRESS;

struct AddressOrGUID {
    AddressOrGUID() = default;
    AddressOrGUID(const AddressOrGUID &input)
    {
        raknet_guid = input.raknet_guid;
        system_address = input.system_address;
    }
    explicit AddressOrGUID(const SystemAddress &input)
    {
        raknet_guid = UNASSIGNED_RAKNET_GUID;
        system_address = input;
    }
    explicit AddressOrGUID(const RakNetGUID &input)
    {
        raknet_guid = input;
        system_address = UNASSIGNED_SYSTEM_ADDRESS;
    }

    AddressOrGUID &operator=(const AddressOrGUID &input) = default;
    AddressOrGUID &operator=(const SystemAddress &input)
    {
        raknet_guid = UNASSIGNED_RAKNET_GUID;
        system_address = input;
        return *this;
    }
    AddressOrGUID &operator=(const RakNetGUID &input)
    {
        raknet_guid = input;
        system_address = UNASSIGNED_SYSTEM_ADDRESS;
        return *this;
    }

    RakNetGUID raknet_guid;
    SystemAddress system_address;
};

}  // namespace RakNet
