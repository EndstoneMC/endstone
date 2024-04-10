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

#include "endstone/event/event.h"
#include "endstone/game_mode.h"

namespace endstone {

/**
 * Called when a server ping is coming in.
 */
class ServerListPingEvent : public Event {
public:
    ServerListPingEvent(std::string remote_host, int remote_port, std::string ping_response)
        : Event(true), ping_response_(std::move(ping_response)), remote_host_(std::move(remote_host)),
          remote_port_(remote_port)
    {
    }

    /**
     * Get the host the ping is coming from.
     *
     * @return The host
     */
    [[nodiscard]] std::string getRemoteHost() const
    {
        return remote_host_;
    }

    /**
     * Get the port the ping is coming from.
     *
     * @return The port
     */
    [[nodiscard]] int getRemotePort() const
    {
        return remote_port_;
    }

    /**
     * Get the unique identifier of the server.
     *
     * @return The server guid
     */
    [[nodiscard]] std::string getServerGuid() const
    {
        return server_guid_;
    }

    /**
     * Get the local port of the server.
     *
     * @return The local port
     */
    [[nodiscard]] int getLocalPort() const
    {
        return local_port_;
    }

    /**
     * Get the local port of the server for IPv6 support
     *
     * @return The local port for IPv6
     */
    [[nodiscard]] int getLocalPortV6() const
    {
        return local_port_v6_;
    }

    /**
     * Get the message of the day message.
     *
     * @return the message of the day
     */
    [[nodiscard]] std::string getMotd() const
    {
        return motd_;
    }

    /**
     * Change the message of the day message.
     *
     * @param motd the message of the day
     */
    void setMotd(std::string motd)
    {
        motd_ = std::move(motd);
    }

    /**
     * Get the network protocol version of this server
     *
     * @return the network protocol version
     */
    [[nodiscard]] int getNetworkProtocolVersion() const
    {
        return network_protocol_version_;
    }

    /**
     * Get the network version of Minecraft that is supported by this server
     *
     * @return the network version of Minecraft
     */
    [[nodiscard]] std::string getMinecraftVersionNetwork() const
    {
        return minecraft_version_network_;
    }

    /**
     * Set the network version of Minecraft that is supported by this server
     *
     * @param minecraft_version_network the network version of Minecraft
     */
    void setMinecraftVersionNetwork(std::string minecraft_version_network)
    {
        minecraft_version_network_ = std::move(minecraft_version_network);
    }

    /**
     * Get the number of players online
     *
     * @return the number of players
     */
    [[nodiscard]] int getNumPlayers() const
    {
        return num_players_;
    }

    /**
     * Set the number of players online.
     *
     * @param num_players the number of players
     */
    void setNumPlayers(int num_players)
    {
        num_players_ = num_players;
    }

    /**
     * Get the maximum number of players allowed.
     *
     * @return the maximum number of players
     */
    [[nodiscard]] int getMaxPlayers() const
    {
        return max_players_;
    }

    /**
     * Set the maximum number of players allowed.
     *
     * @param max_players the maximum number of players
     */
    void setMaxPlayers(int max_players)
    {
        max_players_ = max_players;
    }

    /**
     * Get the level name.
     *
     * @return the level name
     */
    [[nodiscard]] std::string getLevelName() const
    {
        return level_name_;
    }

    /**
     * Set the level name.
     *
     * @param level_name the level name
     */
    void setLevelName(std::string level_name)
    {
        level_name_ = std::move(level_name);
    }

    /**
     * Get the current game mode.
     *
     * @return the game mode
     */
    [[nodiscard]] GameMode getGameMode() const
    {
        return game_mode_;
    }

    /**
     * Set the current game mode.
     *
     * @param game_mode the game mode
     */
    void setGameMode(GameMode game_mode)
    {
        game_mode_ = game_mode;
    }

    inline static const std::string NAME = "ServerListPingEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    [[nodiscard]] bool isCancellable() const override
    {
        return false;
    }

    bool deserialize();
    std::string serialize();

private:
    std::string ping_response_;
    std::string remote_host_;
    int remote_port_;
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
