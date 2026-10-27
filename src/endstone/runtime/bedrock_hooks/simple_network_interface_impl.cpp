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

#include "bedrock/deps/nethernet/simple_network_interface_impl.h"

#include <string>
#include <system_error>
#include <vector>

#include <nlohmann/json.hpp>

#include "bedrock/world/level/game_type.h"
#include "endstone/core/game_mode.h"
#include "endstone/core/network/nethernet_address_cache.h"
#include "endstone/core/server.h"
#include "endstone/event/server/server_list_ping_event.h"
#include "endstone/runtime/hook.h"

namespace {
template <typename T>
T read(const nlohmann::ordered_json &json, const char *key, T fallback)
{
    const auto it = json.find(key);
    if (it == json.end()) {
        return fallback;
    }
    return it->get<T>();
}

template <typename T>
void write(nlohmann::ordered_json &json, const char *key, const T &value)
{
    if (json.contains(key)) {
        json[key] = value;
    }
}
}  // namespace

void NetherNet::SimpleNetworkInterfaceImpl::ReceiveFromSignalingChannel(NetworkID from, std::string_view message,
                                                                        SignalingChannelId source_channel)
{
    endstone::core::NetherNetAddressCache::getInstance().addSession(from, message);
    ENDSTONE_HOOK_CALL_ORIGINAL(&SimpleNetworkInterfaceImpl::ReceiveFromSignalingChannel, this, from, message,
                                source_channel);
}

Bedrock::Result<std::string> NetherNet::SimpleNetworkInterfaceImpl::getMetadata(const HttpMetadataProvider &provider)
{
    auto result = ENDSTONE_HOOK_CALL_ORIGINAL(&SimpleNetworkInterfaceImpl::getMetadata, provider);
    if (!result.asExpected().has_value()) {
        return result;
    }

    auto json = nlohmann::ordered_json::parse(result.asExpected().value(), nullptr, false);
    if (!json.is_object()) {
        return result;
    }

    try {
        const auto game_mode =
            endstone::core::EndstoneGameMode::fromMinecraft(static_cast<GameType>(read<int>(json, "gameType", 0)));
        endstone::ServerListPingEvent event(endstone::SocketAddress{}, read<std::string>(json, "name", ""),
                                            read<int>(json, "protocol", 0), read<std::string>(json, "version", ""),
                                            read<int>(json, "players", 0), read<int>(json, "maxPlayers", 0), "",
                                            read<std::string>(json, "level", ""), game_mode, 0, 0);

        auto &server = endstone::core::EndstoneServer::getInstance();
        server.getPluginManager().callEvent(event);
        if (event.isCancelled()) {
            return Bedrock::ErrorInfo<std::error_code>{
                std::make_error_code(std::errc::operation_canceled),
                Bedrock::CallStack(std::vector<Bedrock::CallStack::FrameWithContext>{}),
                {}};
        }

        write(json, "name", event.getMotd());
        write(json, "version", event.getMinecraftVersionNetwork());
        write(json, "level", event.getLevelName());
        write(json, "players", event.getNumPlayers());
        write(json, "maxPlayers", event.getMaxPlayers());
        if (event.getGameMode() != game_mode) {
            write(json, "gameType",
                  static_cast<int>(endstone::core::EndstoneGameMode::toMinecraft(event.getGameMode())));
        }
        return Bedrock::Result<std::string>(json.dump());
    }
    catch (const nlohmann::json::exception &) {
        return result;
    }
}
