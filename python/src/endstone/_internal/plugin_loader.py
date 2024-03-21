from importlib.metadata import entry_points, metadata
from typing import List

from endstone.plugin import PluginDescription, PluginLoader, Plugin, PluginLoadOrder
from endstone.server import Server
from endstone.command import Command
from endstone.permissions import Permission, PermissionDefault


class PythonPluginLoader(PluginLoader):
    def __init__(self, server: Server):
        PluginLoader.__init__(self, server)
        self._plugins = []

    @staticmethod
    def _build_commands(commands: dict) -> list[Command]:
        results = []
        for name, command in commands.items():
            command = Command(name, **command)
            results.append(command)
        return results

    @staticmethod
    def _build_permissions(permissions: dict) -> list[Permission]:
        results = []
        for name, permission in permissions.items():
            if "default" in permission:
                value = permission["default"]
                if isinstance(value, bool):
                    permission["default"] = PermissionDefault.TRUE if value else PermissionDefault.FALSE
                elif isinstance(value, str):
                    permission["default"] = PermissionDefault.__members__[value.strip().replace(" ", "_").upper()]
                elif not isinstance(value, PermissionDefault):
                    raise TypeError(f"Invalid value for default permission: {value}")

            permission = Permission(name, **permission)
            results.append(permission)
        return results

    def load_plugins(self, directory) -> List[Plugin]:
        loaded_plugins = []
        eps = entry_points(group="endstone")
        for ep in eps:
            cls = ep.load()

            # get metadata
            ep_name = ep.name.replace("-", "_")
            dist_name = "endstone_" + ep_name
            plugin_metadata = metadata(dist_name).json

            # prepare plugin description
            cls_attr = dict(cls.__dict__)
            if "name" not in cls_attr:
                cls_attr["name"] = ep_name.replace("_", " ").title().replace(" ", "")
            if "version" not in cls_attr:
                cls_attr["version"] = plugin_metadata["version"]
            if "commands" in cls_attr:
                cls_attr["commands"] = self._build_commands(cls_attr["commands"])
            if "permissions" in cls_attr:
                cls_attr["permissions"] = self._build_permissions(cls_attr["permissions"])
            if "load" in cls_attr:
                value = cls_attr["load"]
                if isinstance(value, str):
                    cls_attr["load"] = PluginLoadOrder.__members__[value.strip().replace(" ", "_").upper()]
                elif not isinstance(value, PluginLoadOrder):
                    raise TypeError(f"Invalid value for load order: {value}")

            plugin_description = PluginDescription(**cls_attr)

            # instantiate plugin
            plugin = cls()
            if not isinstance(plugin, Plugin):
                raise TypeError(f"Main class {ep.value} does not extend endstone.plugin.Plugin")
            plugin._description = plugin_description
            self._plugins.append(plugin)
            loaded_plugins.append(plugin)

        return loaded_plugins
