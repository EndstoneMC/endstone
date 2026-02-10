import time

import pytest
from endstone import Player
from endstone.command import Command, CommandSender
from endstone.plugin import Plugin

from .tests.conftest import clear_runtime_context, set_runtime_context


class EndstoneTest(Plugin):
    prefix = "Test"
    api_version = "0.11"
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
            "default": "op",
        }
    }

    def on_load(self) -> None:
        self.logger.info("on_load is called!")

    def on_enable(self) -> None:
        self.logger.info("on_enable is called!")
        self.run_tests()

    def on_command(
        self, sender: CommandSender, command: Command, args: list[str]
    ) -> bool:
        if command.name == "test" and isinstance(sender, Player):
            self.run_tests(player=sender)
            return True
        return False

    def on_disable(self) -> None:
        self.logger.info("on_disable is called!")

    def run_tests(self, **kwargs):
        marker = "player" if "player" in kwargs else "not player"
        set_runtime_context(server=self.server, plugin=self, **kwargs)
        try:
            return pytest.main(["-s", "-m", marker, "--pyargs", "endstone_test.tests"])
        finally:
            clear_runtime_context()
