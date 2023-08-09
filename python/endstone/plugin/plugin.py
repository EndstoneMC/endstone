from typing import final

from .plugin_logger import PluginLogger


class Plugin:
    def __init__(self):
        self._enabled = False
        self._logger = PluginLogger(self.__class__.__name__)

    def on_load(self):
        pass

    def on_enable(self):
        pass

    def on_disable(self):
        pass

    @final
    @property
    def logger(self) -> PluginLogger:
        return self._logger

    @final
    def is_enabled(self):
        return self._enabled

    @final
    def _set_enabled(self, enabled: bool):
        if self._enabled is not enabled:
            self._enabled = enabled

            if enabled:
                self.on_enable()
            else:
                self.on_disable()
