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

#include "bedrock/scripting/event_handlers/script_server_network_event_handler.h"

#include "endstone/core/server.h"
#include "endstone/event/player/player_chat_event.h"
#include "endstone/event/server/packet_receive_event.h"
#include "endstone/event/server/packet_send_event.h"
#include "endstone/runtime/vtable_hook.h"

namespace {
bool handleEvent(IncomingPacketEvent &event)
{
    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    if (auto *player = WeakEntityRef(event.sender).tryUnwrap<::Player>(); player) {
        // TODO(refactor): add Player::handleDataPacket and call the event there
        if (event.packet_id == MinecraftPacketIds::SetLocalPlayerAsInit) {
            player->getEndstoneActor<endstone::core::EndstonePlayer>().doFirstSpawn();
        }

        const auto &network = server.getServer().getNetwork();
        ReadOnlyBinaryStream stream(network.receive_buffer_, false);
        auto header = stream.getUnsignedVarInt().logError(Bedrock::LogLevel::Error, LogAreaID::Network);
        if (!header.has_value()) {
            return false;
        }
        const auto packet_id = static_cast<int>(header.value() & 0x3ff);
        auto payload = stream.getView().substr(stream.getReadPointer());

        endstone::PacketReceiveEvent e{player->getEndstoneActor<endstone::core::EndstonePlayer>(), packet_id, payload};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }

        if (e.getPayload().data() == payload.data()) {
            return true;  // Nothing to do, the packet is the same, go back to the original handler
        }

        // Plugins have changed the payload, let's handle it ourselves
        auto packet = MinecraftPackets::createPacket(static_cast<MinecraftPacketIds>(packet_id));
        if (!packet) {
            server.getLogger().error("PacketReceiveEvent: Unknown packet id: {}", packet_id);
            return false;
        }

        const auto &network_id = player->getPersistentComponent<UserEntityIdentifierComponent>()->getNetworkId();
        auto *connection = network._getConnectionFromId(network_id);
        if (!connection || connection->shouldCloseConnection()) {
            return true;
        }

        auto now = std::chrono::steady_clock::now();
        connection->last_packet_time = now;
        packet->setReceiveTimestamp(now);

        ReadOnlyBinaryStream read_stream(e.getPayload(), false);
        auto packet_size = read_stream.getView().size() - read_stream.getReadPointer();
        if (auto result = packet->checkSize(packet_size, true); !result.ignoreError()) {
            server.getLogger().error("PacketReceiveEvent: Bad packet size: {}", packet_size);
            return false;
        }

        if (auto result = packet->read(read_stream); !result.ignoreError()) {
            server.getLogger().error("PacketReceiveEvent: Bad packet!");
            return false;
        }

        packet->handle(network_id, *server.getServer().getMinecraft()->getServerNetworkHandler(), packet);
        return false;
    }

    return true;
}

bool handleEvent(OutgoingPacketEvent & /*event*/)
{
    return true;
}

bool handleEvent(ChatEvent &event)
{
    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    if (auto *player = WeakEntityRef(event.sender).tryUnwrap<::Player>(); player) {
        endstone::PlayerChatEvent e{player->getEndstoneActor<endstone::core::EndstonePlayer>(), event.message};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }

        event.message = std::move(e.getMessage());
        server.getLogger().info("<{}> {}", e.getPlayer().getName(), e.getMessage());
    }
    return true;
}
}  // namespace

GameplayHandlerResult<CoordinatorResult> ScriptServerNetworkEventHandler::handleEvent1(
    MutableServerNetworkGameplayEvent<CoordinatorResult> &event)
{
    const auto result = ENDSTONE_VHOOK_CALL_ORIGINAL(&ScriptServerNetworkEventHandler::handleEvent1, this, event);
    if (result.handler_result == HandlerResult::BypassListeners || result.return_value == CoordinatorResult::Cancel) {
        return result;
    }

    auto visitor = [&](auto &&arg) -> GameplayHandlerResult<CoordinatorResult> {
        using T = std::decay_t<decltype(arg)>;
        if (!handleEvent(arg.value())) {
            return {HandlerResult::BypassListeners, CoordinatorResult::Cancel};
        }
        return result;
    };
    return event.visit(visitor);
}
