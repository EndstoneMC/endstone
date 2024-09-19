

# File plugin\_manager.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**plugin**](dir_53ee43673b2467e53c4cb8c30a2e7d89.md) **>** [**plugin\_manager.h**](plugin__manager_8h.md)

[Go to the documentation of this file](plugin__manager_8h.md)


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

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "endstone/event/event.h"
#include "endstone/event/event_priority.h"

namespace endstone {

class Permission;
class Permissible;
class Plugin;
class PluginLoader;

class PluginManager {
public:
    PluginManager() = default;
    PluginManager(PluginManager const &) = delete;
    PluginManager &operator=(PluginManager const &) = delete;
    virtual ~PluginManager() = default;

    virtual void registerLoader(std::unique_ptr<PluginLoader> loader) = 0;

    [[nodiscard]] virtual Plugin *getPlugin(const std::string &name) const = 0;

    [[nodiscard]] virtual std::vector<Plugin *> getPlugins() const = 0;

    [[nodiscard]] virtual bool isPluginEnabled(const std::string &name) const = 0;

    [[nodiscard]] virtual bool isPluginEnabled(Plugin *plugin) const = 0;

    [[nodiscard]] virtual std::vector<Plugin *> loadPlugins(const std::string &directory) = 0;

    virtual void enablePlugin(Plugin &plugin) const = 0;

    virtual void enablePlugins() const = 0;

    virtual void disablePlugin(Plugin &plugin) = 0;

    virtual void disablePlugins() = 0;

    virtual void clearPlugins() = 0;

    virtual void callEvent(Event &event) = 0;

    virtual void registerEvent(std::string event, std::function<void(Event &)> executor, EventPriority priority,
                               Plugin &plugin, bool ignore_cancelled) = 0;

    [[nodiscard]] virtual Permission *getPermission(std::string name) const = 0;

    virtual Permission *addPermission(std::unique_ptr<Permission> perm) = 0;

    virtual void removePermission(Permission &perm) = 0;

    virtual void removePermission(std::string name) = 0;

    [[nodiscard]] virtual std::unordered_set<Permission *> getDefaultPermissions(bool op) const = 0;

    virtual void recalculatePermissionDefaults(Permission &perm) = 0;

    virtual void subscribeToPermission(std::string permission, Permissible &permissible) = 0;

    virtual void unsubscribeFromPermission(std::string permission, Permissible &permissible) = 0;

    [[nodiscard]] virtual std::unordered_set<Permissible *> getPermissionSubscriptions(
        std::string permission) const = 0;

    virtual void subscribeToDefaultPerms(bool op, Permissible &permissible) = 0;

    virtual void unsubscribeFromDefaultPerms(bool op, Permissible &permissible) = 0;

    [[nodiscard]] virtual std::unordered_set<Permissible *> getDefaultPermSubscriptions(bool op) const = 0;

    [[nodiscard]] virtual std::unordered_set<Permission *> getPermissions() const = 0;
};

}  // namespace endstone
```


