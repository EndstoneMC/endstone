"""
Classes relating to loading and managing plugins.
"""

from __future__ import annotations

import collections.abc
import enum
import pathlib
import typing

import endstone._python
import endstone._python.command
import endstone._python.event
import endstone._python.permissions

__all__: list[str] = [
    "Plugin",
    "PluginCommand",
    "PluginDescription",
    "PluginLoadOrder",
    "PluginLoader",
    "PluginManager",
    "Service",
    "ServiceManager",
    "ServicePriority",
]

class Plugin(endstone._python.command.CommandExecutor):
    """
    Represents a Plugin
    """

    def __init__(self) -> None: ...
    def _get_description(self) -> PluginDescription: ...
    def get_command(self, name: str) -> PluginCommand:
        """
        Gets the command with the given name, specific to this plugin.
        """

    def on_disable(self) -> None:
        """
        Called when this plugin is disabled
        """

    def on_enable(self) -> None:
        """
        Called when this plugin is enabled
        """

    def on_load(self) -> None:
        """
        Called after a plugin is loaded but before it has been enabled.
        """

    @property
    def data_folder(self) -> pathlib.Path:
        """
        Returns the folder that the plugin data's files are located in.
        """

    @property
    def is_enabled(self) -> bool:
        """
        Returns a value indicating whether this plugin is currently enabled
        """

    @property
    def logger(self) -> endstone._python.Logger:
        """
        Returns the plugin logger associated with this server's logger.
        """

    @property
    def name(self) -> str:
        """
        Returns the name of the plugin.
        """

    @property
    def plugin_loader(self) -> PluginLoader:
        """
        Gets the associated PluginLoader responsible for this plugin
        """

    @property
    def server(self) -> endstone._python.Server:
        """
        Returns the Server instance currently running this plugin
        """

class PluginCommand(endstone._python.command.Command):
    """
    Represents a Command belonging to a Plugin
    """

    @property
    def executor(self) -> endstone._python.command.CommandExecutor:
        """
        The CommandExecutor to run when parsing this command
        """

    @executor.setter
    def executor(self, arg1: endstone._python.command.CommandExecutor) -> None: ...
    @property
    def plugin(self) -> Plugin:
        """
        The owner of this PluginCommand
        """

class PluginDescription:
    """
    Represents the basic information about a plugin that the plugin loader needs to know.
    """

    def __init__(
        self,
        name: str,
        version: str,
        description: str | None = None,
        load: endstone._python.plugin.PluginLoadOrder | None = None,
        authors: collections.abc.Sequence[str] | None = None,
        contributors: collections.abc.Sequence[str] | None = None,
        website: str | None = None,
        prefix: str | None = None,
        provides: collections.abc.Sequence[str] | None = None,
        depend: collections.abc.Sequence[str] | None = None,
        soft_depend: collections.abc.Sequence[str] | None = None,
        load_before: collections.abc.Sequence[str] | None = None,
        default_permission: endstone._python.PermissionDefault | None = None,
        commands: collections.abc.Sequence[endstone._python.command.Command] | None = None,
        permissions: collections.abc.Sequence[endstone._python.permissions.Permission] | None = None,
        *args,
        **kwargs,
    ) -> None: ...
    @property
    def api_version(self) -> str:
        """
        Gives the API version which this plugin is designed to support.
        """

    @property
    def authors(self) -> list[str]:
        """
        Gives the list of authors for the plugin.
        """

    @property
    def commands(self) -> list[endstone._python.command.Command]:
        """
        Gives the list of commands the plugin will register at runtime.
        """

    @property
    def contributors(self) -> list[str]:
        """
        Gives the list of contributors for the plugin.
        """

    @property
    def default_permission(self) -> endstone._python.PermissionDefault:
        """
        Gives the default value of permissions registered for the plugin.
        """

    @property
    def depend(self) -> list[str]:
        """
        Gives a list of other plugins that the plugin requires.
        """

    @property
    def description(self) -> str:
        """
        Gives a human-friendly description of the functionality the plugin provides.
        """

    @property
    def full_name(self) -> str:
        """
        Returns the name of a plugin, including the version.
        """

    @property
    def load(self) -> PluginLoadOrder:
        """
        Gives the phase of server startup that the plugin should be loaded.
        """

    @property
    def load_before(self) -> list[str]:
        """
        Gets the list of plugins that should consider this plugin a soft-dependency.
        """

    @property
    def name(self) -> str:
        """
        Gives the name of the plugin. This name is a unique identifier for plugins.
        """

    @property
    def permissions(self) -> list[endstone._python.permissions.Permission]:
        """
        Gives the list of permissions the plugin will register at runtime, immediately proceeding enabling.
        """

    @property
    def prefix(self) -> str:
        """
        Gives the token to prefix plugin-specific logging messages with.
        """

    @property
    def provides(self) -> list[str]:
        """
        Gives the list of other plugin APIs which this plugin provides. These are usable for other plugins to depend on.
        """

    @property
    def soft_depend(self) -> list[str]:
        """
        Gives a list of other plugins that the plugin requires for full functionality.
        """

    @property
    def version(self) -> str:
        """
        Gives the version of the plugin.
        """

    @property
    def website(self) -> str:
        """
        Gives the plugin's or plugin's author's website.
        """

