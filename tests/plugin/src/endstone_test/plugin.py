import gc

import pytest
from endstone import Player
from endstone.command import Command, CommandSender
from endstone.plugin import Plugin

from endstone_test.command_executor import TestCommandExecutor
from endstone_test.listeners import (
    ActorEventListener,
    BlockEventListener,
    LevelEventListener,
    PlayerEventListener,
    ServerEventListener,
    WeatherEventListener,
)
from endstone_test.recorder import EventRecorder

from .tests.conftest import clear_runtime_context, set_runtime_context

MARKER_DEFAULT = "not player and not event"

LISTENERS = (
    ActorEventListener,
    BlockEventListener,
    LevelEventListener,
    PlayerEventListener,
    ServerEventListener,
    WeatherEventListener,
)


class EndstoneTest(Plugin):
    prefix = "Test"
    api_version = "0.12"
    commands = {
        "test": {
            "description": "Run the tests",
            "usages": [
                "/test",
                "/test events",
                "/test sender",
                "/test broadcast",
                "/test map",
                "/test form (message|action|modal)<form_type: FormType>",
                "/test player (toast|title|actionbar|kick|particle|sound)<player_test: PlayerTest>",
                "/test inv (mainhand|offhand|meta)<inv_test: InvTest>",
                "/test spawn <actor: entity_type>",
                "/test block <block: block> [states: block_states]",
            ],
            "permissions": ["endstone_test.command.test"],
        },
        "testoptmsg": {
            "description": "Test command with optional message argument",
            "usages": ["/testoptmsg [msg: message]"],
            "permissions": ["endstone_test.command.testoptmsg"],
        },
    }

    permissions = {
        "endstone_test.command.test": {
            "description": "Allow users to use the /test command.",
            "default": "op",
        },
        "endstone_test.command.testoptmsg": {
            "description": "Allow users to use the /testoptmsg command.",
            "default": "op",
        },
        "endstone_test.granted": {
            "description": "Never granted by default, only via a permission attachment.",
            "default": "false",
        },
    }

    def __init__(self):
        super().__init__()
        self.recorder = EventRecorder(self)

    def on_load(self) -> None:
        self.logger.info("on_load is called!")

    def on_enable(self) -> None:
        self.logger.info("on_enable is called!")
        self.logger.info(f"protocol version: {self.server.protocol_version}")

        self.recorder.create_boss_bar()
        for listener in LISTENERS:
            self.register_events(listener(self))

        self.get_command("test").executor = TestCommandExecutor(self)
        self.run_tests()

    def on_disable(self) -> None:
        self.logger.info("on_disable is called!")
        self.recorder.remove_boss_bar()

    def on_command(
        self, sender: CommandSender, command: Command, args: list[str]
    ) -> bool:
        if command.name == "testoptmsg":
            self.last_testoptmsg_args = list(args)
            return True
        return False

    def run_tests(self, player: Player | None = None) -> int:
        if player is None:
            return self._pytest(MARKER_DEFAULT)
        return self._pytest("player", player=player)

    def run_event_tests(self, player: Player | None = None) -> int:
        context = {} if player is None else {"player": player}
        return self._pytest("event", **context)

    def _pytest(self, marker: str, **context) -> int:
        set_runtime_context(
            server=self.server, plugin=self, recorder=self.recorder, **context
        )
        try:
            return pytest.main(["-s", "-m", marker, "--pyargs", "endstone_test.tests"])
        finally:
            clear_runtime_context()
            gc.collect()
