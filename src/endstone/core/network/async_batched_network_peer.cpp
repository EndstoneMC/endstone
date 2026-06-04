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

#include "endstone/core/network/async_batched_network_peer.h"

#include <boost/asio/post.hpp>

#include "bedrock/network/network_connection.h"
#include "bedrock/network/packet.h"
#include "bedrock/network/packet/clientbound_map_item_data_packet.h"
#include "bedrock/network/packet/resource_pack_stack_packet.h"
#include "bedrock/network/packet/resource_packs_info_packet.h"
#include "bedrock/network/packet/start_game_packet.h"
#include "bedrock/network/raknet_connector.h"
#include "bedrock/network/server_network_system.h"
#include "endstone/core/level/level.h"
#include "endstone/core/map/map_view.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/core/util/socket_address.h"
#include "endstone/event/server/packet_receive_event.h"
#include "endstone/event/server/packet_send_event.h"

namespace endstone::core {

namespace {
void patchPacket(const StartGamePacket &packet)
{
    const auto &server = EndstoneServer::getInstance();
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
        const auto &server = EndstoneServer::getInstance();
        if (server.getAllowClientPacks()) {
            auto &pk = const_cast<ResourcePackStackPacket &>(packet);
            // false, otherwise the client will remove its own non-server-supplied resource packs.
            pk.texture_pack_required = false;
        }
    }
}

void patchPacket(const ClientboundMapItemDataPacket &packet, EndstonePlayer &player)
{
    const auto &server = EndstoneServer::getInstance();
    auto *map = static_cast<EndstoneMapView *>(server.getMap(packet.getMapId().raw_id));
    if (!map) {
        return;
    }

    auto &pk = const_cast<ClientboundMapItemDataPacket &>(packet);
    if (pk.map_pixels_.empty() || pk.decorations_.empty()) {
        return;  // Map creation, no data to be patched
    }

    if (pk.start_x_ < 0 || pk.start_y_ < 0 || pk.width_ <= 0 || pk.height_ <= 0 ||
        pk.start_x_ + pk.width_ > MapConstants::MAP_SIZE || pk.start_y_ + pk.height_ > MapConstants::MAP_SIZE) {
        return;  // Out of bounds
    }

    const auto &render = map->render(player);
    for (auto x = 0; x < pk.width_; ++x) {
        for (auto y = 0; y < pk.height_; ++y) {
            pk.map_pixels_[x + (y * pk.width_)] =
                render.buffer[(pk.start_x_ + x) + ((pk.start_y_ + y) * MapConstants::MAP_SIZE)];
        }
    }

    pk.decorations_.clear();
    for (const auto &cursor : render.cursors) {
        if (cursor.isVisible()) {
            pk.decorations_.emplace_back(std::make_shared<MapDecoration>(
                static_cast<MapDecoration::Type>(cursor.getType()), cursor.getX(), cursor.getY(), cursor.getDirection(),
                cursor.getCaption(), mce::Color::WHITE  // TODO(map): support different colors
                ));
        }
    }
}

void patchPacket(Packet &packet, Player *player)
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
                        static_cast<EndstonePlayer &>(*player));
        }
        break;
    default:
        break;
    }
}
}  // namespace

AsyncBatchedNetworkPeer::AsyncBatchedNetworkPeer(std::shared_ptr<NetworkPeer> compressed_peer,
                                                 EventLoopGroup::EventLoop event_loop)
    : event_loop_(std::move(event_loop))
{
    peer_ = std::move(compressed_peer);  // inherited NetworkPeer::peer_ -- the inner chain root
}

