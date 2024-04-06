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
#include <unordered_map>
#include <utility>
#include <vector>

#include "endstone/command/command_executor.h"
#include "endstone/detail/plugin/plugin_description_builder.h"
#include "endstone/logger.h"
#include "endstone/permissions/permission.h"
#include "endstone/permissions/permission_default.h"
#include "endstone/plugin/plugin_description.h"
#include "endstone/server.h"

namespace endstone {

class PluginCommand;
class PluginLoader;

/**
 * Represents a Plugin
 */
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

    template <typename EventType, typename T>
    void registerEvent(void (T::*func)(EventType &), T &instance, EventPriority priority = EventPriority::Normal,
                       bool ignore_cancelled = false)
    {
        getServer().getPluginManager().registerEvent(
            EventType::NAME, [func, &instance](Event &e) { (instance.*func)(static_cast<EventType &>(e)); }, priority,
            *this, ignore_cancelled);
    }

    template <typename EventType>
    void registerEvent(std::function<void(EventType &)> func, EventPriority priority = EventPriority::Normal,
                       bool ignore_cancelled = false)
    {
        getServer().getPluginManager().registerEvent(
            EventType::NAME, [func](Event &e) { func(static_cast<EventType &>(e)); }, priority, *this,
            ignore_cancelled);
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

    bool enabled_{false};
    PluginLoader *loader_{nullptr};
    Server *server_{nullptr};
    Logger *logger_{nullptr};
};

}  // namespace endstone

#ifndef ENDSTONE_PLUGIN
#define ENDSTONE_PLUGIN(Name, Version, MainClass)                                            \
    class PluginDescriptionBuilderImpl : public endstone::detail::PluginDescriptionBuilder { \
    public:                                                                                  \
        PluginDescriptionBuilderImpl();                                                      \
    };                                                                                       \
    static PluginDescriptionBuilderImpl builder;                                             \
    class EndstonePluginImpl : public MainClass {                                            \
    public:                                                                                  \
        EndstonePluginImpl() = default;                                                      \
        const endstone::PluginDescription &getDescription() const override                   \
        {                                                                                    \
            return description_;                                                             \
        }                                                                                    \
                                                                                             \
    private:                                                                                 \
        endstone::PluginDescription description_ = builder.build(Name, Version);             \
    };                                                                                       \
    extern "C" [[maybe_unused]] ENDSTONE_EXPORT endstone::Plugin *init_endstone_plugin()     \
    {                                                                                        \
        auto *p = new EndstonePluginImpl();                                                  \
        return p;                                                                            \
    }                                                                                        \
    PluginDescriptionBuilderImpl::PluginDescriptionBuilderImpl()
#endif
