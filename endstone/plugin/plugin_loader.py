from __future__ import annotations

import glob
import importlib
import os
import os.path
import site
import subprocess
import sys
import traceback
import warnings
from pathlib import Path
from typing import Any

import pkginfo
from importlib_metadata import EntryPoint, distribution, entry_points, metadata

from endstone import Server
from endstone.command import Command
from endstone.permissions import Permission, PermissionDefault
from endstone.plugin import Plugin, PluginDescription, PluginLoader, PluginLoadOrder

__all__ = ["PythonPluginLoader"]

warnings.simplefilter(action="always", category=FutureWarning)


def find_python() -> Path:
    """Finds the Python executable path.

    Checks ``ENDSTONE_PYTHON_EXECUTABLE`` environment variable first, then
    falls back to platform-specific default locations under ``sys.base_prefix``.

    Returns:
        The resolved path to the Python executable.

    Raises:
        RuntimeError: If no valid Python executable is found.
    """
    paths = []
    if os.environ.get("ENDSTONE_PYTHON_EXECUTABLE", None) is not None:
        paths.append(os.environ["ENDSTONE_PYTHON_EXECUTABLE"])

    if sys.platform == "win32":
        paths.append(os.path.join(sys.base_prefix, "python.exe"))
    else:
        paths.append(
            os.path.join(sys.base_prefix, "bin", "python" + f"{sys.version_info.major}.{sys.version_info.minor}")
        )
        paths.append(os.path.join(sys.base_prefix, "bin", "python" + f"{sys.version_info.major}"))
        paths.append(os.path.join(sys.base_prefix, "bin", "python"))

    # Dedup while preserving order
    seen: set[Path] = set()
    ordered: list[Path] = []
    for p in paths:
        resolved = Path(p).resolve()
        if resolved not in seen:
            seen.add(resolved)
            ordered.append(resolved)

    for path in ordered:
        if path.is_file():
            return path

    raise RuntimeError(f"Unable to find Python executable. Attempted paths: {ordered}")


def _build_commands(commands: dict[str, Any]) -> list[Command]:
    """Converts a plugin's command metadata dict into a list of Command objects.

    Args:
        commands: Mapping of command name to keyword arguments for ``Command``.

    Returns:
        A list of constructed Command instances.
    """
    results = []
    for name, command in commands.items():
        command = Command(name, **command)
        results.append(command)
    return results


def _build_permissions(permissions: dict[str, Any]) -> list[Permission]:
    """Converts a plugin's permission metadata dict into a list of Permission objects.

    The ``"default"`` key in each permission entry is coerced from ``bool`` or
    ``str`` to a ``PermissionDefault`` enum value.

    Args:
        permissions: Mapping of permission name to keyword arguments for ``Permission``.

    Returns:
        A list of constructed Permission instances.

    Raises:
        TypeError: If a ``"default"`` value is not a bool, str, or PermissionDefault.
    """
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


sys.executable = str(find_python())
sys._base_executable = sys.executable  # type: ignore[attr-defined]


