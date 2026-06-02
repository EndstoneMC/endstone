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
                static_cast<MapDecoration::Type>(cursor.getType()), cursor.getX(), cursor.getY(),
                cursor.getDirection(), cursor.getCaption(), mce::Color::WHITE  // TODO(map): support different colors
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
            patchPacket(static_cast<const ClientboundMapItemDataPacket &>(packet), static_cast<EndstonePlayer &>(*player));
        }
        break;
    default:
        break;
    }
}
}  // namespace

AsyncBatchedNetworkPeer::AsyncBatchedNetworkPeer(std::shared_ptr<NetworkPeer> compressed_peer, NetworkThreadPool &pool)
    : pool_(pool), state_(std::make_shared<AsyncConnectionState>(pool, compressed_peer))
{
    peer_ = std::move(compressed_peer);  // NetworkPeer::peer_ (inner chain root)
    mAsyncEnabled = false;
}

AsyncBatchedNetworkPeer::~AsyncBatchedNetworkPeer()
{
    // Flip the close flag and post a fence; the strand-captured shared_ptr<AsyncConnectionState> keeps the state
    // (and inner peer) alive until the last in-flight task drops it. Never block the main thread here.
    state_->closing = true;
    auto state = state_;
    boost::asio::post(state->recv_strand, [state] { (void)state; });
    boost::asio::post(state->send_strand, [state] { (void)state; });
}

void AsyncBatchedNetworkPeer::splice(NetworkConnection &connection, NetworkThreadPool &pool)
{
    auto old_batched = connection.batched_peer.lock();
    if (!old_batched) {
        return;  // local / encryption-disabled fast path: no Batched layer to replace
    }

    // Wrap the SAME CompressedNetworkPeer the old Batched wrapped (copied shared_ptr keeps the inner chain alive).
    auto new_async = std::make_shared<AsyncBatchedNetworkPeer>(old_batched->peer_, pool);

    if (connection.peer == old_batched) {
        connection.peer = new_async;  // Batched was outermost (no PacketTrace)
    }
    else {
        NetworkPeer *cur = connection.peer.get();  // walk PacketTrace (-> Latency?) -> ... to the holder
        while (cur->peer_ && cur->peer_ != old_batched) {
            cur = cur->peer_.get();
        }
        if (cur->peer_ != old_batched) {
            return;  // unexpected chain shape; leave BDS untouched
        }
        cur->peer_ = new_async;  // splice in place of the old Batched
    }
    connection.batched_peer = new_async;  // type-compatible (we derive from BatchedNetworkPeer)
    // Stay synchronous; the recv loop is armed lazily once mAsyncEnabled flips post-auth (see update()).
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
        return std::nullopt;  // broken sub-packet; skip
    }

    const auto header = PacketHeader::fromRaw(result.value());
    const auto &id = getId();
    EndstonePlayer *player = nullptr;
    if (const auto *p = network_handler->getServerPlayer(id, header.getRecipientSubId())) {
        player = &p->getEndstoneActor<EndstonePlayer>();
    }

    const auto payload = stream.getView().substr(stream.getReadPointer());
    PacketReceiveEvent e{player, static_cast<int>(header.getPacketId()), payload,
                         EndstoneSocketAddress::fromNetworkIdentifier(id), static_cast<int>(header.getRecipientSubId())};
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return std::nullopt;
    }

    if (e.getPayload().data() == payload.data()) {
        return data;  // unchanged
    }

    std::string out(data.substr(0, stream.getReadPointer()));
    out.append(e.getPayload().data(), e.getPayload().size());
    return out;
}

void AsyncBatchedNetworkPeer::sendPacket(const std::string &data, Reliability /*reliability*/,
                                         Compressibility compressible)
{
    auto patched = handleSendEvent(data);
    if (!patched) {
        return;  // cancelled or broken
    }
    outgoing_.writeString(*patched, "Data", nullptr);
    if (compressible == Compressibility::Compressible) {
        compressible_bytes_ += patched->size();
    }
}

NetworkPeer::NetworkStatus AsyncBatchedNetworkPeer::getNetworkStatus() const
{
    return peer_->getNetworkStatus();
}

bool AsyncBatchedNetworkPeer::isLocal() const
{
    return peer_->isLocal();
}

bool AsyncBatchedNetworkPeer::isEncrypted() const
{
    return peer_->isEncrypted();
}

bool AsyncBatchedNetworkPeer::isLan() const
{
    return peer_->isLan();
}

Compressibility AsyncBatchedNetworkPeer::computeCompressibility() const
{
    const auto total = static_cast<float>(outgoing_.getBuffer().size());
    if (total <= 0.0F) {
        return Compressibility::Incompressible;
    }
    const auto ratio = static_cast<float>(compressible_bytes_) / total;
    // BDS: little compressible content -> don't compress.
    return ratio <= 0.2F ? Compressibility::Incompressible : Compressibility::Compressible;
}