class PluginLoadOrder(enum.Enum):
    """
    Represents the order in which a plugin should be initialized and enabled.
    """

    POSTWORLD: typing.ClassVar[PluginLoadOrder]  # value = <PluginLoadOrder.POSTWORLD: 1>
    STARTUP: typing.ClassVar[PluginLoadOrder]  # value = <PluginLoadOrder.STARTUP: 0>

class PluginLoader:
    """
    Represents a plugin loader, which handles direct access to specific types of plugins
    """

    def __init__(self, server: endstone._python.Server) -> None: ...
    def disable_plugin(self, plugin: Plugin) -> None:
        """
        Disables the specified plugin
        """

    def enable_plugin(self, plugin: Plugin) -> None:
        """
        Enables the specified plugin
        """

    def load_plugin(self, file: str) -> Plugin:
        """
        Loads the plugin contained in the specified file
        """

    def load_plugins(self, directory: str) -> list[Plugin]:
        """
        Loads the plugin contained within the specified directory
        """

    @property
    def plugin_file_filters(self) -> list[str]:
        """
        Returns a list of all filename filters expected by this PluginLoader
        """

    @property
    def server(self) -> endstone._python.Server:
        """
        Retrieves the Server object associated with the PluginLoader.
        """

class PluginManager:
    """
    Represents a plugin manager that handles all plugins from the Server
    """

    def call_event(self, event: endstone._python.event.Event) -> None:
        """
        Calls an event which will be passed to plugins.
        """

    def clear_plugins(self) -> None:
        """
        Disables and removes all plugins
        """

    def disable_plugin(self, plugin: Plugin) -> None:
        """
        Disables the specified plugin
        """

    def disable_plugins(self) -> None:
        """
        Disables all the loaded plugins
        """

    def enable_plugin(self, plugin: Plugin) -> None:
        """
        Enables the specified plugin
        """

    def enable_plugins(self) -> None:
        """
        Enable all the loaded plugins
        """

    def get_default_perm_subscriptions(
        self, level: endstone._python.PermissionLevel
    ) -> set[endstone._python.permissions.Permissible]:
        """
        Gets a set containing all subscribed Permissibles to the given default list, by permission level.
        """

    def get_default_permissions(
        self, level: endstone._python.PermissionLevel
    ) -> list[endstone._python.permissions.Permission]:
        """
        Gets the default permissions for the given permission level.
        """

    def get_permission(self, name: str) -> endstone._python.permissions.Permission:
        """
        Gets a Permission from its fully qualified name.
        """

    def get_permission_subscriptions(self, permission: str) -> set[endstone._python.permissions.Permissible]:
        """
        Gets a set containing all subscribed Permissibles to the given permission.
        """

    def get_plugin(self, name: str) -> Plugin:
        """
        Checks if the given plugin is loaded and returns it when applicable.
        """

    @typing.overload
    def is_plugin_enabled(self, plugin: str) -> bool:
        """
        Checks if the given plugin is enabled or not
        """

    @typing.overload
    def is_plugin_enabled(self, plugin: Plugin) -> bool:
        """
        Checks if the given plugin is enabled or not
        """

    def load_plugin(self, file: str) -> Plugin:
        """
        Loads the plugin in the specified file
        """

    @typing.overload
    def load_plugins(self, directory: str) -> list[Plugin]:
        """
        Loads the plugin contained within the specified directory
        """

    @typing.overload
    def load_plugins(self, files: collections.abc.Sequence[str]) -> list[Plugin]:
        """
        Loads the plugins in the list of the files
        """

    def recalculate_permission_defaults(self, perm: endstone._python.permissions.Permission) -> None:
        """
        Recalculates the defaults for the given Permission.
        """

    def register_event(
        self,
        name: str,
        executor: collections.abc.Callable[[endstone._python.event.Event], None],
        priority: endstone._python.EventPriority,
        plugin: Plugin,
        ignore_cancelled: bool,
    ) -> None:
        """
        Registers the given event
        """

    @typing.overload
    def remove_permission(self, perm: endstone._python.permissions.Permission) -> None:
        """
        Removes a Permission registration from this plugin manager by permission object.
        """

    @typing.overload
    def remove_permission(self, name: str) -> None:
        """
        Removes a Permission registration from this plugin manager by name.
        """

    def subscribe_to_default_perms(
        self, level: endstone._python.PermissionLevel, permissible: endstone._python.permissions.Permissible
    ) -> None:
        """
        Subscribes to the given Default permissions by permission level.
        """

    def subscribe_to_permission(self, permission: str, permissible: endstone._python.permissions.Permissible) -> None:
        """
        Subscribes the given Permissible for information about the requested Permission.
        """

    def unsubscribe_from_default_perms(
        self, level: endstone._python.PermissionLevel, permissible: endstone._python.permissions.Permissible
    ) -> None:
        """
        Unsubscribes from the given Default permissions by permission level.
        """

    def unsubscribe_from_permission(
        self, permission: str, permissible: endstone._python.permissions.Permissible
    ) -> None:
        """
        Unsubscribes the given Permissible for information about the requested Permission.
        """

    @property
    def permissions(self) -> set[endstone._python.permissions.Permission]:
        """
        Gets a set of all registered permissions.
        """

    @property
    def plugins(self) -> list[Plugin]:
        """
        Gets a list of all currently loaded plugins
        """

