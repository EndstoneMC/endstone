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

    /**
     * Registers the specified plugin loader
     *
     * @param loader PluginLoader to register
     */
    virtual void registerLoader(std::unique_ptr<PluginLoader> loader) = 0;

    /**
     * Checks if the given plugin is loaded and returns it when applicable.
     * Please note that the name of the plugin is case-sensitive
     *
     * @param name Name of the plugin to check
     * @return Plugin if it exists, otherwise nullptr
     */
    [[nodiscard]] virtual Plugin *getPlugin(const std::string &name) const = 0;

    /**
     * Gets a list of all currently loaded plugins
     *
     * @return List of Plugins
     */
    [[nodiscard]] virtual std::vector<Plugin *> getPlugins() const = 0;

    /**
     * Checks if the given plugin is enabled or not
     * Please note that the name of the plugin is case-sensitive.
     *
     * @param name Name of the plugin to check
     * @return true if the plugin is enabled, otherwise false
     */
    [[nodiscard]] virtual bool isPluginEnabled(const std::string &name) const = 0;

    /**
     * Checks if the given plugin is enabled or not
     *
     * @param plugin Plugin to check
     * @return true if the plugin is enabled, otherwise false
     */
    [[nodiscard]] virtual bool isPluginEnabled(Plugin *plugin) const = 0;

    /**
     * Loads the plugin contained within the specified directory
     *
     * @param directory Directory to check for plugins
     * @return A list of all plugins loaded
     */
    [[nodiscard]] virtual std::vector<Plugin *> loadPlugins(const std::string &directory) = 0;

    /**
     * Enables the specified plugin
     * Attempting to enable a plugin that is already enabled will have no effect
     *
     * @param plugin Plugin to enable
     */
    virtual void enablePlugin(Plugin &plugin) const = 0;

    /**
     * Enable all the loaded plugins
     */
    virtual void enablePlugins() const = 0;

    /**
     * Disables the specified plugin
     * Attempting to disable a plugin that is not enabled will have no effect
     *
     * @param plugin Plugin to disable
     */
    virtual void disablePlugin(Plugin &plugin) const = 0;

    /**
     * Disables all the loaded plugins
     */
    virtual void disablePlugins() const = 0;

    /**
     * Disables and removes all plugins
     */
    virtual void clearPlugins() = 0;

    /**
     * Calls an event which will be passed to plugins.
     *
     * @param event Event to be called
     */
    virtual void callEvent(Event &event) = 0;

    /**
     * Registers the given event
     *
     * @param event Event name to register
     * @param executor EventExecutor to register
     * @param priority Priority of this event
     * @param plugin Plugin to register
     * @param ignore_cancelled Do not call executor if event was already
     *     cancelled
     */
    virtual void registerEvent(std::string event, std::function<void(Event &)> executor, EventPriority priority,
                               Plugin &plugin, bool ignore_cancelled) = 0;

    /**
     * Gets a Permission from its fully qualified name
     *
     * @param name Name of the permission
     * @return Permission, or null if none
     */
    [[nodiscard]] virtual Permission *getPermission(std::string name) const = 0;

    /**
     * Adds a Permission to this plugin manager.
     *
     * @param perm Permission to add
     * @return Permission, or nullptr if a permission is already defined with the given name of the new permission
     */
    virtual Permission *addPermission(std::unique_ptr<Permission> perm) = 0;

    /**
     * Removes a Permission registration from this plugin manager.
     *
     * If the specified permission does not exist in this plugin manager, nothing will happen.
     * Removing a permission registration will <b>not</b> remove the permission from any Permissibles that have it.
     *
     * @param perm Permission to remove
     */
    virtual void removePermission(Permission &perm) = 0;

    /**
     * Removes a Permission registration from this plugin manager.
     *
     * If the specified permission does not exist in this plugin manager, nothing will happen.
     * Removing a permission registration will <b>not</b> remove the permission from any Permissibles that have it.
     *
     * @param name Permission to remove
     */
    virtual void removePermission(std::string name) = 0;

    /**
     * Gets the default permissions for the given op status
     *
     * @param op Which set of default permissions to get
     * @return The default permissions
     */
    [[nodiscard]] virtual std::unordered_set<Permission *> getDefaultPermissions(bool op) const = 0;

    /**
     * Recalculates the defaults for the given {@link Permission}.
     *
     * This will have no effect if the specified permission is not registered here.
     *
     * @param perm Permission to recalculate
     */
    virtual void recalculatePermissionDefaults(Permission &perm) = 0;

    /**
     * Subscribes the given Permissible for information about the requested Permission, by name.
     * If the specified Permission changes in any form, the Permissible will be asked to recalculate.
     *
     * @param permission Permission to subscribe to
     * @param permissible Permissible subscribing
     */
    virtual void subscribeToPermission(std::string permission, Permissible &permissible) = 0;

    /**
     * Unsubscribes the given Permissible for information about the requested Permission, by name.
     *
     * @param permission Permission to unsubscribe from
     * @param permissible Permissible subscribing
     */
    virtual void unsubscribeFromPermission(std::string permission, Permissible &permissible) = 0;

    /**
     * Gets a set containing all subscribed Permissibles to the given permission, by name
     *
     * @param permission Permission to query for
     * @return Set containing all subscribed permissions
     */
    [[nodiscard]] virtual std::unordered_set<Permissible *> getPermissionSubscriptions(
        std::string permission) const = 0;

    /**
     * Subscribes to the given Default permissions by operator status
     *
     * If the specified defaults change in any form, the Permissible will be asked to recalculate.
     *
     * @param op Default list to subscribe to
     * @param permissible Permissible subscribing
     */
    virtual void subscribeToDefaultPerms(bool op, Permissible &permissible) = 0;

    /**
     * Unsubscribes from the given Default permissions by operator status
     *
     * @param op Default list to unsubscribe from
     * @param permissible Permissible subscribing
     */
    virtual void unsubscribeFromDefaultPerms(bool op, Permissible &permissible) = 0;

    /**
     * Gets a set containing all subscribed {@link Permissible}s to the given default list, by op status
     *
     * @param op Default list to query for
     * @return Set containing all subscribed permissions
     */
    [[nodiscard]] virtual std::unordered_set<Permissible *> getDefaultPermSubscriptions(bool op) const = 0;

    /**
     * Gets a set of all registered permissions.
     *
     * This set is a copy and will not be modified live.
     *
     * @return Set containing all current registered permissions
     */
    [[nodiscard]] virtual std::unordered_set<Permission *> getPermissions() const = 0;
};

}  // namespace endstone
