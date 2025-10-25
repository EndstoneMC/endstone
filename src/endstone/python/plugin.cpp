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

class PyPluginLoader : public PluginLoader {
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

    auto plugin_loader = py::class_<PluginLoader, PyPluginLoader>(
        m, "PluginLoader", "Represents a plugin loader, which handles direct access to specific types of plugins");
    auto plugin_command = py::class_<PluginCommand, Command, std::shared_ptr<PluginCommand>>(
        m, "PluginCommand", "Represents a Command belonging to a Plugin");

    py::class_<PluginDescription>(
        m, "PluginDescription", "Represents the basic information about a plugin that the plugin loader needs to know.")
        .def(py::init(&createPluginDescription), py::arg("name"), py::arg("version"),
             py::arg("description") = py::none(), py::arg("load") = py::none(), py::arg("authors") = py::none(),
             py::arg("contributors") = py::none(), py::arg("website") = py::none(), py::arg("prefix") = py::none(),
             py::arg("provides") = py::none(), py::arg("depend") = py::none(), py::arg("soft_depend") = py::none(),
             py::arg("load_before") = py::none(), py::arg("default_permission") = py::none(),
             py::arg("commands") = py::none(), py::arg("permissions") = py::none())
        .def_property_readonly("name", &PluginDescription::getName,
                               "Gives the name of the plugin. This name is a unique identifier for plugins.")
        .def_property_readonly("version", &PluginDescription::getVersion, "Gives the version of the plugin.")
        .def_property_readonly("full_name", &PluginDescription::getFullName,
                               "Returns the name of a plugin, including the version.")
        .def_property_readonly("api_version", &PluginDescription::getAPIVersion,
                               "Gives the API version which this plugin is designed to support.")
        .def_property_readonly("description", &PluginDescription::getDescription,
                               "Gives a human-friendly description of the functionality the plugin provides.")
        .def_property_readonly("load", &PluginDescription::getLoad,
                               "Gives the phase of server startup that the plugin should be loaded.")
        .def_property_readonly("authors", &PluginDescription::getAuthors, "Gives the list of authors for the plugin.")
        .def_property_readonly("contributors", &PluginDescription::getContributors,
                               "Gives the list of contributors for the plugin.")
        .def_property_readonly("website", &PluginDescription::getWebsite,
                               "Gives the plugin's or plugin's author's website.")
        .def_property_readonly("prefix", &PluginDescription::getPrefix,
                               "Gives the token to prefix plugin-specific logging messages with.")
        .def_property_readonly("provides", &PluginDescription::getProvides,
                               "Gives the list of other plugin APIs which this plugin provides. These are usable for "
                               "other plugins to depend on.")
        .def_property_readonly("depend", &PluginDescription::getDepend,
                               "Gives a list of other plugins that the plugin requires.")
        .def_property_readonly("soft_depend", &PluginDescription::getSoftDepend,
                               "Gives a list of other plugins that the plugin requires for full functionality.")
        .def_property_readonly("load_before", &PluginDescription::getLoadBefore,
                               "Gets the list of plugins that should consider this plugin a soft-dependency.")
        .def_property_readonly("default_permission", &PluginDescription::getDefaultPermission,
                               "Gives the default value of permissions registered for the plugin.")
        .def_property_readonly("commands", &PluginDescription::getCommands,
                               "Gives the list of commands the plugin will register at runtime.")
        .def_property_readonly(
            "permissions", &PluginDescription::getPermissions,
            "Gives the list of permissions the plugin will register at runtime, immediately proceeding enabling.");

