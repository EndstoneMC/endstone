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

#if defined(__GNUC__) && __GNUC__ < 8
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

#include "endstone/plugin/plugin_loader.h"
#include "endstone/server.h"

EndstonePluginManager::EndstonePluginManager(Server &server) : server_(server) {}

void EndstonePluginManager::registerLoader(std::unique_ptr<PluginLoader> loader)
{
    auto patterns = loader->getPluginFileFilters();
    for (const auto &pattern : patterns) {
        file_associations_[pattern] = std::move(loader);
    }
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
        plugins.push_back(plugin.get());
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
    auto it = std::find_if(plugins_.begin(), plugins_.end(), [plugin](const std::unique_ptr<Plugin> &p) {
        return p.get() == plugin;
    });

    // If plugin is in the vector and is enabled, return true
    return it != plugins_.end() && plugin->isEnabled();
}

Plugin *EndstonePluginManager::loadPlugin(const fs::path &file)
{
    if (!exists(file)) {
        server_.getLogger().error("Could not load plugin from '{}': Provided file does not exist.", file.string());
    }

    for (const auto &[pattern, loader] : file_associations_) {
        std::regex r(pattern);
        if (std::regex_search(file.string(), r)) {
            auto plugin = loader->loadPlugin(file.string());

            if (plugin) {
                auto *const plugin_ptr = plugin.get();
                auto name = plugin->getDescription().getName();

                if (!std::regex_match(name, PluginDescription::ValidName)) {
                    server_.getLogger().error(
                        "Could not load plugin from '{}': Plugin name contains invalid characters.", file.string());
                    return nullptr;
                }

                lookup_names_[name] = plugin_ptr;
                plugins_.push_back(std::move(plugin));
                return lookup_names_[name];
            }
        }
    }

    return nullptr;
}

std::vector<Plugin *> EndstonePluginManager::loadPlugins(const fs::path &directory)
{
    if (!exists(directory)) {
        server_.getLogger().error(
            "Error occurred when trying to load plugins in '{}': Provided directory does not exist.",
            directory.string());
        return {};
    }

    if (!is_directory(directory)) {
        server_.getLogger().error(
            "Error occurred when trying to load plugins in '{}': Provided path is not a directory.",
            directory.string());
        return {};
    }

    std::vector<Plugin *> loaded_plugins;

    for (const auto &entry : fs::directory_iterator(directory)) {
        fs::path file;

        // If it's a regular file, try to load it as a plugin.
        if (is_regular_file(entry.status())) {
            file = entry.path();
        }
        // If it's a subdirectory, look for a plugin.toml inside it.
        else if (is_directory(entry.status())) {
            file = entry.path() / "plugin.toml";
            if (!exists(file) || !is_regular_file(file)) {
                continue;
            }
        }
        else {
            continue;
        }

        auto *plugin = loadPlugin(file);
        if (plugin) {
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
