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

#include "bedrock/deps/raknet/raknet_types.h"

#include <string>

namespace RakNet {

std::uint16_t SystemAddress::GetPort() const
{
    return ntohs(address.addr4.sin_port);
}

void SystemAddress::ToString(const bool write_port, char *dest, const char port_delimiter) const
{
    if (*this == UNASSIGNED_SYSTEM_ADDRESS) {
        constexpr auto unassigned = "UNASSIGNED_SYSTEM_ADDRESS";
        std::copy_n(unassigned, std::strlen(unassigned) + 1, dest);
        return;
    }

    char buffer[INET6_ADDRSTRLEN] = {};
    int ret = 0;
    if (address.addr4.sin_family == AF_INET) {
        ret = getnameinfo((sockaddr *)&address.addr4, sizeof(sockaddr_in), buffer, sizeof(buffer), nullptr, 0,
                          NI_NUMERICHOST);
    }
    else if (address.addr6.sin6_family == AF_INET6) {
        ret = getnameinfo((sockaddr *)&address.addr6, sizeof(sockaddr_in6), buffer, sizeof(buffer), nullptr, 0,
                          NI_NUMERICHOST);
    }

    if (ret != 0) {
        dest[0] = '\0';
        return;
    }

    std::string result = buffer;
    if (write_port) {
        result += port_delimiter;
        result += std::to_string(ntohs(address.addr4.sin_port));
    }

    std::ranges::copy(result, dest);
    dest[result.size()] = '\0';
}

bool SystemAddress::EqualsExcludingPort(const SystemAddress &right) const
{
    return (address.addr4.sin_family == AF_INET &&
            address.addr4.sin_addr.s_addr == right.address.addr4.sin_addr.s_addr) ||
           (address.addr4.sin_family == AF_INET6 &&
            memcmp(address.addr6.sin6_addr.s6_addr, right.address.addr6.sin6_addr.s6_addr,
                   sizeof(address.addr6.sin6_addr.s6_addr)) == 0);
}

bool SystemAddress::operator==(const SystemAddress &right) const
{
    return address.addr4.sin_port == right.address.addr4.sin_port && EqualsExcludingPort(right);
}

}  // namespace RakNet
