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

#include "endstone/core/network/batched_network_peer.h"

#include <cstdint>
#include <limits>
#include <string>
#include <utility>

#include <gsl/util>

#include "bedrock/network/packet.h"
#include "bedrock/network/packet/clientbound_map_item_data_packet.h"
#include "bedrock/network/packet/resource_pack_stack_packet.h"
#include "bedrock/network/packet/resource_packs_info_packet.h"
#include "bedrock/network/packet/start_game_packet.h"
#include "bedrock/network/server_network_system.h"
#include "bedrock/server/server_instance.h"
#include "endstone/core/level/level.h"
#include "endstone/core/map/map_view.h"
#include "endstone/core/player.h"
#include "endstone/core/plugin/plugin_manager.h"
#include "endstone/core/server.h"
#include "endstone/core/util/socket_address.h"
#include "endstone/event/server/packet_receive_event.h"
#include "endstone/event/server/packet_send_event.h"

namespace {
void patchPacket(const StartGamePacket &packet)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (const auto *level = server.getEndstoneLevel(); level && !level->getHandle().isClientSideGenerationEnabled()) {
        auto &pk = const_cast<StartGamePacket &>(packet);
        pk.payload.settings.setRandomSeed(0);
    }
}

void patchPacket(const ResourcePacksInfoPacket &packet)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto &pk = const_cast<ResourcePacksInfoPacket &>(packet);
    for (auto &pack_info : pk.payload.resource_packs) {
        if (const auto *key = server.getContentKey(pack_info.pack_id_version)) {
            pack_info.content_key = *key;
        }
    }
}

void patchPacket(const ResourcePackStackPacket &packet)
{
    if (packet.payload.texture_pack_required) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        if (server.getAllowClientPacks()) {
            auto &pk = const_cast<ResourcePackStackPacket &>(packet);
            // false, otherwise the client will remove its own non-server-supplied resource packs.
            pk.payload.texture_pack_required = false;
        }
    }
}

// Bedrock keys map decorations by tracked actor and replaces on a match; Java keys them by slot.
// The client rebuilds its list from every packet, so a slot works here too -- at an impossible y.
MapItemTrackedActor::UniqueId decorationKey(std::size_t slot)
{
    return BlockPos{static_cast<int>(slot), std::numeric_limits<int>::min(), 0};
}

