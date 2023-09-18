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

#include "endstone/plugin/simple_plugin_manager.h"

#include <algorithm>
#include <utility>

#include "endstone/command/plugin_command.h"
#include "endstone/plugin/plugin_manager.h"
#include "endstone/server.h"

SimplePluginManager::SimplePluginManager(Server &server, SimpleCommandMap &command_map) noexcept
    : server_(server), command_map_(command_map)
{
}

void SimplePluginManager::registerLoader(std::unique_ptr<PluginLoader> loader) noexcept
{
    auto patterns = loader->getPluginFileFilters();
    for (const auto &pattern : patterns) {
        file_associations_[pattern] = std::move(loader);
    }
}

Plugin *SimplePluginManager::getPlugin(const std::string &name) const noexcept
{
    auto it = lookup_names_.find(name);
    if (it != lookup_names_.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<Plugin *> SimplePluginManager::getPlugins() const noexcept
{
    std::vector<Plugin *> plugins;
    plugins.reserve(plugins_.size());
    for (const auto &plugin : plugins_) {
        plugins.push_back(plugin.get());
    }
    return plugins;
}

bool SimplePluginManager::isPluginEnabled(const std::string &name) const noexcept
{
    return isPluginEnabled(getPlugin(name));
}

bool SimplePluginManager::isPluginEnabled(Plugin *plugin) const noexcept
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

Plugin *SimplePluginManager::loadPlugin(const std::filesystem::path &file) noexcept
{
    if (!exists(file)) {
        server_.getLogger().error("Could not load plugin from '{}': Provided file does not exist.", file.string());
    }

    for (const auto &[pattern, loader] : file_associations_) {
        std::regex r(pattern);
        if (std::regex_search(file.string(), r)) {
            auto plugin = loader->loadPlugin(file.string());

            if (plugin) {
                const auto plugin_ptr = plugin.get();
                auto name = plugin->getDescription().getName();

                if (!std::regex_match(name, PluginDescription::VALID_NAME)) {
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

std::vector<Plugin *> SimplePluginManager::loadPlugins(const std::filesystem::path &directory) noexcept
{
    if (!std::filesystem::exists(directory)) {
        server_.getLogger().error(
            "Error occurred when trying to load plugins in '{}': Provided directory does not exist.",
            directory.string());
        return {};
    }

    if (!std::filesystem::is_directory(directory)) {
        server_.getLogger().error(
            "Error occurred when trying to load plugins in '{}': Provided path is not a directory.",
            directory.string());
        return {};
    }

    std::vector<Plugin *> loaded_plugins;

    for (const auto &entry : std::filesystem::directory_iterator(directory)) {
        std::filesystem::path file;

        // If it's a regular file, try to load it as a plugin.
        if (std::filesystem::is_regular_file(entry.status())) {
            file = entry.path();
        }
        // If it's a subdirectory, look for a plugin.toml inside it.
        else if (std::filesystem::is_directory(entry.status())) {
            file = entry.path() / "plugin.toml";
            if (!std::filesystem::exists(file) || !std::filesystem::is_regular_file(file)) {
                continue;
            }
        }
        else {
            continue;
        }

        auto plugin = loadPlugin(file);
        if (plugin) {
            loaded_plugins.push_back(plugin);
        }
    }

    return loaded_plugins;
}

void SimplePluginManager::enablePlugin(Plugin &plugin) const noexcept
{
    if (!plugin.isEnabled()) {
        auto commands = plugin.getDescription().getCommands();

        if (!commands.empty()) {
            auto plugin_commands = std::vector<std::shared_ptr<Command>>(commands.size());
            std::transform(commands.begin(), commands.end(), plugin_commands.begin(), [&plugin](const auto &command) {
                return std::make_shared<PluginCommand>(*command, plugin);
            });

            command_map_.registerAll(plugin.getDescription().getName(), plugin_commands);
        }

        plugin.getPluginLoader().enablePlugin(plugin);
    }
}

void SimplePluginManager::disablePlugin(Plugin &plugin) const noexcept
{
    if (plugin.isEnabled()) {
        plugin.getPluginLoader().disablePlugin(plugin);
    }
}

void SimplePluginManager::disablePlugins() const noexcept
{
    for (const auto &plugin : plugins_) {
        disablePlugin(*plugin);
    }
}

void SimplePluginManager::clearPlugins() noexcept
{
    disablePlugins();
    plugins_.clear();
    lookup_names_.clear();
}
