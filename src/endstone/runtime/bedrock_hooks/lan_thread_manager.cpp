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

#include "bedrock/deps/nethernet/lan_thread_manager.h"

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "bedrock/core/utility/binary_stream.h"
#include "bedrock/network/nethernet_server_locator.h"
#include "endstone/core/game_mode.h"
#include "endstone/core/server.h"
#include "endstone/core/util/socket_address.h"
#include "endstone/event/server/server_list_ping_event.h"
#include "endstone/runtime/hook.h"

namespace {
constexpr std::size_t MAX_APPLICATION_DATA_LENGTH = 1148;

int hexValue(const char c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    return -1;
}

std::optional<std::string> fromHex(const std::string_view hex)
{
    if (hex.size() % 2 != 0) {
        return std::nullopt;
    }
    std::string bytes(hex.size() / 2, '\0');
    for (std::size_t i = 0; i < bytes.size(); ++i) {
        const auto high = hexValue(hex[i * 2]);
        const auto low = hexValue(hex[i * 2 + 1]);
        if (high < 0 || low < 0) {
            return std::nullopt;
        }
        bytes[i] = static_cast<char>(high << 4 | low);
    }
    return bytes;
}

std::string toHex(const std::string_view bytes)
{
    constexpr std::string_view digits = "0123456789abcdef";
    std::string hex;
    hex.reserve(bytes.size() * 2);
    for (const auto byte : bytes) {
        hex += digits[static_cast<unsigned char>(byte) >> 4];
        hex += digits[static_cast<unsigned char>(byte) & 0xF];
    }
    return hex;
}

}  // namespace

void NetherNet::LanThreadManager::SendLanBroadcastResponse(const webrtc::SocketAddress &destination, NetworkID from,
                                                           std::string data)
{
    const auto bytes = fromHex(data).value_or("");
    ReadOnlyBinaryStream input(bytes, false);
    NetherNetServerLocator::ServerData server_data;
    if (!server_data.read(input).ignoreError()) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&LanThreadManager::SendLanBroadcastResponse, this, destination, from,
                                    std::move(data));
        return;
    }

    auto &server = endstone::core::EndstoneServer::getInstance();
    const auto game_mode = endstone::core::EndstoneGameMode::fromMinecraft(server_data.game_type);
    endstone::ServerListPingEvent event(endstone::core::EndstoneSocketAddress::fromWebRtcSocketAddress(destination),
                                        server_data.server_name, server_data.protocol_version, server_data.app_version,
                                        server_data.num_players, server_data.max_num_players, from.toString(),
                                        server_data.level_name, game_mode, 0);
    server.getPluginManager().callEvent(event);
    if (event.isCancelled()) {
        return;
    }

    server_data.server_name = event.getMotd();
    server_data.app_version = event.getMinecraftVersionNetwork();
    server_data.level_name = event.getLevelName();
    server_data.num_players = event.getNumPlayers();
    server_data.max_num_players = event.getMaxPlayers();
    if (event.getGameMode() != game_mode) {
        server_data.game_type = endstone::core::EndstoneGameMode::toMinecraft(event.getGameMode());
    }

    BinaryStream output;
    server_data.write(output);
    auto response = toHex(output.getBuffer());
    if (response.size() > MAX_APPLICATION_DATA_LENGTH) {
        server.getLogger().warning("LAN discovery response is {} bytes, exceeding the {} byte limit.",
                                   response.size(), MAX_APPLICATION_DATA_LENGTH);
        ENDSTONE_HOOK_CALL_ORIGINAL(&LanThreadManager::SendLanBroadcastResponse, this, destination, from,
                                    std::move(data));
        return;
    }

    ENDSTONE_HOOK_CALL_ORIGINAL(&LanThreadManager::SendLanBroadcastResponse, this, destination, from,
                                std::move(response));
}
