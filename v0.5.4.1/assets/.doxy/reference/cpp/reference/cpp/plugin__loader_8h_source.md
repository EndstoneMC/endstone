

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

#include <cctype>
#include <filesystem>
#include <memory>
#include <string>
#include <utility>
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

    [[nodiscard]] virtual std::vector<Plugin *> loadPlugins(const std::string &directory) = 0;

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


