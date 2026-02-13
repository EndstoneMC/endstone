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

#pragma once

#include <string>
#include <utility>

#include "endstone/event/cancellable.h"
#include "endstone/event/server/server_event.h"
#include "endstone/game_mode.h"
#include "endstone/util/socket_address.h"

namespace endstone {

/**
 * @brief Called when a server ping is coming in.
 */
class ServerListPingEvent : public Cancellable<ServerEvent> {
public:
    ENDSTONE_EVENT(ServerListPingEvent);
    ServerListPingEvent(SocketAddress address, std::string motd, int network_protocol_version,
                        std::string minecraft_version_network, int num_players, int max_players,
                        std::string server_guid, std::string level_name, GameMode game_mode, int local_port,
                        int local_port_v6)
        : Cancellable(true), address_(std::move(address)), motd_(std::move(motd)),
          network_protocol_version_(network_protocol_version),
          minecraft_version_network_(std::move(minecraft_version_network)), num_players_(num_players),
          max_players_(max_players), server_guid_(std::move(server_guid)), level_name_(std::move(level_name)),
          game_mode_(game_mode), local_port_(local_port), local_port_v6_(local_port_v6)
    {
    }

    /**
     * @brief Get the address the ping is coming from.
     *
     * @return the address
     */
    [[nodiscard]] SocketAddress getAddress() const { return address_; }

    /**
     * @brief Get the unique identifier of the server.
     *
     * @return The server guid
     */
    [[nodiscard]] std::string getServerGuid() const { return server_guid_; }

    /**
     * @brief Set the unique identifier of the server.
     *
     * @param guid the server guid
     */
    void setServerGuid(std::string guid) { server_guid_ = std::move(guid); }

    /**
     * @brief Get the local port of the server.
     *
     * @return The local port
     */
    [[nodiscard]] int getLocalPort() const { return local_port_; }

    /**
     * @brief Set the local port of the server.
     *
     * @param port the local port
     */
    void setLocalPort(int port) { local_port_ = port; }

    /**
     * @brief Get the local port of the server for IPv6 support
     *
     * @return The local port for IPv6
     */
    [[nodiscard]] int getLocalPortV6() const { return local_port_v6_; }

    /**
     * @brief Set the local port of the server for IPv6 support.
     *
     * @param port the local port for IPv6
     */
    void setLocalPortV6(int port) { local_port_v6_ = port; }

    /**
     * @brief Get the message of the day message.
     *
     * @return the message of the day
     */
    [[nodiscard]] std::string getMotd() const { return motd_; }

    /**
     * @brief Change the message of the day message.
     *
     * @param motd the message of the day
     */
    void setMotd(std::string motd) { motd_ = std::move(motd); }

    /**
     * @brief Get the network protocol version of this server
     *
     * @return the network protocol version
     */
    [[nodiscard]] int getNetworkProtocolVersion() const { return network_protocol_version_; }

    /**
     * @brief Get the network version of Minecraft that is supported by this server
     *
     * @return the network version of Minecraft
     */
    [[nodiscard]] std::string getMinecraftVersionNetwork() const { return minecraft_version_network_; }

    /**
     * @brief Set the network version of Minecraft that is supported by this server
     *
     * @param minecraft_version_network the network version of Minecraft
     */
    void setMinecraftVersionNetwork(std::string minecraft_version_network)
    {
        minecraft_version_network_ = std::move(minecraft_version_network);
    }

    /**
     * @brief Get the number of players online
     *
     * @return the number of players
     */
    [[nodiscard]] int getNumPlayers() const { return num_players_; }

    /**
     * @brief Set the number of players online.
     *
     * @param num_players the number of players
     */
    void setNumPlayers(int num_players) { num_players_ = num_players; }

    /**
     * @brief Get the maximum number of players allowed.
     *
     * @return the maximum number of players
     */
    [[nodiscard]] int getMaxPlayers() const { return max_players_; }

    /**
     * @brief Set the maximum number of players allowed.
     *
     * @param max_players the maximum number of players
     */
    void setMaxPlayers(int max_players) { max_players_ = max_players; }

    /**
     * @brief Get the level name.
     *
     * @return the level name
     */
    [[nodiscard]] std::string getLevelName() const { return level_name_; }

    /**
     * @brief Set the level name.
     *
     * @param level_name the level name
     */
    void setLevelName(std::string level_name) { level_name_ = std::move(level_name); }

    /**
     * @brief Get the current game mode.
     *
     * @return the game mode
     */
    [[nodiscard]] GameMode getGameMode() const { return game_mode_; }

    /**
     * @brief Set the current game mode.
     *
     * @param game_mode the game mode
     */
    void setGameMode(GameMode game_mode) { game_mode_ = game_mode; }

private:
    SocketAddress address_;
    std::string motd_;
    int network_protocol_version_;
    std::string minecraft_version_network_;
    int num_players_;
    int max_players_;
    std::string server_guid_;
    std::string level_name_;
    GameMode game_mode_;
    int local_port_;
    int local_port_v6_;
};

}  // namespace endstone
