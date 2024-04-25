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

#include "bedrock/network/network_identifier.h"

#include <string>

std::string NetworkIdentifier::getAddress() const
{
    char str[INET6_ADDRSTRLEN] = {0};
    if (address.addr4.sin_family == AF_INET) {
        if (!inet_ntop(AF_INET, &(address.addr4.sin_addr), str, INET6_ADDRSTRLEN)) {
            return "0.0.0.0";
        }
    }
    else {
        if (!inet_ntop(AF_INET6, &(address.addr6.sin6_addr), str, INET6_ADDRSTRLEN)) {
            return "::";
        }
    }
    return str;
}

std::uint16_t NetworkIdentifier::getPort() const
{
    return ntohs(address.addr4.sin_port);
}