class Service:
    """
    Represents a list of methods.
    """

    def __init__(self) -> None: ...

class ServiceManager:
    """
    Represent a service manager that manages services and service providers.
    """

    def load(self, name: str) -> Service: ...
    def register(self, name: str, provider: Service, plugin: Plugin, priority: ServicePriority) -> None:
        """
        Register a provider of a service.
        """

    @typing.overload
    def unregister(self, name: str, provider: Service) -> None:
        """
        Unregister a particular provider for a particular service.
        """

    @typing.overload
    def unregister(self, provider: Service) -> None:
        """
        Unregister a particular provider.
        """

    def unregister_all(self, plugin: Plugin) -> None:
        """
        Unregister all the services registered by a particular plugin.
        """

class ServicePriority(enum.IntEnum):
    """
    Represents various priorities of a provider.
    """

    HIGH: typing.ClassVar[ServicePriority]  # value = <ServicePriority.HIGH: 3>
    HIGHEST: typing.ClassVar[ServicePriority]  # value = <ServicePriority.HIGHEST: 4>
    LOW: typing.ClassVar[ServicePriority]  # value = <ServicePriority.LOW: 1>
    LOWEST: typing.ClassVar[ServicePriority]  # value = <ServicePriority.LOWEST: 0>
    NORMAL: typing.ClassVar[ServicePriority]  # value = <ServicePriority.NORMAL: 2>

    @classmethod
    def __new__(cls, value): ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
