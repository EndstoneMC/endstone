//
// Created by Vincent on 20/08/2023.
//

#ifndef ENDSTONE_PLUGIN_MANAGER_H
#define ENDSTONE_PLUGIN_MANAGER_H

#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_loader.h"

class Permission;
class Permissible;

class PluginManager {
public:
    virtual ~PluginManager() = default;

    /**
     * Registers the specified plugin loader
     *
     * @param loader the PluginLoader to register
     */
    virtual void registerLoader(std::unique_ptr<PluginLoader> loader) = 0;

    /**
     * Checks if the given plugin is loaded and returns it when applicable
     *
     * Please note that the name of the plugin is case-sensitive
     *
     * @param name Name of the plugin to check
     * @return Plugin if it exists, otherwise nullptr
     */
    virtual Plugin *getPlugin(const std::string &name) const = 0;

    /**
     * Gets a list of all currently loaded plugins
     *
     * @return Array of Plugins
     */
    virtual std::vector<Plugin *> getPlugins() const = 0;

    /**
     * Checks if the given plugin is enabled or not
     *
     * Please note that the name of the plugin is case-sensitive.
     *
     * @param name Name of the plugin to check
     * @return true if the plugin is enabled, otherwise false
     */
    virtual bool isPluginEnabled(const std::string &name) const = 0;

    /**
     * Checks if the given plugin is enabled or not
     *
     * @param plugin Plugin to check
     * @return true if the plugin is enabled, otherwise false
     */
    virtual bool isPluginEnabled(Plugin *plugin) const = 0;

    /**
     * Loads the plugin in the specified file
     *
     * File must be valid according to the current enabled Plugin interfaces
     *
     * @param file File containing the plugin to load
     * @return The Plugin loaded, or nullptr if it was invalid
     */
    virtual Plugin *loadPlugin(const std::filesystem::path &file) = 0;

    /**
     * Loads the plugins contained within the specified directory
     *
     * @param directory Directory to check for plugins
     * @return A list of all plugins loaded
     */
    virtual std::vector<Plugin *> loadPlugins(const std::filesystem::path &directory) = 0;

    /**
     * Enables the specified plugin
     *
     * Attempting to enable a plugin that is already enabled will have no effect
     *
     * @param plugin Plugin to enable
     */
    virtual void enablePlugin(Plugin &plugin) const = 0;

    /**
     * Disables the specified plugin
     *
     * Attempting to disable a plugin that is not enabled will have no effect
     *
     * @param plugin Plugin to disable
     */
    virtual void disablePlugin(Plugin &plugin) const = 0;

    /**
     * Disables all the loaded plugins
     */
    virtual void disablePlugins() = 0;

    /**
     * Disables and removes all plugins
     */
    virtual void clearPlugins() = 0;

    /**
     * Gets a Permission from its fully qualified name
     *
     * @param name Name of the permission
     * @return Permission, or null if none
     */
    virtual const Permission &getPermission(const std::string &name) const = 0;

    /**
     * Adds a Permission to this plugin manager.
     *
     * @param permission Permission to add
     * @throws std::exception Thrown when a permission with the same
     *     name already exists
     */
    virtual void addPermission(const Permission &permission) = 0;

    /**
     * Removes a Permission registration from this plugin manager.
     *
     * If the specified permission does not exist in this plugin manager,
     * nothing will happen.
     *
     * Removing a permission registration will <b>not</b> remove the
     * permission from any Permissibles that have it.
     *
     * @param permission Permission to remove
     */
    virtual void removePermission(const Permission &permission) = 0;

    /**
     * Removes a Permission registration from this plugin manager.
     *
     * If the specified permission does not exist in this plugin manager,
     * nothing will happen.
     *
     * Removing a permission registration will <b>not</b> remove the
     * permission from any Permissibles that have it.
     *
     * @param name Permission to remove
     */
    virtual void removePermission(const std::string &name) = 0;

    /**
     * Gets the default permissions for the given op status
     *
     * @param op Which set of default permissions to get
     * @return The default permissions
     */
    virtual std::vector<Permission> getDefaultPermissions(bool op) const = 0;

    /**
     * Recalculates the defaults for the given Permission.
     *
     * This will have no effect if the specified permission is not registered here.
     *
     * @param permission Permission to recalculate
     */
    virtual void recalculatePermissionDefaults(const Permission &permission) = 0;

    /**
     * Subscribes the given Permissible for information about the requested
     * Permission, by name.
     *
     * If the specified Permission changes in any form, the Permissible will
     * be asked to recalculate.
     *
     * @param permission Permission to subscribe to
     * @param permissible Permissible subscribing
     */
    virtual void subscribeToPermission(const std::string &permission, const Permissible &permissible) = 0;

    /**
     * Unsubscribes the given Permissible for information about the requested
     * Permission, by name.
     *
     * @param permission Permission to unsubscribe from
     * @param permissible Permissible subscribing
     */
    virtual void unsubscribeFromPermission(const std::string &permission, const Permissible &permissible) = 0;

    /**
     * Gets a set containing all subscribed Permissibles to the given permission, by name
     *
     * @param permission Permission to query for
     * @return Set containing all subscribed permissions
     */
    virtual std::vector<Permissible> getPermissionSubscriptions(const std::string &permission) const = 0;

    /**
     * Subscribes to the given Default permissions by operator status
     *
     * If the specified defaults change in any form, the Permissible will be
     * asked to recalculate.
     *
     * @param op Default list to subscribe to
     * @param permissible Permissible subscribing
     */
    virtual void subscribeToDefaultPermissions(bool op, const Permissible &permissible) = 0;

    /**
     * Unsubscribes from the given Default permissions by operator status
     *
     * @param op Default list to unsubscribe from
     * @param permissible Permissible subscribing
     */
    virtual void unsubscribeFromDefaultPermissions(bool op, const Permissible &permissible) = 0;

    /**
     * Gets a set containing all subscribed Permissibles to the given
     * default list, by op status
     *
     * @param op Default list to query for
     * @return Set containing all subscribed permissions
     */
    virtual std::vector<Permissible> getDefaultPermissionSubscriptions(bool op) const = 0;

    /**
     * Gets a set of all registered permissions.
     *
     * @return Set containing all current registered permissions
     */
    virtual std::vector<Permissible> getPermissions() const = 0;
};

#endif // ENDSTONE_PLUGIN_MANAGER_H
