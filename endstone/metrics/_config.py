import uuid
from pathlib import Path

import tomlkit


class MetricsConfig:
    """
    A simple config for bStats.
    """

    def __init__(self, file: Path, default_enabled: bool) -> None:
        self._file = file
        self._default_enabled = default_enabled

        self._server_uuid: uuid.UUID | None = None
        self._enabled = False
        self._log_errors = False
        self._log_sent_data = False
        self._log_response_status_text = False
        self._did_exist_before = True

        self.setup_config()

    @property
    def server_uuid(self) -> uuid.UUID:
        return self._server_uuid

    @property
    def enabled(self) -> bool:
        return self._enabled

    @property
    def log_errors_enabled(self) -> bool:
        return self._log_errors

    @property
    def log_sent_data_enabled(self) -> bool:
        return self._log_sent_data

    @property
    def log_response_status_text_enabled(self) -> bool:
        return self._log_response_status_text

    @property
    def did_exist_before(self) -> bool:
        return self._did_exist_before

    def setup_config(self) -> None:
        """
        Creates the config file if it does not exist and read its content.
        """
        if not self._file.exists():
            self._did_exist_before = False
            self.save_default_config()

        self.load_config()

    def save_default_config(self) -> None:
        """
        Creates a config file with the default content.
        """
        comments = [
            "bStats (https://bStats.org) collects some basic information for plugin authors, like",
            "how many people use their plugin and their total player count. It's recommended to keep",
            "bStats enabled, but if you're not comfortable with this, you can turn this setting off.",
            "There is no performance penalty associated with having metrics enabled, and data sent to",
            "bStats is fully anonymous.",
        ]
        config = tomlkit.document()
        for comment in comments:
            config.add(tomlkit.comment(comment))

        config.add("enabled", self._default_enabled)
        config.add("server-uuid", str(uuid.uuid4()))
        config.add("log-errors", False)
        config.add("log-sent-data", False)
        config.add("log-response-status-text", False)

        self._file.parent.mkdir(parents=True, exist_ok=True)
        with open(self._file, "w", encoding="utf-8") as f:
            tomlkit.dump(config, f)

    def load_config(self) -> None:
        """
        Reads the content of the config file.
        """
        with open(self._file, "r", encoding="utf-8") as f:
            config = tomlkit.load(f)

        if "server-uuid" not in config:
            # Found a malformed config file with no UUID. Let's recreate it.
            self.save_default_config()
            self.load_config()
            return

        self._enabled = config.get("enabled", True)
        self._server_uuid = uuid.UUID(config.get("server-uuid"))
        self._log_errors = config.get("log-errors", False)
        self._log_sent_data = config.get("log-sent-data", False)
        self._log_response_status_text = config.get("log-response-status-text", False)
