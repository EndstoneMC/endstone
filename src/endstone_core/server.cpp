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

#include "endstone/detail/server.h"

#include <filesystem>
#include <memory>

namespace fs = std::filesystem;

#include <boost/algorithm/string.hpp>

#include "bedrock/common/game_version.h"
#include "bedrock/core/threading.h"
#include "bedrock/network/server_network_handler.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/scores/server_scoreboard.h"
#include "endstone/color_format.h"
#include "endstone/command/plugin_command.h"
#include "endstone/detail/command/command_map.h"
#include "endstone/detail/command/console_command_sender.h"
#include "endstone/detail/level/level.h"
#include "endstone/detail/logger_factory.h"
#include "endstone/detail/permissions/default_permissions.h"
#include "endstone/detail/plugin/cpp_plugin_loader.h"
#include "endstone/detail/plugin/python_plugin_loader.h"
#include "endstone/event/server/broadcast_message_event.h"
#include "endstone/event/server/server_load_event.h"
#include "endstone/plugin/plugin.h"

#if !defined(ENDSTONE_VERSION)
#error ENDSTONE_VERSION is not defined
#endif

namespace endstone::detail {

EndstoneServer::EndstoneServer(ServerInstance &server_instance)
    : server_instance_(server_instance), logger_(LoggerFactory::getLogger("Server"))
{
    plugin_manager_ = std::make_unique<EndstonePluginManager>(*this);
    scheduler_ = std::make_unique<EndstoneScheduler>(*this);
    start_time_ = std::chrono::system_clock::now();
}

void EndstoneServer::init()
{
    getLogger().info(ColorFormat::DarkAqua + ColorFormat::Bold +
                         "This server is running {} version: {} (Minecraft: {})",
                     getName(), getVersion(), getMinecraftVersion());
    command_sender_ = std::make_unique<EndstoneConsoleCommandSender>();
    command_sender_->recalculatePermissions();
}

std::string EndstoneServer::getName() const
{
    return "Endstone";
}

std::string EndstoneServer::getVersion() const
{
    return ENDSTONE_VERSION;
}

std::string EndstoneServer::getMinecraftVersion() const
{
    return Common::getGameVersionString();
}

Logger &EndstoneServer::getLogger() const
{
    return logger_;
}

EndstoneCommandMap &EndstoneServer::getCommandMap() const
{
    return *command_map_;
}

void EndstoneServer::setCommandMap(std::unique_ptr<EndstoneCommandMap> command_map)
{
    command_map_ = std::move(command_map);
}

MinecraftCommands &EndstoneServer::getMinecraftCommands() const
{
    return server_instance_.getMinecraft().getCommands();
}

PluginManager &EndstoneServer::getPluginManager() const
{
    return *plugin_manager_;
}

PluginCommand *EndstoneServer::getPluginCommand(std::string name) const
{
    if (auto *command = command_map_->getCommand(name)) {
        return command->asPluginCommand();
    }
    return nullptr;
}

ConsoleCommandSender &EndstoneServer::getCommandSender() const
{
    return *command_sender_;
}

bool EndstoneServer::dispatchCommand(CommandSender &sender, std::string command) const
{
    auto origin = CommandOrigin::fromEndstone(sender);
    CommandContext ctx{command, std::move(origin), CommandVersion::CurrentVersion};
    auto result = getMinecraftCommands().executeCommand(ctx, true);
    return result.success;
}

void EndstoneServer::loadPlugins()
{
    plugin_manager_->registerLoader(std::make_unique<CppPluginLoader>(*this));
    plugin_manager_->registerLoader(std::make_unique<PythonPluginLoader>(*this));

    auto plugin_dir = fs::current_path() / "plugins";

    if (exists(plugin_dir)) {
        plugin_manager_->loadPlugins(plugin_dir.string());
    }
    else {
        create_directories(plugin_dir);
    }
}

void EndstoneServer::enablePlugins(PluginLoadOrder type)
{
    if (type == PluginLoadOrder::PostWorld) {
        command_map_->setPluginCommands();
        DefaultPermissions::registerCorePermissions();
    }

    auto plugins = plugin_manager_->getPlugins();
    for (auto *plugin : plugins) {
        if (!plugin->isEnabled() && plugin->getDescription().getLoad() == type) {
            enablePlugin(*plugin);
        }
    }
}

void EndstoneServer::enablePlugin(Plugin &plugin)
{
    auto perms = plugin.getDescription().getPermissions();
    for (const auto &perm : perms) {
        if (plugin_manager_->addPermission(std::make_unique<Permission>(perm)) == nullptr) {
            getLogger().warning("Plugin {} tried to register permission '{}' that was already registered.",
                                plugin.getDescription().getFullName(), perm.getName());
        }
    }
    plugin_manager_->dirtyPermissibles(true);
    plugin_manager_->dirtyPermissibles(false);
    plugin_manager_->enablePlugin(plugin);
}

void EndstoneServer::disablePlugins() const
{
    plugin_manager_->disablePlugins();
}

Scheduler &EndstoneServer::getScheduler() const
{
    return *scheduler_;
}

Level *EndstoneServer::getLevel() const
{
    return level_.get();
}

void EndstoneServer::setLevel(std::unique_ptr<EndstoneLevel> level)
{
    level_ = std::move(level);
}

std::vector<Player *> EndstoneServer::getOnlinePlayers() const
{
    std::vector<Player *> result;
    result.reserve(players_.size());
    for (const auto &[id, player] : players_) {
        result.push_back(player);
    }
    return result;
}

int EndstoneServer::getMaxPlayers() const
{
    return getServerNetworkHandler().max_players_;
}

void EndstoneServer::setMaxPlayers(int max_players)
{
    if (max_players < 0) {
        getLogger().error("Unable to set the max number of players to a negative value: {}.", max_players);
        return;
    }
    if (max_players > ENDSTONE_MAX_PLAYERS) {
        getLogger().warning("Unable to set the max number of players to a value greater than {}.",
                            ENDSTONE_MAX_PLAYERS);
        return;
    }
    getServerNetworkHandler().max_players_ = max_players;
    getServerNetworkHandler().updateServerAnnouncement();
}

Player *EndstoneServer::getPlayer(endstone::UUID id) const
{
    auto it = players_.find(id);
    if (it != players_.end()) {
        return it->second;
    }
    return nullptr;
}

Player *EndstoneServer::getPlayer(std::string name) const
{
    for (const auto &[_, player] : players_) {
        if (boost::iequals(player->getName(), name)) {
            return player;
        }
    }
    return nullptr;
}

void EndstoneServer::shutdown()
{
    static_cast<EndstoneScheduler &>(getScheduler()).runTask([this]() {
        server_instance_.getMinecraft().requestServerShutdown("");
    });
}

void EndstoneServer::reload()
{
    plugin_manager_->clearPlugins();
    command_map_->clearCommands();
    reloadData();

    // TODO(server): Wait for at most 2.5 seconds for all async tasks to finish, otherwise issue a warning
    loadPlugins();
    enablePlugins(PluginLoadOrder::Startup);
    enablePlugins(PluginLoadOrder::PostWorld);
    ServerLoadEvent event{ServerLoadEvent::LoadType::Reload};
    getPluginManager().callEvent(event);

    // sync commands
    for (const auto &[uuid, player] : players_) {
        player->updateCommands();
    }
}

void EndstoneServer::reloadData()
{
    server_instance_.getMinecraft().requestResourceReload();
    level_->getHandle().loadFunctionManager();
}

void EndstoneServer::broadcast(const std::string &message, const std::string &permission) const
{
    std::unordered_set<const CommandSender *> recipients;
    for (const auto *permissible : getPluginManager().getPermissionSubscriptions(permission)) {
        const auto *sender = permissible->asCommandSender();
        if (sender != nullptr && sender->hasPermission(permission)) {
            recipients.insert(sender);
        }
    }

    BroadcastMessageEvent event{!isPrimaryThread(), message, recipients};
    getPluginManager().callEvent(event);

    if (event.isCancelled()) {
        return;
    }

    for (const auto &recipient : recipients) {
        recipient->sendMessage(event.getMessage());
    }
}

void EndstoneServer::broadcastMessage(const std::string &message) const
{
    broadcast(message, BroadcastChannelUser);
}

bool EndstoneServer::isPrimaryThread() const
{
    return Bedrock::Threading::getServerThread().isOnThread();
}

Scoreboard *EndstoneServer::getScoreboard() const
{
    return scoreboard_.get();
}

void EndstoneServer::setScoreboard(std::unique_ptr<EndstoneScoreboard> scoreboard)
{
    scoreboard_ = std::move(scoreboard);
}

std::shared_ptr<Scoreboard> EndstoneServer::getNewScoreboard()
{
    auto registry = CommandSoftEnumRegistry();
    auto board = ServerScoreboard::create(registry, nullptr, level_->getHandle()._getGameplayUserManagerStackRef());
    board->setPacketSender(level_->getHandle().getPacketSender());
    auto result = std::make_shared<EndstoneScoreboard>(std::move(board));
    scoreboards_.emplace_back(result);
    return result;
}

float EndstoneServer::getCurrentMillisecondsPerTick()
{
    return current_mspt_;
}

float EndstoneServer::getAverageMillisecondsPerTick()
{
    return std::accumulate(average_mspt_, average_mspt_ + TargetTicksPerSecond, 0.0F) / TargetTicksPerSecond;
}

float EndstoneServer::getCurrentTicksPerSecond()
{
    return current_tps_;
}

float EndstoneServer::getAverageTicksPerSecond()
{
    return std::accumulate(average_tps_, average_tps_ + TargetTicksPerSecond, 0.0F) / TargetTicksPerSecond;
}

float EndstoneServer::getCurrentTickUsage()
{
    return current_usage_;
}

float EndstoneServer::getAverageTickUsage()
{
    return std::accumulate(average_usage_, average_usage_ + TargetTicksPerSecond, 0.0F) / TargetTicksPerSecond;
}

std::chrono::system_clock::time_point EndstoneServer::getStartTime()
{
    return start_time_;
}

EndstoneScoreboard &EndstoneServer::getPlayerBoard(const EndstonePlayer &player) const
{
    auto it = player_boards_.find(&player);
    if (it == player_boards_.end()) {
        return *scoreboard_;
    }
    return *it->second;
}

void EndstoneServer::setPlayerBoard(EndstonePlayer &player, Scoreboard &scoreboard)
{
    auto &old_board = getPlayerBoard(player).getHandle();
    auto &new_board = static_cast<EndstoneScoreboard &>(scoreboard).getHandle();

    if (&old_board == &new_board) {
        return;
    }

    // remove player from the old board
    getPlayerBoard(player).resetScores(&player);

    // add player to the new board
    new_board.onPlayerJoined(player.getHandle());

    // update tracking records
    if (&scoreboard == scoreboard_.get()) {
        player_boards_.erase(&player);
    }
    else {
        player_boards_[&player] = std::static_pointer_cast<EndstoneScoreboard>(scoreboard.shared_from_this());
    }
}

void EndstoneServer::removePlayerBoard(EndstonePlayer &player)
{
    player_boards_.erase(&player);
}

::ServerNetworkHandler &EndstoneServer::getServerNetworkHandler() const
{
    return *server_instance_.getMinecraft().getServerNetworkHandler();
}

void EndstoneServer::tick(std::uint64_t current_tick, const std::function<void()> &tick_function)
{
    using namespace std::chrono;

    const auto tick_time = steady_clock::now();

    scheduler_->mainThreadHeartbeat(current_tick);
    tick_function();

    current_mspt_ = static_cast<float>(duration_cast<milliseconds>(steady_clock::now() - tick_time).count());
    current_tps_ = std::min(static_cast<float>(TargetTicksPerSecond), 1000.0F / std::max(1.0F, current_mspt_));
    current_usage_ = std::min(1.0F, current_mspt_ / TargetMillisecondsPerTick);
    const auto idx = current_tick % TargetTicksPerSecond;
    average_mspt_[idx] = current_mspt_;
    average_tps_[idx] = current_tps_;
    average_usage_[idx] = current_usage_;
}

}  // namespace endstone::detail
