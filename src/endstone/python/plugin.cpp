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

#include "endstone_python.h"

namespace py = pybind11;

namespace endstone::python {

class PyPlugin : public Plugin, public py::trampoline_self_life_support {
public:
    using Plugin::Plugin;

    [[nodiscard]] const PluginDescription &getDescription() const override
    {
        PYBIND11_OVERRIDE_PURE_NAME(const PluginDescription &, Plugin, "_get_description", getDescription);
    }

    void onLoad() override
    {
        PYBIND11_OVERRIDE_NAME(void, Plugin, "on_load", onLoad);
    }

    void onEnable() override
    {
        PYBIND11_OVERRIDE_NAME(void, Plugin, "on_enable", onEnable);
    }

    void onDisable() override
    {
        PYBIND11_OVERRIDE_NAME(void, Plugin, "on_disable", onDisable);
    }

    bool onCommand(CommandSender &sender, const Command &command, const std::vector<std::string> &args) override
    {
        PYBIND11_OVERRIDE_NAME(bool, Plugin, "on_command", onCommand, std::ref(sender), std::ref(command),
                               std::ref(args));
    }
};

class PyPluginLoader : public PluginLoader, public py::trampoline_self_life_support {
public:
    using PluginLoader::PluginLoader;

    [[nodiscard]] Plugin *loadPlugin(std::string file) override
    {
        try {
            PYBIND11_OVERRIDE_PURE_NAME(Plugin *, PluginLoader, "load_plugin", loadPlugin, file);
        }
        catch (std::exception &e) {
            server_.getLogger().error("Error occurred when trying to load plugins in '{}': {}", file, e.what());
            return {};
        }
    }

    std::vector<Plugin *> loadPlugins(std::string directory) override
    {
        try {
            PYBIND11_OVERRIDE_PURE_NAME(std::vector<Plugin *>, PluginLoader, "load_plugins", loadPlugins, directory);
        }
        catch (std::exception &e) {
            server_.getLogger().error("Error occurred when trying to load plugins in '{}': {}", directory, e.what());
            return {};
        }
    }

    [[nodiscard]] std::vector<std::string> getPluginFileFilters() const override
    {
        return {"\\.whl"};
    }

    void enablePlugin(Plugin &plugin) const override
    {
        PYBIND11_OVERRIDE_NAME(void, PluginLoader, "enable_plugin", enablePlugin, std::ref(plugin));
    }

