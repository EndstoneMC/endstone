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

#include "bedrock/deps/nethernet/nethernet_transport.h"

#include <format>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <variant>

#include <nlohmann/json.hpp>

#include "bedrock/deps/nethernet/simple_network_interface_impl.h"
#include "bedrock/network/server_network_handler.h"
#include "bedrock/server/server_instance.h"
#include "bedrock/shared_constants.h"
#include "bedrock/world/level/level.h"
#include "endstone/core/game_mode.h"
#include "endstone/core/level/level.h"
#include "endstone/core/logger_factory.h"
#include "endstone/core/network/http_signaling_server.h"
#include "endstone/core/server.h"
#include "endstone/event/server/server_list_ping_event.h"
#include "endstone/runtime/hook.h"

NetherNet::INetherNetTransportInterface *NetherNet::TransportFactoryImpl::createTransportInterface(
    const NetworkID &local_id, const TransportConfiguration &configuration,
    INetherNetTransportInterfaceCallbacks *callbacks)
{
    const auto *http = std::get_if<TransportConfiguration::Http>(&configuration.default_signaling_channel);
    if (http == nullptr) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&TransportFactoryImpl::createTransportInterface, this, local_id,
                                           configuration, callbacks);
    }

    auto &config = const_cast<TransportConfiguration &>(configuration);
    const auto port = http->port;
    auto bind_address = http->bind_address;
    config.default_signaling_channel = std::monostate{};
    if (config.max_udp_port == 0 && !config.global_udp_port) {
        // #blameMojang - BDS runs the same NetherNet code as a player hosting a world from their console.
        // Every player who joins gets their own UDP port. Fine at home, useless on a rented server with
        // one open port, where only the first player can get in.
        // Fix: put every player on the signaling port. Set both bounds, a zero minimum skips the cache.
        config.min_udp_port = port;
        config.max_udp_port = port;
        config.global_udp_port = true;
    }

    auto *transport = ENDSTONE_HOOK_CALL_ORIGINAL(&TransportFactoryImpl::createTransportInterface, this, local_id,
                                                  configuration, callbacks);
    auto *impl = static_cast<NetherNet::SimpleNetworkInterfaceImpl *>(transport);
    if (transport == nullptr || impl->signaling_interface_) {
        return transport;
    }

    auto signaling_server = std::make_shared<endstone::core::EndstoneHttpSignalingServer>(
        port, std::move(bind_address), [](const endstone::core::HttpRequest &request) -> std::optional<std::string> {
            auto &server = endstone::core::EndstoneServer::getInstance();
            const auto handler = server.getServer().getMinecraft()->getServerNetworkHandler();
            if (handler->server_name_.empty()) {
                return "";
            }
            auto &level = *server.getEndstoneLevel();
            const auto game_mode =
                endstone::core::EndstoneGameMode::fromMinecraft(level.getHandle().getDefaultGameType());
            const auto event = std::make_shared<endstone::ServerListPingEvent>(
                endstone::SocketAddress{request.remote_address, 0}, handler->server_name_, server.getProtocolVersion(),
                std::format("{}.{}.{}", SharedConstants::MajorVersion, SharedConstants::MinorVersion,
                            SharedConstants::PatchVersion),
                static_cast<int>(server.getOnlinePlayers().size()), server.getMaxPlayers(), "", level.getName(),
                game_mode, 0);
            server.getPluginManager().callEvent(event);
            if (event->isCancelled()) {
                return std::nullopt;
            }
            return nlohmann::ordered_json{
                {"name", event->getMotd()},
                {"protocol", event->getNetworkProtocolVersion()},
                {"version", event->getMinecraftVersionNetwork()},
                {"level", event->getLevelName()},
                {"players", event->getNumPlayers()},
                {"maxPlayers", event->getMaxPlayers()},
                {"gameType", static_cast<int>(endstone::core::EndstoneGameMode::toMinecraft(event->getGameMode()))},
            }
                .dump();
        });
    transport->SetSignalingInterface(signaling_server);
    if (const auto error = signaling_server->start()) {
        endstone::core::LoggerFactory::getLogger("NetherNet")
            .error("NetherNet could not listen on port {} for signaling: {}", port, error.message());
        transport->SetSignalingInterface(nullptr);
        return transport;
    }
    return transport;
}
