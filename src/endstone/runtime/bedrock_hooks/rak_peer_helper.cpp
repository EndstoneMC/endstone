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

#include "bedrock/network/rak_peer_helper.h"

#include <ranges>
#include <string>
#include <vector>

#include <magic_enum/magic_enum.hpp>

#include "bedrock/deps/raknet/bit_stream.h"
#include "bedrock/deps/raknet/message_identifiers.h"
#include "bedrock/deps/raknet/plugin_interface.h"
#include "endstone/core/server.h"
#include "endstone/core/util/socket_address.h"
#include "endstone/event/server/server_list_ping_event.h"
#include "endstone/runtime/hook.h"

constexpr unsigned int MAX_OFFLINE_DATA_LENGTH = 400;
constexpr unsigned char OFFLINE_MESSAGE_DATA_ID[16] = {0x00, 0xFF, 0xFF, 0x00, 0xFE, 0xFE, 0xFE, 0xFE,
                                                       0xFD, 0xFD, 0xFD, 0xFD, 0x12, 0x34, 0x56, 0x78};

static endstone::ServerListPingEvent callServerListPingEvent(endstone::SocketAddress address, std::string_view data)
{
    std::vector<std::string> parts;
    for (auto part : data | std::views::split(';')) {
        parts.emplace_back(part.begin(), part.end());
    }

    const auto n = parts.size();
    auto motd = n > 1 ? parts[1] : "";
    auto network_protocol_version = n > 2 ? std::stoi(parts[2]) : 0;
    auto minecraft_version_network = n > 3 ? parts[3] : "";
    auto num_players = n > 4 ? std::stoi(parts[4]) : 0;
    auto max_players = n > 5 ? std::stoi(parts[5]) : 0;
    auto server_guid = n > 6 ? parts[6] : "";
    auto level_name = n > 7 ? parts[7] : "";
    auto game_mode = n > 8 ? magic_enum::enum_cast<endstone::GameMode>(parts[8]).value_or(endstone::GameMode::Survival)
                           : endstone::GameMode::Survival;
    auto local_port = n > 10 ? std::stoi(parts[10]) : 0;
    auto local_port_v6 = n > 11 ? std::stoi(parts[11]) : 0;

    endstone::ServerListPingEvent event(std::move(address), motd, network_protocol_version, minecraft_version_network,
                                        num_players, max_players, server_guid, level_name, game_mode, local_port,
                                        local_port_v6);

    auto &server = endstone::core::EndstoneServer::getInstance();
    server.getPluginManager().callEvent(event);
    return event;
}

RakNet::RakPeerInterface *gRakPeer = nullptr;

bool handleIncomingDatagram(RakNet::RNS2RecvStruct *recv)
{
    // #blameMojang - MCPE-228407: Mojang's custom RakNet packet 0x86 handler reads SystemAddress
    // without checking if the packet is large enough to contain one. Networking 101: validate before read.
    // Reported to Mojang, closed as "Won't Fix". Classic.
    // Fix: drop undersized packets before they reach the vulnerable code path.
    if (static_cast<unsigned char>(recv->data[0]) == 0x86) {
        int expected_size = sizeof(unsigned char) + sizeof(unsigned char);
        if (recv->data[0] == 4) {  // ipv4
            expected_size += sizeof(std::uint32_t) + sizeof(std::uint16_t);
        }
        else {  // ipv6
            expected_size += sizeof(sockaddr_in6);
        }
        if (recv->bytes_read < expected_size) {
            return false;
        }
    }
    if (recv->data[0] == ID_UNCONNECTED_PING &&
        recv->bytes_read >= sizeof(unsigned char) + sizeof(RakNet::Time) + sizeof(OFFLINE_MESSAGE_DATA_ID)) {
        char *ping_data;
        std::uint32_t ping_size;
        gRakPeer->GetOfflinePingResponse(&ping_data, &ping_size);
        if (ping_size < 2 || (ping_data[0] << 8 | ping_data[1]) != ping_size - 2) {
            return true;
        }

        // call ServerListPingEvent with the default offline ping response
        auto address = endstone::core::EndstoneSocketAddress::fromSystemAddress(recv->system_address);
        auto event = callServerListPingEvent(address, std::string_view(ping_data + 2, ping_size - 2));
        if (event.isCancelled()) {
            return false;
        }

        // parse ping request
        RakNet::BitStream is((unsigned char *)recv->data, recv->bytes_read, false);
        is.IgnoreBits(8);
        RakNet::Time sendPingTime;
        is.Read(sendPingTime);
        is.IgnoreBytes(sizeof(OFFLINE_MESSAGE_DATA_ID));
        auto remoteGuid = RakNet::UNASSIGNED_RAKNET_GUID;
        is.Read(remoteGuid);

        // prepare ping response
        auto response =
            fmt::format("MCPE;{};{};{};{};{};{};{};{};1;{};{};0;", event.getMotd(), event.getNetworkProtocolVersion(),
                        event.getMinecraftVersionNetwork(), event.getNumPlayers(), event.getMaxPlayers(),
                        event.getServerGuid(), event.getLevelName(), magic_enum::enum_name(event.getGameMode()),
                        event.getLocalPort(), event.getLocalPortV6());
        RakNet::BitStream os;
        os.Write(static_cast<RakNet::MessageID>(ID_UNCONNECTED_PONG));
        os.Write(sendPingTime);
        os.Write(gRakPeer->GetMyGUID());
        os.WriteAlignedBytes(OFFLINE_MESSAGE_DATA_ID, sizeof(OFFLINE_MESSAGE_DATA_ID));
        os.Write(static_cast<std::uint16_t>(response.size()));
        os.Write(response.data(), response.size());

        // send directly via socket
        RakNet::RNS2_SendParameters bsp;
        bsp.data = reinterpret_cast<char *>(os.GetData());
        bsp.length = os.GetNumberOfBytesUsed();
        bsp.system_address = recv->system_address;
        recv->socket->Send(&bsp, _FILE_AND_LINE_);
        return false;
    }
    return true;
}

RakNet::StartupResult RakPeerHelper::peerStartup(RakNet::RakPeerInterface *peer, const ConnectionDefinition &def,
                                                 PeerPurpose purpose)
{
    ConnectionDefinition new_def = def;
    if (peer && purpose == PeerPurpose::Gameplay) {
        new_def.max_num_connections = SharedConstants::NetworkDefaultMaxConnections;
        peer->SetLimitIPConnectionFrequency(true);  // limit connections from the same ip in 100 milliseconds.
        peer->SetIncomingDatagramEventHandler(handleIncomingDatagram);
        gRakPeer = peer;
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&RakPeerHelper::peerStartup, this, peer, new_def, purpose);
}
