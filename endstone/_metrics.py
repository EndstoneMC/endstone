import platform
from pathlib import Path
from typing import Any, Dict

import psutil
from endstone_bstats import AdvancedPie, DrilldownPie, MetricsBase, MetricsConfig, SimplePie, SingleLineChart

from endstone import Server, __minecraft_version__


class Metrics(MetricsBase):
    def __init__(self, server: Server):
        self._server = server

        # Get the config file
        bstats_folder = Path("plugins") / "bstats"
        config_file = bstats_folder / "config.toml"
        self._config = MetricsConfig(config_file, True)

        super().__init__(
            platform="server-implementation",
            server_uuid=self._config.server_uuid,
            service_id=23296,
            log_errors=self._config.log_errors_enabled,
            log_sent_data=self._config.log_sent_data_enabled,
            log_response_status_text=self._config.log_response_status_text_enabled,
        )

        self.add_custom_chart(SingleLineChart("players", lambda: len(self._server.online_players)))
        self.add_custom_chart(SimplePie("endstone_version", lambda: self._server.version))
        self.add_custom_chart(SimplePie("minecraft_version", lambda: __minecraft_version__))
        self.add_custom_chart(DrilldownPie("online_mode", self._get_online_mode))
        self.add_custom_chart(DrilldownPie("python_version", self._get_python_version))
        self.add_custom_chart(AdvancedPie("player_platform", self._get_player_platforms))
        self.add_custom_chart(AdvancedPie("player_game_version", self._get_player_game_versions))

    @property
    def enabled(self) -> bool:
        return self._config.enabled

    @property
    def service_enabled(self) -> bool:
        return True

    def append_platform_data(self, platform_data: Dict[str, Any]) -> None:
        os_name = platform.system()
        if os_name == "Windows":
            platform_data["osName"] = f"Windows {platform.release()}"
            platform_data["osVersion"] = platform.version()
        elif os_name == "Linux":
            platform_data["osName"] = "Linux"
            platform_data["osVersion"] = platform.release()

        os_arch = platform.machine().lower()
        if os_arch == "x86_64":
            os_arch = "amd64"
        platform_data["osArch"] = os_arch
        platform_data["coreCount"] = psutil.cpu_count(logical=False)

    def log_info(self, message: str) -> None:
        self._server.logger.info(message)

    def log_error(self, message: str, exception: Exception) -> None:
        self._server.logger.warning(f"{message}: {exception}")

    def _get_online_mode(self) -> dict[str, dict[str, int]]:
        value = "true" if self._server.online_mode else "false"
        return {
            value: {
                value: 1,
            },
        }

    def _get_python_version(self) -> dict[str, dict[str, int]]:
        python_impl = platform.python_implementation()
        major, minor, patch = platform.python_version_tuple()
        return {
            f"{python_impl} {major}.{minor}": {
                f"{major}.{minor}.{patch}": 1,
            },
        }

    def _get_player_platforms(self) -> dict[str, int]:
        result: dict[str, int] = {}
        for player in self._server.online_players:
            if player.device_os not in result:
                result[player.device_os] = 1
            else:
                result[player.device_os] += 1

        return result

    def _get_player_game_versions(self) -> dict[str, int]:
        result: dict[str, int] = {}
        for player in self._server.online_players:
            if player.game_version not in result:
                result[player.game_version] = 1
            else:
                result[player.game_version] += 1

        return result
