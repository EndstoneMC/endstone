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

#include "endstone/logger.h"
#include "endstone/plugin/plugin_description.h"
#include "endstone/server.h"

class PluginLoader;

class Plugin {
public:
    Plugin() = default;
    Plugin(const Plugin &) = delete;
    Plugin &operator=(const Plugin &) = delete;

    virtual ~Plugin() = default;

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
     * Returns a value indicating whether or not this plugin is currently
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

private:
    friend class PluginLoader;
    friend class PluginLoaderBase;

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

#ifndef ENDSTONE_PLUGIN
#if defined(WIN32)
#define ENDSTONE_PLUGIN(ClassName)                                \
    extern "C" __declspec(dllexport) Plugin *EndstonePluginInit() \
    {                                                             \
        return new ClassName();                                   \
    }
#else
#define ENDSTONE_PLUGIN(ClassName)                                                 \
    extern "C" __attribute__((visibility("default"))) Plugin *EndstonePluginInit() \
    {                                                                              \
        return new ClassName();                                                    \
    }
#endif
#endif
