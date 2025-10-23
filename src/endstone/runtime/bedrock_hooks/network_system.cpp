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

#include "bedrock/network/network_system.h"

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

#include "bedrock/network/packet.h"
#include "bedrock/network/packet/clientbound_map_item_data_packet.h"
#include "bedrock/network/packet/resource_pack_stack_packet.h"
#include "bedrock/network/packet/resource_packs_info_packet.h"
#include "bedrock/network/packet/start_game_packet.h"
#include "endstone/core/level/level.h"
#include "endstone/core/map/map_view.h"
#include "endstone/core/server.h"
#include "endstone/core/util/socket_address.h"
#include "endstone/event/server/packet_send_event.h"
#include "endstone/runtime/hook.h"

namespace {
void patchPacket(const StartGamePacket &packet)
{
    if (packet.getName() != "StartGamePacket") {
        return;
    }
    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    const auto *level = static_cast<endstone::core::EndstoneLevel *>(server.getLevel());
    if (level && !level->getHandle().isClientSideGenerationEnabled()) {
        auto &pk = const_cast<StartGamePacket &>(packet);
        pk.settings.setRandomSeed(0);
    }
}

void patchPacket(const ResourcePacksInfoPacket &packet)
{
    if (packet.getName() != "ResourcePacksInfoPacket") {
        return;
    }
    // TODO(refactor): inject content key here instead of adding to ServerNetworkHandler
}

void patchPacket(const ResourcePackStackPacket &packet)
{
    if (packet.getName() != "ResourcePackStackPacket") {
        return;
    }
    if (packet.texture_pack_required) {
        const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
        if (server.getAllowClientPacks()) {
            auto &pk = const_cast<ResourcePackStackPacket &>(packet);
            // false, otherwise the client will remove its own non-server-supplied resource packs.
            pk.texture_pack_required = false;
        }
    }
}

void patchPacket(const ClientboundMapItemDataPacket &packet, endstone::core::EndstonePlayer &player)
{
    if (packet.getName() != "ClientboundMapItemDataPacket") {
        return;
    }

    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto *map = static_cast<endstone::core::EndstoneMapView *>(server.getMap(packet.getMapId().raw_id));
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
            pk.decorations_.emplace_back(
                std::make_shared<MapDecoration>(static_cast<MapDecoration::Type>(cursor.getType()), cursor.getX(),
                                                cursor.getY(), cursor.getDirection(), cursor.getCaption(),
                                                mce::Color::WHITE  // TODO(map): support different colors
                                                ));
        }
    }
}
}  // namespace

void NetworkSystem::send(const NetworkIdentifier &network_id, const Packet &packet, SubClientId sender_sub_id)
{
    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    const auto *server_player =
        server.getServer().getMinecraft()->getServerNetworkHandler()->getServerPlayer(network_id, sender_sub_id);
    endstone::Player *player = nullptr;
    if (server_player) {
        player = &server_player->getEndstoneActor<endstone::core::EndstonePlayer>();
    }

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

    std::vector<NetworkIdentifierWithSubId> recipients;
    recipients.emplace_back(network_id, sender_sub_id);
    for (const auto &queue : incoming_packets) {
        if (!queue) {
            continue;
        }
        if (queue->callback_obj.allowOutgoingPacket(recipients, packet) != OutgoingPacketFilterResult::Allowed) {
            return;
        }
    }

    BinaryStream stream;
    const auto packet_id = static_cast<int>(packet.getId());
    const auto header = packet_id | (static_cast<unsigned>(sender_sub_id) << 10) |
                        (static_cast<unsigned>(packet.getSenderSubId()) << 12);
    stream.writeUnsignedVarInt(header, "Header Data", nullptr);
    packet.writeWithSerializationMode(stream, getPacketReflectionCtx(),
                                      packet_overrides_->getOverrideModeForPacket(packet.getId()));

    ReadOnlyBinaryStream read_stream(stream.getView(), false);
    read_stream.getUnsignedVarInt();
    auto payload = read_stream.getView().substr(read_stream.getReadPointer());
    endstone::PacketSendEvent e{player, packet_id, payload,
                                endstone::core::EndstoneSocketAddress::fromNetworkIdentifier(network_id),
                                static_cast<int>(sender_sub_id)};
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return;
    }

    if (e.getPayload().data() != payload.data()) {
        // Plugins have changed the payload, let's re-encode the packet
        stream.reset();
        stream.writeUnsignedVarInt(header, "Header Data", nullptr);
        stream.writeRawBytes(e.getPayload());
    }

    if (auto *connection = _getConnectionFromId(network_id)) {
        connection->last_packet_time = std::chrono::steady_clock::now();
    }
    _sendInternal(network_id, packet, stream.getBuffer());
}

void NetworkSystem::sendToMultiple(const std::vector<NetworkIdentifierWithSubId> &recipients, const Packet &packet)
{
    for (const auto &recipient : recipients) {
        send(recipient.id, packet, recipient.sub_client_id);
    }
}
