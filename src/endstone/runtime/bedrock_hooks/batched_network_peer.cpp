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

#include <optional>
#include <string>
#include <string_view>

#include "bedrock/core/sem_ver/sem_version.h"
#include "bedrock/network/packet.h"
#include "bedrock/network/packet/clientbound_map_item_data_packet.h"
#include "bedrock/network/packet/resource_pack_stack_packet.h"
#include "bedrock/network/packet/resource_packs_info_packet.h"
#include "bedrock/network/packet/set_score_packet.h"
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

void patchPacket(const ClientboundMapItemDataPacket &packet,
                 const endstone::NotNull<endstone::core::EndstonePlayer> &player)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto *map = static_cast<endstone::core::EndstoneMapView *>(server.getMap(packet.getMapId().raw_id));
    if (!map) {
        return;
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

    // Tracked actor ids and decorations go on the wire as parallel arrays
    pk.payload.unique_ids.clear();
    pk.payload.decorations.clear();
    for (const auto &cursor : render.cursors) {
        if (cursor.isVisible()) {
            pk.payload.unique_ids.emplace_back(ActorUniqueID::INVALID_ID);
            pk.payload.decorations.emplace_back(
                std::make_shared<MapDecoration>(static_cast<MapDecoration::Type>(cursor.getType()), cursor.getX(),
                                                cursor.getY(), cursor.getDirection(), cursor.getCaption(),
                                                mce::Color::WHITE  // TODO(map): support different colors
                                                ));
        }
    }
}

// #blameMojang - 1.26.44 writes a fixed `true` ahead of RemoveScore's objective name but left the
// protocol version at 2168, so a 1.26.40-43 client negotiates the same version and mis-parses every
// scoreboard removal.
// TODO(1.26.50): drop once the protocol version moves past 2168.
std::optional<std::string> downgradeSetScorePayload(std::string_view payload)
{
    ReadOnlyBinaryStream in{payload, false};
    auto count = in.getUnsignedVarInt().discardError();
    if (!count) {
        return std::nullopt;
    }

    BinaryStream out;
    out.writeUnsignedVarInt(count.value(), "Score Info", nullptr);

    for (unsigned int i = 0; i < count.value(); ++i) {
        auto action = in.getUnsignedVarInt().discardError();
        if (!action) {
            return std::nullopt;
        }
        const auto entry_action = static_cast<ScorePacketEntryAction>(action.value());
        if (entry_action > ScorePacketEntryAction::ChangeFakePlayer) {
            return std::nullopt;
        }
        out.writeUnsignedVarInt(action.value(), "Action", nullptr);

        auto action_name = in.getString(32).discardError();
        if (!action_name) {
            return std::nullopt;
        }
        out.writeString(action_name.value(), "Action", nullptr);

        auto scoreboard_id = in.getVarInt64().discardError();
        if (!scoreboard_id) {
            return std::nullopt;
        }
        out.writeVarInt64(scoreboard_id.value(), "Scoreboard Id", nullptr);

        if (entry_action == ScorePacketEntryAction::Remove) {
            auto keyed_marker = in.getBool().discardError();
            if (!keyed_marker) {
                return std::nullopt;
            }
            auto has_objective_name = in.getBool().discardError();
            if (!has_objective_name) {
                return std::nullopt;
            }
            out.writeBool(has_objective_name.value(), "Objective Name", nullptr);
            if (has_objective_name.value()) {
                auto objective_name = in.getString(in.getUnreadLength()).discardError();
                if (!objective_name) {
                    return std::nullopt;
                }
                out.writeString(objective_name.value(), "Objective Name", nullptr);
            }
            continue;
        }

        auto objective_name = in.getString(in.getUnreadLength()).discardError();
        if (!objective_name) {
            return std::nullopt;
        }
        out.writeString(objective_name.value(), "Objective Name", nullptr);

        auto score_value = in.getSignedInt().discardError();
        if (!score_value) {
            return std::nullopt;
        }
        out.writeSignedInt(score_value.value(), "Score Value", nullptr);

        if (entry_action == ScorePacketEntryAction::ChangeFakePlayer) {
            auto fake_player_name = in.getString(in.getUnreadLength()).discardError();
            if (!fake_player_name) {
                return std::nullopt;
            }
            out.writeString(fake_player_name.value(), "Fake Player Name", nullptr);
        }
        else {
            auto unique_id = in.getVarInt64().discardError();
            if (!unique_id) {
                return std::nullopt;
            }
            out.writeVarInt64(unique_id.value(), "Player Unique Id", nullptr);
        }
    }

    if (in.getUnreadLength() != 0) {
        return std::nullopt;
    }
    return out.getBuffer();
}

void patchPacket(Packet &packet, const endstone::Nullable<endstone::Player> &player)
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
                        player.cast<endstone::core::EndstonePlayer>());
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
    endstone::Nullable<endstone::Player> player;
    if (server_player) {
        player = server_player->getEndstoneActor<endstone::core::EndstonePlayer>();
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
    case MinecraftPacketIds::SetScore: {
        // TODO(1.26.50): drop with downgradeSetScorePayload once the protocol version moves past 2168.
        if (player) {
            SemVersion client_version;
            auto result =
                SemVersion::fromString(player->getGameVersion(), client_version, SemVersion::ParseOption::NoWildcards);
            if (result != SemVersion::MatchType::None && client_version >= SemVersion{1, 26, 40} &&
                client_version < SemVersion{1, 26, 44}) {
                if (auto downgraded = downgradeSetScorePayload(payload)) {
                    e.setPayload(*downgraded);
                }
            }
        }
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
        endstone::Nullable<endstone::Player> player;
        if (const auto *p = network_handler->getServerPlayer(id, header.getRecipientSubId())) {
            player = p->getEndstoneActor<endstone::core::EndstonePlayer>();
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
    // The innermost peer is transport-specific, so find the connection we belong to instead.
    static const NetworkIdentifier invalid = [] {
        NetworkIdentifier id{};
        id.type = NetworkIdentifier::Type::Invalid;
        return id;
    }();

    const auto &server = endstone::core::EndstoneServer::getInstance();
    for (const auto &connection : server.getServer().getNetwork().getConnections()) {
        for (const auto *peer = connection->peer.get(); peer != nullptr; peer = peer->peer_.get()) {
            if (peer == this) {
                return connection->id;
            }
        }
    }
    return invalid;
}
