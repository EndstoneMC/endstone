import os
import platform
import sys
from pathlib import Path
from typing import Any, Dict

from endstone import Server
from endstone_bstats import MetricsBase, MetricsConfig, AdvancedPie, SimplePie


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

        self.add_custom_chart(SimplePie("endstone_version", lambda: self._server.version))
        self.add_custom_chart(SimplePie("minecraft_version", lambda: self._server.minecraft_version))
        self.add_custom_chart(SimplePie("python_version", lambda: f"{sys.version_info.major}.{sys.version_info.minor}"))
        self.add_custom_chart(AdvancedPie("player_platform", self._get_player_platforms))
        self.add_custom_chart(AdvancedPie("player_game_version", self._get_player_game_versions))

    @property
    def enabled(self) -> bool:
        return self._config.enabled

    @property
    def service_enabled(self) -> bool:
        return True

    def append_platform_data(self, platform_data: Dict[str, Any]) -> None:
        platform_data["playerAmount"] = len(self._server.online_players)

        platform_data["osName"] = platform.system()
        platform_data["osArch"] = platform.machine().lower()
        platform_data["osVersion"] = platform.release()
        platform_data["coreCount"] = os.cpu_count()

    def log_info(self, message: str) -> None:
        self._server.logger.info(message)

    def log_error(self, message: str, exception: Exception) -> None:
        self._server.logger.warning(f"{message}: {exception}")

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
