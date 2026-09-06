from endstone.plugin import Plugin


class PluginLifecycleFixture(Plugin):
    """Minimal plugin used only as a PluginEnable/PluginDisable target."""

    api_version = "0.12"
