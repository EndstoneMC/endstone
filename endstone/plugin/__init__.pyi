"""
Classes relating to loading and managing plugins.
"""

from __future__ import annotations

import collections.abc
import enum
import pathlib
import typing

from endstone import Logger, Server
from endstone.command import Command, CommandExecutor
from endstone.event import Event, EventPriority
from endstone.permissions import Permissible, Permission, PermissionDefault, PermissionLevel

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

class Plugin(CommandExecutor):
    """
    Represents a Plugin
    """

    version = None
    api_version = None
    description = None
    load = None
    authors = None
    contributors = None
    website = None
    prefix = None
    provides = None
    depend = None
    soft_depend = None
    load_before = None
    commands = None
    default_permission = None
    permissions = None

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
    def logger(self) -> Logger:
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
    def server(self) -> Server:
        """
        Returns the Server instance currently running this plugin
        """

    def register_events(self, listener: object) -> None: ...
    @property
    def config(self) -> dict: ...
    def reload_config(self) -> dict: ...
    def save_config(self) -> None: ...
    def save_default_config(self) -> None: ...
    def save_resources(self, path: str, replace: bool = False) -> None: ...

class PluginCommand(Command):
    """
    Represents a Command belonging to a Plugin
    """

    @property
    def executor(self) -> CommandExecutor:
        """
        The CommandExecutor to run when parsing this command
        """

    @executor.setter
    def executor(self, arg1: CommandExecutor) -> None: ...
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
        load: PluginLoadOrder | None = None,
        authors: list[str] | None = None,
        contributors: list[str] | None = None,
        website: str | None = None,
        prefix: str | None = None,
        provides: list[str] | None = None,
        depend: list[str] | None = None,
        soft_depend: list[str] | None = None,
        load_before: list[str] | None = None,
        default_permission: PermissionDefault | None = None,
        commands: list[Command] | None = None,
        permissions: list[Permission] | None = None,
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
    def commands(self) -> list[Command]:
        """
        Gives the list of commands the plugin will register at runtime.
        """

    @property
    def contributors(self) -> list[str]:
        """
        Gives the list of contributors for the plugin.
        """

    @property
    def default_permission(self) -> PermissionDefault:
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
    def permissions(self) -> list[Permission]:
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

    def __init__(self, server: Server) -> None: ...
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
    def server(self) -> Server:
        """
        Retrieves the Server object associated with the PluginLoader.
        """

class PluginManager:
    """
    Represents a plugin manager that handles all plugins from the Server
    """

    def call_event(self, event: Event) -> None:
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

    def get_default_perm_subscriptions(self, level: PermissionLevel) -> set[Permissible]:
        """
        Gets a set containing all subscribed Permissibles to the given default list, by permission level.
        """

    def get_default_permissions(self, level: PermissionLevel) -> list[Permission]:
        """
        Gets the default permissions for the given permission level.
        """

    def get_permission(self, name: str) -> Permission:
        """
        Gets a Permission from its fully qualified name.
        """

    def get_permission_subscriptions(self, permission: str) -> set[Permissible]:
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
    def load_plugins(self, files: list[str]) -> list[Plugin]:
        """
        Loads the plugins in the list of the files
        """

    def recalculate_permission_defaults(self, perm: Permission) -> None:
        """
        Recalculates the defaults for the given Permission.
        """

    def register_event(
        self,
        name: str,
        executor: collections.abc.Callable[[Event], None],
        priority: EventPriority,
        plugin: Plugin,
        ignore_cancelled: bool,
    ) -> None:
        """
        Registers the given event
        """

    @typing.overload
    def remove_permission(self, perm: Permission) -> None:
        """
        Removes a Permission registration from this plugin manager by permission object.
        """

    @typing.overload
    def remove_permission(self, name: str) -> None:
        """
        Removes a Permission registration from this plugin manager by name.
        """

    def subscribe_to_default_perms(self, level: PermissionLevel, permissible: Permissible) -> None:
        """
        Subscribes to the given Default permissions by permission level.
        """

    def subscribe_to_permission(self, permission: str, permissible: Permissible) -> None:
        """
        Subscribes the given Permissible for information about the requested Permission.
        """

    def unsubscribe_from_default_perms(self, level: PermissionLevel, permissible: Permissible) -> None:
        """
        Unsubscribes from the given Default permissions by permission level.
        """

    def unsubscribe_from_permission(self, permission: str, permissible: Permissible) -> None:
        """
        Unsubscribes the given Permissible for information about the requested Permission.
        """

    @property
    def permissions(self) -> set[Permission]:
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
