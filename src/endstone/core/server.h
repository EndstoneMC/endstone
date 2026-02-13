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

#include "bedrock/resources/resource_pack_repository_interface.h"
#include "bedrock/server/server_instance.h"
#include "bedrock/shared_constants.h"
#include "endstone/core/ban/ip_ban_list.h"
#include "endstone/core/ban/player_ban_list.h"
#include "endstone/core/command/command_map.h"
#include "endstone/core/crash_handler.h"
#include "endstone/core/lang/language.h"
#include "endstone/core/level/level.h"
#include "endstone/core/packs/endstone_pack_source.h"
#include "endstone/core/plugin/plugin_manager.h"
#include "endstone/core/plugin/service_manager.h"
#include "endstone/core/scheduler/scheduler.h"
#include "endstone/core/scoreboard/scoreboard.h"
#include "endstone/core/signal_handler.h"
#include "endstone/server.h"

namespace endstone::core {
class EndstoneConsoleCommandSender;
class EndstoneMetrics;
class EndstonePlayer;
class EndstoneServer : public Server {
public:
    explicit EndstoneServer();
    ~EndstoneServer() override;
    EndstoneServer(EndstoneServer const &) = delete;
    EndstoneServer(EndstoneServer &&) = delete;
    EndstoneServer &operator=(EndstoneServer const &) = delete;
    EndstoneServer &operator=(EndstoneServer &&) = delete;

    [[nodiscard]] std::string getName() const override;
    [[nodiscard]] std::string getVersion() const override;
    [[nodiscard]] std::string getMinecraftVersion() const override;
    [[nodiscard]] int getProtocolVersion() const override;

    [[nodiscard]] Logger &getLogger() const override;
    [[nodiscard]] Language &getLanguage() const override;
    [[nodiscard]] EndstoneCommandMap &getCommandMap() const;
    [[nodiscard]] PluginManager &getPluginManager() const override;
    [[nodiscard]] PluginCommand *getPluginCommand(std::string name) const override;
    [[nodiscard]] ConsoleCommandSender &getCommandSender() const override;
    [[nodiscard]] std::shared_ptr<ConsoleCommandSender> getCommandSenderPtr() const;
    [[nodiscard]] bool dispatchCommand(CommandSender &sender, std::string command_line) const override;

    void loadPlugins();
    void enablePlugins(PluginLoadOrder type);
    void disablePlugins() const;

    [[nodiscard]] Scheduler &getScheduler() const override;
    [[nodiscard]] EndstoneScheduler &getEndstoneScheduler() const;

    [[nodiscard]] Level *getLevel() const override;
    [[nodiscard]] EndstoneLevel *getEndstoneLevel() const;

    [[nodiscard]] std::vector<Player *> getOnlinePlayers() const override;
    [[nodiscard]] int getMaxPlayers() const override;
    void setMaxPlayers(int max_players) override;
    [[nodiscard]] Player *getPlayer(UUID id) const override;
    [[nodiscard]] Player *getPlayer(std::string name) const override;

    [[nodiscard]] int getPort() const override;
    [[nodiscard]] int getPortV6() const override;
    [[nodiscard]] bool getOnlineMode() const override;
    void shutdown() override;
    void reload() override;
    void reloadData() override;

    void broadcast(const Message &message, const std::string &permission) const override;
    void broadcastMessage(const Message &message) const override;

    [[nodiscard]] bool isPrimaryThread() const override;

    [[nodiscard]] ItemFactory &getItemFactory() const override;
    [[nodiscard]] Scoreboard *getScoreboard() const override;
    [[nodiscard]] std::shared_ptr<Scoreboard> createScoreboard() override;
    float getCurrentMillisecondsPerTick() override;
    float getAverageMillisecondsPerTick() override;
    float getCurrentTicksPerSecond() override;
    float getAverageTicksPerSecond() override;
    float getCurrentTickUsage() override;
    float getAverageTickUsage() override;
    [[nodiscard]] std::chrono::system_clock::time_point getStartTime() override;
    [[nodiscard]] std::unique_ptr<BossBar> createBossBar(std::string title, BarColor color,
                                                         BarStyle style) const override;
    [[nodiscard]] std::unique_ptr<BossBar> createBossBar(std::string title, BarColor color, BarStyle style,
                                                         std::vector<BarFlag> flags) const override;
    [[nodiscard]] std::unique_ptr<BlockData> createBlockData(std::string type) const override;
    [[nodiscard]] std::unique_ptr<BlockData> createBlockData(std::string type,
                                                                     BlockStates block_states) const override;
    [[nodiscard]] PlayerBanList &getBanList() const override;
    [[nodiscard]] IpBanList &getIpBanList() const override;
    [[nodiscard]] ServiceManager &getServiceManager() const override;
    [[nodiscard]] IRegistry *_getRegistry(const std::string &type) const override;
    [[nodiscard]] MapView *getMap(std::int64_t id) const override;
    [[nodiscard]] MapView &createMap(const Dimension &dimension) const override;

