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
#include <memory>
#include <string>
#include <vector>

#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_loader.h"

class Permissible;
class Permission;

class PluginManager {
public:
    virtual ~PluginManager() noexcept = default;
    virtual void registerLoader(std::unique_ptr<PluginLoader> loader) noexcept = 0;
    [[nodiscard]] virtual Plugin *getPlugin(const std::string &name) const noexcept = 0;
    [[nodiscard]] virtual std::vector<Plugin *> getPlugins() const noexcept = 0;
    [[maybe_unused]] [[nodiscard]] virtual bool isPluginEnabled(const std::string &name) const noexcept = 0;
    virtual bool isPluginEnabled(Plugin *plugin) const noexcept = 0;
    virtual Plugin *loadPlugin(const std::filesystem::path &file) noexcept = 0;
    virtual std::vector<Plugin *> loadPlugins(const std::filesystem::path &directory) noexcept = 0;
    virtual void enablePlugin(Plugin &plugin) const noexcept = 0;
    virtual void disablePlugin(Plugin &plugin) const noexcept = 0;
    virtual void disablePlugins() const noexcept = 0;
    [[maybe_unused]] virtual void clearPlugins() noexcept = 0;

    [[nodiscard]] virtual std::vector<Permissible> getPermissionSubscriptions(
        std::string permission) const noexcept = 0;
    virtual void recalculatePermissionDefaults(Permission &permission) noexcept = 0;
};
