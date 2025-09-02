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

#include "bedrock/network/network_connection.h"

#include <endstone/core/util/socket_address.h>

#include "endstone/core/server.h"
#include "endstone/event/server/packet_receive_event.h"
#include "endstone/runtime/hook.h"

NetworkPeer::DataStatus NetworkConnection::receivePacket(std::string &receive_buffer,
                                                         const NetworkPeer::PacketRecvTimepointPtr &timepoint_ptr)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto network_handler = server.getServer().getMinecraft()->getServerNetworkHandler();
    auto &network = server.getServer().getNetwork();

    while (true) {
        const auto status =
            ENDSTONE_HOOK_CALL_ORIGINAL(&NetworkConnection::receivePacket, this, receive_buffer, timepoint_ptr);
        if (status != NetworkPeer::DataStatus::HasData) {
            return status;
        }

        ReadOnlyBinaryStream stream(receive_buffer, false);
        const auto header_result = stream.getUnsignedVarInt().logError(Bedrock::LogLevel::Error, LogAreaID::Network);
        if (!header_result) {
            continue;
        }

        const auto header = header_result.value();
        const auto packet_id = static_cast<MinecraftPacketIds>(header & 0x3ff);
        const auto sub_client_id = static_cast<SubClientId>((header >> 12) & 0x3);

        endstone::core::EndstonePlayer *player = nullptr;
        if (const auto *p = network_handler->getServerPlayer(id, sub_client_id)) {
            player = &p->getEndstoneActor<endstone::core::EndstonePlayer>();
        }

        const auto payload = stream.getView().substr(stream.getReadPointer());
        endstone::PacketReceiveEvent e{player, static_cast<int>(packet_id), payload,
                                       endstone::core::EndstoneSocketAddress::fromNetworkIdentifier(id),
                                       static_cast<int>(sub_client_id)};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            continue;
        }

        if (e.getPayload().data() == payload.data()) {
            return status;  // Nothing to do, the packet is the same, go back to the original handler
        }

        // Plugins have changed the payload, let's handle it ourselves
        auto packet = MinecraftPackets::createPacket(packet_id);
        if (!packet) {
            server.getLogger().error("PacketReceiveEvent: Unknown packet id: {}", static_cast<int>(packet_id));
            continue;
        }

        auto now = std::chrono::steady_clock::now();
        last_packet_time = now;
        packet->setReceiveTimestamp(now);

        ReadOnlyBinaryStream read_stream(e.getPayload(), false);
        auto packet_size = read_stream.getView().size() - read_stream.getReadPointer();
        if (auto result = packet->checkSize(packet_size, true); !result.ignoreError()) {
            server.getLogger().error("PacketReceiveEvent: Bad packet size: {}", packet_size);
            continue;
        }

        if (auto result = packet->readNoHeader(read_stream, network.getPacketReflectionCtx(), sub_client_id);
            !result.ignoreError()) {
            server.getLogger().error("PacketReceiveEvent: Bad packet!");
            continue;
        }

        packet->handle(id, *network_handler, packet);
    }
}
