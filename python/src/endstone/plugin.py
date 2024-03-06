from endstone._internal.endstone_python import PluginDescription, PluginLoader, PluginManager
from endstone._internal.endstone_python import Plugin as _Plugin

__all__ = [
    "Plugin",
    "PluginDescription",
    "PluginLoader",
    "PluginManager",
]


class Plugin(_Plugin):
    def __init__(self):
        _Plugin.__init__(self)
        self._description: PluginDescription

    def _get_description(self) -> PluginDescription:
        return self._description

    @property
    def description(self) -> PluginDescription:
        """
        Returns the details of this plugin
        """
        # noinspection PyUnresolvedReferences
        return self._get_description()
