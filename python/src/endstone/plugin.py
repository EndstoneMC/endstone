__all__ = [
    "Plugin",
    "PluginDescription",
    "PluginLoader",
    "PluginManager",
]

for name in __all__:
    from importlib import import_module

    module = import_module("endstone._internal.endstone_python")
    globals()[f"_{name}"] = module.__dict__[name]
    del module

_Plugin = globals()["_Plugin"]
PluginDescription = globals()["_PluginDescription"]
PluginLoader = globals()["_PluginLoader"]
PluginManager = globals()["_PluginManager"]


class Plugin(_Plugin):
    def __init__(self):
        _Plugin.__init__(self)
        self._description: PluginDescription

    def _get_description(self) -> PluginDescription:
        return self._description

    @property
    def description(self) -> PluginDescription:
        # noinspection PyUnresolvedReferences
        return self._get_description()
