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

#include <BitStream.h>
#include <MessageIdentifiers.h>
#include <PluginInterface2.h>
#include <RakPeer.h>

#include <format>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

#include <magic_enum/magic_enum.hpp>

#include "endstone/core/server.h"
#include "endstone/core/util/socket_address.h"
#include "endstone/event/server/server_list_ping_event.h"
#include "endstone/runtime/hook.h"

constexpr unsigned int MAX_OFFLINE_DATA_LENGTH = 400;
constexpr unsigned char OFFLINE_MESSAGE_DATA_ID[16] = {0x00, 0xFF, 0xFF, 0x00, 0xFE, 0xFE, 0xFE, 0xFE,
                                                       0xFD, 0xFD, 0xFD, 0xFD, 0x12, 0x34, 0x56, 0x78};

static std::vector<std::string> splitAdvertisement(std::string_view data)
{
    std::vector<std::string> parts;
    for (auto part : data | std::views::split(';')) {
        parts.emplace_back(part.begin(), part.end());
    }
    return parts;
}

// BDS appends fields to the advertisement over time, so keep every field it sent and rewrite only the ones
// the event models. Anything we don't know about passes through untouched.
static std::string buildAdvertisement(std::vector<std::string> parts, const endstone::ServerListPingEvent &event)
{
    parts.resize(std::max<std::size_t>(parts.size(), 14));
    parts[0] = "MCPE";
    parts[1] = event.getMotd();
    parts[2] = std::to_string(event.getNetworkProtocolVersion());
    parts[3] = event.getMinecraftVersionNetwork();
    parts[4] = std::to_string(event.getNumPlayers());
    parts[5] = std::to_string(event.getMaxPlayers());
    parts[6] = event.getServerGuid();
    parts[7] = event.getLevelName();
    parts[8] = magic_enum::enum_name(event.getGameMode());
    parts[10] = std::to_string(event.getLocalPort());

    std::string response = parts[0];
    for (std::size_t i = 1; i < parts.size(); ++i) {
        response += ';';
        response += parts[i];
    }
    return response;
}

static endstone::ServerListPingEvent callServerListPingEvent(endstone::SocketAddress address,
                                                             const std::vector<std::string> &parts)
{
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

    endstone::ServerListPingEvent event(std::move(address), motd, network_protocol_version, minecraft_version_network,
                                        num_players, max_players, server_guid, level_name, game_mode, local_port);

    auto &server = endstone::core::EndstoneServer::getInstance();
    server.getPluginManager().callEvent(event);
    return event;
}

RakNet::RakPeer *gRakPeer = nullptr;

static bool handleUnconnectedPing(RakNet::RNS2RecvStruct *recv)
{
    if (recv->bytesRead < sizeof(unsigned char) + sizeof(RakNet::Time) + sizeof(OFFLINE_MESSAGE_DATA_ID)) {
        return true;
    }
    char *ping_data;
    std::uint32_t ping_size;
    gRakPeer->GetOfflinePingResponse(&ping_data, &ping_size);
    if (ping_size < 2 || (ping_data[0] << 8 | ping_data[1]) != ping_size - 2) {
        return true;
    }

    // call ServerListPingEvent with the default offline ping response
    auto address = endstone::core::EndstoneSocketAddress::fromSystemAddress(recv->systemAddress);
    auto parts = splitAdvertisement(std::string_view(ping_data + 2, ping_size - 2));
    auto event = callServerListPingEvent(address, parts);
    if (event.isCancelled()) {
        return false;
    }

    // parse ping request
    RakNet::BitStream is((unsigned char *)recv->data, recv->bytesRead, false);
    is.IgnoreBits(8);
    RakNet::Time sendPingTime;
    is.Read(sendPingTime);
    is.IgnoreBytes(sizeof(OFFLINE_MESSAGE_DATA_ID));
    auto remoteGuid = RakNet::UNASSIGNED_RAKNET_GUID;
    is.Read(remoteGuid);

    // prepare ping response
    auto response = buildAdvertisement(std::move(parts), event);
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
    bsp.systemAddress = recv->systemAddress;
    recv->socket->Send(&bsp, _FILE_AND_LINE_);
    return false;
}

bool handleIncomingDatagram(RakNet::RNS2RecvStruct *recv)
{
    if (recv->data[0] == ID_UNCONNECTED_PING) {
        return handleUnconnectedPing(recv);
    }
    return true;
}

RakNet::StartupResult RakPeerHelper::peerStartup(RakNet::RakPeerInterface *peer, const ConnectionDefinition &def,
                                                 PeerPurpose purpose)
{
    ConnectionDefinition new_def = def;
    if (peer && purpose == PeerPurpose::Gameplay) {
        new_def.max_num_connections = SharedConstants::NetworkDefaultMaxConnections;
        // peer->SetLimitIPConnectionFrequency(true);  // limit connections from the same ip in 100 milliseconds.
        peer->SetIncomingDatagramEventHandler(handleIncomingDatagram);
        gRakPeer = static_cast<RakNet::RakPeer *>(peer);
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&RakPeerHelper::peerStartup, this, peer, new_def, purpose);
}

namespace RakNet {
struct ShadowBanList {};
}  // namespace RakNet

[[noreturn]] static void rakPeerLinkStub(const char *name)
{
    throw std::runtime_error(std::string("RakNet::RakPeer::") + name + " is a link stub and must never be called");
}

void RakNet::RakPeer::InitializeConfiguration(std::unique_ptr<RakNet::ShadowBanList>)
{
    rakPeerLinkStub("InitializeConfiguration");
}
bool RakNet::RakPeer::SetApplicationHandshakeCompleted(RakNet::AddressOrGUID)
{
    rakPeerLinkStub("SetApplicationHandshakeCompleted");
}
void RakNet::RakPeer::SetAllowUnconnectedPings(bool)
{
    rakPeerLinkStub("SetAllowUnconnectedPings");
}
bool RakNet::RakPeer::GetAllowUnconnectedPings() const
{
    rakPeerLinkStub("GetAllowUnconnectedPings");
}
void RakNet::RakPeer::resetMyGUID()
{
    rakPeerLinkStub("resetMyGUID");
}
unsigned int RakNet::RakPeer::GetNumberOfAdapters()
{
    rakPeerLinkStub("GetNumberOfAdapters");
}
RakNet::NetworkAdapter &RakNet::RakPeer::GetLocalAdapter(unsigned int)
{
    rakPeerLinkStub("GetLocalAdapter");
}