    py::class_<Plugin, CommandExecutor, PyPlugin, py::smart_holder>(m, "Plugin", "Represents a Plugin")
        .def(py::init<>())
        .def("on_load", &Plugin::onLoad, "Called after a plugin is loaded but before it has been enabled.")
        .def("on_enable", &Plugin::onEnable, "Called when this plugin is enabled")
        .def("on_disable", &Plugin::onDisable, "Called when this plugin is disabled")
        .def("_get_description", &Plugin::getDescription, py::return_value_policy::reference)
        .def_property_readonly("logger", &Plugin::getLogger, py::return_value_policy::reference,
                               "Returns the plugin logger associated with this server's logger.")
        .def_property_readonly("plugin_loader", &Plugin::getPluginLoader, py::return_value_policy::reference,
                               "Gets the associated PluginLoader responsible for this plugin")
        .def_property_readonly("server", &Plugin::getServer, py::return_value_policy::reference,
                               "Returns the Server instance currently running this plugin")
        .def_property_readonly("is_enabled", &Plugin::isEnabled,
                               "Returns a value indicating whether this plugin is currently enabled")
        .def_property_readonly("name", &Plugin::getName, "Returns the name of the plugin.")
        .def("get_command", &Plugin::getCommand, py::return_value_policy::reference, py::arg("name"),
             "Gets the command with the given name, specific to this plugin.")
        .def_property_readonly("data_folder", &Plugin::getDataFolder,
                               "Returns the folder that the plugin data's files are located in.");

    plugin_loader  //
        .def(py::init<Server &>(), py::arg("server"))
        .def("load_plugin", &PluginLoader::loadPlugin, py::arg("file"), py::return_value_policy::reference,
             "Loads the plugin contained in the specified file")
        .def("load_plugins", &PluginLoader::loadPlugins, py::arg("directory"),
             py::return_value_policy::reference_internal, "Loads the plugin contained within the specified directory")
        .def("enable_plugin", &PluginLoader::enablePlugin, py::arg("plugin"), "Enables the specified plugin")
        .def("disable_plugin", &PluginLoader::enablePlugin, py::arg("plugin"), "Disables the specified plugin")
        .def_property_readonly("plugin_file_filters", &PluginLoader::getPluginFileFilters,
                               "Returns a list of all filename filters expected by this PluginLoader")
        .def_property_readonly("server", &PluginLoader::getServer, py::return_value_policy::reference,
                               "Retrieves the Server object associated with the PluginLoader.");

