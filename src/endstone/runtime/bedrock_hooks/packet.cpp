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

#include "endstone/core/server.h"
#include "endstone/runtime/hook.h"

class PlayerPacketHandler : public IPacketHandlerDispatcher {
public:
    explicit PlayerPacketHandler(const IPacketHandlerDispatcher &original) : original_(original) {}
    void handle(const NetworkIdentifier &network_id, NetEventCallback &callback,
                std::shared_ptr<Packet> &packet) const override
    {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        const auto network_handler = server.getServer().getMinecraft()->getServerNetworkHandler();
        if (const auto *p = network_handler->getServerPlayer(network_id, packet->getClientSubId())) {
            if (p->getEndstoneActor<endstone::core::EndstonePlayer>().handlePacket(*packet)) {
                original_.handle(network_id, callback, packet);
            }
        }
    }

private:
    const IPacketHandlerDispatcher &original_;
};

std::shared_ptr<Packet> MinecraftPackets::createPacket(MinecraftPacketIds id)
{
    auto packet = ENDSTONE_HOOK_CALL_ORIGINAL(&MinecraftPackets::createPacket, id);
    switch (id) {
    case MinecraftPacketIds::PlayerEquipment:
    case MinecraftPacketIds::PlayerAction:
    case MinecraftPacketIds::SetLocalPlayerAsInit:
    case MinecraftPacketIds::PlayerAuthInputPacket: {
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
