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

#include "endstone/core/event/handlers/server_network_event_handler.h"

#include "endstone/core/server.h"
#include "endstone/event/player/player_chat_event.h"
#include "endstone/event/server/packet_receive_event.h"
#include "endstone/event/server/packet_send_event.h"

namespace endstone::core {

EndstoneServerNetworkEventHandler::EndstoneServerNetworkEventHandler(std::unique_ptr<ServerNetworkEventHandler> handle)
    : handle_(std::move(handle))
{
}

GameplayHandlerResult<CoordinatorResult> EndstoneServerNetworkEventHandler::handleEvent(
    MutableServerNetworkGameplayEvent<CoordinatorResult> &event)
{
    auto visitor = [&](auto &&arg) -> GameplayHandlerResult<CoordinatorResult> {
        using T = std::decay_t<decltype(arg)>;
        if (!handleEvent(arg.value())) {
            return {HandlerResult::BypassListeners, CoordinatorResult::Cancel};
        }
        return handle_->handleEvent(event);
    };
    return event.visit(visitor);
}

std::unique_ptr<ServerNetworkEventHandler> EndstoneServerNetworkEventHandler::unwrap()
{
    return std::move(handle_);
}

bool EndstoneServerNetworkEventHandler::handleEvent(ChatEvent &event)
{
    const auto &server = entt::locator<EndstoneServer>::value();
    if (auto *player = WeakEntityRef(event.sender).tryUnwrap<::Player>(); player) {
        PlayerChatEvent e{player->getEndstoneActor<EndstonePlayer>(), event.message};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }

        event.message = std::move(e.getMessage());
        server.getLogger().info("<{}> {}", e.getPlayer().getName(), e.getMessage());
    }
    return true;
}

bool EndstoneServerNetworkEventHandler::handleEvent(IncomingPacketEvent &event)
{
    const auto &server = entt::locator<EndstoneServer>::value();
    if (auto *player = WeakEntityRef(event.sender).tryUnwrap<::Player>(); player) {
        const auto &network = server.getServer().getNetwork();
        // TODO(refactor): add Player::handleDataPacket and call the event there
        ReadOnlyBinaryStream stream(network.receive_buffer_, false);
        auto header = stream.getUnsignedVarInt().logError(Bedrock::LogLevel::Error, LogAreaID::Network);
        if (!header.has_value()) {
            return false;
        }
        const auto packet_id = static_cast<int>(header.value() & 0x3ff);
        const auto sender_sub_id = (header.value() >> 10) & 3;
        const auto target_sub_id = (header.value() >> 12) & 3;
        PacketReceiveEvent e{player->getEndstoneActor<EndstonePlayer>(), packet_id,
                             stream.getView().substr(stream.getReadPointer())};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }

        if (event.packet_id == MinecraftPacketIds::SetLocalPlayerAsInit) {
            static_cast<::ServerPlayer *>(player)->setLocalPlayerAsInitialized();
        }
    }

    return true;
}

bool EndstoneServerNetworkEventHandler::handleEvent(OutgoingPacketEvent &event)
{
    const auto &server = entt::locator<EndstoneServer>::value();
    for (auto it = event.recipients.begin(); it != event.recipients.end();) {
        const auto &recipient = *it;
        if (const auto *player = WeakEntityRef(recipient).tryUnwrap<::Player>(); player) {
            const auto &network = server.getServer().getNetwork();
            ReadOnlyBinaryStream stream(network.send_stream_.getView(), false);
            auto header = stream.getUnsignedVarInt().logError(Bedrock::LogLevel::Error, LogAreaID::Network);
            if (!header.has_value()) {
                return false;
            }
            const auto packet_id = static_cast<int>(header.value() & 0x3ff);
            const auto sender_sub_id = (header.value() >> 10) & 3;
            const auto target_sub_id = (header.value() >> 12) & 3;
            PacketSendEvent e{player->getEndstoneActor<EndstonePlayer>(), packet_id,
                              stream.getView().substr(stream.getReadPointer())};
            server.getPluginManager().callEvent(e);
            if (e.isCancelled()) {
                it = event.recipients.erase(it);
            }
            else {
                ++it;
            }
        }
    }
    return true;
}

}  // namespace endstone::core