    py::class_<PluginManager>(m, "PluginManager",
                              "Represents a plugin manager that handles all plugins from the Server")
        .def("get_plugin", &PluginManager::getPlugin, py::arg("name"), py::return_value_policy::reference,
             "Checks if the given plugin is loaded and returns it when applicable.")
        .def_property_readonly("plugins", &PluginManager::getPlugins, "Gets a list of all currently loaded plugins")
        .def("is_plugin_enabled", py::overload_cast<const std::string &>(&PluginManager::isPluginEnabled, py::const_),
             py::arg("plugin"), "Checks if the given plugin is enabled or not")
        .def("is_plugin_enabled", py::overload_cast<Plugin *>(&PluginManager::isPluginEnabled, py::const_),
             py::arg("plugin"), "Checks if the given plugin is enabled or not")
        .def("load_plugin", &PluginManager::loadPlugin, py::arg("file"), py::return_value_policy::reference,
             "Loads the plugin in the specified file")
        .def("load_plugins", py::overload_cast<std::string>(&PluginManager::loadPlugins), py::arg("directory"),
             py::return_value_policy::reference_internal, "Loads the plugin contained within the specified directory")
        .def("load_plugins", py::overload_cast<std::vector<std::string>>(&PluginManager::loadPlugins), py::arg("files"),
             py::return_value_policy::reference_internal, "Loads the plugins in the list of the files")
        .def("enable_plugin", &PluginManager::enablePlugin, py::arg("plugin"), "Enables the specified plugin")
        .def("enable_plugins", &PluginManager::enablePlugins, "Enable all the loaded plugins")
        .def("disable_plugin", &PluginManager::disablePlugin, py::arg("plugin"), "Disables the specified plugin")
        .def("disable_plugins", &PluginManager::disablePlugins, "Disables all the loaded plugins")
        .def("clear_plugins", &PluginManager::clearPlugins, "Disables and removes all plugins")
        .def("call_event", &PluginManager::callEvent, py::arg("event"),
             "Calls an event which will be passed to plugins.")
        .def(
            "register_event",
            [](PluginManager &self, std::string event, const std::function<void(Event *)> &executor,
               EventPriority priority, Plugin &plugin, bool ignore_cancelled) {
                self.registerEvent(
                    std::move(event), [executor](Event &e) { executor(&e); }, priority, plugin, ignore_cancelled);
            },
            py::arg("name"), py::arg("executor"), py::arg("priority"), py::arg("plugin"), py::arg("ignore_cancelled"),
            "Registers the given event")
        .def("get_permission", &PluginManager::getPermission, py::arg("name"), py::return_value_policy::reference,
             "Gets a Permission from its fully qualified name.")
        .def("remove_permission", py::overload_cast<Permission &>(&PluginManager::removePermission), py::arg("perm"),
             "Removes a Permission registration from this plugin manager by permission object.")
        .def("remove_permission", py::overload_cast<std::string>(&PluginManager::removePermission), py::arg("name"),
             "Removes a Permission registration from this plugin manager by name.")
        .def("get_default_permissions", &PluginManager::getDefaultPermissions, py::arg("level"),
             py::return_value_policy::reference_internal,
             "Gets the default permissions for the given permission level.")
        .def("recalculate_permission_defaults", &PluginManager::recalculatePermissionDefaults, py::arg("perm"),
             "Recalculates the defaults for the given Permission.")
        .def("subscribe_to_permission", &PluginManager::subscribeToPermission, py::arg("permission"),
             py::arg("permissible"), "Subscribes the given Permissible for information about the requested Permission.")
        .def("unsubscribe_from_permission", &PluginManager::unsubscribeFromPermission, py::arg("permission"),
             py::arg("permissible"),
             "Unsubscribes the given Permissible for information about the requested Permission.")
        .def("get_permission_subscriptions", &PluginManager::getPermissionSubscriptions, py::arg("permission"),
             py::return_value_policy::reference_internal,
             "Gets a set containing all subscribed Permissibles to the given permission.")
        .def("subscribe_to_default_perms", &PluginManager::subscribeToDefaultPerms, py::arg("level"),
             py::arg("permissible"), "Subscribes to the given Default permissions by permission level.")
        .def("unsubscribe_from_default_perms", &PluginManager::unsubscribeFromDefaultPerms, py::arg("level"),
             py::arg("permissible"), "Unsubscribes from the given Default permissions by permission level.")
        .def("get_default_perm_subscriptions", &PluginManager::getDefaultPermSubscriptions, py::arg("level"),
             py::return_value_policy::reference_internal,
             "Gets a set containing all subscribed Permissibles to the given default list, by permission level.")
        .def_property_readonly("permissions", &PluginManager::getPermissions,
                               "Gets a set of all registered permissions.");

    plugin_command
        .def_property("executor", &PluginCommand::getExecutor,
                      py::cpp_function(&PluginCommand::setExecutor, py::keep_alive<1, 2>()),
                      py::return_value_policy::reference, "The CommandExecutor to run when parsing this command")
        .def_property_readonly("plugin", &PluginCommand::getPlugin, "The owner of this PluginCommand");

    py::native_enum<ServicePriority>(m, "ServicePriority", "enum.IntEnum",
                                     "Represents various priorities of a provider.")
        .value("LOWEST", ServicePriority::Lowest)
        .value("LOW", ServicePriority::Low)
        .value("NORMAL", ServicePriority::Normal)
        .value("HIGH", ServicePriority::High)
        .value("HIGHEST", ServicePriority::Highest)
        .finalize();

    py::class_<Service, std::shared_ptr<Service>>(m, "Service", "Represents a list of methods.").def(py::init<>());

    py::class_<ServiceManager>(m, "ServiceManager",
                               "Represent a service manager that manages services and service providers.")
        .def("register", &ServiceManager::registerService, py::arg("name"), py::arg("provider"), py::arg("plugin"),
             py::arg("priority"), "Register a provider of a service.", py::keep_alive<1, 3>())
        .def("unregister_all", &ServiceManager::unregisterAll, py::arg("plugin"),
             "Unregister all the services registered by a particular plugin.")
        .def("unregister", py::overload_cast<std::string, const Service &>(&ServiceManager::unregister),
             py::arg("name"), py::arg("provider"), "Unregister a particular provider for a particular service.")
        .def("unregister", py::overload_cast<const Service &>(&ServiceManager::unregister), py::arg("provider"),
             "Unregister a particular provider.")
        .def("load", &ServiceManager::get, py::arg("name"));
}

}  // namespace endstone::python
