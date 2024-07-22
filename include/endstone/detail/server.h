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

#include <memory>
#include <string>
#include <string_view>

#include "bedrock/server/server_instance.h"
#include "endstone/command/console_command_sender.h"
#include "endstone/detail/command/command_map.h"
#include "endstone/detail/plugin/plugin_manager.h"
#include "endstone/detail/scheduler/scheduler.h"
#include "endstone/detail/scoreboard/scoreboard.h"
#include "endstone/level/level.h"
#include "endstone/plugin/plugin_manager.h"
#include "endstone/server.h"

namespace endstone::detail {

class EndstoneServer : public Server {
public:
    explicit EndstoneServer(ServerInstance &server_instance);
    EndstoneServer(EndstoneServer const &) = delete;
    EndstoneServer(EndstoneServer &&) = delete;
    EndstoneServer &operator=(EndstoneServer const &) = delete;
    EndstoneServer &operator=(EndstoneServer &&) = delete;
    ~EndstoneServer() override = default;

    [[nodiscard]] std::string getName() const override;
    [[nodiscard]] std::string getVersion() const override;
    [[nodiscard]] std::string getMinecraftVersion() const override;

    [[nodiscard]] Logger &getLogger() const override;
    [[nodiscard]] EndstoneCommandMap &getCommandMap() const;
    [[nodiscard]] MinecraftCommands &getMinecraftCommands() const;
    [[nodiscard]] PluginManager &getPluginManager() const override;
    [[nodiscard]] PluginCommand *getPluginCommand(std::string name) const override;
    [[nodiscard]] ConsoleCommandSender &getCommandSender() const override;
    [[nodiscard]] bool dispatchCommand(CommandSender &sender, std::string command) const override;

    void loadPlugins();
    void enablePlugins(PluginLoadOrder type);
    void disablePlugins() const;

    [[nodiscard]] Scheduler &getScheduler() const override;

    [[nodiscard]] std::vector<Level *> getLevels() const override;
    [[nodiscard]] Level *getLevel(std::string name) const override;
    void addLevel(std::unique_ptr<Level> level);

    [[nodiscard]] std::vector<Player *> getOnlinePlayers() const override;
    [[nodiscard]] int getMaxPlayers() const override;
    void setMaxPlayers(int max_players) override;
    [[nodiscard]] Player *getPlayer(endstone::UUID id) const override;
    [[nodiscard]] Player *getPlayer(std::string name) const override;

    void broadcast(const std::string &message, const std::string &permission) const override;
    void broadcastMessage(const std::string &message) const override;

    [[nodiscard]] bool isPrimaryThread() const override;

    [[nodiscard]] Scoreboard *getScoreboard() const override;
    void setScoreboard(std::unique_ptr<EndstoneScoreboard> scoreboard);
    [[nodiscard]] ::ServerNetworkHandler &getServerNetworkHandler() const;

private:
    friend class EndstonePlayer;

    void enablePlugin(Plugin &plugin);
    ServerInstance &server_instance_;
    Logger &logger_;
    std::unique_ptr<EndstoneCommandMap> command_map_;
    std::unique_ptr<EndstonePluginManager> plugin_manager_;
    std::unique_ptr<ConsoleCommandSender> command_sender_;
    std::unique_ptr<EndstoneScheduler> scheduler_;
    std::unordered_map<std::string, std::unique_ptr<Level>> levels_;
    std::unordered_map<UUID, Player *> players_;
    std::unique_ptr<EndstoneScoreboard> scoreboard_;
};

}  // namespace endstone::detail
