from typing import final

from .plugin_description import PluginDescription
from .plugin_logger import PluginLogger


class Plugin:
    def __init__(self) -> None:
        pass

    def on_load(self) -> None:
        pass

    def on_enable(self) -> None:
        pass

    def on_disable(self) -> None:
        pass

    # noinspection PyAttributeOutsideInit
    @final
    def _init(self, description: PluginDescription) -> None:
        self._enabled = False
        self._description = description
        self._logger = PluginLogger(self)

    @final
    @property
    def logger(self) -> PluginLogger:
        assert self._logger is not None
        return self._logger

    @final
    @property
    def description(self) -> PluginDescription:
        assert self._description is not None
        return self._description

    @final
    def is_enabled(self) -> bool:
        return self._enabled

    @final
    def _set_enabled(self, enabled: bool) -> None:
        if self._enabled is not enabled:
            self._enabled = enabled

            if enabled:
                self.on_enable()
            else:
                self.on_disable()
