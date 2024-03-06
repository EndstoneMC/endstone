from importlib.metadata import entry_points, metadata
from typing import List

from endstone.plugin import PluginDescription, PluginLoader, Plugin
from endstone.server import Server


class PythonPluginLoader(PluginLoader):
    def __init__(self, server: Server):
        PluginLoader.__init__(self, server)
        self._plugins = []

    def load_plugins(self, directory) -> List[Plugin]:
        loaded_plugins = []
        eps = entry_points(group="endstone")
        for ep in eps:
            cls = ep.load()
            plugin = cls()
            if not isinstance(plugin, Plugin):
                raise TypeError("Main class {ep.value} does not extend endstone.plugin.Plugin")

            plugin_metadata = metadata(ep.name.replace("-", "_")).json
            plugin._description = PluginDescription(
                name=plugin_metadata["name"].replace("_", " ").replace("-", " ").title().replace(" ", ""),
                version=plugin_metadata["version"],
                description=plugin_metadata.get("summary"),
                authors=[plugin_metadata.get("author_email")],
            )

            self._plugins.append(plugin)
            loaded_plugins.append(plugin)

        return loaded_plugins
