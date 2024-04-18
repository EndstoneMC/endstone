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

#include "bedrock/common.h"
#include "bedrock/threading.h"
#include "bedrock/world/actor/player/player.h"
#include "endstone/command/plugin_command.h"
#include "endstone/detail/command/command_map.h"
#include "endstone/detail/level.h"
#include "endstone/detail/logger_factory.h"
#include "endstone/detail/permissions/default_permissions.h"
#include "endstone/detail/plugin/cpp_plugin_loader.h"
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

Logger &EndstoneServer::getLogger() const
{
    return logger_;
}

EndstoneCommandMap &EndstoneServer::getCommandMap() const
{
    return *command_map_;
}

MinecraftCommands &EndstoneServer::getMinecraftCommands()
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

CommandSender &EndstoneServer::getCommandSender() const
{
    return command_sender_;
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

Player *EndstoneServer::getPlayer(endstone::UUID id) const
{
    for (const auto *level : getLevels()) {
        auto uuid = mce::UUID{id.bits.most_significant, id.bits.least_significant};
        auto *player = static_cast<const EndstoneLevel *>(level)->getBedrockLevel().getPlayer(uuid);
        if (player) {
            return &player->getEndstonePlayer();
        }
    }
    return nullptr;
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

bool EndstoneServer::isPrimaryThread() const
{
    return Bedrock::Threading::getServerThread().isOnThread();
}

}  // namespace endstone::detail