void patchPacket(const ClientboundMapItemDataPacket &packet, endstone::core::EndstonePlayer &player)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto *map = static_cast<endstone::core::EndstoneMapView *>(server.getMap(packet.getMapId().raw_id));
    if (!map || map->isVanilla()) {
        return;  // Nothing renders on top, so BDS already filled in the pixels and the decorations
    }

    auto &pk = const_cast<ClientboundMapItemDataPacket &>(packet);
    if (pk.payload.map_pixels.empty() && pk.payload.decorations.empty()) {
        return;  // Map creation, no data to be patched
    }

    const auto &render = map->render(player);

    // Patch pixels only when this packet carries a texture update
    if (!pk.payload.map_pixels.empty()) {
        if (pk.payload.start_x < 0 || pk.payload.start_y < 0 || pk.payload.width <= 0 || pk.payload.height <= 0 ||
            pk.payload.start_x + pk.payload.width > MapConstants::MAP_SIZE ||
            pk.payload.start_y + pk.payload.height > MapConstants::MAP_SIZE) {
            return;  // Out of bounds
        }
        for (auto x = 0; x < pk.payload.width; ++x) {
            for (auto y = 0; y < pk.payload.height; ++y) {
                pk.payload.map_pixels[x + (y * pk.payload.width)] =
                    render.buffer[(pk.payload.start_x + x) + ((pk.payload.start_y + y) * MapConstants::MAP_SIZE)];
            }
        }
    }

    // Tracked actor ids and decorations go on the wire as parallel arrays.
    pk.payload.unique_ids.clear();
    pk.payload.decorations.clear();
    for (const auto &cursor : render.cursors) {
        if (cursor.isVisible()) {
            pk.payload.unique_ids.emplace_back(decorationKey(pk.payload.decorations.size()));
            pk.payload.decorations.emplace_back(
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

bool isPatched(MinecraftPacketIds id)
{
    return id == MinecraftPacketIds::StartGame || id == MinecraftPacketIds::ResourcePacksInfo ||
           id == MinecraftPacketIds::ResourcePackStack || id == MinecraftPacketIds::MapData;
}

Compressibility getCompressibleState(std::size_t compressible_bytes, std::size_t data_to_send)
{
    const auto ratio = static_cast<float>(compressible_bytes) / static_cast<float>(data_to_send);
    return ratio <= 0.2F ? Compressibility::Incompressible : Compressibility::Compressible;
}
}  // namespace

namespace endstone::core {

EndstoneBatchedNetworkPeer::EndstoneBatchedNetworkPeer(NetworkIdentifier id, std::shared_ptr<NetworkPeer> peer,
                                                       std::shared_ptr<::BatchedNetworkPeer> async_peer)
    : id_(std::move(id)), async_peer_(std::move(async_peer))
{
    peer_ = std::move(peer);
    compressible_bytes_ = 0;
    incoming_data_.emplace(incoming_data_buffer_, false);
    sent_packets_ = 0;
    async_enabled_ = false;
    async_peer_->async_enabled_ = true;
}

void EndstoneBatchedNetworkPeer::sendPacket(const std::string &data, Reliability /*reliability*/,
                                            Compressibility compressible)
{
    const auto &server = EndstoneServer::getInstance();
    ReadOnlyBinaryStream peek(data, false);
    if (const auto result = peek.getUnsignedVarInt().discardError();
        result && !isPatched(PacketHeader::fromRaw(result.value()).getPacketId()) &&
        !server.getEndstonePluginManager().isEventRegistered<PacketSendEvent>()) {
        writePacket(data, compressible);
        return;
    }

    auto &network = server.getServer().getNetwork();
    // A packet a plugin sends from here would overwrite data, which sendToMultiple then sends to the next player
    auto buffer = network.send_stream_.getAndReleaseData();
    const auto restore = gsl::finally([&] {
        network.send_stream_.getAndReleaseData();
        network.send_buffer_ = std::move(buffer);
    });

    ReadOnlyBinaryStream stream(buffer, false);
    auto result = stream.getUnsignedVarInt().discardError();
    if (!result) {
        server.getLogger().critical("BatchedNetworkPeer::sendPacket: Failed to parse raw packet header!");
        return;
    }

    // Parse packet header
    auto header = PacketHeader::fromRaw(result.value());

    // Get player object - if exists
    const auto *server_player =
        server.getServer().getMinecraft()->getServerNetworkHandler()->getServerPlayer(id_, header.getRecipientSubId());
    endstone::Player *player = nullptr;
    if (server_player) {
        player = &server_player->getEndstoneActor<EndstonePlayer>();
    }

    // Create packet send event
    auto payload = stream.getView().substr(stream.getReadPointer());
    const auto address = player ? player->getAddress() : EndstoneSocketAddress::fromNetworkIdentifier(id_);
    PacketSendEvent e{player, static_cast<int>(header.getPacketId()), payload, address,
                      static_cast<int>(header.getRecipientSubId())};

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
        const auto new_payload = e.getPayload();
        const auto *bytes = reinterpret_cast<const unsigned char *>(new_payload.data());
        out.writeRawBytes({bytes, bytes + new_payload.size()}, nullptr, nullptr);
        writePacket(out.getBuffer(), compressible);
    }
    else {
        writePacket(buffer, compressible);
    }
}

NetworkPeer::NetworkStatus EndstoneBatchedNetworkPeer::getNetworkStatus() const
{
    return peer_->getNetworkStatus();
}

void EndstoneBatchedNetworkPeer::update()
{
    flush({});
    if (peer_) {
        peer_->update();
    }
}

void EndstoneBatchedNetworkPeer::flush(std::function<void()> &&callback)
{
    if (async_enabled_) {
        // The peer BDS built queues the batch on its TaskGroup, which sends it on the network worker
        batchingPeer().flush(std::move(callback));
        return;
    }

    const auto data = outgoing_data_.getAndReleaseData();
    if (data.empty()) {
        if (callback) {
            callback();
        }
        return;
    }

    peer_->sendPacket(data, Reliability::ReliableOrdered, getCompressibleState(compressible_bytes_, data.size()));
    if (callback) {
        callback();
    }
    compressible_bytes_ = 0;
}

NetworkPeer::DataStatus EndstoneBatchedNetworkPeer::_receivePacket(std::string &out_data,
                                                                   const PacketRecvTimepointPtr &timepoint_ptr)
{
    const auto &server = EndstoneServer::getInstance();
    if (!server.getEndstonePluginManager().isEventRegistered<PacketReceiveEvent>()) {
        return readPacket(out_data, timepoint_ptr);
    }

    auto network_handler = server.getServer().getMinecraft()->getServerNetworkHandler();
    while (true) {
        const auto status = readPacket(out_data, timepoint_ptr);
        if (status != DataStatus::HasData) {
            return status;
        }

        ReadOnlyBinaryStream stream(out_data, false);
        auto result = stream.getUnsignedVarInt().discardError();
        if (!result) {
            return DataStatus::BrokenData;
        }

        const auto header = PacketHeader::fromRaw(result.value());

        EndstonePlayer *player = nullptr;
        if (const auto *p = network_handler->getServerPlayer(id_, header.getSenderSubId())) {
            player = &p->getEndstoneActor<EndstonePlayer>();
        }

        const auto payload = stream.getView().substr(stream.getReadPointer());
        const auto address = player ? player->getAddress() : EndstoneSocketAddress::fromNetworkIdentifier(id_);
        PacketReceiveEvent e{player, static_cast<int>(header.getPacketId()), payload, address,
                             static_cast<int>(header.getSenderSubId())};
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

void EndstoneBatchedNetworkPeer::writePacket(std::string_view data, Compressibility compressible)
{
    auto &batch = batchingPeer();
    batch.outgoing_data_.writeString(data, nullptr, nullptr);
    if (compressible == Compressibility::Compressible) {
        batch.compressible_bytes_ += data.size();
    }
    if (getCompressibleState(batch.compressible_bytes_, batch.outgoing_data_.getBuffer().size()) ==
        Compressibility::Incompressible) {
        flush({});
    }
}

NetworkPeer::DataStatus EndstoneBatchedNetworkPeer::readPacket(std::string &out_data,
                                                               const PacketRecvTimepointPtr &timepoint_ptr)
{
    if (incoming_data_->getUnreadLength() == 0) {
        if (const auto status = peer_->_receivePacket(incoming_data_buffer_, timepoint_ptr);
            status != DataStatus::HasData) {
            return status;
        }
        if (incoming_data_buffer_.empty()) {
            return DataStatus::BrokenData;
        }
        incoming_data_.emplace(incoming_data_buffer_, false);
    }

    auto status = DataStatus::NoData;
    if (incoming_data_->getUnreadLength() != 0) {
        out_data.clear();
        if (const auto length = incoming_data_->getUnsignedVarInt().discardError()) {
            const auto start = incoming_data_->getReadPointer();
            if (length.value() > incoming_data_->getUnreadLength()) {
                incoming_data_->setReadPointer(incoming_data_->getLength());
            }
            else {
                out_data.assign(incoming_data_->getView().substr(start, length.value()));
                incoming_data_->setReadPointer(start + length.value());
            }
        }
        status = DataStatus::HasData;
    }

    if (incoming_data_->hasOverflowed()) {
        incoming_data_buffer_.clear();
        incoming_data_.emplace(incoming_data_buffer_, false);
        return DataStatus::BrokenData;
    }
    return status;
}

::BatchedNetworkPeer &EndstoneBatchedNetworkPeer::batchingPeer()
{
    if (!async_enabled_) {
        return *this;
    }
    if (!outgoing_data_.getBuffer().empty()) {
        // Hand over what was batched before BDS switched the connection to async flushing
        const auto pending = outgoing_data_.getAndReleaseData();
        const auto *bytes = reinterpret_cast<const unsigned char *>(pending.data());
        async_peer_->outgoing_data_.writeRawBytes({bytes, bytes + pending.size()}, nullptr, nullptr);
        async_peer_->compressible_bytes_ += compressible_bytes_;
        compressible_bytes_ = 0;
    }
    return *async_peer_;
}

}  // namespace endstone::core
