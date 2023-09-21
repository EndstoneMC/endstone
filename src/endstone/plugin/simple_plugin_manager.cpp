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

#include "endstone/plugin/simple_plugin_manager.h"

#include <algorithm>
#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "endstone/command/plugin_command.h"
#include "endstone/command/simple_command_map.h"
#include "endstone/permission/permissible.h"
#include "endstone/permission/permission.h"
#include "endstone/plugin/plugin_loader.h"
#include "endstone/server.h"

SimplePluginManager::SimplePluginManager(Server &server, SimpleCommandMap &command_map) noexcept
    : server_(server), command_map_(command_map)
{
}

void SimplePluginManager::registerLoader(std::unique_ptr<PluginLoader> loader) noexcept
{
    auto patterns = loader->getPluginFileFilters();
    for (const auto &pattern : patterns) {
        file_associations_[pattern] = std::move(loader);
    }
}

Plugin *SimplePluginManager::getPlugin(const std::string &name) const noexcept
{
    auto it = lookup_names_.find(name);
    if (it != lookup_names_.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<Plugin *> SimplePluginManager::getPlugins() const noexcept
{
    std::vector<Plugin *> plugins;
    plugins.reserve(plugins_.size());
    for (const auto &plugin : plugins_) {
        plugins.push_back(plugin.get());
    }
    return plugins;
}

bool SimplePluginManager::isPluginEnabled(const std::string &name) const noexcept
{
    return isPluginEnabled(getPlugin(name));
}

bool SimplePluginManager::isPluginEnabled(Plugin *plugin) const noexcept
{
    if (!plugin) {
        return false;
    }

    // Check if the plugin exists in the vector
    auto it = std::find_if(plugins_.begin(), plugins_.end(), [plugin](const std::unique_ptr<Plugin> &p) {
        return p.get() == plugin;
    });

    // If plugin is in the vector and is enabled, return true
    return it != plugins_.end() && plugin->isEnabled();
}

Plugin *SimplePluginManager::loadPlugin(const std::filesystem::path &file) noexcept
{
    if (!exists(file)) {
        server_.getLogger().error("Could not load plugin from '{}': Provided file does not exist.", file.string());
    }

    for (const auto &[pattern, loader] : file_associations_) {
        std::regex r(pattern);
        if (std::regex_search(file.string(), r)) {
            auto plugin = loader->loadPlugin(file.string());

            if (plugin) {
                auto *const plugin_ptr = plugin.get();
                auto name = plugin->getDescription().getName();

                if (!std::regex_match(name, PluginDescription::ValidName)) {
                    server_.getLogger().error(
                        "Could not load plugin from '{}': Plugin name contains invalid characters.", file.string());
                    return nullptr;
                }

                lookup_names_[name] = plugin_ptr;
                plugins_.push_back(std::move(plugin));
                return lookup_names_[name];
            }
        }
    }

    return nullptr;
}

std::vector<Plugin *> SimplePluginManager::loadPlugins(const std::filesystem::path &directory) noexcept
{
    if (!std::filesystem::exists(directory)) {
        server_.getLogger().error(
            "Error occurred when trying to load plugins in '{}': Provided directory does not exist.",
            directory.string());
        return {};
    }

    if (!std::filesystem::is_directory(directory)) {
        server_.getLogger().error(
            "Error occurred when trying to load plugins in '{}': Provided path is not a directory.",
            directory.string());
        return {};
    }

    std::vector<Plugin *> loaded_plugins;

    for (const auto &entry : std::filesystem::directory_iterator(directory)) {
        std::filesystem::path file;

        // If it's a regular file, try to load it as a plugin.
        if (std::filesystem::is_regular_file(entry.status())) {
            file = entry.path();
        }
        // If it's a subdirectory, look for a plugin.toml inside it.
        else if (std::filesystem::is_directory(entry.status())) {
            file = entry.path() / "plugin.toml";
            if (!std::filesystem::exists(file) || !std::filesystem::is_regular_file(file)) {
                continue;
            }
        }
        else {
            continue;
        }

        auto *plugin = loadPlugin(file);
        if (plugin) {
            loaded_plugins.push_back(plugin);
        }
    }

    return loaded_plugins;
}

void SimplePluginManager::enablePlugin(Plugin &plugin) const noexcept
{
    if (!plugin.isEnabled()) {
        auto commands = plugin.getDescription().getCommands();

        if (!commands.empty()) {
            auto plugin_commands = std::vector<std::shared_ptr<Command>>(commands.size());
            std::transform(commands.begin(), commands.end(), plugin_commands.begin(), [&plugin](const auto &command) {
                return std::make_shared<PluginCommand>(*command, plugin);
            });

            command_map_.registerAll(plugin.getDescription().getName(), plugin_commands);
        }

        plugin.getPluginLoader().enablePlugin(plugin);
    }
}

void SimplePluginManager::disablePlugin(Plugin &plugin) const noexcept
{
    if (plugin.isEnabled()) {
        plugin.getPluginLoader().disablePlugin(plugin);
    }
}

void SimplePluginManager::disablePlugins() const noexcept
{
    for (const auto &plugin : plugins_) {
        disablePlugin(*plugin);
    }
}

void SimplePluginManager::clearPlugins() noexcept
{
    disablePlugins();
    plugins_.clear();
    lookup_names_.clear();
}

Permission *SimplePluginManager::getPermission(const std::string &name) noexcept
{
    auto lower_name = name;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    auto it = permissions_.find(lower_name);
    if (it != permissions_.end()) {
        return it->second.get();
    }

    return nullptr;
}

bool SimplePluginManager::addPermission(const std::string &name) noexcept
{
    auto lower_name = name;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    return addPermission(std::make_shared<Permission>(lower_name));
}

bool SimplePluginManager::addPermission(const std::shared_ptr<Permission> &permission) noexcept
{
    return addPermission(permission, true);
}

bool SimplePluginManager::addPermission(const std::shared_ptr<Permission> &permission, bool dirty) noexcept
{
    if (!permission) {
        return false;
    }

    auto [it, success] = permissions_.insert({permission->getName(), permission});

    if (success) {
        permission->plugin_manager_ = this;
        calculatePermissionDefault(*it->second, dirty);
    }

    return success;
}

void SimplePluginManager::removePermission(const std::string &name) noexcept
{
    auto lower_name = name;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    permissions_.erase(lower_name);
}

std::vector<Permission *> SimplePluginManager::getDefaultPermissions(PermissibleRole role) const noexcept
{
    return default_permissions_.find(role)->second;
}

void SimplePluginManager::recalculatePermissionDefaults(Permission &permission) noexcept
{
    auto lower_name = permission.getName();
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    for (auto &[role, permissions] : default_permissions_) {
        permissions.erase(std::remove(permissions.begin(), permissions.end(), &permission), permissions.end());
    }

    calculatePermissionDefault(permission, true);
}

void SimplePluginManager::calculatePermissionDefault(Permission &permission, bool dirty) noexcept
{
    for (auto &[role, permissions] : default_permissions_) {
        if (!permission.getDefault().isGrantedFor(role)) {
            continue;
        }

        permissions.push_back(&permission);
        if (dirty) {
            dirtyPermissibles(role);
        }
    }
}

void SimplePluginManager::dirtyPermissibles() const noexcept
{
    for (const auto &[role, _] : default_subscriptions_) {
        dirtyPermissibles(role);
    }
}

void SimplePluginManager::dirtyPermissibles(PermissibleRole role) const noexcept
{
    auto permissibles = getDefaultPermissionSubscriptions(role);
    for (const auto &p : permissibles) {
        p->recalculatePermissions();
    }
}

void SimplePluginManager::subscribeToPermission(const std::string &permission, Permissible &permissible) noexcept
{
    auto lower_name = permission;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    auto &map = permission_subscriptions_[lower_name];
    map.insert_or_assign(&permissible, true);
}

void SimplePluginManager::unsubscribeFromPermission(const std::string &permission, Permissible &permissible) noexcept
{
    auto lower_name = permission;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    auto map = permission_subscriptions_[lower_name];
    map.erase(&permissible);
}

std::vector<Permissible *> SimplePluginManager::getPermissionSubscriptions(std::string permission) const noexcept
{
    auto lower_name = permission;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    auto it = permission_subscriptions_.find(lower_name);
    if (it == permission_subscriptions_.end()) {
        return {};
    }

    std::vector<Permissible *> keys;
    const auto &map = it->second;
    keys.reserve(map.size());

    for (const auto &pair : map) {
        keys.push_back(pair.first);
    }
    return keys;
}

void SimplePluginManager::subscribeToDefaultPermissions(Permissible &permissible) noexcept
{
    auto &map = default_subscriptions_[permissible.getRole()];
    map.insert_or_assign(&permissible, true);
}

void SimplePluginManager::unsubscribeFromDefaultPermissions(Permissible &permissible) noexcept
{
    auto &map = default_subscriptions_[permissible.getRole()];
    map.erase(&permissible);
}

std::vector<Permissible *> SimplePluginManager::getDefaultPermissionSubscriptions(PermissibleRole role) const noexcept
{
    auto it = default_subscriptions_.find(role);
    if (it == default_subscriptions_.end()) {
        return {};
    }

    std::vector<Permissible *> keys;
    const auto &map = it->second;
    keys.reserve(map.size());

    for (const auto &pair : map) {
        keys.push_back(pair.first);
    }
    return keys;
}

std::vector<Permission *> SimplePluginManager::getPermissions() const noexcept
{
    std::vector<Permission *> permissions;
    permissions.reserve(permissions_.size());
    for (const auto &permission : permissions_) {
        permissions.push_back(permission.second.get());
    }

    return permissions;
}
