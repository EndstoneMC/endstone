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

#define _CRT_SECURE_NO_WARNINGS  // std::getenv

#include "bedrock/deps/nethernet/nethernet_transport.h"

#include <charconv>
#include <cstdlib>
#include <string_view>
#include <variant>

#include <entt/locator/locator.hpp>

#include "endstone/core/server.h"
#include "endstone/runtime/hook.h"

namespace {

std::string_view getEnv(const char *name)
{
    const char *value = std::getenv(name);
    return value != nullptr ? std::string_view{value} : std::string_view{};
}

std::uint16_t parsePort(const std::string_view value)
{
    unsigned int port = 0;
    const auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), port);
    if (ec != std::errc{} || ptr != value.data() + value.size() || port == 0 || port > 0xffff) {
        return 0;
    }
    return static_cast<std::uint16_t>(port);
}

// Hosting panels run the server behind a bridge and publish one address, which they hand to the
// process in the environment. Advertising it is what lets a client reach a server it can already
// see, without the operator copying an address into server.properties.
bool addPublishedAddress(NetherNet::TransportConfiguration &config, const std::uint16_t local_port)
{
    const auto address = getEnv("SERVER_IP");
    if (address.empty() || address == "0.0.0.0" || address == "::") {
        return false;
    }
    if (config.known_mapped_address_range_count != 0) {
        return false;
    }

    const auto published = parsePort(getEnv("SERVER_PORT"));
    auto &range = config.known_mapped_address_ranges[0];
    range.internal_address.reset();
    range.internal_port_min = local_port;
    range.internal_port_max = local_port;
    range.external_address = std::string{address};
    range.external_port_offset = (published != 0 ? published : local_port) - local_port;
    config.known_mapped_address_range_count = 1;
    if (entt::locator<endstone::core::EndstoneServer>::has_value()) {
        endstone::core::EndstoneServer::getInstance().getLogger().info(
            "Configured {}:{} as the mapped address for NetherNet.", address, published != 0 ? published : local_port);
    }
    return true;
}

}  // namespace

NetherNet::INetherNetTransportInterface *NetherNet::TransportFactoryImpl::createTransportInterface(
    const NetworkID &local_id, const TransportConfiguration &configuration,
    INetherNetTransportInterfaceCallbacks *callbacks)
{
    const auto *http = std::get_if<TransportConfiguration::Http>(&configuration.default_signaling_channel);
    if (http != nullptr) {
        auto &config = const_cast<TransportConfiguration &>(configuration);
        // #blameMojang - BDS runs the same NetherNet code as a player hosting a world from their console.
        // Every player who joins gets their own UDP port. Fine at home, useless on a rented server with
        // one open port, where only the first player can get in.
        // Fix: put every player on the signaling port. Set both bounds, a zero minimum skips the cache.
        config.min_udp_port = http->port;
        config.max_udp_port = http->port;
        config.global_udp_port = true;
        addPublishedAddress(config, http->port);
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&TransportFactoryImpl::createTransportInterface, this, local_id, configuration,
                                       callbacks);
}
