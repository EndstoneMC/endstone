

# File plugin\_loader.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**plugin**](dir_53ee43673b2467e53c4cb8c30a2e7d89.md) **>** [**plugin\_loader.h**](plugin__loader_8h.md)

[Go to the documentation of this file](plugin__loader_8h.md)


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

class PluginLoader {
public:
    explicit PluginLoader(Server &server) : server_(server) {}
    PluginLoader(const PluginLoader &) = delete;
    PluginLoader &operator=(const PluginLoader &) = delete;

    virtual ~PluginLoader() = default;

    [[nodiscard]] virtual Plugin *loadPlugin(std::string file) = 0;

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

    [[nodiscard]] virtual std::vector<std::string> getPluginFileFilters() const = 0;

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

    [[nodiscard]] Server &getServer() const
    {
        return server_;
    }

protected:
    Server &server_;
};

}  // namespace endstone
```


