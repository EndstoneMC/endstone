from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from endstone.plugin import Plugin

from endstone.util.logging import Logger


class PluginLogger(Logger):
    def __init__(self, plugin: "Plugin"):
        super().__init__(plugin.__class__.__name__)
        prefix = plugin.get_description().get_prefix()
        self.plugin_name = prefix if prefix is not None else plugin.get_description().get_name()

    def log(self, level, msg, *args):
        super().log(level, f"[{self.plugin_name}] {msg}", *args)