    [[nodiscard]] EndstoneScoreboard &getPlayerBoard(const EndstonePlayer &player) const;
    void setPlayerBoard(EndstonePlayer &player, Scoreboard &scoreboard);
    void removePlayerBoard(EndstonePlayer &player);

    void tick(std::uint64_t current_tick, const std::function<void()> &tick_function);
    void init(ServerInstance &server_instance);
    void setLevel(::Level &level);
    void initRegistries();
    void setResourcePackRepository(IResourcePackRepository &repo);
    void initPackSource(const PackSourceFactory &pack_source_factory);
    [[nodiscard]] PackSource &getPackSource() const;
    [[nodiscard]] bool getAllowClientPacks() const;
    [[nodiscard]] bool logCommands() const;
    [[nodiscard]] bool isServerTextEnabled(ServerTextEvent event) const;

    [[nodiscard]] ServerInstance &getServer() const;
    [[nodiscard]] RakNetConnector &getRakNetConnector() const;

    [[nodiscard]] static EndstoneServer &getInstance();

private:
    friend class EndstonePlayer;
    void enablePlugin(Plugin &plugin);
    void loadResourcePacks();

    ServerInstance *server_instance_{nullptr};
    Logger &logger_;
    std::unique_ptr<CrashHandler> crash_handler_;
    std::unique_ptr<SignalHandler> signal_handler_;
    std::unique_ptr<EndstonePlayerBanList> player_ban_list_;
    std::unique_ptr<EndstoneIpBanList> ip_ban_list_;
    std::unique_ptr<EndstoneLanguage> language_;
    std::unique_ptr<EndstonePluginManager> plugin_manager_;
    std::unique_ptr<EndstoneServiceManager> service_manager_;
    std::shared_ptr<EndstoneConsoleCommandSender> command_sender_;
    std::unique_ptr<EndstoneScheduler> scheduler_;
    std::unique_ptr<EndstoneCommandMap> command_map_;
    std::unique_ptr<EndstoneLevel> level_;
    std::unordered_map<std::string, std::unique_ptr<IRegistry>> registries_;
    std::shared_ptr<EndstoneScoreboard> scoreboard_;
    std::unique_ptr<EndstoneMetrics> metrics_;
    std::unordered_map<UUID, std::shared_ptr<EndstoneScoreboard>> player_boards_;
    std::chrono::system_clock::time_point start_time_;
    IResourcePackRepository *resource_pack_repository_ = nullptr;
    std::unique_ptr<EndstonePackSource> resource_pack_source_;
    int tick_counter_ = 0;
    float current_mspt_ = SharedConstants::MilliSecondsPerTick * 1.0F;
    float average_mspt_[SharedConstants::TicksPerSecond] = {SharedConstants::MilliSecondsPerTick};
    float current_tps_ = SharedConstants::TicksPerSecond * 1.0F;
    float average_tps_[SharedConstants::TicksPerSecond] = {SharedConstants::TicksPerSecond};
    float current_usage_ = 0.0F;
    float average_usage_[SharedConstants::TicksPerSecond] = {0.0F};
    // TODO(config): move the following the a separate class/struct
    bool allow_client_packs_ = false;
    bool log_commands_ = true;
    ServerTextSettings text_settings_;
    ::Bedrock::PubSub::Subscription on_gameplay_user_removed_;
    ::Bedrock::PubSub::Subscription on_chunk_load_;
    ::Bedrock::PubSub::Subscription on_chunk_unload_;
    ::Bedrock::PubSub::Subscription on_map_created_;
};

}  // namespace endstone::core
