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

class PluginLoader {
public:
    explicit PluginLoader(Server &server) noexcept : server_(server) {}

    virtual ~PluginLoader() noexcept = default;
    virtual std::unique_ptr<Plugin> loadPlugin(const std::string &file) noexcept = 0;
    [[nodiscard]] virtual std::vector<std::string> getPluginFileFilters() const noexcept = 0;
    virtual void enablePlugin(Plugin &plugin) const noexcept
    {
        if (!plugin.isEnabled()) {
            plugin.getLogger().info("Enabling {}", plugin.getDescription().getFullName());
            plugin.setEnabled(true);
        }
    }
    virtual void disablePlugin(Plugin &plugin) const noexcept
    {
        if (plugin.isEnabled()) {
            plugin.getLogger().info("Disabling {}", plugin.getDescription().getFullName());
            plugin.setEnabled(false);
        }
    }

    [[nodiscard]] Server &getServer() const
    {
        return server_;
    }

protected:
    virtual void initPlugin(Plugin &plugin, std::unique_ptr<Logger> logger) noexcept
    {
        plugin.loader_ = this;
        plugin.server_ = &server_;
        plugin.logger_ = std::move(logger);
    }

private:
    Server &server_;
};