class PythonPluginLoader(PluginLoader):
    """Plugin loader for Python plugins distributed as wheel packages.

    Discovers plugins via ``endstone`` entry points and installs wheel files
    using pip into a local prefix (``plugins/.local``). On destruction (e.g.
    server reload), previously installed plugin packages are uninstalled via
    pip to ensure a clean state.
    """

    SUPPORTED_API = ["0.5", "0.6", "0.7", "0.8", "0.9", "0.10", "0.11", "0.12"]

    def __init__(self, server: Server):
        PluginLoader.__init__(self, server)
        self._plugins: list[Plugin] = []
        self._uninstall_plugins()
        self._invalidate_caches()

    def __del__(self) -> None:
        self._uninstall_plugins()

    @staticmethod
    def _uninstall_plugins() -> None:
        """Uninstalls all currently installed endstone plugin distributions via pip."""
        dists = [ep.dist.name for ep in entry_points(group="endstone")]  # type: ignore[union-attr]
        if not dists:
            return
        subprocess.run(
            [
                sys.executable,
                "-m",
                "pip",
                "uninstall",
                *dists,
                "-y",
                "--quiet",
                "--disable-pip-version-check",
            ],
        )

    def _invalidate_caches(self) -> None:
        """Clears Python import caches and registers the local prefix as a site directory."""
        importlib.invalidate_caches()
        for module in list(sys.modules.keys()):
            if module.startswith("endstone_"):
                del sys.modules[module]

        self._prefix = os.path.join("plugins", ".local")
        for site_dir in site.getsitepackages(prefixes=[self._prefix]):
            site.addsitedir(site_dir)

    def load_plugin(self, file: str) -> Plugin | None:  # type: ignore[override]
        """Installs a wheel file via pip and loads the plugin from it.

        Args:
            file: Path to a ``.whl`` file.

        Returns:
            The loaded Plugin instance, or None if no valid entry point was found.

        Raises:
            ValueError: If the package name cannot be determined from the wheel.
        """
        dist_name: str | None = pkginfo.Wheel(file).name
        if dist_name is None:
            raise ValueError(f"Could not determine package name from {file}")
        subprocess.run(
            [
                sys.executable,
                "-m",
                "pip",
                "install",
                file,
                "--prefix",
                self._prefix,
                "--quiet",
                "--no-warn-script-location",
                "--disable-pip-version-check",
            ],
        )

        eps = distribution(dist_name).entry_points.select(group="endstone")
        for ep in eps:
            plugin = self._load_plugin_from_ep(ep)
            if plugin:
                return plugin

        return None

    def load_plugins(self, directory: str) -> list[Plugin]:
        """Loads all plugins from registered entry points and wheel files in the given directory.

        Args:
            directory: Path to the directory containing ``.whl`` files.

        Returns:
            A list of successfully loaded Plugin instances.
        """
        loaded_plugins = []

        if not self._plugins:
            for ep in entry_points(group="endstone"):
                plugin = self._load_plugin_from_ep(ep)
                if plugin:
                    loaded_plugins.append(plugin)

        for file in glob.glob(os.path.join(directory, "*.whl")):
            plugin = self.load_plugin(file)
            if plugin:
                loaded_plugins.append(plugin)

        return loaded_plugins

    def _load_plugin_from_ep(self, ep: EntryPoint) -> Plugin | None:
        """Loads a single plugin from an entry point.

        Validates the distribution naming convention, checks API version
        compatibility, reads distribution metadata, and instantiates the
        plugin class.

        Args:
            ep: An ``endstone`` group entry point.

        Returns:
            The loaded Plugin instance, or None if loading fails.
        """
        if ep.dist is None:
            return None
        if not ep.dist.name.replace("_", "-").startswith("endstone-"):
            self.server.logger.error(
                f"Error occurred when trying to load plugin from entry point '{ep.name}': Invalid name."
            )
            self.server.logger.error(
                f"The name of distribution ({ep.dist.name}) does not start with 'endstone-' or 'endstone_'."
            )
            return None

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
                f"* If not, please change the entry point name from '{ep.name}' to '{ep.dist.name[9:]}'."
            )
            return None

        # get distribution metadata
        try:
            plugin_metadata = metadata(ep.dist.name).json
            cls = ep.load()
        except BaseException as e:
            self.server.logger.error(f"Error occurred when trying to load plugin from entry point '{ep.name}':")
            self.server.logger.error("".join(traceback.format_exception(e)))
            return None

        # prepare plugin description
        cls_attr = dict(cls.__dict__)
        name = ep.name.replace("-", "_")
        cls_attr.pop("name", None)  # ignore name from class attrs; always use the entry point name
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
            return None

        load = cls_attr.pop("load", None)
        if load is not None:
            if isinstance(load, str):
                load = PluginLoadOrder.__members__[load.strip().replace(" ", "_").upper()]
            elif not isinstance(load, PluginLoadOrder):
                raise TypeError(f"Invalid value for load order: {load}")

        description = cls_attr.pop("description", plugin_metadata.get("summary", None))
        author_email = plugin_metadata.get("author_email", "")
        authors = cls_attr.pop("authors", author_email.split(",") if isinstance(author_email, str) else author_email)
        website = cls_attr.pop("website", "; ".join(plugin_metadata.get("project_url", [])))

        commands = cls_attr.pop("commands", {})
        commands = _build_commands(commands)

        permissions = cls_attr.pop("permissions", {})
        permissions = _build_permissions(permissions)

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
            self.server.logger.error(f"Main class {ep.value} does not extend endstone.plugin.Plugin")
            return None

        plugin._description = plugin_description
        self._plugins.append(plugin)
        return plugin
