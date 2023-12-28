import warnings

from endstone._internal.endstone_python import Plugin as _Plugin
from endstone._internal.endstone_python import PluginLoader, PluginDescription

__all__ = ["Plugin", "PluginLoader", "PluginDescription"]


class Plugin(_Plugin):
    def __init__(self):
        _Plugin.__init__(self)
        self._description: PluginDescription

    def _get_description(self) -> PluginDescription:
        warnings.warn(
            (
                "The method '_get_description()' is deprecated and for internal use only. "
                "Use the 'description' property of the Plugin class instead."
            ),
            DeprecationWarning,
        )
        return self._description

    @property
    def description(self) -> PluginDescription:
        # noinspection PyUnresolvedReferences
        return self._get_description()
