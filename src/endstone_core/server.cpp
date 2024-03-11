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
#include "endstone/detail/command/command_map.h"
#include "endstone/detail/logger_factory.h"
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
        auto plugins = plugin_manager_->loadPlugins(plugin_dir.string());

        std::vector<std::string> plugin_names;
        plugin_names.reserve(plugins.size());
        std::transform(plugins.begin(), plugins.end(), std::back_inserter(plugin_names), [](const auto &plugin) {
            std::string name = plugin->getName();
            std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
            return name;
        });
        command_map_->addEnumValues("PluginName", plugin_names);
    }
    else {
        create_directories(plugin_dir);
    }
}

void EndstoneServer::enablePlugins() const
{
    plugin_manager_->enablePlugins();
}

void EndstoneServer::enablePlugin(Plugin &plugin) const
{
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
    auto *command = command_map_->getCommand(name);
    // TODO: avoid dynamic_cast according to Google's guideline
    // see https://google.github.io/styleguide/cppguide.html#Run-Time_Type_Information__RTTI_
    return dynamic_cast<PluginCommand *>(command);
}

PluginCommand *EndstoneServer::registerPluginCommand(std::shared_ptr<PluginCommand> command) const
{
    auto name = command->getName();
    if (command_map_->registerCommand(command)) {
        return getPluginCommand(name);
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

}  // namespace endstone::detail
