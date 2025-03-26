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
#include "bedrock/network/packet/start_game_packet.h"
#include "endstone/core/level/level.h"
#include "endstone/core/server.h"
#include "endstone/event/server/packet_send_event.h"
#include "endstone/runtime/hook.h"

namespace {
void patchPacket(const StartGamePacket &packet)
{
    static bool client_side_generation_enabled = []() {
        const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
        const auto *level = static_cast<endstone::core::EndstoneLevel *>(server.getLevel());
        return level->getHandle().isClientSideGenerationEnabled();
    }();

    if (!client_side_generation_enabled) {
        auto &pk = const_cast<StartGamePacket &>(packet);
        pk.settings.setRandomSeed(0);
    }
}

void patchPacket(const ResourcePackStackPacket &packet)
{
    auto &pk = const_cast<ResourcePackStackPacket &>(packet);
    // https://github.com/pmmp/PocketMine-MP/blob/c80a4d/src/network/mcpe/handler/ResourcePacksPacketHandler.php#L177
    // always false, otherwise it may force the client to remove its own non-server-supplied resource packs.
    pk.texture_pack_required = false;
}

}  // namespace

void NetworkSystem::send(const NetworkIdentifier &network_id, const Packet &packet, SubClientId sender_sub_id)
{
    if (packet.getName() != "DataPacket") {
        switch (packet.getId()) {
        case MinecraftPacketIds::StartGame:
            patchPacket(static_cast<const StartGamePacket &>(packet));
            break;
        case MinecraftPacketIds::ResourcePackStack:
            patchPacket(static_cast<const ResourcePackStackPacket &>(packet));
            break;
        default:
            break;
        }
    }

    ENDSTONE_HOOK_CALL_ORIGINAL(&NetworkSystem::send, this, network_id, packet, sender_sub_id);
}

NetworkConnection *NetworkSystem::_getConnectionFromId(const NetworkIdentifier &id) const
{
    for (const auto &connection : connections_) {
        if (connection->id == id) {
            return connection.get();
        }
    }
    return nullptr;
}

void NetworkSystem::_sendInternal(const NetworkIdentifier &id, const Packet &packet, const std::string &data)
{
    const auto *connection = _getConnectionFromId(id);
    if (!connection || connection->shouldCloseConnection()) {
        return;
    }
    if (!connection->peer) {
        return;
    }
    connection->peer->sendPacket(data, packet.getReliability(), packet.getCompressible());
}
