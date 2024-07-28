

# File plugin.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**plugin**](dir_53ee43673b2467e53c4cb8c30a2e7d89.md) **>** [**plugin.h**](plugin_8h.md)

[Go to the documentation of this file](plugin_8h.md)


```C++
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
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "endstone/command/command_executor.h"
#include "endstone/detail/plugin/plugin_description_builder.h"
#include "endstone/logger.h"
#include "endstone/permissions/permission.h"
#include "endstone/plugin/plugin_description.h"
#include "endstone/server.h"

namespace endstone {

class PluginCommand;
class PluginLoader;

namespace detail {
class EndstonePluginManager;
}

class Plugin : public CommandExecutor {
public:
    Plugin() = default;
    Plugin(const Plugin &) = delete;
    Plugin &operator=(const Plugin &) = delete;

    ~Plugin() override = default;

    [[nodiscard]] virtual const PluginDescription &getDescription() const = 0;

    virtual void onLoad() {}

    virtual void onEnable() {}

    virtual void onDisable() {}

    [[nodiscard]] Logger &getLogger() const
    {
        return *logger_;
    }

    [[nodiscard]] bool isEnabled() const
    {
        return enabled_;
    }

    [[nodiscard]] PluginLoader &getPluginLoader() const
    {
        return *loader_;
    }

    [[nodiscard]] Server &getServer() const
    {
        return *server_;
    }

    [[nodiscard]] std::string getName() const
    {
        return getDescription().getName();
    };

    [[nodiscard]] PluginCommand *getCommand(std::string name) const
    {
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
        return getServer().getPluginCommand(name);
    }

    [[nodiscard]] const std::filesystem::path &getDataFolder() const
    {
        return data_folder_;
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

protected:
    friend class PluginLoader;
    friend class detail::EndstonePluginManager;

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

private:
    bool enabled_{false};
    PluginLoader *loader_{nullptr};
    Server *server_{nullptr};
    Logger *logger_{nullptr};
    std::filesystem::path data_folder_;
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
```


