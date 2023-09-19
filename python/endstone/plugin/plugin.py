# noinspection PyProtectedMember
from endstone._bindings import PluginBase, PluginDescription

from endstone.command import Command, CommandSender


# noinspection PyProtectedMember
class Plugin(PluginBase):
    def __init__(self):
        PluginBase.__init__(self)
        self._description: PluginDescription

    def on_load(self) -> None:
        pass

    def on_enable(self) -> None:
        pass

    def on_disable(self) -> None:
        pass

    # noinspection PyMethodOverriding
    def on_command(self, sender: CommandSender, command: Command, label: str, args: list[str]) -> bool:
        return False

    def _get_description(self) -> PluginDescription:
        return self._description

    description = property(fget=_get_description)
    logger = property(fget=PluginBase._get_logger)
    plugin_loader = property(fget=PluginBase._get_plugin_loader)
    server = property(fget=PluginBase._get_server)
