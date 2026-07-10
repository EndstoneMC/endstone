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

#include "bedrock/network/batched_network_peer.h"

#include "bedrock/network/packet.h"
#include "bedrock/network/packet/clientbound_map_item_data_packet.h"
#include "bedrock/network/packet/resource_pack_stack_packet.h"
#include "bedrock/network/packet/resource_packs_info_packet.h"
#include "bedrock/network/packet/start_game_packet.h"
#include "bedrock/network/raknet_connector.h"
#include "bedrock/network/server_network_system.h"
#include "bedrock/server/server_instance.h"
#include "endstone/core/level/level.h"
#include "endstone/core/map/map_view.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/core/util/socket_address.h"
#include "endstone/event/server/packet_receive_event.h"
#include "endstone/event/server/packet_send_event.h"
#include "endstone/runtime/hook.h"

namespace {
void patchPacket(const StartGamePacket &packet)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (const auto *level = server.getEndstoneLevel(); level && !level->getHandle().isClientSideGenerationEnabled()) {
        auto &pk = const_cast<StartGamePacket &>(packet);
        pk.settings.setRandomSeed(0);
    }
}

void patchPacket(const ResourcePacksInfoPacket &packet)
{
    (void)packet;
    // TODO(refactor): inject content key here instead of adding to ServerNetworkHandler
}

void patchPacket(const ResourcePackStackPacket &packet)
{
    if (packet.texture_pack_required) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        if (server.getAllowClientPacks()) {
            auto &pk = const_cast<ResourcePackStackPacket &>(packet);
            // false, otherwise the client will remove its own non-server-supplied resource packs.
            pk.texture_pack_required = false;
        }
    }
}

void patchPacket(const ClientboundMapItemDataPacket &packet, endstone::core::EndstonePlayer &player)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto *map = static_cast<endstone::core::EndstoneMapView *>(server.getMap(packet.getMapId().raw_id));
    if (!map) {
        return;
    }

    auto &pk = const_cast<ClientboundMapItemDataPacket &>(packet);
    if (pk.map_pixels_.empty() && pk.decorations_.empty()) {
        return;  // Map creation, no data to be patched
    }

    const auto &render = map->render(player);

    // Patch pixels only when this packet carries a texture update
    if (!pk.map_pixels_.empty()) {
        if (pk.start_x_ < 0 || pk.start_y_ < 0 || pk.width_ <= 0 || pk.height_ <= 0 ||
            pk.start_x_ + pk.width_ > MapConstants::MAP_SIZE || pk.start_y_ + pk.height_ > MapConstants::MAP_SIZE) {
            return;  // Out of bounds
        }
        for (auto x = 0; x < pk.width_; ++x) {
            for (auto y = 0; y < pk.height_; ++y) {
                pk.map_pixels_[x + (y * pk.width_)] =
                    render.buffer[(pk.start_x_ + x) + ((pk.start_y_ + y) * MapConstants::MAP_SIZE)];
            }
        }
    }

    pk.decorations_.clear();
    for (const auto &cursor : render.cursors) {
        if (cursor.isVisible()) {
            pk.decorations_.emplace_back(
                std::make_shared<MapDecoration>(static_cast<MapDecoration::Type>(cursor.getType()), cursor.getX(),
                                                cursor.getY(), cursor.getDirection(), cursor.getCaption(),
                                                mce::Color::WHITE  // TODO(map): support different colors
                                                ));
        }
    }
}

void patchPacket(Packet &packet, endstone::Player *player)
{
    switch (packet.getId()) {
    case MinecraftPacketIds::StartGame:
        patchPacket(static_cast<const StartGamePacket &>(packet));
        break;
    case MinecraftPacketIds::ResourcePacksInfo:
        patchPacket(static_cast<const ResourcePacksInfoPacket &>(packet));
        break;
    case MinecraftPacketIds::ResourcePackStack:
        patchPacket(static_cast<const ResourcePackStackPacket &>(packet));
        break;
    case MinecraftPacketIds::MapData:
        if (player) {
            patchPacket(static_cast<const ClientboundMapItemDataPacket &>(packet),
                        static_cast<endstone::core::EndstonePlayer &>(*player));
        }
        break;
    default:
        break;
    }
}
}  // namespace

