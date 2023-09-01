from typing import final

from endstone._bindings import PluginBase, Logger, PluginLoader, PluginDescription

from endstone.command import Command, CommandSender


class Plugin(PluginBase):
    def __init__(self):
        PluginBase.__init__(self)

    def on_load(self) -> None:
        pass

    def on_enable(self) -> None:
        pass

    def on_disable(self) -> None:
        pass

    def on_command(self, sender: CommandSender, command: Command, label: str, *args) -> bool:
        return False

    # noinspection PyAttributeOutsideInit
    @final
    def _init(self, description: PluginDescription) -> None:
        self._description = description

    @property
    def description(self) -> PluginDescription:
        return self._get_description()

    @property
    def logger(self) -> Logger:
        return self._get_logger()

    @property
    def plugin_loader(self) -> PluginLoader:
        return self._get_plugin_loader()

    def _get_description(self):
        return self._description
