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

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "endstone/command/command_executor.h"
#include "endstone/logger.h"
#include "endstone/plugin/plugin_description.h"
#include "endstone/server.h"

class PluginLoader;
class PluginCommand;
class PluginDescription;

class Plugin : public CommandExecutor {
public:
    Plugin() noexcept = default;
    ~Plugin() noexcept override = default;

    [[nodiscard]] virtual const PluginDescription &getDescription() const noexcept = 0;

    /**
     * Called after a plugin is loaded but before it has been enabled.
     *
     * When multiple plugins are loaded, the onLoad() for all plugins is
     * called before any onEnable() is called.
     */
    virtual void onLoad() noexcept {}

    /**
     * Called when this plugin is enabled
     */
    virtual void onEnable() noexcept {}

    /**
     * Called when this plugin is disabled
     */
    virtual void onDisable() noexcept {}

    bool onCommand(const CommandSender &sender, const Command &command, const std::string &label,
                   const std::vector<std::string> &args) noexcept override
    {
        return false;
    }

    /**
     * Returns the plugin logger associated with this server's logger. The
     * returned logger automatically tags all log messages with the plugin's
     * name.
     *
     * @return Logger associated with this plugin
     */
    [[nodiscard]] Logger &getLogger() const noexcept
    {
        return *logger_;
    }

    /**
     * Returns a value indicating whether or not this plugin is currently
     * enabled
     *
     * @return true if this plugin is enabled, otherwise false
     */
    [[nodiscard]] bool isEnabled() const noexcept
    {
        return enabled_;
    }

    /**
     * Gets the associated PluginLoader responsible for this plugin
     *
     * @return PluginLoader that controls this plugin
     */
    [[nodiscard]] PluginLoader &getPluginLoader() const noexcept
    {
        return *loader_;
    }

    /**
     * Returns the Server instance currently running this plugin
     *
     * @return Server running this plugin
     */
    [[nodiscard]] Server &getServer() const noexcept
    {
        return *server_;
    }

    /**
     * Gets the command with the given name, specific to this plugin.
     *
     * @param name name or alias of the command
     * @return the plugin command if found, otherwise null
     */
    [[nodiscard]] PluginCommand *getCommand(const std::string &name) const noexcept
    {
        auto alias = name;
        std::transform(alias.begin(), alias.end(), alias.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        auto *command = getServer().getPluginCommand(alias);
        if (!command) {
            auto prefix = getDescription().getName();
            std::transform(prefix.begin(), prefix.end(), prefix.begin(), [](unsigned char c) {
                return std::tolower(c);
            });

            command = getServer().getPluginCommand(prefix + ":" + alias);
        }

        return command;
    }

private:
    friend class PluginLoader;

    /**
     * Sets the enabled state of this plugin
     *
     * @param enabled true if enabled, otherwise false
     */
    void setEnabled(bool enabled) noexcept
    {
        if (enabled_ != enabled) {
            enabled_ = enabled;

            if (enabled_) {
                onEnable();
            }
            else {
                onDisable();
            }
        }
    }

    bool enabled_ = false;
    PluginLoader *loader_ = nullptr;
    Server *server_ = nullptr;
    std::unique_ptr<Logger> logger_;
};

#ifndef ENDSTONE_PLUGIN_CLASS
#define ENDSTONE_PLUGIN_CLASS(ClassName) (extern "C" ENDSTONE_API Plugin * createPlugin() { return new ClassName(); })
#endif
