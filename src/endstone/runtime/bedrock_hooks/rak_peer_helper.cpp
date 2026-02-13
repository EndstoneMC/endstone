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

#include "bedrock/deps/raknet/bit_stream.h"
#include "bedrock/deps/raknet/message_identifiers.h"
#include "bedrock/deps/raknet/plugin_interface.h"
#include "endstone/core/server.h"
#include "endstone/event/server/server_list_ping_event.h"
#include "endstone/runtime/hook.h"

constexpr unsigned int MAX_OFFLINE_DATA_LENGTH = 400;
constexpr unsigned char OFFLINE_MESSAGE_DATA_ID[16] = {0x00, 0xFF, 0xFF, 0x00, 0xFE, 0xFE, 0xFE, 0xFE,
                                                       0xFD, 0xFD, 0xFD, 0xFD, 0x12, 0x34, 0x56, 0x78};

endstone::ServerListPingEvent callServerListPingEvent(std::string_view data)
{
    // TODO:
    return {};
}

class ServerListPingListener : public RakNet::PluginInterface2 {
public:
    RakNet::PluginReceiveResult OnReceive(RakNet::Packet *packet) override
    {
        if (packet->data[0] == ID_UNCONNECTED_PING &&
            packet->length >= sizeof(unsigned char) + sizeof(RakNet::Time) + sizeof(OFFLINE_MESSAGE_DATA_ID)) {
            // get socket for sending direct UDP payload
            auto *socket = rakPeerInterface->GetSocket(packet->systemAddress);
            if (!socket) {
                return RakNet::RR_CONTINUE_PROCESSING;
            }

            // call ServerListPingEvent with the default offline ping response
            char *data;
            std::uint32_t length;
            rakPeerInterface->GetOfflinePingResponse(&data, &length);
            if ((data[0] << 8 | data[1]) != length - 2) {
                return RakNet::RR_CONTINUE_PROCESSING;
            }
            const auto event = callServerListPingEvent(std::string_view(data + 2, length - 2));
            if (event.isCancelled()) {
                return RakNet::RR_STOP_PROCESSING_AND_DEALLOCATE;
            }

            // parse ping request
            RakNet::BitStream is(packet->data, packet->length, false);
            is.IgnoreBits(8);
            RakNet::Time sendPingTime;
            is.Read(sendPingTime);
            is.IgnoreBytes(sizeof(OFFLINE_MESSAGE_DATA_ID));
            auto remoteGuid = RakNet::UNASSIGNED_RAKNET_GUID;
            is.Read(remoteGuid);

            // prepare ping response
            auto response =
                fmt::format("MCPE;{};{};{};{};{};{};{};{};1;{};{};0;", event.getMotd(),
                            event.getNetworkProtocolVersion(), event.getMinecraftVersionNetwork(),
                            event.getNumPlayers(), event.getMaxPlayers(), event.getServerGuid(), event.getLevelName(),
                            magic_enum::enum_name(event.getGameMode()), event.getLocalPort(), event.getLocalPortV6());
            RakNet::BitStream os;
            os.Write(static_cast<RakNet::MessageID>(ID_UNCONNECTED_PONG));
            os.Write(sendPingTime);
            os.Write(rakPeerInterface->GetMyGUID());
            os.WriteAlignedBytes(OFFLINE_MESSAGE_DATA_ID, sizeof(OFFLINE_MESSAGE_DATA_ID));
            os.Write(static_cast<std::uint16_t>(response.size()));
            os.Write(response.data(), response.size());

            // send directly via socket
            RakNet::RNS2_SendParameters bsp;
            bsp.data = reinterpret_cast<char *>(os.GetData());
            bsp.length = os.GetNumberOfBytesUsed();
            bsp.system_address = packet->systemAddress;
            socket->Send(&bsp, _FILE_AND_LINE_);
            return RakNet::RR_STOP_PROCESSING_AND_DEALLOCATE;
        }
        return RakNet::RR_CONTINUE_PROCESSING;
    }

    static ServerListPingListener &getInstance()
    {
        static ServerListPingListener instance;
        return instance;
    }
};

RakNet::StartupResult RakPeerHelper::peerStartup(RakNet::RakPeerInterface *peer, const ConnectionDefinition &def,
                                                 PeerPurpose purpose)
{
    auto new_def = def;
    new_def.max_num_connections = SharedConstants::NetworkDefaultMaxConnections;
    if (peer && purpose == PeerPurpose::Gameplay) {
        peer->SetLimitIPConnectionFrequency(true);  // limit connections from the same ip in 100 milliseconds.
        peer->AttachPlugin(&ServerListPingListener::getInstance());
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&RakPeerHelper::peerStartup, this, peer, new_def, purpose);
}