void AsyncBatchedNetworkPeer::splice(NetworkConnection &connection, EventLoopGroup &group)
{
    auto old_batched = connection.batched_peer.lock();
    if (!old_batched) {
        return;  // no Batched layer (local / encryption-disabled)
    }

    // Wrap the same inner chain the old Batched wrapped; the copied shared_ptr keeps it alive.
    auto new_async = std::make_shared<AsyncBatchedNetworkPeer>(old_batched->peer_, group.next());

    if (connection.peer == old_batched) {
        connection.peer = new_async;
    }
    else {
        NetworkPeer *cur = connection.peer.get();  // walk PacketTrace -> ... down to the holder of old_batched
        while (cur->peer_ && cur->peer_ != old_batched) {
            cur = cur->peer_.get();
        }
        if (cur->peer_ != old_batched) {
            return;  // unexpected chain shape; leave BDS untouched
        }
        cur->peer_ = new_async;
    }
    connection.batched_peer = new_async;  // assignable because we derive from BatchedNetworkPeer
}

const NetworkIdentifier &AsyncBatchedNetworkPeer::getId() const
{
    auto peer = peer_;
    while (peer->peer_) {
        peer = peer->peer_;
    }
    return static_cast<RakNetConnector::RakNetNetworkPeer &>(*peer).getId();
}

