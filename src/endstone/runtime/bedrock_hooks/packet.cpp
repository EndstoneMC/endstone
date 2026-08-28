
#include <unordered_map>
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

#include "bedrock/network/packet.h"
#include "bedrock/network/packet/request_network_settings_packet.h"
#include "bedrock/server/server_instance.h"
#include "bedrock/shared_constants.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/core/util/socket_address.h"
#include "endstone/core/util/uuid.h"
#include "endstone/runtime/hook.h"

namespace {
// TODO(1.26.50): drop with the rest of the 1.26.44 shims once 1.26.44 clients are gone.
void acceptWireCompatibleProtocol(int &client_network_version)
{
    if (client_network_version == 2168) {
        client_network_version = SharedConstants::NetworkProtocolVersion;
    }
}

class ProtocolVersionHandler : public IPacketHandlerDispatcher {
public:
    explicit ProtocolVersionHandler(const IPacketHandlerDispatcher &original) : original_(original) {}
    void handle(const NetworkIdentifier &network_id, NetEventCallback &callback,
                std::shared_ptr<Packet> &packet) const override
    {
        switch (packet->getId()) {
        case MinecraftPacketIds::RequestNetworkSettings:
            acceptWireCompatibleProtocol(
                static_cast<RequestNetworkSettingsPacket &>(*packet).payload.client_network_version);
            break;
        default:
            break;
        }
        original_.handle(network_id, callback, packet);
    }

private:
    const IPacketHandlerDispatcher &original_;
};

class PlayerPacketHandler : public IPacketHandlerDispatcher {
public:
    explicit PlayerPacketHandler(const IPacketHandlerDispatcher &original) : original_(original) {}
    void handle(const NetworkIdentifier &network_id, NetEventCallback &callback,
                std::shared_ptr<Packet> &packet) const override
    {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        const auto network_handler = server.getServer().getMinecraft()->getServerNetworkHandler();
        if (const auto *p = network_handler->getServerPlayer(network_id, packet->getSenderSubId())) {
            if (p->getEndstoneActor<endstone::core::EndstonePlayer>().handlePacket(*packet)) {
                original_.handle(network_id, callback, packet);
            }
        }
    }

private:
    const IPacketHandlerDispatcher &original_;
};
}  // namespace

std::shared_ptr<Packet> MinecraftPackets::createPacket(MinecraftPacketIds id)
{
    auto packet = ENDSTONE_HOOK_CALL_ORIGINAL(&MinecraftPackets::createPacket, id);
    switch (id) {
    case MinecraftPacketIds::RequestNetworkSettings: {
        static std::unordered_map<MinecraftPacketIds, std::unique_ptr<ProtocolVersionHandler>> handlers;
        if (packet->handler_) {
            handlers.emplace(id, std::make_unique<ProtocolVersionHandler>(*packet->handler_));
            packet->handler_ = handlers[id].get();
        }
        break;
    }
    case MinecraftPacketIds::PlayerEquipment:
    case MinecraftPacketIds::PlayerAction:
    case MinecraftPacketIds::PlayerSkin:
    case MinecraftPacketIds::SetLocalPlayerAsInit:
    case MinecraftPacketIds::PlayerAuthInputPacket:
    case MinecraftPacketIds::Emote: {
        static std::unordered_map<MinecraftPacketIds, std::unique_ptr<PlayerPacketHandler>> handlers;
        if (packet->handler_) {
            handlers.emplace(id, std::make_unique<PlayerPacketHandler>(*packet->handler_));
            packet->handler_ = handlers[id].get();
        }
        break;
    }
    default:
        break;
    }
    return packet;
}
