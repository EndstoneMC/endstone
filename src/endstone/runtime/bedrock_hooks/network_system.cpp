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

#include "bedrock/network/packet.h"
#include "bedrock/network/packet/resource_pack_stack_packet.h"
#include "bedrock/network/packet/resource_packs_info_packet.h"
#include "bedrock/network/packet/start_game_packet.h"
#include "endstone/core/level/level.h"
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
}  // namespace

void NetworkSystem::send(const NetworkIdentifier &network_id, const Packet &packet, SubClientId sender_sub_id)
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
                        (static_cast<unsigned>(packet.getClientSubId()) << 12);
    stream.writeUnsignedVarInt(header, "Header Data", nullptr);
    packet.writeWithSerializationMode(stream, getPacketReflectionCtx(),
                                      packet_overrides_->getOverrideModeForPacket(packet.getId()));

    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    const auto *server_player =
        server.getServer().getMinecraft()->getServerNetworkHandler()->getServerPlayer(network_id, sender_sub_id);
    endstone::Player *player = nullptr;
    if (server_player) {
        player = &server_player->getEndstoneActor<endstone::core::EndstonePlayer>();
    }

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
