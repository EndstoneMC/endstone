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

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_loader.h"

namespace endstone {

class PluginManager {
public:
    PluginManager() = default;
    PluginManager(PluginManager const &) = delete;
    PluginManager &operator=(PluginManager const &) = delete;
    virtual ~PluginManager() = default;

    /**
     * Registers the specified plugin loader
     *
     * @param loader PluginLoader to register
     */
    virtual void registerLoader(std::unique_ptr<PluginLoader> loader) = 0;

    /**
     * Checks if the given plugin is loaded and returns it when applicable.
     * Please note that the name of the plugin is case-sensitive
     *
     * @param name Name of the plugin to check
     * @return Plugin if it exists, otherwise nullptr
     */
    [[nodiscard]] virtual Plugin *getPlugin(const std::string &name) const = 0;

    /**
     * Gets a list of all currently loaded plugins
     *
     * @return List of Plugins
     */
    [[nodiscard]] virtual std::vector<Plugin *> getPlugins() const = 0;

    /**
     * Checks if the given plugin is enabled or not
     * Please note that the name of the plugin is case-sensitive.
     *
     * @param name Name of the plugin to check
     * @return true if the plugin is enabled, otherwise false
     */
    [[nodiscard]] virtual bool isPluginEnabled(const std::string &name) const = 0;

    /**
     * Checks if the given plugin is enabled or not
     *
     * @param plugin Plugin to check
     * @return true if the plugin is enabled, otherwise false
     */
    [[nodiscard]] virtual bool isPluginEnabled(Plugin *plugin) const = 0;

    /**
     * Loads the plugin contained within the specified directory
     *
     * @param directory Directory to check for plugins
     * @return A list of all plugins loaded
     */
    [[nodiscard]] virtual std::vector<Plugin *> loadPlugins(const std::string &directory) = 0;

    /**
     * Enables the specified plugin
     * Attempting to enable a plugin that is already enabled will have no effect
     *
     * @param plugin Plugin to enable
     */
    virtual void enablePlugin(Plugin &plugin) const = 0;

    /**
     * Enable all the loaded plugins
     */
    virtual void enablePlugins() const = 0;

    /**
     * Disables the specified plugin
     * Attempting to disable a plugin that is not enabled will have no effect
     *
     * @param plugin Plugin to disable
     */
    virtual void disablePlugin(Plugin &plugin) const = 0;

    /**
     * Disables all the loaded plugins
     */
    virtual void disablePlugins() const = 0;

    /**
     * Disables and removes all plugins
     */
    virtual void clearPlugins() = 0;
};

}  // namespace endstone
