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
#include <utility>
#include <vector>

#include "endstone/command/command_executor.h"
#include "endstone/logger.h"
#include "endstone/plugin/plugin_description.h"
#include "endstone/server.h"

namespace endstone {

class PluginCommand;
class PluginLoader;

class Plugin : public CommandExecutor {
public:
    Plugin() = default;
    Plugin(const Plugin &) = delete;
    Plugin &operator=(const Plugin &) = delete;

    ~Plugin() override = default;

    /**
     * Returns the details of this plugin
     * @return Details of this plugin
     */
    [[nodiscard]] virtual const PluginDescription &getDescription() const = 0;

    /**
     * Called after a plugin is loaded but before it has been enabled.
     *
     * When multiple plugins are loaded, the onLoad() for all plugins is
     * called before any onEnable() is called.
     */
    virtual void onLoad() {}

    /**
     * Called when this plugin is enabled
     */
    virtual void onEnable() {}

    /**
     * Called when this plugin is disabled
     */
    virtual void onDisable() {}

    /**
     * Returns the plugin logger associated with this server's logger. The
     * returned logger automatically tags all log messages with the plugin's
     * name.
     *
     * @return Logger associated with this plugin
     */
    [[nodiscard]] Logger &getLogger() const
    {
        return *logger_;
    }

    /**
     * Returns a value indicating whether this plugin is currently
     * enabled
     *
     * @return true if this plugin is enabled, otherwise false
     */
    [[nodiscard]] bool isEnabled() const
    {
        return enabled_;
    }

    /**
     * Gets the associated PluginLoader responsible for this plugin
     *
     * @return PluginLoader that controls this plugin
     */
    [[nodiscard]] PluginLoader &getPluginLoader() const
    {
        return *loader_;
    }

    /**
     * Returns the Server instance currently running this plugin
     *
     * @return Server running this plugin
     */
    [[nodiscard]] Server &getServer() const
    {
        return *server_;
    }

    /**
     * Returns the name of the plugin.
     *
     * This should return the bare name of the plugin and should be used for
     * comparison.
     *
     * @return name of the plugin
     */
    [[nodiscard]] std::string getName() const
    {
        return getDescription().getName();
    };

    /**
     * Registers a new PluginCommand.
     *
     * @param name The name of the command.
     * @param description The description of the command. Default is an empty string.
     * @param usages A vector containing different usage examples of the command. Default is an empty vector.
     * @param aliases A vector containing different aliases for the command. Default is an empty vector.
     * @return A pointer to the registered PluginCommand.
     * @return a pointer to the registered plugin command
     */
    PluginCommand *registerCommand(std::string name, std::string description = "", std::vector<std::string> usages = {},
                                   std::vector<std::string> aliases = {})
    {
        return getServer().registerPluginCommand(std::make_unique<PluginCommand>(
            *this, std::move(name), std::move(description), std::move(usages), std::move(aliases)));
    }

    /**
     * Gets the command with the given name, specific to this plugin.
     *
     * @param name name or alias of the command
     * @return the plugin command if found, otherwise null
     */
    [[nodiscard]] PluginCommand *getCommand(std::string name) const
    {
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
        return getServer().getPluginCommand(name);
    }

private:
    friend class PluginLoader;

    /**
     * Sets the enabled state of this plugin
     *
     * @param enabled true if enabled, otherwise false
     */
    void setEnabled(bool enabled)
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
    Logger *logger_ = nullptr;
};

class PluginCommand : public Command {
public:
    PluginCommand(Plugin &owner, std::string name, std::string description = "", std::vector<std::string> usages = {},
                  std::vector<std::string> aliases = {})
        : Command(std::move(name), std::move(description), std::move(usages), std::move(aliases)), owner_(owner)
    {
    }

    bool execute(CommandSender &sender, const std::vector<std::string> &args) const override
    {
        if (!owner_.isEnabled()) {
            sender.sendMessage("Cannot execute command '{}' in plugin {}. Plugin is disabled.", getName(),
                               getPlugin().getDescription().getFullName());
            return false;
        }

        try {
            return getExecutor().onCommand(sender, *this, args);
        }
        catch (std::exception &e) {
            getPlugin().getLogger().error("Unhandled exception executing command '{}' in plugin {}", getName(),
                                          owner_.getDescription().getFullName());
            getPlugin().getLogger().error(e.what());
            return false;
        }
    }

    /**
     * Sets the CommandExecutor to run when parsing this command
     *
     * @param executor New executor to run
     */
    virtual void setExecutor(std::shared_ptr<CommandExecutor> executor)
    {
        executor_ = std::move(executor);
    }

    /**
     * Gets the CommandExecutor associated with this command
     *
     * @return CommandExecutor object linked to this command
     */
    [[nodiscard]] virtual CommandExecutor &getExecutor() const
    {
        if (executor_) {
            return *executor_;
        }
        return owner_;
    }

    /**
     * Gets the owner of this PluginCommand
     *
     * @return Plugin that owns this command
     */
    [[maybe_unused]] [[nodiscard]] Plugin &getPlugin() const
    {
        return owner_;
    }

private:
    Plugin &owner_;
    std::shared_ptr<CommandExecutor> executor_;
};

}  // namespace endstone

#ifndef ENDSTONE_PLUGIN
#if defined(WIN32)
#define ENDSTONE_PLUGIN(ClassName)                                          \
    extern "C" __declspec(dllexport) endstone::Plugin *EndstonePluginInit() \
    {                                                                       \
        return new ClassName();                                             \
    }
#else
#define ENDSTONE_PLUGIN(ClassName)                                                           \
    extern "C" __attribute__((visibility("default"))) endstone::Plugin *EndstonePluginInit() \
    {                                                                                        \
        return new ClassName();                                                              \
    }
#endif
#endif
