from endstone._plugin import Plugin as IPlugin

from .plugin_logger import PluginLogger


class Plugin(IPlugin):
    def __init__(self):
        IPlugin.__init__(self)
        self._logger = PluginLogger(self.__class__.__name__)

    def get_logger(self) -> PluginLogger:
        return self._logger
