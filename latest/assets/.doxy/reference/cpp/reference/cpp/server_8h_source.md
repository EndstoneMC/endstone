

# File server.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**server.h**](server_8h.md)

[Go to the documentation of this file](server_8h.md)


```C++
// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include <chrono>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "endstone/level/level.h"
#include "endstone/logger.h"
#include "endstone/player.h"
#include "endstone/scoreboard/scoreboard.h"
#include "endstone/util/uuid.h"

namespace endstone {

class ConsoleCommandSender;
class Scheduler;
class PluginCommand;
class PluginManager;

class Server {
public:
    Server() = default;
    Server(const Server &) = delete;
    Server &operator=(const Server &) = delete;

    virtual ~Server() = default;

    [[nodiscard]] virtual std::string getName() const = 0;

    [[nodiscard]] virtual std::string getVersion() const = 0;

    [[nodiscard]] virtual std::string getMinecraftVersion() const = 0;

    [[nodiscard]] virtual Logger &getLogger() const = 0;

    [[nodiscard]] virtual PluginManager &getPluginManager() const = 0;

    [[nodiscard]] virtual PluginCommand *getPluginCommand(std::string name) const = 0;

    [[nodiscard]] virtual ConsoleCommandSender &getCommandSender() const = 0;

    [[nodiscard]] virtual bool dispatchCommand(CommandSender &sender, std::string command) const = 0;

    [[nodiscard]] virtual Scheduler &getScheduler() const = 0;

    [[nodiscard]] virtual Level *getLevel() const = 0;

    [[nodiscard]] virtual std::vector<Player *> getOnlinePlayers() const = 0;

    [[nodiscard]] virtual int getMaxPlayers() const = 0;

    virtual void setMaxPlayers(int max_players) = 0;

    [[nodiscard]] virtual Player *getPlayer(endstone::UUID id) const = 0;

    [[nodiscard]] virtual Player *getPlayer(std::string name) const = 0;

    virtual void shutdown() = 0;

    virtual void broadcast(const std::string &message, const std::string &permission) const = 0;

    virtual void broadcastMessage(const std::string &message) const = 0;

    template <typename... Args>
    void broadcastMessage(const fmt::format_string<Args...> format, Args &&...args) const
    {
        broadcastMessage(fmt::format(format, std::forward<Args>(args)...));
    }

    [[nodiscard]] virtual bool isPrimaryThread() const = 0;

    [[nodiscard]] virtual Scoreboard *getScoreboard() const = 0;

    [[nodiscard]] virtual std::shared_ptr<Scoreboard> getNewScoreboard() = 0;

    [[nodiscard]] virtual std::chrono::system_clock::time_point getStartTime() = 0;

    inline static const std::string BroadcastChannelAdmin = "endstone.broadcast.admin";

    inline static const std::string BroadcastChannelUser = "endstone.broadcast.user";
};

}  // namespace endstone
```


