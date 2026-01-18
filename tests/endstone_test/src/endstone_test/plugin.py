import pytest
from endstone.plugin import Plugin, PluginLoadOrder

from .tests.conftest import clear_runtime_context, set_runtime_context


class EndstoneTest(Plugin):
    prefix = "Test"
    api_version = "0.11"
    load = PluginLoadOrder.POSTWORLD
    commands = {
        "test": {
            "description": "Run the tests",
            "usages": ["/test"],
            "permissions": ["endstone_test.command.test"],
        }
    }

    permissions = {
        "endstone_test.command.test": {
            "description": "Allow users to use the /test command.",
            "default": "console",
        }
    }

    def on_load(self) -> None:
        self.logger.info("on_load is called!")

    def on_enable(self) -> None:
        self.logger.info("on_enable is called!")
        self.run_tests()

    def on_disable(self) -> None:
        self.logger.info("on_disable is called!")

    def run_tests(self, **kwargs):
        set_runtime_context(server=self.server, plugin=self, **kwargs)
        try:
            return pytest.main(["-s", "--pyargs", "endstone_test.tests"])
        finally:
            clear_runtime_context()
