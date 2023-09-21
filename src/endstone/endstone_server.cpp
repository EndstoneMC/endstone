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

#include "endstone/endstone_server.h"

#include "bedrock/command_registry.h"
#include "endstone/command/plugin_command.h"
#include "endstone/logger_factory.h"
#include "endstone/plugin/cpp/cpp_plugin_loader.h"
#include "endstone/plugin/python/python_plugin_loader.h"
#include "endstone/plugin/simple_plugin_manager.h"
#include "endstone/versioning.h"

EndstoneServer::EndstoneServer() noexcept : logger_(LoggerFactory::getLogger("Server"))
{
    command_map_ = std::make_unique<SimpleCommandMap>(*this);
    plugin_manager_ = std::make_unique<SimplePluginManager>(*this, *command_map_);
}

void EndstoneServer::loadPlugins() noexcept
{
    plugin_manager_->registerLoader(std::make_unique<PythonPluginLoader>(*this, "endstone.plugin", "ZipPluginLoader"));
    plugin_manager_->registerLoader(
        std::make_unique<PythonPluginLoader>(*this, "endstone.plugin", "SourcePluginLoader"));
    plugin_manager_->registerLoader(std::make_unique<CppPluginLoader>(*this));

    auto plugin_folder = std::filesystem::current_path() / "plugins";

    if (exists(plugin_folder)) {
        auto plugins = plugin_manager_->loadPlugins(plugin_folder);
        for (const auto &plugin : plugins) {
            plugin->getLogger().info("Loading {}", plugin->getDescription().getFullName());
            plugin->onLoad();
        }
    }
    else {
        create_directories(plugin_folder);
    }
}

void EndstoneServer::enablePlugins() const noexcept
{
    auto plugins = plugin_manager_->getPlugins();
    for (const auto &plugin : plugins) {
        if (!plugin->isEnabled()) {
            enablePlugin(*plugin);
        }
    }

    command_map_->setFallbackCommands();
    setBedrockCommands();
    // TODO(command): send new available command packet to clients
}

void EndstoneServer::enablePlugin(Plugin &plugin) const noexcept
{
    auto permissions = plugin.getDescription().getPermissions();
    for (const auto &permission : permissions) {
        if (!plugin_manager_->addPermission(permission, false)) {
            getLogger().warning("Plugin {} tried to register permission {} but it's already registered.",
                                plugin.getDescription().getFullName(), permission->getName());
        }
    }

    plugin_manager_->dirtyPermissibles();
    plugin_manager_->enablePlugin(plugin);
}

void EndstoneServer::disablePlugins() const noexcept
{
    plugin_manager_->disablePlugins();
}

Logger &EndstoneServer::getLogger() const noexcept
{
    return logger_;
}

bool EndstoneServer::dispatchCommand(CommandSender &sender, const std::string &command_line) const noexcept
{
    if (command_map_->dispatch(sender, command_line)) {
        return true;
    }

    sender.sendMessage(ChatColor::Red + "Unknown command. Type \"/help\" for help.");
    return false;
}

CommandSender &EndstoneServer::getConsoleSender() const noexcept
{
    return *console_;
}

[[maybe_unused]] SimpleCommandMap &EndstoneServer::getCommandMap() const noexcept
{
    return *command_map_;
}

void EndstoneServer::setBedrockCommands() const noexcept
{
    for (const auto &item : CommandRegistry::mBedrockCommands) {
        command_map_->registerCommand("minecraft", item.second);
    }
}

PluginCommand *EndstoneServer::getPluginCommand(const std::string &name) const noexcept
{
    auto *command = command_map_->getCommand(name);
    return dynamic_cast<PluginCommand *>(command);
}

PluginManager &EndstoneServer::getPluginManager() const noexcept
{
    return *plugin_manager_;
}
