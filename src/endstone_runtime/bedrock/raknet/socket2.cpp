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

#include "bedrock/network/raknet/socket2.h"

#include <sstream>

#include <entt/entt.hpp>

#include "endstone/detail/hook.h"
#include "endstone/detail/server.h"
#include "endstone/event/server/server_list_ping_event.h"
#include "endstone/plugin/plugin_manager.h"

using endstone::detail::EndstoneServer;

namespace RakNet {

RNS2SendResult RNS2_Windows_Linux_360::Send_Windows_Linux_360NoVDP(RNS2Socket socket,
                                                                   RNS2_SendParameters *send_parameters,
                                                                   const char *file, unsigned int line)
{
    enum MessageIdentifiers : unsigned char {
        UnconnectedPong = 28
    };

    if (send_parameters->data[0] != MessageIdentifiers::UnconnectedPong) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&RNS2_Windows_Linux_360::Send_Windows_Linux_360NoVDP, socket,
                                           send_parameters, file, line);
    }

    constexpr static int head_size = sizeof(char) + sizeof(std::uint64_t) + sizeof(std::uint64_t) + 16;
    const char *data = send_parameters->data;
    std::size_t strlen = data[head_size] << 8 | data[head_size + 1];
    if (strlen == 0) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&RNS2_Windows_Linux_360::Send_Windows_Linux_360NoVDP, socket,
                                           send_parameters, file, line);
    }

    std::string ping_response{data + head_size + 2, strlen};
    auto &server = entt::locator<EndstoneServer>::value();
    char buffer[64];
    send_parameters->system_address.ToString(false, buffer);
    endstone::ServerListPingEvent event(std::string(buffer), send_parameters->system_address.GetPort(), ping_response);
    if (!event.deserialize()) {
        server.getLogger().error("Unable to parse ping response: {}", ping_response);
        return ENDSTONE_HOOK_CALL_ORIGINAL(&RNS2_Windows_Linux_360::Send_Windows_Linux_360NoVDP, socket,
                                           send_parameters, file, line);
    }

    server.getPluginManager().callEvent(event);

    std::vector<char> packet;
    packet.reserve(256);
    packet.insert(packet.end(), data, data + head_size);

    ping_response = event.serialize();
    strlen = ping_response.length();
    packet.push_back(static_cast<char>((strlen >> 8) & 0xFF));
    packet.push_back(static_cast<char>(strlen & 0xFF));
    packet.insert(packet.end(), ping_response.begin(), ping_response.end());

    send_parameters->data = packet.data();
    send_parameters->length = static_cast<int>(packet.size());
    return ENDSTONE_HOOK_CALL_ORIGINAL(&RNS2_Windows_Linux_360::Send_Windows_Linux_360NoVDP, socket, send_parameters,
                                       file, line);
}
}  // namespace RakNet
