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

#include "bedrock/network/server_network_handler.h"

#include <entt/entt.hpp>
#include <magic_enum/magic_enum.hpp>

#include "bedrock/locale/i18n.h"
#include "bedrock/network/packet/disconnect_packet.h"
#include "endstone/core/entity/components/flag_components.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/core/util/socket_address.h"
#include "endstone/event/player/player_kick_event.h"
#include "endstone/event/player/player_login_event.h"
#include "endstone/runtime/hook.h"

void ServerNetworkHandler::disconnectClientWithMessage(const NetworkIdentifier &id, const SubClientId sub_id,
                                                       const Connection::DisconnectFailReason reason,
                                                       const std::string &message,
                                                       std::optional<std::string> filtered_message, bool skip_message)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto disconnect_message = message;
    if (auto *player = getServerPlayer(id, sub_id)) {
        if (player->hasComponent<endstone::core::InternalDisconnectFlagComponent>()) {
            player->addOrRemoveComponent<endstone::core::InternalDisconnectFlagComponent>(false);
        }
        else {
            auto kick_message = getI18n().get(message, nullptr);
            endstone::PlayerKickEvent e{player->getEndstoneActor<endstone::core::EndstonePlayer>(), kick_message};
            server.getPluginManager().callEvent(e);
            if (e.isCancelled()) {
                return;
            }
            if (e.getReason() != disconnect_message) {
                disconnect_message = e.getReason();
            }
        }
    }

    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerNetworkHandler::disconnectClientWithMessage, this, id, sub_id, reason,
                                disconnect_message, std::move(filtered_message), skip_message);

    // BUGFIX:
    // Close the connection immediately so no further packets from this client can be processed.
    // The original code sends a disconnection packet to the client and waits for the client to confirm.
    // A malicious client may ignore it and keep sending packets, making the server vulnerable to packet spam.
    if (sub_id == SubClientId::PrimaryClient) {
        server.getServer().getNetwork().closeConnection(id, reason, message);
    }
}

bool ServerNetworkHandler::tryToLoadPlayer(ServerPlayer &server_player, const ConnectionRequest &connection_request,
                                          const PlayerAuthenticationInfo &player_info)
{
    const auto new_player = ENDSTONE_HOOK_CALL_ORIGINAL(&ServerNetworkHandler::tryToLoadPlayer, this, server_player,
                                                        connection_request, player_info);
    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto &endstone_player = server_player.getEndstoneActor<endstone::core::EndstonePlayer>();
    endstone_player.initFromConnectionRequest(&connection_request);

    endstone::PlayerLoginEvent e{endstone_player};
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        const auto identifier = server_player.getPersistentComponent<UserEntityIdentifierComponent>();
        disconnect(identifier->getNetworkId(), identifier->getSubClientId(), e.getKickMessage());
    }
    return new_player;
}

// ServerPlayer &ServerNetworkHandler::_createNewPlayer(const NetworkIdentifier &network_id,
//                                                      const SubClientConnectionRequest &sub_client_connection_request,
//                                                      const PlayerAuthenticationInfo &player_info,
//                                                      SubClientId sub_client_id)
// {
//     auto &server_player = ENDSTONE_HOOK_CALL_ORIGINAL(&ServerNetworkHandler::_createNewPlayer, this, network_id,
//                                                       sub_client_connection_request, player_info, sub_client_id);
//     const auto &server = endstone::core::EndstoneServer::getInstance();
//     auto &endstone_player = server_player.getEndstoneActor<endstone::core::EndstonePlayer>();
//     endstone_player.initFromConnectionRequest(&sub_client_connection_request);
//
//     endstone::PlayerLoginEvent e{endstone_player};
//     server.getPluginManager().callEvent(e);
//
//     if (e.isCancelled()) {
//         const auto identifier = server_player.getPersistentComponent<UserEntityIdentifierComponent>();
//         disconnect(identifier->getNetworkId(), identifier->getSubClientId(), e.getKickMessage());
//     }
//     return server_player;
// }

ServerPlayer *ServerNetworkHandler::getServerPlayer(const NetworkIdentifier &source, SubClientId sub_id)
{
    for (const auto &entity_context : level_->getUsers()) {
        if (!entity_context.hasValue()) {
            continue;
        }
        auto &player = entity_context.value();
        const auto *component = player.tryGetComponent<UserEntityIdentifierComponent>();
        if (!component) {
            continue;
        }

        if (component->getNetworkId() == source && component->getSubClientId() == sub_id) {
            return static_cast<ServerPlayer *>(Actor::tryGetFromEntity(player));
        }
    }
    return nullptr;
}

void ServerNetworkHandler::disconnect(NetworkIdentifier const &network_id, SubClientId sub_client_id,
                                      std::string const &reason)
{
    // Add InternalDisconnectFlagComponent to indicate this is an intended disconnect by us internally
    // to avoid calling PlayerKickEvent
    if (auto *player = getServerPlayer(network_id, sub_client_id)) {
        player->addOrRemoveComponent<endstone::core::InternalDisconnectFlagComponent>(true);
    }
    disconnectClientWithMessage(network_id, sub_client_id, Connection::DisconnectFailReason::NoReason, reason,
                                std::nullopt, false);
}

std::optional<PlayerAuthenticationInfo> ServerNetworkHandler::_validateLoginPacket(const NetworkIdentifier &source,
                                                                                   const LoginPacket &packet)
{
    // Check for IP-bans
    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto address = endstone::core::EndstoneSocketAddress::fromNetworkIdentifier(source);
    auto network_handler = server.getServer().getMinecraft()->getServerNetworkHandler();
    if (server.getIpBanList().isBanned(address.getHostname())) {
        network_handler->disconnect(source, SubClientId::PrimaryClient, "You have been IP banned from this server.");
        return std::nullopt;
    }

    if (!packet.payload.connection_request) {
        return std::nullopt;
    }

    // Run the original logics first so invalid login attempts will be rejected right away.
    auto auth_info = ENDSTONE_HOOK_CALL_ORIGINAL(&ServerNetworkHandler::_validateLoginPacket, this, source, packet);
    if (!auth_info) {
        return auth_info;
    }

    const auto &info = *auth_info;
    const auto &name =
        info.xuid.empty() ? packet.payload.connection_request->getClientThirdPartyName() : info.xbox_live_name;
    const auto uuid = endstone::core::EndstoneUUID::fromMinecraft(info.authenticated_uuid);
    const auto &xuid = info.xuid;
    if (server.getBanList().isBanned(name, uuid, xuid)) {
        const gsl::not_null ban_entry = server.getBanList().getBanEntry(name, uuid, xuid);
        if (const auto reason = ban_entry->getReason(); !reason.empty()) {
            server.getServer().getMinecraft()->getServerNetworkHandler()->disconnect(
                source, SubClientId::PrimaryClient, "You have been banned from this server. Reason: " + reason);
        }
        else {
            server.getServer().getMinecraft()->getServerNetworkHandler()->disconnect(
                source, SubClientId::PrimaryClient, "You have been banned from this server.");
        }
        return std::nullopt;
    }
    // TODO(event): PlayerPreLoginEvent
    return auth_info;
}

bool ServerNetworkHandler::_isServerTextEnabled(ServerTextEvent const &event) const
{
    if (event == ServerTextEvent::PlayerConnection || event == ServerTextEvent::PlayerChangedSkin) {
        // Note: return false to prevent BDS from sending the messages - we will take over
        return false;
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&ServerNetworkHandler::_isServerTextEnabled, this, event);
}
