from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from endstone.plugin import Plugin

from endstone._logger import Logger, LogLevel


class PluginLogger(Logger):
    def __init__(self, plugin: "Plugin"):
        Logger.__init__(self, plugin.__class__.__name__)
        prefix = plugin.description.prefix
        self.plugin_name = prefix if prefix is not None else plugin.description.name
        self.set_level(LogLevel.ALL)

    def log(self, level, msg):
        Logger.log(self, level, f"[{self.plugin_name}] {msg}")

    def verbose(self, msg):
        self.log(LogLevel.VERBOSE, msg)

    def info(self, msg):
        self.log(LogLevel.INFO, msg)

    def warning(self, msg):
        self.log(LogLevel.WARNING, msg)

    def error(self, msg):
        self.log(LogLevel.ERROR, msg)
