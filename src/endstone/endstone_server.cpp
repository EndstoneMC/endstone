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

EndstoneServer::EndstoneServer()
    : logger_(LoggerFactory::getLogger("Server")), command_map_(std::make_unique<SimpleCommandMap>(*this)),
      plugin_manager_(std::make_unique<SimplePluginManager>(*this, *command_map_))
{
}

void EndstoneServer::loadPlugins()
{
    try {
        plugin_manager_->registerLoader(
            std::make_unique<PythonPluginLoader>(*this, "endstone.plugin", "ZipPluginLoader"));
        plugin_manager_->registerLoader(
            std::make_unique<PythonPluginLoader>(*this, "endstone.plugin", "SourcePluginLoader"));
        plugin_manager_->registerLoader(std::make_unique<CppPluginLoader>(*this));

        auto plugin_folder = std::filesystem::current_path() / "plugins";

        if (exists(plugin_folder)) {
            auto plugins = plugin_manager_->loadPlugins(plugin_folder);
            for (const auto &plugin : plugins) {
                try {
                    plugin->getLogger()->info("Loading {}", plugin->getDescription().getFullName());
                    plugin->onLoad();
                }
                catch (std::exception &e) {
                    logger_->error("Error occurred when initializing {}: {}", plugin->getDescription().getFullName(),
                                   e.what());
                }
            }
        }
        else {
            create_directories(plugin_folder);
        }
    }
    catch (std::exception &e) {
        logger_->error("Error occurred when trying to load plugins: {}", e.what());
    }
}

void EndstoneServer::enablePlugins()
{
    auto plugins = plugin_manager_->getPlugins();
    for (const auto &plugin : plugins) {
        plugin_manager_->enablePlugin(*plugin);
    }

    command_map_->setFallbackCommands();
    setBedrockCommands();
    // TODO(permission): add permissions from plugin's description
    // TODO(command): send new available command packet to clients
}

void EndstoneServer::disablePlugins()
{
    plugin_manager_->disablePlugins();
}

std::shared_ptr<Logger> EndstoneServer::getLogger()
{
    return logger_;
}

bool EndstoneServer::dispatchCommand(CommandSender &sender, const std::string &command_line)
{
    if (command_map_->dispatch(sender, command_line)) {
        return true;
    }

    sender.sendMessage(ChatColor::Red + "Unknown command. Type \"/help\" for help.");

    return false;
}
CommandSender &EndstoneServer::getConsoleSender()
{
    return *console_;
}

SimpleCommandMap &EndstoneServer::getCommandMap() const
{
    return *command_map_;
}

void EndstoneServer::setBedrockCommands()
{
    for (const auto &item : CommandRegistry::bedrock_commands_) {
        command_map_->registerCommand("minecraft", item.second);
    }
}

std::shared_ptr<PluginCommand> EndstoneServer::getPluginCommand(const std::string &name)
{
    auto command = command_map_->getCommand(name);
    return std::dynamic_pointer_cast<PluginCommand>(command);
}
