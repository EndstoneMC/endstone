from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from endstone.plugin import Plugin

from endstone.util.logging import Logger, LogLevel


class PluginLogger(Logger):
    def __init__(self, plugin: "Plugin"):
        super().__init__(plugin.__class__.__name__)
        prefix = plugin.description.prefix
        self.plugin_name = prefix if prefix is not None else plugin.description.name
        self.set_level(LogLevel.ALL)

    def log(self, level, msg, *args):
        super().log(level, f"[{self.plugin_name}] {msg}", *args)
