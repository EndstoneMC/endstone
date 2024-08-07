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
    void init();

    [[nodiscard]] std::string getName() const override;
    [[nodiscard]] std::string getVersion() const override;
    [[nodiscard]] std::string getMinecraftVersion() const override;

    [[nodiscard]] Logger &getLogger() const override;
    [[nodiscard]] EndstoneCommandMap &getCommandMap() const;
    void setCommandMap(std::unique_ptr<EndstoneCommandMap> command_map);
    [[nodiscard]] MinecraftCommands &getMinecraftCommands() const;
    [[nodiscard]] PluginManager &getPluginManager() const override;
    [[nodiscard]] PluginCommand *getPluginCommand(std::string name) const override;
    [[nodiscard]] ConsoleCommandSender &getCommandSender() const override;
    [[nodiscard]] bool dispatchCommand(CommandSender &sender, std::string command) const override;

    void loadPlugins();
    void enablePlugins(PluginLoadOrder type);
    void disablePlugins() const;

    [[nodiscard]] Scheduler &getScheduler() const override;

    [[nodiscard]] Level *getLevel() const override;
    void setLevel(std::unique_ptr<EndstoneLevel> level);

    [[nodiscard]] std::vector<Player *> getOnlinePlayers() const override;
    [[nodiscard]] int getMaxPlayers() const override;
    void setMaxPlayers(int max_players) override;
    [[nodiscard]] Player *getPlayer(endstone::UUID id) const override;
    [[nodiscard]] Player *getPlayer(std::string name) const override;

    void shutdown() override;
    void reload() override;
    void reloadData() override;

    void broadcast(const std::string &message, const std::string &permission) const override;
    void broadcastMessage(const std::string &message) const override;

    [[nodiscard]] bool isPrimaryThread() const override;

    [[nodiscard]] Scoreboard *getScoreboard() const override;
    void setScoreboard(std::unique_ptr<EndstoneScoreboard> scoreboard);
    [[nodiscard]] std::shared_ptr<Scoreboard> getNewScoreboard() override;
    float getCurrentMillisecondsPerTick() override;
    float getAverageMillisecondsPerTick() override;
    float getCurrentTicksPerSecond() override;
    float getAverageTicksPerSecond() override;
    float getCurrentTickUsage() override;
    float getAverageTickUsage() override;
    [[nodiscard]] std::chrono::system_clock::time_point getStartTime() override;
    [[nodiscard]] EndstoneScoreboard &getPlayerBoard(const EndstonePlayer &player) const;
    void setPlayerBoard(EndstonePlayer &player, Scoreboard &scoreboard);
    void removePlayerBoard(EndstonePlayer &player);
    [[nodiscard]] ::ServerNetworkHandler &getServerNetworkHandler() const;
    void tick(std::uint64_t current_tick, const std::function<void()> &tick_function);

    static constexpr int TargetTicksPerSecond = 20;
    static constexpr int TargetMillisecondsPerTick = 1000 / TargetTicksPerSecond;

private:
    friend class EndstonePlayer;

    void enablePlugin(Plugin &plugin);
    ServerInstance &server_instance_;
    Logger &logger_;
    std::unique_ptr<EndstoneCommandMap> command_map_;
    std::unique_ptr<EndstonePluginManager> plugin_manager_;
    std::unique_ptr<ConsoleCommandSender> command_sender_;
    std::unique_ptr<EndstoneScheduler> scheduler_;
    std::unique_ptr<EndstoneLevel> level_;
    std::unordered_map<UUID, Player *> players_;
    std::shared_ptr<EndstoneScoreboard> scoreboard_;
    std::vector<std::weak_ptr<EndstoneScoreboard>> scoreboards_;
    std::unordered_map<const EndstonePlayer *, std::shared_ptr<EndstoneScoreboard>> player_boards_;
    std::chrono::system_clock::time_point start_time_;

    int tick_counter_ = 0;
    float current_mspt_ = TargetMillisecondsPerTick * 1.0F;
    float average_mspt_[TargetTicksPerSecond] = {TargetMillisecondsPerTick};
    float current_tps_ = TargetTicksPerSecond * 1.0F;
    float average_tps_[TargetTicksPerSecond] = {TargetTicksPerSecond};
    float current_usage_ = 0.0F;
    float average_usage_[TargetTicksPerSecond] = {0.0F};
};

}  // namespace endstone::detail
