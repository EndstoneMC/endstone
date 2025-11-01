import platform
from pathlib import Path
from typing import Any, Callable, Dict

import psutil

from endstone.plugin import Plugin

from ._base import MetricsBase
from ._charts import DrilldownPie, SimplePie
from ._config import MetricsConfig

__all__ = ["Metrics"]


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
        return self._plugin.is_enabled

    def append_platform_data(self, platform_data: Dict[str, Any]) -> None:
        """
        Appends platform-specific data to the provided dict.

        Args:
            platform_data (Dict[str, Any]): The dict to append data to.
        """
        platform_data["playerAmount"] = len(self._plugin.server.online_players)
        platform_data["onlineMode"] = self._plugin.server.online_mode
        platform_data["minecraftVersion"] = self._plugin.server.minecraft_version

        os_name = platform.system()
        if os_name == "Windows":
            platform_data["osName"] = f"Windows {platform.release()}"
            platform_data["osVersion"] = platform.version()
        elif os_name == "Linux":
            platform_data["osName"] = "Linux"
            platform_data["osVersion"] = platform.release()

        platform_data["osArch"] = platform.machine().lower()
        platform_data["coreCount"] = psutil.cpu_count(logical=False)

    def append_service_data(self, service_data: Dict[str, Any]):
        """
        Appends service-specific data to the provided dict.

        Args:
            service_data (Dict[str, Any]): The dict to append data to.
        """
        service_data["pluginVersion"] = self._plugin.description.version

    def submit_task(self, task: Callable[[], None]) -> None:
        self._plugin.server.scheduler.run_task(self._plugin, task)

    def log_info(self, message: str) -> None:
        self._plugin.logger.info(message)

    def log_error(self, message: str, exception: Exception) -> None:
        self._plugin.logger.warning(f"{message}: {exception}")
