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

#include <filesystem>
#include <regex>
#include <string>
#include <vector>

#include "endstone/event/server/plugin_disable_event.h"
#include "endstone/event/server/plugin_enable_event.h"
#include "endstone/logger.h"
#include "endstone/plugin/plugin.h"
#include "endstone/server.h"

namespace endstone {

/**
 * @brief Represents a plugin loader, which handles direct access to specific types of plugins.
 */
class PluginLoader {
public:
    explicit PluginLoader(Server &server) : server_(server) {}
    PluginLoader(const PluginLoader &) = delete;
    PluginLoader &operator=(const PluginLoader &) = delete;

    virtual ~PluginLoader() = default;

    /**
     * Loads the plugin contained in the specified file
     *
     * @param file File to attempt to load
     * @return Plugin that was contained in the specified file, or nullptr if unsuccessful
     */
    [[nodiscard]] virtual Plugin *loadPlugin(std::string file) = 0;

    /**
     * @brief Loads the plugin contained within the specified directory
     *
     * @param directory Directory to check for plugins
     * @return A list of all plugins loaded
     */
    [[nodiscard]] virtual std::vector<Plugin *> loadPlugins(std::string directory)
    {
        auto &logger = server_.getLogger();

        auto dir = std::filesystem::path(directory);
        if (!exists(dir)) {
            logger.error("Error occurred when trying to load plugins in '{}': Provided directory does not exist.",
                         dir.string());
            return {};
        }

        if (!is_directory(dir)) {
            logger.error("Error occurred when trying to load plugins in '{}': Provided path is not a directory.",
                         dir.string());
            return {};
        }

        std::vector<Plugin *> loaded_plugins;

        for (const auto &entry : std::filesystem::directory_iterator(dir)) {
            if (!is_regular_file(entry.status())) {
                continue;
            }

            const auto &file = entry.path();
            for (const auto &pattern : getPluginFileFilters()) {
                if (std::regex r(pattern); std::regex_search(file.string(), r)) {
                    if (auto *plugin = loadPlugin(file.string())) {
                        loaded_plugins.push_back(plugin);
                    }
                }
            }
        }

        return loaded_plugins;
    }

    /**
     * @brief Returns a list of all filename filters expected by this PluginLoader
     *
     * @return The filters
     */
    [[nodiscard]] virtual std::vector<std::string> getPluginFileFilters() const = 0;

    /**
     * @brief Enables the specified plugin
     * Attempting to enable a plugin that is already enabled will have no effect
     *
     * @param plugin Plugin to enable
     */
    virtual void enablePlugin(Plugin &plugin) const
    {
        if (!plugin.isEnabled()) {
            plugin.getLogger().info("Enabling {}", plugin.getDescription().getFullName());
            try {
                plugin.setEnabled(true);
            }
            catch (std::exception &e) {
                plugin.getLogger().error("Error occurred when enabling {}.", plugin.getDescription().getFullName());
                plugin.getLogger().error(e.what());
                return;
            }
            PluginEnableEvent event(plugin);
            server_.getPluginManager().callEvent(event);
        }
    }

    /**
     * @brief Disables the specified plugin
     * Attempting to disable a plugin that is not enabled will have no effect
     *
     * @param plugin Plugin to disable
     */
    virtual void disablePlugin(Plugin &plugin) const
    {
        if (plugin.isEnabled()) {
            plugin.getLogger().info("Disabling {}", plugin.getDescription().getFullName());
            try {
                plugin.setEnabled(false);
            }
            catch (std::exception &e) {
                plugin.getLogger().error("Error occurred when disabling {}.", plugin.getDescription().getFullName());
                plugin.getLogger().error(e.what());
                return;
            }
            PluginDisableEvent event(plugin);
            server_.getPluginManager().callEvent(event);
        }
    }

    /**
     * @brief Retrieves the Server object associated with the PluginLoader.
     *
     * This function returns a reference to the Server object that the PluginLoader is associated with.
     *
     * @return The Server reference.
     */
    [[nodiscard]] Server &getServer() const { return server_; }

protected:
    Server &server_;
};

}  // namespace endstone