void BatchedNetworkPeer::sendPacket(const std::string &data, Reliability reliability, Compressibility compressible)
{
    ReadOnlyBinaryStream stream(data, false);
    auto result = stream.getUnsignedVarInt().discardError();
    if (!result) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        server.getLogger().critical("BatchedNetworkPeer::sendPacket: Failed to parse raw packet header!");
        return;
    }

    // Parse packet header
    auto header = PacketHeader::fromRaw(result.value());
    const auto &id = getId();

    // Get player object - if exists
    const auto &server = endstone::core::EndstoneServer::getInstance();
    const auto *server_player =
        server.getServer().getMinecraft()->getServerNetworkHandler()->getServerPlayer(id, header.getSenderSubId());
    endstone::Player *player = nullptr;
    if (server_player) {
        player = &server_player->getEndstoneActor<endstone::core::EndstonePlayer>();
    }

    // Create packet send event
    auto payload = stream.getView().substr(stream.getReadPointer());
    endstone::PacketSendEvent e{player, static_cast<int>(header.getPacketId()), payload,
                                endstone::core::EndstoneSocketAddress::fromNetworkIdentifier(id),
                                static_cast<int>(header.getSenderSubId())};

    // Patch specific outbound packets (deserialize -> modify -> re-serialize)
    switch (header.getPacketId()) {
    case MinecraftPacketIds::StartGame:
    case MinecraftPacketIds::ResourcePacksInfo:
    case MinecraftPacketIds::ResourcePackStack:
    case MinecraftPacketIds::MapData: {
        auto packet = MinecraftPackets::createPacket(header.getPacketId());
        if (!packet) {
            server.getLogger().critical("BatchedNetworkPeer::sendPacket: Unknown packet id: {}",
                                        static_cast<int>(header.getPacketId()));
            return;
        }

        auto &network = server.getServer().getNetwork();
        if (!packet->readNoHeader(stream, network.getPacketReflectionCtx(), header.getSenderSubId()).ignoreError()) {
            server.getLogger().critical("BatchedNetworkPeer::sendPacket: Failed to parse packet with id: {}",
                                        static_cast<int>(packet->getId()));
            return;
        }

        patchPacket(*packet, player);

        BinaryStream out;
        packet->writeWithSerializationMode(out, network.getPacketReflectionCtx(),
                                           network.getPacketOverrides().getOverrideModeForPacket(packet->getId()));
        e.setPayload(out.getBuffer());
        break;
    }
    default:
        break;
    }

    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return;
    }

    if (e.getPayload().data() != payload.data()) {
        BinaryStream out;
        header.write(out);
        out.writeRawBytes(e.getPayload());
        ENDSTONE_HOOK_CALL_ORIGINAL(&BatchedNetworkPeer::sendPacket, this, out.getBuffer(), reliability, compressible);
    }
    else {
        ENDSTONE_HOOK_CALL_ORIGINAL(&BatchedNetworkPeer::sendPacket, this, data, reliability, compressible);
    }
}

NetworkPeer::DataStatus BatchedNetworkPeer::_receivePacket(std::string &out_data,
                                                           const PacketRecvTimepointPtr &timepoint_ptr)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto network_handler = server.getServer().getMinecraft()->getServerNetworkHandler();
    while (true) {
        const auto status =
            ENDSTONE_HOOK_CALL_ORIGINAL(&BatchedNetworkPeer::_receivePacket, this, out_data, timepoint_ptr);
        if (status != DataStatus::HasData) {
            return status;
        }

        ReadOnlyBinaryStream stream(out_data, false);
        auto result = stream.getUnsignedVarInt().discardError();
        if (!result) {
            return DataStatus::BrokenData;
        }

        const auto header = PacketHeader::fromRaw(result.value());
        const auto &id = getId();
        endstone::core::EndstonePlayer *player = nullptr;
        if (const auto *p = network_handler->getServerPlayer(id, header.getRecipientSubId())) {
            player = &p->getEndstoneActor<endstone::core::EndstonePlayer>();
        }

        const auto payload = stream.getView().substr(stream.getReadPointer());
        endstone::PacketReceiveEvent e{player, static_cast<int>(header.getPacketId()), payload,
                                       endstone::core::EndstoneSocketAddress::fromNetworkIdentifier(id),
                                       static_cast<int>(header.getRecipientSubId())};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            continue;
        }

        if (e.getPayload().data() == payload.data()) {
            return status;  // Nothing to do, the packet is the same, return immediately
        }

        // Plugins have changed the payload, keep header and replace the rest
        out_data.resize(stream.getReadPointer());
        out_data.append(e.getPayload().data(), e.getPayload().size());
        return status;
    }
}

const NetworkIdentifier &BatchedNetworkPeer::getId() const
{
    auto peer = peer_;
    while (peer->peer_) {
        peer = peer->peer_;
    }
    return static_cast<RakNetConnector::RakNetNetworkPeer &>(*peer).getId();
}
