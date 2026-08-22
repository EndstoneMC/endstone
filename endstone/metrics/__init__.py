import collections.abc
import platform
import typing
from pathlib import Path

import lazy_loader as lazy
import psutil

from endstone._python.metrics import DrilldownPie, SimplePie
from endstone.plugin import Plugin

from ._arch import host_arch
from .base import MetricsBase
from .config import MetricsConfig


def _get_python_version() -> dict[str, dict[str, int]]:
    python_impl = platform.python_implementation()
    major, minor, patch = platform.python_version_tuple()
    return {
        f"{python_impl} {major}.{minor}": {
            f"{python_impl} {major}.{minor}.{patch}": 1,
        },
    }


class Metrics(MetricsBase):
    def __init__(self, plugin: Plugin, service_id: int) -> None:
        """
        Creates a new Metrics instance.

        Args:
            plugin (Plugin): Your plugin instance.
            service_id (int): The id of the service.
                              It can be found at https://bstats.org/what-is-my-plugin-id
        """

        self._plugin = plugin

        # Get the config file
        config_folder = Path(plugin.data_folder).parent / "bstats"
        config_file = config_folder / "config.toml"
        self._config = MetricsConfig(config_file, True)

        super().__init__(
            platform="bukkit",
            server_uuid=self._config.server_uuid,
            service_id=service_id,
            log_errors=self._config.log_errors_enabled,
            log_sent_data=self._config.log_sent_data_enabled,
            log_response_status_text=self._config.log_response_status_text_enabled,
        )

        self.add_custom_chart(SimplePie("endstone_version", lambda: self._plugin.server.version))
        self.add_custom_chart(DrilldownPie("python_version", _get_python_version))

    @property
    def enabled(self) -> bool:
        return self._config.enabled

    @property
    def service_enabled(self) -> bool:
        return self._plugin is not None and self._plugin.is_enabled

    def append_platform_data(self, platform_data: dict[str, typing.Any]) -> None:
        """
        Appends platform-specific data to the provided dict.

        Args:
            platform_data (dict[str, typing.Any]): The dict to append data to.
        """
        server = self._plugin.server
        platform_data["playerAmount"] = len(server.online_players)
        platform_data["onlineMode"] = server.online_mode
        platform_data["bukkitVersion"] = f"{server.version} (MC: {server.minecraft_version})"
        platform_data["bukkitName"] = server.name

        os_name = platform.system()
        if os_name == "Windows":
            platform_data["osName"] = f"Windows {platform.release()}"
            platform_data["osVersion"] = platform.version()
        elif os_name == "Linux":
            platform_data["osName"] = "Linux"
            platform_data["osVersion"] = platform.release()

        platform_data["osArch"] = host_arch()
        platform_data["coreCount"] = psutil.cpu_count(logical=False)

    def append_service_data(self, service_data: dict[str, typing.Any]) -> None:
        """
        Appends service-specific data to the provided dict.

        Args:
            service_data (dict[str, typing.Any]): The dict to append data to.
        """
        service_data["pluginVersion"] = self._plugin.plugin_description.version

    def submit_task(self, task: collections.abc.Callable[[], None]) -> None:
        if self._plugin is not None and not self._shutdown:
            self._plugin.server.scheduler.run_task(self._plugin, task)

    def log_info(self, message: str) -> None:
        if self._plugin is not None:
            self._plugin.logger.info(message)

    def log_error(self, message: str, exception: Exception) -> None:
        if self._plugin is not None:
            self._plugin.logger.warning(f"{message}: {exception}")

    def shutdown(self) -> None:
        try:
            super().shutdown()
        finally:
            self._plugin = None


__getattr__, __dir__, __all__ = lazy.attach(
    "endstone._python",
    submod_attrs={
        "metrics": [
            "AdvancedBarChart",
            "AdvancedPie",
            "CustomChart",
            "MultiLineChart",
            "SimpleBarChart",
            "SingleLineChart",
        ]
    },
)

__all__.extend(["DrilldownPie", "Metrics", "MetricsBase", "MetricsConfig", "SimplePie"])
