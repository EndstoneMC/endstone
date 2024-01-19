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

#include "endstone/logger.h"
#include "endstone/plugin/plugin.h"
#include "endstone/server.h"

class PluginLoader {
public:
    explicit PluginLoader(Server &server) noexcept : server_(server) {}
    PluginLoader(const PluginLoader &) = delete;
    PluginLoader &operator=(const PluginLoader &) = delete;

    virtual ~PluginLoader() = default;
    [[nodiscard]] virtual std::vector<Plugin *> loadPlugins(const std::string &directory) noexcept = 0;

    virtual void enablePlugin(Plugin &plugin) const
    {
        if (!plugin.isEnabled()) {
            plugin.getLogger().info("Enabling {}", plugin.getDescription().getFullName());
            plugin.setEnabled(true);
        }
    }

    virtual void disablePlugin(Plugin &plugin) const
    {
        if (plugin.isEnabled()) {
            plugin.getLogger().info("Disabling {}", plugin.getDescription().getFullName());
            plugin.setEnabled(false);
        }
    }

    [[nodiscard]] virtual Server &getServer() const
    {
        return server_;
    }

protected:
    void initPlugin(Plugin &plugin, Logger &logger)
    {
        plugin.loader_ = this;
        plugin.server_ = &server_;
        plugin.logger_ = &logger;
    }

private:
    Server &server_;
};
