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
#include <unordered_map>
#include <vector>

#include "endstone/command/simple_command_map.h"
#include "endstone/permission/permissible.h"
#include "endstone/permission/permission.h"
#include "endstone/plugin/plugin_loader.h"
#include "endstone/plugin/plugin_manager.h"
#include "endstone/server.h"

class SimplePluginManager : public PluginManager {
public:
    explicit SimplePluginManager(Server &server, SimpleCommandMap &command_map) noexcept;

    void registerLoader(std::unique_ptr<PluginLoader> loader) noexcept override;
    [[nodiscard]] Plugin *getPlugin(const std::string &name) const noexcept override;
    [[nodiscard]] std::vector<Plugin *> getPlugins() const noexcept override;
    [[nodiscard]] bool isPluginEnabled(const std::string &name) const noexcept override;
    bool isPluginEnabled(Plugin *plugin) const noexcept override;
    Plugin *loadPlugin(const std::filesystem::path &file) noexcept override;
    std::vector<Plugin *> loadPlugins(const std::filesystem::path &directory) noexcept override;
    void enablePlugin(Plugin &plugin) const noexcept override;
    void disablePlugin(Plugin &plugin) const noexcept override;
    void disablePlugins() const noexcept override;
    void clearPlugins() noexcept override;

    [[nodiscard]] Permission *getPermission(const std::string &name) noexcept override;
    bool addPermission(const std::string &name) noexcept override;
    bool addPermission(const std::shared_ptr<Permission> &permission) noexcept override;
    bool addPermission(const std::shared_ptr<Permission> &permission, bool dirty) noexcept;
    void removePermission(const std::string &name) noexcept override;
    [[nodiscard]] std::vector<Permission *> getDefaultPermissions(PermissibleRole role) const noexcept override;
    void recalculatePermissionDefaults(Permission &permission) noexcept override;
    void subscribeToPermission(const std::string &permission, Permissible &permissible) noexcept override;
    void unsubscribeFromPermission(const std::string &permission, Permissible &permissible) noexcept override;
    [[nodiscard]] std::vector<Permissible *> getPermissionSubscriptions(std::string permission) const noexcept override;
    void subscribeToDefaultPermissions(Permissible &permissible) noexcept override;
    void unsubscribeFromDefaultPermissions(Permissible &permissible) noexcept override;
    [[nodiscard]] std::vector<Permissible *> getDefaultPermissionSubscriptions(
        PermissibleRole role) const noexcept override;
    [[nodiscard]] std::vector<Permission *> getPermissions() const noexcept override;
    void dirtyPermissibles() const noexcept;

private:
    void calculatePermissionDefault(Permission &permission, bool dirty) noexcept;
    void dirtyPermissibles(PermissibleRole role) const noexcept;

    Server &server_;
    std::map<std::string, std::unique_ptr<PluginLoader>> file_associations_;
    std::vector<std::unique_ptr<Plugin>> plugins_;
    std::map<std::string, Plugin *> lookup_names_;
    SimpleCommandMap &command_map_;
    std::unordered_map<std::string, std::shared_ptr<Permission>> permissions_;
    std::unordered_map<PermissibleRole, std::vector<Permission *>> default_permissions_;
    std::unordered_map<std::string, std::unordered_map<Permissible *, bool>> permission_subscriptions_;
    std::unordered_map<PermissibleRole, std::unordered_map<Permissible *, bool>> default_subscriptions_;
};
