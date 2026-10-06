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

#include <algorithm>
#include <charconv>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>
#include <variant>

#include <entt/locator/locator.hpp>

#include "endstone/core/network/stun_client.h"
#include "endstone/core/server.h"
#include "endstone/runtime/hook.h"

namespace {

constexpr auto StunTimeout = std::chrono::seconds(2);

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

std::uint16_t publishedPort(const std::uint16_t local_port)
{
    const auto published = parsePort(getEnv("SERVER_PORT"));
    return published != 0 ? published : local_port;
}

NetherNet::MappedAddressRange *findMapping(NetherNet::TransportConfiguration &config, const std::uint16_t local_port)
{
    const auto count = std::min(static_cast<std::size_t>(std::max(config.known_mapped_address_range_count, 0)),
                                std::size(config.known_mapped_address_ranges));
    for (std::size_t i = 0; i < count; ++i) {
        auto &range = config.known_mapped_address_ranges[i];
        if (local_port >= range.internal_port_min && local_port <= range.internal_port_max) {
            return &range;
        }
    }
    return nullptr;
}

// Bedrock reflects the first range that covers a port, so an operator's own public mapping wins.
std::optional<std::uint16_t> setMappedAddress(NetherNet::TransportConfiguration &config, const std::uint16_t local_port,
                                              const std::string &address, const std::uint16_t external_port)
{
    if (auto *range = findMapping(config, local_port)) {
        if (range->external_address.has_value()) {
            return std::nullopt;
        }
        range->external_address = address;
        return static_cast<std::uint16_t>(local_port + range->external_port_offset);
    }

    const auto count = static_cast<std::size_t>(std::max(config.known_mapped_address_range_count, 0));
    if (count >= std::size(config.known_mapped_address_ranges)) {
        return std::nullopt;
    }
    auto &range = config.known_mapped_address_ranges[count];
    range.internal_address.reset();
    range.internal_port_min = local_port;
    range.internal_port_max = local_port;
    range.external_address = address;
    range.external_port_offset = external_port - local_port;
    config.known_mapped_address_range_count = static_cast<int>(count + 1);
    return external_port;
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

    const auto port = setMappedAddress(config, local_port, std::string{address}, publishedPort(local_port));
    if (!port) {
        return false;
    }
    if (entt::locator<endstone::core::EndstoneServer>::has_value()) {
        endstone::core::EndstoneServer::getInstance().getLogger().info(
            "Configured {}:{} as the mapped address for NetherNet.", address, *port);
    }
    return true;
}

// The transport reads the mapped addresses once, when it is built, so the STUN answer has to be here first.
void addStunAddress(NetherNet::TransportConfiguration &config, const std::uint16_t local_port)
{
    if (!entt::locator<endstone::core::EndstoneServer>::has_value()) {
        return;
    }
    if (const auto *range = findMapping(config, local_port); range != nullptr && range->external_address.has_value()) {
        return;
    }
    auto &server = endstone::core::EndstoneServer::getInstance();
    const auto &uris = server.getStunServers();
    if (uris.empty()) {
        return;
    }

    const auto address = endstone::core::queryPublicAddress(uris, StunTimeout);
    if (!address) {
        server.getLogger().warning("Could not discover the public address for NetherNet from the STUN servers.");
        return;
    }
    if (const auto port = setMappedAddress(config, local_port, *address, publishedPort(local_port))) {
        server.getLogger().info("Discovered {}:{} as the mapped address for NetherNet.", *address, *port);
    }
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
        addStunAddress(config, http->port);
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&TransportFactoryImpl::createTransportInterface, this, local_id, configuration,
                                       callbacks);
}
