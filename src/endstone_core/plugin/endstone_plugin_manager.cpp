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

#include "endstone_core/plugin/endstone_plugin_manager.h"

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "endstone/plugin/plugin_loader.h"
#include "endstone/server.h"

EndstonePluginManager::EndstonePluginManager(Server &server) : server_(server) {}

void EndstonePluginManager::registerLoader(std::unique_ptr<PluginLoader> loader)
{
    plugin_loaders_.push_back(std::move(loader));
}

Plugin *EndstonePluginManager::getPlugin(const std::string &name) const
{
    auto it = lookup_names_.find(name);
    if (it != lookup_names_.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<Plugin *> EndstonePluginManager::getPlugins() const
{
    std::vector<Plugin *> plugins;
    plugins.reserve(plugins_.size());
    for (const auto &plugin : plugins_) {
        plugins.push_back(plugin);
    }
    return plugins;
}

bool EndstonePluginManager::isPluginEnabled(const std::string &name) const
{
    return isPluginEnabled(getPlugin(name));
}

bool EndstonePluginManager::isPluginEnabled(Plugin *plugin) const
{
    if (!plugin) {
        return false;
    }

    // Check if the plugin exists in the vector
    auto it = std::find_if(plugins_.begin(), plugins_.end(), [plugin](const auto &p) {
        return p == plugin;
    });

    // If plugin is in the vector and is enabled, return true
    return it != plugins_.end() && plugin->isEnabled();
}

std::vector<Plugin *> EndstonePluginManager::loadPlugins(const std::string &directory)
{
    std::vector<Plugin *> loaded_plugins;

    for (const auto &loader : plugin_loaders_) {
        auto plugins = loader->loadPlugins(directory);
        for (const auto &plugin : plugins) {
            if (!plugin) {
                continue;
            }

            auto name = plugin->getDescription().getName();

            if (!std::regex_match(name, PluginDescription::ValidName)) {
                server_.getLogger().error("Could not load plugin '{}': Plugin name contains invalid characters.", name);
                continue;
            }

            plugins_.push_back(plugin);
            lookup_names_[name] = plugin;
            loaded_plugins.push_back(plugin);
        }
    }

    return loaded_plugins;
}

void EndstonePluginManager::enablePlugin(Plugin &plugin) const
{
    if (!plugin.isEnabled()) {
        plugin.getPluginLoader().enablePlugin(plugin);
    }
}

void EndstonePluginManager::disablePlugin(Plugin &plugin) const
{
    if (plugin.isEnabled()) {
        plugin.getPluginLoader().disablePlugin(plugin);
    }
}

void EndstonePluginManager::disablePlugins() const
{
    for (const auto &plugin : plugins_) {
        disablePlugin(*plugin);
    }
}

void EndstonePluginManager::clearPlugins()
{
    disablePlugins();
    plugins_.clear();
    lookup_names_.clear();
}
