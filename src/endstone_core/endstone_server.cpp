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

#include "endstone_core/endstone_server.h"

#include <filesystem>
#include <memory>
namespace fs = std::filesystem;

#include "bedrock/common.h"
#include "endstone_core/logger_factory.h"
#include "endstone_core/plugin/cpp_plugin_loader.h"

#if !defined(ENDSTONE_VERSION)
#error ENDSTONE_VERSION is not defined
#endif

EndstoneServer::EndstoneServer() : logger_(LoggerFactory::getLogger("EndstoneServer"))
{
    plugin_manager_ = std::make_unique<EndstonePluginManager>(*this);
    plugin_manager_->registerLoader(std::make_unique<CppPluginLoader>(*this));
}

void EndstoneServer::loadPlugins()
{
    auto plugin_dir = fs::current_path() / "plugins";

    if (exists(plugin_dir)) {
        auto plugins = plugin_manager_->loadPlugins(plugin_dir.string());
        for (const auto &plugin : plugins) {
            plugin->getLogger().info("Loading {}", plugin->getDescription().getFullName());
            plugin->onLoad();
        }
    }
    else {
        create_directories(plugin_dir);
    }
}

void EndstoneServer::enablePlugins() const
{
    auto plugins = plugin_manager_->getPlugins();
    for (const auto &plugin : plugins) {
        if (!plugin->isEnabled()) {
            enablePlugin(*plugin);
        }
    }
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

PluginManager &EndstoneServer::getPluginManager() const
{
    return *plugin_manager_;
}

std::string_view EndstoneServer::getVersion() const
{
    return ENDSTONE_VERSION;
}

std::string EndstoneServer::getMinecraftVersion() const
{
    return Common::getGameVersionString();
}