void AsyncBatchedNetworkPeer::flushSync()
{
    if (outgoing_.getBuffer().empty()) {
        return;
    }
    const auto compressible = computeCompressibility();
    std::string batch = outgoing_.getAndReleaseData();
    compressible_bytes_ = 0;
    peer_->sendPacket(batch, Reliability::ReliableOrdered, compressible);
}

void AsyncBatchedNetworkPeer::flush(std::function<void()> &&callback)
{
    if (!mAsyncEnabled) {
        flushSync();
        if (callback) {
            callback();
        }
        return;
    }

    if (outgoing_.getBuffer().empty()) {
        if (callback) {
            callback();
        }
        return;
    }

    const auto compressible = computeCompressibility();
    auto batch = std::make_shared<std::string>(outgoing_.getAndReleaseData());
    compressible_bytes_ = 0;

    auto state = state_;
    auto cb = std::make_shared<std::function<void()>>(std::move(callback));
    boost::asio::post(state->send_strand, [state, batch, compressible, cb] {
        if (!state->closing) {
            state->inner_peer->sendPacket(*batch, NetworkPeer::Reliability::ReliableOrdered, compressible);
        }
        if (*cb) {
            NetworkThreadPool::getInstance().postToMain([cb] { (*cb)(); });
        }
    });
}

NetworkPeer::DataStatus AsyncBatchedNetworkPeer::splitNext(AsyncConnectionState &state, std::string &out)
{
    for (;;) {
        if (!state.incoming || state.incoming->getUnreadLength() == 0) {
            std::string batch;
            auto timepoint = std::make_shared<NetworkPeer::PacketRecvTimepoint>();
            const auto status = state.inner_peer->_receivePacket(batch, timepoint);
            if (status != DataStatus::HasData) {
                return status;
            }
            state.incoming_buffer = std::move(batch);
            state.incoming.emplace(state.incoming_buffer, false);
        }

        auto &stream = *state.incoming;
        auto len_result = stream.getUnsignedVarInt().discardError();
        if (!len_result) {
            state.incoming.reset();
            return DataStatus::BrokenData;
        }
        const auto len = len_result.value();
        const auto view = stream.getView();
        const auto start = stream.getReadPointer();
        if (start + len > view.size()) {
            state.incoming.reset();
            return DataStatus::BrokenData;
        }
        out.assign(view.substr(start, len));
        stream.setReadPointer(start + len);
        return DataStatus::HasData;
    }
}

void AsyncBatchedNetworkPeer::recvLoop(std::shared_ptr<AsyncConnectionState> state)
{
    while (!state->closing) {
        {
            // Backpressure: once this many decoded sub-packets await the main thread, stop draining RakNet (it
            // buffers upstream) until the main thread catches up. Never dropped (would break the decrypt chain).
            constexpr std::size_t inbound_capacity = 1024;
            std::lock_guard lock(state->inbound_mutex);
            if (state->inbound.size() >= inbound_capacity) {
                break;
            }
        }

        std::string packet;
        const auto status = splitNext(*state, packet);
        if (status != DataStatus::HasData) {
            break;  // NoData / BrokenData
        }

        std::lock_guard lock(state->inbound_mutex);
        state->inbound.push(std::move(packet));
    }
    state->recv_scheduled = false;
}

NetworkPeer::DataStatus AsyncBatchedNetworkPeer::_receivePacket(std::string &out_data,
                                                                const PacketRecvTimepointPtr &timepoint_ptr)
{
    for (;;) {
        std::string raw;
        DataStatus status;
        if (!activated_) {
            // Pre-activation: split synchronously on the main thread (no strand running yet).
            (void)timepoint_ptr;
            status = splitNext(*state_, raw);
        }
        else {
            std::lock_guard lock(state_->inbound_mutex);
            if (state_->inbound.empty()) {
                status = DataStatus::NoData;
            }
            else {
                raw = std::move(state_->inbound.front());
                state_->inbound.pop();
                status = DataStatus::HasData;
            }
        }

        if (status != DataStatus::HasData) {
            return status;
        }

        auto patched = handleReceiveEvent(raw);
        if (!patched) {
            continue;  // cancelled -> try the next packet
        }
        out_data = std::move(*patched);
        return DataStatus::HasData;
    }
}

void AsyncBatchedNetworkPeer::update()
{
    if (!activated_ && mAsyncEnabled) {
        // Transition to async: drain any pending pre-auth batch synchronously, then start the recv loop.
        activated_ = true;
        flushSync();
    }

    if (activated_) {
        // Re-arm the recv loop once per tick if it isn't already scheduled.
        if (!state_->recv_scheduled.exchange(true)) {
            auto state = state_;
            boost::asio::post(state->recv_strand, [state] { recvLoop(state); });
        }
        // Serialize the inner-chain update with sends (send_strand) to avoid racing recvLoop.
        auto state = state_;
        boost::asio::post(state->send_strand, [state] {
            if (!state->closing) {
                state->inner_peer->update();
            }
        });
        pool_.drainMainQueue();
    }
    else {
        flushSync();
        peer_->update();
    }
}

}  // namespace endstone::core
