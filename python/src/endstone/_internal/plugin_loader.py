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

            ep_name = ep.name.replace("-", "_")
            dist_name = "endstone_" + ep_name
            plugin_metadata = metadata(dist_name).json
            plugin._description = PluginDescription(
                name=ep_name.replace("_", " ").title().replace(" ", ""),
                version=plugin_metadata["version"],
                description=plugin_metadata.get("summary"),
                authors=[plugin_metadata.get("author_email")],
            )
            # TODO: pass plugin_metadata.get("project_url") to PluginDescription

            self._plugins.append(plugin)
            loaded_plugins.append(plugin)

        return loaded_plugins
