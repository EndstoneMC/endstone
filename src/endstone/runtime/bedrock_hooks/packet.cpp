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

#include <endstone/core/util/uuid.h>

#include "bedrock/network/packet/login_packet.h"
#include "bedrock/network/packet/sub_client_login_packet.h"
#include "endstone/core/server.h"
#include "endstone/core/util/socket_address.h"
#include "endstone/runtime/hook.h"

namespace {
class LoginPacketHandler : public IPacketHandlerDispatcher {
public:
    explicit LoginPacketHandler(const IPacketHandlerDispatcher &original) : original_(original) {}
    void handle(const NetworkIdentifier &network_id, NetEventCallback &callback,
                std::shared_ptr<Packet> &packet) const override
    {
        // Run the original logics first so invalid login attempts will be rejected right away.
        original_.handle(network_id, callback, packet);

        // Endstone begins: additional checks e.g. ban / ban-ip
        const auto &server = endstone::core::EndstoneServer::getInstance();
        auto address = endstone::core::EndstoneSocketAddress::fromNetworkIdentifier(network_id);
        if (server.getIpBanList().isBanned(address.getHostname())) {
            server.getServer().getMinecraft()->getServerNetworkHandler()->disconnect(
                network_id, SubClientId::PrimaryClient, "You have been IP banned from this server.");
            return;
        }

        auto &pk = static_cast<LoginPacket &>(*packet);
        if (!pk.connection_request) {
            return;
        }

        const auto &info = pk.connection_request->getAuthenticationInfo();
        const auto name = info.xuid.empty() ? pk.connection_request->getClientThirdPartyName() : info.xbox_live_name;
        const auto uuid = endstone::core::EndstoneUUID::fromMinecraft(info.authenticated_uuid);
        const auto xuid = info.xuid;
        if (server.getBanList().isBanned(name, uuid, xuid)) {
            const gsl::not_null ban_entry = server.getBanList().getBanEntry(name, uuid, xuid);
            if (const auto reason = ban_entry->getReason(); !reason.empty()) {
                server.getServer().getMinecraft()->getServerNetworkHandler()->disconnect(
                    network_id, SubClientId::PrimaryClient, "You have been banned from this server. Reason: " + reason);
            }
            else {
                server.getServer().getMinecraft()->getServerNetworkHandler()->disconnect(
                    network_id, SubClientId::PrimaryClient, "You have been banned from this server.");
            }
            return;
        }
        // TODO(event): PlayerPreLoginEvent
        // Endstone ends
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
        if (const auto *p = network_handler->getServerPlayer(network_id, packet->getClientSubId())) {
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
    case MinecraftPacketIds::Login: {
        static LoginPacketHandler handler(*packet->handler_);
        packet->handler_ = &handler;
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
