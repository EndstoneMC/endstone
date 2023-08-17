from typing import final

from endstone._plugin import IPlugin

from endstone.plugin import PluginLogger
from endstone.plugin.plugin_description import PluginDescription


class Plugin(IPlugin):
    def __init__(self) -> None:
        IPlugin.__init__(self)

    # noinspection PyAttributeOutsideInit
    @final
    def _init(self, description: PluginDescription) -> None:
        self._enabled = False
        self._description = description
        self._logger = PluginLogger(self)

    @final
    def get_description(self) -> PluginDescription:
        return self._description

    def on_load(self) -> None:
        pass

    def on_enable(self) -> None:
        pass

    def on_disable(self) -> None:
        pass

    @final
    def is_enabled(self):
        return self._enabled

    @final
    def get_logger(self) -> PluginLogger:
        return self._logger
