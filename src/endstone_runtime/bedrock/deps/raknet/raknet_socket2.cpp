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

#include "bedrock/deps/raknet/raknet_socket2.h"

#include <sstream>

#include <entt/entt.hpp>

#include "bedrock/deps/raknet/get_time.h"
#include "bedrock/deps/raknet/message_identifiers.h"
#include "bedrock/deps/raknet/raknet_defines.h"
#include "bedrock/deps/raknet/socket_defines.h"
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

void RNS2_Berkley::RecvFromBlocking(RNS2RecvStruct *recv_from_struct) const
{
    sockaddr_storage their_addr;
    socklen_t sock_len = sizeof(their_addr);
    memset(&their_addr, 0, sizeof(their_addr));
    recv_from_struct->bytes_read = recvfrom__(rns2_socket_, recv_from_struct->data, MAXIMUM_MTU_SIZE, 0,
                                              reinterpret_cast<sockaddr *>(&their_addr), &sock_len);
    if (recv_from_struct->bytes_read <= 0) {
        return;
    }

    recv_from_struct->time_read = RakNet::GetTimeUS();

    if (their_addr.ss_family == AF_INET) {
        memcpy(&recv_from_struct->system_address.address.addr4, reinterpret_cast<sockaddr_in *>(&their_addr),
               sizeof(sockaddr_in));
        recv_from_struct->system_address.debug_port = ntohs(recv_from_struct->system_address.address.addr4.sin_port);
    }
    else {
        memcpy(&recv_from_struct->system_address.address.addr6, reinterpret_cast<sockaddr_in6 *>(&their_addr),
               sizeof(sockaddr_in6));
        recv_from_struct->system_address.debug_port = ntohs(recv_from_struct->system_address.address.addr6.sin6_port);
    }
}

unsigned int RNS2_Berkley::RecvFromLoopInt()
{
    is_recv_from_loop_thread_active_.Increment();
    while (!end_threads_) {
        RNS2RecvStruct *recv_from_struct;
        recv_from_struct = binding_.event_handler->AllocRNS2RecvStruct(_FILE_AND_LINE_);
        if (recv_from_struct != nullptr) {
            recv_from_struct->socket = this;
            RecvFromBlocking(recv_from_struct);
            if (recv_from_struct->bytes_read > 0) {
                binding_.event_handler->OnRNS2Recv(recv_from_struct);
            }
            else {
                binding_.event_handler->DeallocRNS2RecvStruct(recv_from_struct, _FILE_AND_LINE_);
            }
        }
    }
    is_recv_from_loop_thread_active_.Decrement();
    return 0;
}

}  // namespace RakNet
