from typing import final

from endstone._plugin import PluginLogger

from .plugin_description import PluginDescription


class Plugin:
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
        self._logger = None  # Declaration only. Real value to be set by PythonPluginLoader::loadPlugin from C++

    @final
    @property
    def description(self) -> PluginDescription:
        return self._description

    @final
    @property
    def logger(self) -> PluginLogger:
        return self._logger

    @final
    @property
    def enabled(self):
        return self._enabled

    @final
    def _set_enabled(self, enabled: bool):
        if self._enabled is not enabled:
            self._enabled = enabled

            if self._enabled:
                self.on_enable()
            else:
                self.on_disable()
