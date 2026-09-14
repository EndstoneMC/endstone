import gc
import os
from collections.abc import Iterable

import pytest
from endstone import ColorFormat, Player
from endstone.command import Command, CommandSender
from endstone.inventory import ItemStack
from endstone.plugin import Plugin

from endstone_test.automation import AutomatedEventRunner
from endstone_test.checks import CHECKS, WAND_ITEMS, WAND_PREFIX
from endstone_test.command_executor import TestCommandExecutor
from endstone_test.listeners import (
    ActorEventListener,
    BlockEventListener,
    CommandEventListener,
    InventoryEventListener,
    LevelEventListener,
    PlayerEventListener,
    ServerEventListener,
    WeatherEventListener,
)
from endstone_test.recorder import EventRecorder
from endstone_test.reporting import logging_to

from .tests.conftest import clear_runtime_context, set_runtime_context

MARKER_DEFAULT = "not player and not event"

LISTENERS = (
    ActorEventListener,
    BlockEventListener,
    CommandEventListener,
    InventoryEventListener,
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
                "/test autoevents",
                "/test sender",
                "/test broadcast",
                "/test map",
                "/test form (message|action|modal)<form_type: FormType>",
                "/test player (toast|title|actionbar|kick|particle|sound)<player_test: PlayerTest>",
                "/test inv (mainhand|offhand|meta)<inv_test: InvTest>",
                "/test spawn <actor: entity_type>",
                "/test block <block: block> [states: block_states]",
                "/test chunk [x: int] [z: int] [radius: int]",
                "/test chunkapi [x: int] [z: int]",
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
        self.announced: dict[str, str] = {}
        self.automated_event_runner: AutomatedEventRunner | None = None
        self._automated_cancel_events: set[str] = set()

    def on_load(self) -> None:
        self.logger.info("on_load is called!")

    def on_enable(self) -> None:
        self.logger.info("on_enable is called!")
        self.logger.info(f"protocol version: {self.server.protocol_version}")

        self.recorder.create_boss_bar()
        for listener in LISTENERS:
            self.register_events(listener(self))

        self.get_command("test").executor = TestCommandExecutor(self)
        if os.getenv("ENDSTONE_TEST_SKIP_PYTEST", "").strip().lower() not in {"1", "true"}:
            self.run_tests()
        else:
            self.logger.info("Skipping embedded pytest suite for targeted event scenarios.")
        if os.getenv("ENDSTONE_TEST_AUTO_EVENTS", "").strip().lower() in {"1", "true"}:
            self.start_automated_events()

    def on_disable(self) -> None:
        self.logger.info("on_disable is called!")
        self._automated_cancel_events.clear()
        if self.automated_event_runner is not None:
            self.automated_event_runner.cancel()
        self.recorder.remove_boss_bar()

    def set_automated_cancel_events(self, event_names: Iterable[str]) -> None:
        self._automated_cancel_events = set(event_names)

    def should_cancel_event(self, event_name: str) -> bool:
        return event_name in self._automated_cancel_events

    def start_automated_events(self) -> bool:
        if self.automated_event_runner is not None and self.automated_event_runner.running:
            self.logger.warning("Automated event runner is already running.")
            return False

        result_path = os.getenv(
            "ENDSTONE_TEST_RESULT_FILE", "endstone-event-test-result.json"
        )
        self.automated_event_runner = AutomatedEventRunner(
            self,
            self.recorder,
            result_path=result_path,
            stop_server=os.getenv("ENDSTONE_TEST_AUTO_EVENTS_STOP", "").strip().lower()
            in {"1", "true"},
        )
        return self.automated_event_runner.start()

    def handle_script_message(self, message_id: str, message: str) -> None:
        """Forward GameTest control messages to the active unified runner."""
        if self.automated_event_runner is not None:
            self.automated_event_runner.handle_gametest_message(message_id, message)

    def on_command(
        self, sender: CommandSender, command: Command, args: list[str]
    ) -> bool:
        if command.name == "testoptmsg":
            self.last_testoptmsg_args = list(args)
            return True
        return False

    def restock_all(self) -> None:
        for player in self.server.online_players:
            self.restock(player)

    def guide_all(self) -> None:
        for player in self.server.online_players:
            self.restock(player)
            self.guide(player)

    def guide(self, player: Player) -> None:
        pending = next((c for c in CHECKS if not self.recorder.checked(c.key)), None)
        if pending is None:
            player.send_action_bar(
                ColorFormat.GREEN + "All events and checks exercised!"
            )
            return

        done = len(CHECKS) - sum(1 for c in CHECKS if not self.recorder.checked(c.key))
        player.send_action_bar(
            f"{ColorFormat.YELLOW}[{done}/{len(CHECKS)}] "
            f"{ColorFormat.WHITE}{pending.hint}"
        )

        if self.announced.get(player.name) == pending.key:
            return
        self.announced[player.name] = pending.key
        player.send_message(
            f"{ColorFormat.GOLD}{ColorFormat.BOLD}{pending.key}{ColorFormat.RESET} "
            f"{ColorFormat.GRAY}-- {pending.hint}"
        )
        if pending.detail:
            player.send_message(f"{ColorFormat.GRAY}{pending.detail}")

    def restock(self, player: Player) -> None:
        index = next(
            (i for i, c in enumerate(CHECKS) if not self.recorder.checked(c.key)), None
        )
        if index is None:
            return

        pending = CHECKS[index]
        while not pending.items and not pending.wand and index:
            index -= 1
            pending = CHECKS[index]
        if not pending.items and not pending.wand:
            return

        for item_id, amount in pending.items:
            if not player.inventory.contains(item_id):
                item = ItemStack(item_id, amount)
                meta = item.item_meta
                meta.lore = [pending.key, pending.hint]
                item.set_item_meta(meta)
                player.inventory.add_item(item)

        if pending.wand is not None:
            name = f"{WAND_PREFIX}{pending.wand}"
            held = [
                slot
                for slot in player.inventory.contents
                if slot is not None and slot.item_meta.display_name == name
            ]
            if not held:
                item = ItemStack(WAND_ITEMS[pending.wand], 1)
                meta = item.item_meta
                meta.display_name = name
                meta.lore = [pending.key, pending.hint]
                item.set_item_meta(meta)
                player.inventory.add_item(item)

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
            with logging_to(self.logger):
                return pytest.main(
                    [
                        "-s",
                        "--color=yes",
                        "-m",
                        marker,
                        "--pyargs",
                        "endstone_test.tests",
                    ]
                )
        finally:
            clear_runtime_context()
            gc.collect()
