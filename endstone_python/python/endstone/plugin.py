import warnings

from importlib import import_module

for name in (__all__ := ["Plugin", "PluginLoader", "PluginDescription"]):
    module = import_module("endstone._internal.endstone_python")
    globals()[f"_{name}"] = module.__dict__[name]
    del module

_Plugin = globals()["_Plugin"]
PluginLoader = globals()["_PluginLoader"]
PluginDescription = globals()["_PluginDescription"]


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
