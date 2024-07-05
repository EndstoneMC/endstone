

# File server\_list\_ping\_event.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**event**](dir_f1d783c0ad83ee143d16e768ebca51c8.md) **>** [**server**](dir_77022909323d5ad872c4820a738a5429.md) **>** [**server\_list\_ping\_event.h**](server__list__ping__event_8h.md)

[Go to the documentation of this file](server__list__ping__event_8h.md)


```C++
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
#include "endstone/event/server/server_event.h"
#include "endstone/game_mode.h"

namespace endstone {

class ServerListPingEvent : public ServerEvent {
public:
    ServerListPingEvent(std::string remote_host, int remote_port, std::string ping_response)
        : ServerEvent(true), ping_response_(std::move(ping_response)), remote_host_(std::move(remote_host)),
          remote_port_(remote_port)
    {
    }

    [[nodiscard]] std::string getRemoteHost() const
    {
        return remote_host_;
    }

    [[nodiscard]] int getRemotePort() const
    {
        return remote_port_;
    }

    [[nodiscard]] std::string getServerGuid() const
    {
        return server_guid_;
    }

    [[nodiscard]] int getLocalPort() const
    {
        return local_port_;
    }

    [[nodiscard]] int getLocalPortV6() const
    {
        return local_port_v6_;
    }

    [[nodiscard]] std::string getMotd() const
    {
        return motd_;
    }

    void setMotd(std::string motd)
    {
        motd_ = std::move(motd);
    }

    [[nodiscard]] int getNetworkProtocolVersion() const
    {
        return network_protocol_version_;
    }

    [[nodiscard]] std::string getMinecraftVersionNetwork() const
    {
        return minecraft_version_network_;
    }

    void setMinecraftVersionNetwork(std::string minecraft_version_network)
    {
        minecraft_version_network_ = std::move(minecraft_version_network);
    }

    [[nodiscard]] int getNumPlayers() const
    {
        return num_players_;
    }

    void setNumPlayers(int num_players)
    {
        num_players_ = num_players;
    }

    [[nodiscard]] int getMaxPlayers() const
    {
        return max_players_;
    }

    void setMaxPlayers(int max_players)
    {
        max_players_ = max_players;
    }

    [[nodiscard]] std::string getLevelName() const
    {
        return level_name_;
    }

    void setLevelName(std::string level_name)
    {
        level_name_ = std::move(level_name);
    }

    [[nodiscard]] GameMode getGameMode() const
    {
        return game_mode_;
    }

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
```


