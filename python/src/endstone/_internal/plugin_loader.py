import glob
import importlib
import os
import os.path
import shutil
import site
import subprocess
import sys
from typing import List

from importlib_metadata import entry_points, metadata

from endstone import Server
from endstone._internal.metrics import Metrics
from endstone.command import Command
from endstone.permissions import Permission, PermissionDefault
from endstone.plugin import Plugin, PluginDescription, PluginLoader, PluginLoadOrder

__all__ = ["PythonPluginLoader"]


def find_python():
    paths = []
    if sys.platform == "win32":
        paths.append(os.path.join(sys.prefix, "python.exe"))
    else:
        paths.append(os.path.join(sys.prefix, "bin", "python" + f"{sys.version_info.major}.{sys.version_info.minor}"))
        paths.append(os.path.join(sys.prefix, "bin", "python" + f"{sys.version_info.major}"))
        paths.append(os.path.join(sys.prefix, "bin", "python"))

    for path in paths:
        if os.path.isfile(path):
            return path

    raise RuntimeError(f"Unable to find Python executable. Attempted paths: {paths}")


class PythonPluginLoader(PluginLoader):
    SUPPORTED_API = ["0.5"]

    def __init__(self, server: Server):
        PluginLoader.__init__(self, server)
        self._plugins = []
        sys.executable = find_python()
        self._metrics = Metrics(self.server)

    def __del__(self):
        self._metrics.shutdown()

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
        importlib.invalidate_caches()
        for module in list(sys.modules.keys()):
            if module.startswith("endstone_"):
                del sys.modules[module]

        env = os.environ.copy()
        env.pop("LD_PRELOAD", "")

        prefix = os.path.join(directory, ".local")
        shutil.rmtree(prefix, ignore_errors=True)

        for file in glob.glob(os.path.join(directory, "*.whl")):
            subprocess.run(
                [
                    sys.executable,
                    "-m",
                    "pip",
                    "install",
                    file,
                    "--prefix",
                    prefix,
                    "--quiet",
                    "--no-warn-script-location",
                    "--disable-pip-version-check",
                ],
                env=env,
            )

        for site_dir in site.getsitepackages(prefixes=[prefix]):
            site.addsitedir(site_dir)

        loaded_plugins = []
        eps = entry_points(group="endstone")
        for ep in eps:
            # enforce naming convention
            if not ep.dist.name.replace("_", "-").startswith("endstone-"):
                self.server.logger.error(
                    f"Error occurred when trying to load plugin from entry point '{ep.name}': Invalid name."
                )
                self.server.logger.error(
                    f"The name of distribution ({ep.dist.name}) does not start with 'endstone-' or 'endstone_'."
                )
                continue

            dist_name = "endstone-" + ep.name.replace("_", "-")
            if ep.dist.name.replace("_", "-") != dist_name:
                self.server.logger.error(
                    f"Error occurred when trying to load plugin from entry point '{ep.name}': Invalid name."
                )
                self.server.logger.error("You need to make **ONE** of the following changes.")
                self.server.logger.error(
                    f"* If you intend to use the current entry point name ({ep.name}), "
                    f"please change the distribution name from '{ep.dist.name}' to '{dist_name}'."
                )
                self.server.logger.error(
                    f"* If not, " f"please change the entry point name from '{ep.name}' to '{ep.dist.name[9:]}'."
                )
                continue

            # get distribution metadata
            try:
                plugin_metadata = metadata(ep.dist.name).json
                cls = ep.load()
            except Exception as e:
                self.server.logger.error(f"Error occurred when trying to load plugin from entry point '{ep.name}': {e}")
                continue

            # prepare plugin description
            cls_attr = dict(cls.__dict__)
            name = cls_attr.pop("name", ep.name.replace("-", "_"))
            version = cls_attr.pop("version", plugin_metadata["version"])

            api_version = cls_attr.pop("api_version", None)
            if api_version is None:
                self.server.logger.warning(
                    f"Plugin '{name}' does not specify an API version. This may prevent the plugin from loading in "
                    f"future releases."
                )
            elif api_version not in self.SUPPORTED_API:
                self.server.logger.error(
                    f"Error occurred when trying to load plugin '{name}': plugin was designed for API version: "
                    f"{api_version} which is not compatible with this server."
                )
                continue

            load = cls_attr.pop("load", None)
            if load is not None:
                if isinstance(load, str):
                    load = PluginLoadOrder.__members__[load.strip().replace(" ", "_").upper()]
                elif not isinstance(load, PluginLoadOrder):
                    raise TypeError(f"Invalid value for load order: {load}")

            description = cls_attr.pop("description", plugin_metadata.get("summary", None))
            authors = cls_attr.pop("authors", plugin_metadata.get("author_email", "").split(","))
            website = cls_attr.pop("website", "; ".join(plugin_metadata.get("project_url", [])))

            commands = cls_attr.pop("commands", {})
            commands = self._build_commands(commands)

            permissions = cls_attr.pop("permissions", {})
            permissions = self._build_permissions(permissions)

            plugin_description = PluginDescription(
                name=name,
                version=version,
                load=load,
                description=description,
                authors=authors,
                website=website,
                commands=commands,
                permissions=permissions,
                **cls_attr,
            )

            # instantiate plugin
            plugin = cls()
            if not isinstance(plugin, Plugin):
                raise TypeError(f"Main class {ep.value} does not extend endstone.plugin.Plugin")
            plugin._description = plugin_description
            self._plugins.append(plugin)
            loaded_plugins.append(plugin)

        return loaded_plugins