    void disablePlugin(Plugin &plugin) const override
    {
        PYBIND11_OVERRIDE_NAME(void, PluginLoader, "disable_plugin", disablePlugin, std::ref(plugin));
    }
};

namespace {
PluginDescription createPluginDescription(
    std::string name, std::string version, const std::optional<std::string> &description,
    std::optional<PluginLoadOrder> load, const std::optional<std::vector<std::string>> &authors,
    const std::optional<std::vector<std::string>> &contributors, const std::optional<std::string> &website,
    const std::optional<std::string> &prefix, const std::optional<std::vector<std::string>> &provides,
    const std::optional<std::vector<std::string>> &depend, const std::optional<std::vector<std::string>> &soft_depend,
    const std::optional<std::vector<std::string>> &load_before, std::optional<PermissionDefault> default_permission,
    const std::optional<std::vector<Command>> &commands, const std::optional<std::vector<Permission>> &permissions,
    const py::args & /*args*/, const py::kwargs & /*kwargs*/)
{
    return {std::move(name),
            std::move(version),
            description.value_or(""),
            load.value_or(PluginLoadOrder::PostWorld),
            authors.value_or(std::vector<std::string>{}),
            contributors.value_or(std::vector<std::string>{}),
            website.value_or(""),
            prefix.value_or(""),
            provides.value_or(std::vector<std::string>{}),
            depend.value_or(std::vector<std::string>{}),
            soft_depend.value_or(std::vector<std::string>{}),
            load_before.value_or(std::vector<std::string>{}),
            default_permission.value_or(PermissionDefault::Operator),
            commands.value_or(std::vector<Command>{}),
            permissions.value_or(std::vector<Permission>{})};
}
}  // namespace

void init_plugin(py::module &m)
{
    py::native_enum<PluginLoadOrder>(m, "PluginLoadOrder", "enum.Enum",
                                     "Represents the order in which a plugin should be initialized and enabled.")
        .value("STARTUP", PluginLoadOrder::Startup)
        .value("POSTWORLD", PluginLoadOrder::PostWorld)
        .finalize();

    auto plugin_loader = py::classh<PluginLoader, PyPluginLoader>(
        m, "PluginLoader", "Represents a plugin loader, which handles direct access to specific types of plugins.");
    auto plugin_command = py::class_<PluginCommand, Command, std::shared_ptr<PluginCommand>>(
        m, "PluginCommand", "Represents a Command belonging to a Plugin.");

    py::class_<PluginDescription>(
        m, "PluginDescription", "Represents the basic information about a plugin that the plugin loader needs to know.")
        .def(py::init(&createPluginDescription), py::arg("name"), py::arg("version"),
             py::arg("description") = py::none(), py::arg("load") = py::none(), py::arg("authors") = py::none(),
             py::arg("contributors") = py::none(), py::arg("website") = py::none(), py::arg("prefix") = py::none(),
             py::arg("provides") = py::none(), py::arg("depend") = py::none(), py::arg("soft_depend") = py::none(),
             py::arg("load_before") = py::none(), py::arg("default_permission") = py::none(),
             py::arg("commands") = py::none(), py::arg("permissions") = py::none())
        .def_property_readonly("name", &PluginDescription::getName,
                               "The name of the plugin. This name is a unique identifier for plugins.")
        .def_property_readonly("version", &PluginDescription::getVersion, "The version of the plugin.")
        .def_property_readonly("full_name", &PluginDescription::getFullName,
                               "A descriptive name of the plugin and respective version.")
        .def_property_readonly("api_version", &PluginDescription::getAPIVersion,
                               "The API version which this plugin is designed to support.")
        .def_property_readonly("description", &PluginDescription::getDescription,
                               "A human-friendly description of the functionality the plugin provides.")
        .def_property_readonly("load", &PluginDescription::getLoad,
                               "The phase of server startup that the plugin should be loaded.")
        .def_property_readonly("authors", &PluginDescription::getAuthors, "The list of authors for the plugin.")
        .def_property_readonly("contributors", &PluginDescription::getContributors,
                               "The list of contributors for the plugin.")
        .def_property_readonly("website", &PluginDescription::getWebsite,
                               "The plugin's or plugin's author's website.")
        .def_property_readonly("prefix", &PluginDescription::getPrefix,
                               "The token to prefix plugin-specific logging messages with.")
        .def_property_readonly("provides", &PluginDescription::getProvides,
                               "The list of other plugin APIs which this plugin provides. These are usable for other "
                               "plugins to depend on.")
        .def_property_readonly("depend", &PluginDescription::getDepend,
                               "The list of other plugins that the plugin requires.")
        .def_property_readonly("soft_depend", &PluginDescription::getSoftDepend,
                               "The list of other plugins that the plugin requires for full functionality.")
        .def_property_readonly("load_before", &PluginDescription::getLoadBefore,
                               "The list of plugins that should consider this plugin a soft-dependency.")
        .def_property_readonly("default_permission", &PluginDescription::getDefaultPermission,
                               "The default value of permissions registered for the plugin.")
        .def_property_readonly("commands", &PluginDescription::getCommands,
                               "The list of commands the plugin will register at runtime.")
        .def_property_readonly(
            "permissions", &PluginDescription::getPermissions,
            "The list of permissions the plugin will register at runtime, immediately proceeding enabling.");

    py::class_<Plugin, CommandExecutor, PyPlugin, py::smart_holder>(m, "Plugin", "Represents a Plugin.")
        .def(py::init<>())
        .def("on_load", &Plugin::onLoad, R"doc(
    Called after a plugin is loaded but before it has been enabled.

    When multiple plugins are loaded, the `on_load()` for all plugins is called before any `on_enable()` is called.
)doc")
        .def("on_enable", &Plugin::onEnable, "Called when this plugin is enabled.")
        .def("on_disable", &Plugin::onDisable, "Called when this plugin is disabled.")
        .def("_get_description", &Plugin::getDescription, py::return_value_policy::reference)
        .def_property_readonly("logger", &Plugin::getLogger, py::return_value_policy::reference, R"doc(
    The plugin logger associated with this server's logger.

    The returned logger automatically tags all log messages with the plugin's name.
)doc")
        .def_property_readonly("plugin_loader", &Plugin::getPluginLoader, py::return_value_policy::reference,
                               "The associated `PluginLoader` responsible for this plugin.")
        .def_property_readonly("server", &Plugin::getServer, py::return_value_policy::reference,
                               "The `Server` instance currently running this plugin.")
        .def_property_readonly("is_enabled", &Plugin::isEnabled, "Whether this plugin is currently enabled.")
        .def_property_readonly("name", &Plugin::getName, R"doc(
    The name of the plugin.

    This is the bare name of the plugin and should be used for comparison.
)doc")
        .def("get_command", &Plugin::getCommand, py::return_value_policy::reference, py::arg("name"), R"doc(
    Gets the command with the given name, specific to this plugin.

    Args:
        name: Name or alias of the command.

    Returns:
        The plugin command if found, otherwise `None`.
)doc")
        .def_property_readonly("data_folder", &Plugin::getDataFolder,
                               "The folder that the plugin data's files are located in. The folder may not yet exist.");

    plugin_loader  //
        .def(py::init<Server &>(), py::arg("server"))
        .def("load_plugin", &PluginLoader::loadPlugin, py::arg("file"), py::return_value_policy::reference, R"doc(
    Loads the plugin contained in the specified file.

    Args:
        file: File to attempt to load.

    Returns:
        `Plugin` that was contained in the specified file, or `None` if unsuccessful.
)doc")
        .def("load_plugins", &PluginLoader::loadPlugins, py::arg("directory"),
             py::return_value_policy::reference_internal, R"doc(
    Loads the plugin contained within the specified directory.

    Args:
        directory: Directory to check for plugins.

    Returns:
        A list of all plugins loaded.
)doc")
        .def("enable_plugin", &PluginLoader::enablePlugin, py::arg("plugin"), R"doc(
    Enables the specified plugin.

    Attempting to enable a plugin that is already enabled will have no effect.

    Args:
        plugin: Plugin to enable.
)doc")
        .def("disable_plugin", &PluginLoader::enablePlugin, py::arg("plugin"), R"doc(
    Disables the specified plugin.

    Attempting to disable a plugin that is not enabled will have no effect.

    Args:
        plugin: Plugin to disable.
)doc")
        .def_property_readonly("plugin_file_filters", &PluginLoader::getPluginFileFilters,
                               "A list of all filename filters expected by this `PluginLoader`.")
        .def_property_readonly("server", &PluginLoader::getServer, py::return_value_policy::reference,
                               "The `Server` object associated with the `PluginLoader`.");

    py::classh<PluginManager>(m, "PluginManager",
                              "Represents a plugin manager that handles all plugins from the `Server`.")
        .def("get_plugin", &PluginManager::getPlugin, py::arg("name"), py::return_value_policy::reference, R"doc(
    Checks if the given plugin is loaded and returns it when applicable.

    Please note that the name of the plugin is case-sensitive.

    Args:
        name: Name of the plugin to check.

    Returns:
        `Plugin` if it exists, otherwise `None`.
)doc")
        .def_property_readonly("plugins", &PluginManager::getPlugins, "List of all currently loaded plugins.")
        .def("is_plugin_enabled", py::overload_cast<const std::string &>(&PluginManager::isPluginEnabled, py::const_),
             py::arg("plugin"), R"doc(
    Checks if the given plugin is enabled or not.

    Please note that the name of the plugin is case-sensitive.

    Args:
        plugin: Name of the plugin to check.

    Returns:
        `True` if the plugin is enabled, otherwise `False`.
)doc")
        .def("is_plugin_enabled", py::overload_cast<Plugin *>(&PluginManager::isPluginEnabled, py::const_),
             py::arg("plugin"), R"doc(
    Checks if the given plugin is enabled or not.

    Args:
        plugin: Plugin to check.

    Returns:
        `True` if the plugin is enabled, otherwise `False`.
)doc")
        .def("load_plugin", &PluginManager::loadPlugin, py::arg("file"), py::return_value_policy::reference, R"doc(
    Loads the plugin in the specified file.

    File must be valid according to the current enabled `Plugin` interfaces.

    Args:
        file: File containing the plugin to load.

    Returns:
        The `Plugin` loaded, or `None` if it was invalid.
)doc")
        .def("load_plugins", py::overload_cast<std::string>(&PluginManager::loadPlugins), py::arg("directory"),
             py::return_value_policy::reference_internal, R"doc(
    Loads the plugin contained within the specified directory.

    Args:
        directory: Directory to check for plugins.

    Returns:
        A list of all plugins loaded.
)doc")
        .def("load_plugins", py::overload_cast<std::vector<std::string>>(&PluginManager::loadPlugins), py::arg("files"),
             py::return_value_policy::reference_internal, R"doc(
    Loads the plugins in the list of the files.

    Args:
        files: List of files containing plugins to load.

    Returns:
        A list of all plugins loaded.
)doc")
        .def("enable_plugin", &PluginManager::enablePlugin, py::arg("plugin"), R"doc(
    Enables the specified plugin.

    Attempting to enable a plugin that is already enabled will have no effect.

    Args:
        plugin: Plugin to enable.
)doc")
        .def("enable_plugins", &PluginManager::enablePlugins, "Enable all the loaded plugins.")
        .def("disable_plugin", &PluginManager::disablePlugin, py::arg("plugin"), R"doc(
    Disables the specified plugin.

    Attempting to disable a plugin that is not enabled will have no effect.

    Args:
        plugin: Plugin to disable.
)doc")
        .def("disable_plugins", &PluginManager::disablePlugins, "Disables all the loaded plugins.")
        .def("clear_plugins", &PluginManager::clearPlugins, "Disables and removes all plugins.")
        .def("call_event", &PluginManager::callEvent, py::arg("event"), R"doc(
    Calls an event which will be passed to plugins.

    Args:
        event: Event to be called.
)doc")
        .def(
            "register_event",
            [](PluginManager &self, std::string event, const std::function<void(Event *)> &executor,
               EventPriority priority, Plugin &plugin, bool ignore_cancelled) {
                self.registerEvent(
                    std::move(event), [executor](Event &e) { executor(&e); }, priority, plugin, ignore_cancelled);
            },
            py::arg("name"), py::arg("executor"), py::arg("priority"), py::arg("plugin"), py::arg("ignore_cancelled"),
            R"doc(
    Registers the given event.

    Args:
        name: Event name to register.
        executor: `EventExecutor` to register.
        priority: Priority of this event.
        plugin: Plugin to register.
        ignore_cancelled: Do not call executor if event was already cancelled.
)doc")
        .def("get_permission", &PluginManager::getPermission, py::arg("name"), py::return_value_policy::reference,
             R"doc(
    Gets a `Permission` from its fully qualified name.

    Args:
        name: Name of the permission.

    Returns:
        `Permission`, or `None` if none.
)doc")
        .def("remove_permission", py::overload_cast<Permission &>(&PluginManager::removePermission), py::arg("perm"),
             R"doc(
    Removes a `Permission` registration from this plugin manager.

    If the specified permission does not exist in this plugin manager, nothing will happen.
    Removing a permission registration will not remove the permission from any `Permissibles` that have it.

    Args:
        perm: Permission to remove.
)doc")
        .def("remove_permission", py::overload_cast<std::string>(&PluginManager::removePermission), py::arg("name"),
             R"doc(
    Removes a `Permission` registration from this plugin manager.

    If the specified permission does not exist in this plugin manager, nothing will happen.
    Removing a permission registration will not remove the permission from any `Permissibles` that have it.

    Args:
        name: Permission to remove.
)doc")
        .def("get_default_permissions", &PluginManager::getDefaultPermissions, py::arg("level"),
             py::return_value_policy::reference_internal, R"doc(
    Gets the default permissions for the given permission level.

    Args:
        level: Which set of default permissions to get.

    Returns:
        The default permissions.
)doc")
        .def("recalculate_permission_defaults", &PluginManager::recalculatePermissionDefaults, py::arg("perm"), R"doc(
    Recalculates the defaults for the given `Permission`.

    This will have no effect if the specified permission is not registered here.

    Args:
        perm: Permission to recalculate.
)doc")
        .def("subscribe_to_permission", &PluginManager::subscribeToPermission, py::arg("permission"),
             py::arg("permissible"), R"doc(
    Subscribes the given `Permissible` for information about the requested `Permission`, by name.

    If the specified `Permission` changes in any form, the `Permissible` will be asked to recalculate.

    Args:
        permission: Permission to subscribe to.
        permissible: Permissible subscribing.
)doc")
        .def("unsubscribe_from_permission", &PluginManager::unsubscribeFromPermission, py::arg("permission"),
             py::arg("permissible"), R"doc(
    Unsubscribes the given `Permissible` for information about the requested `Permission`, by name.

    Args:
        permission: Permission to unsubscribe from.
        permissible: Permissible subscribing.
)doc")
        .def("get_permission_subscriptions", &PluginManager::getPermissionSubscriptions, py::arg("permission"),
             py::return_value_policy::reference_internal, R"doc(
    Gets a set containing all subscribed `Permissibles` to the given permission, by name.

    Args:
        permission: Permission to query for.

    Returns:
        Set containing all subscribed permissions.
)doc")
        .def("subscribe_to_default_perms", &PluginManager::subscribeToDefaultPerms, py::arg("level"),
             py::arg("permissible"), R"doc(
    Subscribes to the given Default permissions by permission level.

    If the specified defaults change in any form, the `Permissible` will be asked to recalculate.

    Args:
        level: Default list to subscribe to.
        permissible: Permissible subscribing.
)doc")
        .def("unsubscribe_from_default_perms", &PluginManager::unsubscribeFromDefaultPerms, py::arg("level"),
             py::arg("permissible"), R"doc(
    Unsubscribes from the given Default permissions by permission level.

    Args:
        level: Default list to unsubscribe from.
        permissible: Permissible subscribing.
)doc")
        .def("get_default_perm_subscriptions", &PluginManager::getDefaultPermSubscriptions, py::arg("level"),
             py::return_value_policy::reference_internal, R"doc(
    Gets a set containing all subscribed `Permissibles` to the given default list, by permission level.

    Args:
        level: Default list to query for.

    Returns:
        Set containing all subscribed permissions.
)doc")
        .def_property_readonly("permissions", &PluginManager::getPermissions,
                               "Set of all registered permissions.");

    plugin_command
        .def_property("executor", &PluginCommand::getExecutor,
                      py::cpp_function(&PluginCommand::setExecutor, py::keep_alive<1, 2>()),
                      py::return_value_policy::reference, "The `CommandExecutor` to run when parsing this command.")
        .def_property_readonly("plugin", &PluginCommand::getPlugin, "The owner of this `PluginCommand`.");

    py::native_enum<ServicePriority>(m, "ServicePriority", "enum.IntEnum",
                                     "Represents various priorities of a provider.")
        .value("LOWEST", ServicePriority::Lowest)
        .value("LOW", ServicePriority::Low)
        .value("NORMAL", ServicePriority::Normal)
        .value("HIGH", ServicePriority::High)
        .value("HIGHEST", ServicePriority::Highest)
        .finalize();

    py::class_<Service, std::shared_ptr<Service>>(m, "Service", "Services represent a list of methods.")
        .def(py::init<>());

    py::classh<ServiceManager>(m, "ServiceManager", R"doc(
    Represent a service manager that manages services and service providers.

    Services are an interface specifying a list of methods that a provider must implement.
    Providers are implementations of these services.
)doc")
        .def("register", &ServiceManager::registerService, py::arg("name"), py::arg("provider"), py::arg("plugin"),
             py::arg("priority"), py::keep_alive<1, 3>(), R"doc(
    Register a provider of a service.

    Args:
        name: Service name.
        provider: Service provider to register.
        plugin: Plugin associated with the service.
        priority: Priority of the provider.
)doc")
        .def("unregister_all", &ServiceManager::unregisterAll, py::arg("plugin"), R"doc(
    Unregister all the services registered by a particular plugin.

    Args:
        plugin: The plugin.
)doc")
        .def("unregister", py::overload_cast<std::string, const Service &>(&ServiceManager::unregister),
             py::arg("name"), py::arg("provider"), R"doc(
    Unregister a particular provider for a particular service.

    Args:
        name: The service name.
        provider: The service provider implementation.
)doc")
        .def("unregister", py::overload_cast<const Service &>(&ServiceManager::unregister), py::arg("provider"), R"doc(
    Unregister a particular provider.

    Args:
        provider: The service provider implementation.
)doc")
        .def("load", &ServiceManager::get, py::arg("name"), R"doc(
    Queries for a provider.

    This may return `None` if no provider has been registered for the service. The highest priority provider is returned.

    Args:
        name: The service name.

    Returns:
        The highest priority provider, or `None` if none is registered.
)doc");
}

}  // namespace endstone::python
