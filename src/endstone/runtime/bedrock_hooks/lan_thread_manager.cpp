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
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "bedrock/core/utility/binary_stream.h"
#include "bedrock/world/level/game_type.h"
#include "endstone/core/game_mode.h"
#include "endstone/core/server.h"
#include "endstone/core/util/socket_address.h"
#include "endstone/event/server/server_list_ping_event.h"
#include "endstone/runtime/hook.h"

namespace {
constexpr std::uint8_t SERVER_DATA_VERSION = 7;
constexpr std::size_t MAX_APPLICATION_DATA_LENGTH = 1148;

struct ServerData {
    std::string name;
    int protocol;
    std::string version;
    std::string level;
    int players;
    int max_players;
    GameType game_type;
    std::string remaining;
};

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

std::optional<int> readVarInt(ReadOnlyBinaryStream &stream)
{
    const auto value = stream.getVarInt64().discardError();
    if (!value) {
        return std::nullopt;
    }
    return static_cast<int>(value.value());
}

std::optional<std::string> readString(ReadOnlyBinaryStream &stream)
{
    auto value = stream.getString(stream.getLength()).discardError();
    if (!value) {
        return std::nullopt;
    }
    return std::move(value.value());
}

std::optional<ServerData> readServerData(const std::string_view data)
{
    ReadOnlyBinaryStream stream(data, false);
    const auto data_version = stream.getByte().discardError();
    if (!data_version || data_version.value() != SERVER_DATA_VERSION) {
        return std::nullopt;
    }
    auto name = readString(stream);
    const auto protocol = name ? readVarInt(stream) : std::nullopt;
    auto version = protocol ? readString(stream) : std::nullopt;
    auto level = version ? readString(stream) : std::nullopt;
    const auto players = level ? readVarInt(stream) : std::nullopt;
    const auto max_players = players ? readVarInt(stream) : std::nullopt;
    const auto game_type = max_players ? readVarInt(stream) : std::nullopt;
    if (!game_type) {
        return std::nullopt;
    }
    return ServerData{std::move(*name),
                      *protocol,
                      std::move(*version),
                      std::move(*level),
                      *players,
                      *max_players,
                      static_cast<GameType>(*game_type),
                      std::string(data.substr(stream.getReadPointer()))};
}

std::string writeServerData(const ServerData &server_data)
{
    BinaryStream stream;
    stream.writeByte(SERVER_DATA_VERSION, "dataVersion", nullptr);
    stream.writeString(server_data.name, "name", nullptr);
    stream.writeVarInt(server_data.protocol, "protocol", nullptr);
    stream.writeString(server_data.version, "version", nullptr);
    stream.writeString(server_data.level, "level", nullptr);
    stream.writeVarInt(server_data.players, "players", nullptr);
    stream.writeVarInt(server_data.max_players, "maxPlayers", nullptr);
    stream.writeVarInt(static_cast<int>(server_data.game_type), "gameType", nullptr);
    return stream.getBuffer() + server_data.remaining;
}
}  // namespace

void NetherNet::LanThreadManager::SendLanBroadcastResponse(const webrtc::SocketAddress &destination, NetworkID from,
                                                           std::string data)
{
    const auto bytes = fromHex(data);
    auto server_data = bytes ? readServerData(*bytes) : std::nullopt;
    if (!server_data) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&LanThreadManager::SendLanBroadcastResponse, this, destination, from,
                                    std::move(data));
        return;
    }

    auto &server = endstone::core::EndstoneServer::getInstance();
    const auto game_mode = endstone::core::EndstoneGameMode::fromMinecraft(server_data->game_type);
    endstone::ServerListPingEvent event(endstone::core::EndstoneSocketAddress::fromWebRtcSocketAddress(destination),
                                        server_data->name, server_data->protocol, server_data->version,
                                        server_data->players, server_data->max_players, from.toString(),
                                        server_data->level, game_mode, 0, 0);
    server.getPluginManager().callEvent(event);
    if (event.isCancelled()) {
        return;
    }

    server_data->name = event.getMotd();
    server_data->version = event.getMinecraftVersionNetwork();
    server_data->level = event.getLevelName();
    server_data->players = event.getNumPlayers();
    server_data->max_players = event.getMaxPlayers();
    if (event.getGameMode() != game_mode) {
        server_data->game_type = endstone::core::EndstoneGameMode::toMinecraft(event.getGameMode());
    }

    auto response = toHex(writeServerData(*server_data));
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
