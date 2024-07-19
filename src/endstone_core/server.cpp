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
#include "endstone/command/plugin_command.h"
#include "endstone/detail/command/command_map.h"
#include "endstone/detail/command/console_command_sender.h"
#include "endstone/detail/level/level.h"
#include "endstone/detail/logger_factory.h"
#include "endstone/detail/permissions/default_permissions.h"
#include "endstone/detail/plugin/cpp_plugin_loader.h"
#include "endstone/event/server/broadcast_message_event.h"
#include "endstone/plugin/plugin.h"

#if !defined(ENDSTONE_VERSION)
#error ENDSTONE_VERSION is not defined
#endif

namespace endstone::detail {

EndstoneServer::EndstoneServer(ServerInstance &server_instance)
    : server_instance_(server_instance), logger_(LoggerFactory::getLogger("Server"))
{
    command_map_ = std::make_unique<EndstoneCommandMap>(*this);
    plugin_manager_ = std::make_unique<EndstonePluginManager>(*this);
    plugin_manager_->registerLoader(std::make_unique<CppPluginLoader>(*this));
    command_sender_ = std::make_unique<EndstoneConsoleCommandSender>();
    scheduler_ = std::make_unique<EndstoneScheduler>(*this);
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
        command_map_->initialise();
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

std::vector<Level *> EndstoneServer::getLevels() const
{
    std::vector<Level *> levels;
    levels.reserve(levels_.size());
    for (const auto &it : levels_) {
        levels.push_back(it.second.get());
    }
    return levels;
}

Level *EndstoneServer::getLevel(std::string name) const
{
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
    auto it = levels_.find(name);
    if (it == levels_.end()) {
        return nullptr;
    }
    return it->second.get();
}

void EndstoneServer::addLevel(std::unique_ptr<Level> level)
{
    auto name = level->getName();
    if (getLevel(name) != nullptr) {
        getLogger().error("Level {} is a duplicate of another level and has been prevented from loading.", name);
        return;
    }
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
    levels_[name] = std::move(level);
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

void EndstoneServer::setScoreboard(std::unique_ptr<EndstoneScoreboard> scoreboard)
{
    scoreboard_ = std::move(scoreboard);
}

::ServerNetworkHandler &EndstoneServer::getServerNetworkHandler() const
{
    return *server_instance_.getMinecraft().getServerNetworkHandler();
}

}  // namespace endstone::detail