std::optional<std::string> AsyncBatchedNetworkPeer::handleSendEvent(const std::string &data)
{
    ReadOnlyBinaryStream stream(data, false);
    auto result = stream.getUnsignedVarInt().discardError();
    if (!result) {
        const auto &server = EndstoneServer::getInstance();
        server.getLogger().critical("AsyncBatchedNetworkPeer::sendPacket: Failed to parse raw packet header!");
        return std::nullopt;
    }

    auto header = PacketHeader::fromRaw(result.value());
    const auto &id = getId();

    const auto &server = EndstoneServer::getInstance();
    const auto *server_player =
        server.getServer().getMinecraft()->getServerNetworkHandler()->getServerPlayer(id, header.getSenderSubId());
    Player *player = nullptr;
    if (server_player) {
        player = &server_player->getEndstoneActor<EndstonePlayer>();
    }

    auto payload = stream.getView().substr(stream.getReadPointer());
    PacketSendEvent e{player, static_cast<int>(header.getPacketId()), payload,
                      EndstoneSocketAddress::fromNetworkIdentifier(id), static_cast<int>(header.getSenderSubId())};

    switch (header.getPacketId()) {
    case MinecraftPacketIds::StartGame:
    case MinecraftPacketIds::ResourcePacksInfo:
    case MinecraftPacketIds::ResourcePackStack:
    case MinecraftPacketIds::MapData: {
        auto packet = MinecraftPackets::createPacket(header.getPacketId());
        if (!packet) {
            server.getLogger().critical("AsyncBatchedNetworkPeer::sendPacket: Unknown packet id: {}",
                                        static_cast<int>(header.getPacketId()));
            return std::nullopt;
        }

        auto &network = server.getServer().getNetwork();
        if (!packet->readNoHeader(stream, network.getPacketReflectionCtx(), header.getSenderSubId()).ignoreError()) {
            server.getLogger().critical("AsyncBatchedNetworkPeer::sendPacket: Failed to parse packet with id: {}",
                                        static_cast<int>(packet->getId()));
            return std::nullopt;
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
        return std::nullopt;
    }

    if (e.getPayload().data() != payload.data()) {
        BinaryStream out;
        header.write(out);
        out.writeRawBytes(e.getPayload());
        return out.getBuffer();
    }
    return data;
}

std::optional<std::string> AsyncBatchedNetworkPeer::handleReceiveEvent(const std::string &data)
{
    const auto &server = EndstoneServer::getInstance();
    auto network_handler = server.getServer().getMinecraft()->getServerNetworkHandler();

    ReadOnlyBinaryStream stream(data, false);
    auto result = stream.getUnsignedVarInt().discardError();
    if (!result) {
        return std::nullopt;
    }

    const auto header = PacketHeader::fromRaw(result.value());
    const auto &id = getId();
    EndstonePlayer *player = nullptr;
    if (const auto *p = network_handler->getServerPlayer(id, header.getRecipientSubId())) {
        player = &p->getEndstoneActor<EndstonePlayer>();
    }

    const auto payload = stream.getView().substr(stream.getReadPointer());
    PacketReceiveEvent e{player, static_cast<int>(header.getPacketId()), payload,
                         EndstoneSocketAddress::fromNetworkIdentifier(id),
                         static_cast<int>(header.getRecipientSubId())};
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return std::nullopt;
    }

    if (e.getPayload().data() == payload.data()) {
        return data;
    }

    std::string out(data.substr(0, stream.getReadPointer()));
    out.append(e.getPayload().data(), e.getPayload().size());
    return out;
}

void AsyncBatchedNetworkPeer::sendPacket(const std::string &data, Reliability reliability, Compressibility compressible)
{
    auto patched = handleSendEvent(data);
    if (!patched) {
        return;
    }
    BatchedNetworkPeer::sendPacket(*patched, reliability, compressible);
}

void AsyncBatchedNetworkPeer::flush(std::function<void()> &&callback)
{
    if (!activated_) {
        BatchedNetworkPeer::flush(std::move(callback));
        return;
    }

    std::string batch = outgoing_data_.getAndReleaseData();
    if (batch.empty()) {
        if (callback) {
            callback();
        }
        return;
    }

    const auto compressible = getCompressibleState(batch.size());
    compressible_bytes_ = 0;

    auto data = std::make_shared<std::string>(std::move(batch));
    auto self = shared_from_this();
    boost::asio::post(event_loop_, [self, data, compressible] {
        self->peer_->sendPacket(*data, Reliability::ReliableOrdered, compressible);
    });

    // The batch is handed off to the posted task, so the callback runs here on the main thread, where BDS expects it.
    if (callback) {
        callback();
    }
}

void AsyncBatchedNetworkPeer::recvLoop()
{
    constexpr std::size_t recv_queue_capacity = 1024;
    // Decode batches on the worker until the main thread falls behind; the rest waits in the inner peer (never
    // dropped -- that would desync the decrypt counter).
    while (recv_queue_.size_approx() < recv_queue_capacity) {
        std::string packet;
        auto timepoint = std::make_shared<PacketRecvTimepoint>();
        if (BatchedNetworkPeer::_receivePacket(packet, timepoint) != DataStatus::HasData) {
            break;
        }
        recv_queue_.enqueue(std::move(packet));
    }
    recv_scheduled_ = false;
}

NetworkPeer::DataStatus AsyncBatchedNetworkPeer::_receivePacket(std::string &out_data,
                                                                const PacketRecvTimepointPtr &timepoint_ptr)
{
    for (;;) {
        std::string raw;
        DataStatus status;
        if (activated_) {
            status = recv_queue_.try_dequeue(raw) ? DataStatus::HasData : DataStatus::NoData;
        }
        else {
            status = BatchedNetworkPeer::_receivePacket(raw, timepoint_ptr);
        }

        if (status != DataStatus::HasData) {
            return status;
        }

        auto data = handleReceiveEvent(raw);
        if (!data) {
            continue;
        }
        out_data = std::move(*data);
        return DataStatus::HasData;
    }
}

void AsyncBatchedNetworkPeer::update()
{
    if (!activated_ && async_enabled_) {
        activated_ = true;
        BatchedNetworkPeer::flush({});  // drain the pre-auth batch before the event loop takes over the inner chain
    }

    if (!activated_) {
        BatchedNetworkPeer::update();
        return;
    }

    // One event loop serializes recv, send and the inner-chain update. Re-arm recvLoop at most once per tick.
    if (!recv_scheduled_.exchange(true)) {
        auto self = shared_from_this();
        boost::asio::post(event_loop_, [self] { self->recvLoop(); });
    }
    auto self = shared_from_this();
    boost::asio::post(event_loop_, [self] { self->peer_->update(); });
}

}  // namespace endstone::core
