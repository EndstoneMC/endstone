import os
import sys

from endstone import Player
from endstone.boss import BarColor, BarStyle, BossBar
from endstone.command import Command, CommandSender
from endstone.event import PlayerJoinEvent, event_handler
from endstone.plugin import Plugin

from endstone_test.command_executor import TestCommandExecutor
from endstone_test.harness import api_recorder
from endstone_test.harness.pytest_pass import run_pytest

CONTROL_ENV = "ENDSTONE_TEST_CONTROL"
RUN_ID_ENV = "ENDSTONE_TEST_RUN_ID"
HARNESS_STARTS = "_endstone_test_harness_starts"

api_recorder.install_from_env()


class BossBarListener:
    def __init__(self, boss_bars: list[BossBar]):
        self.boss_bars = boss_bars

    @event_handler
    def on_player_join(self, event: PlayerJoinEvent) -> None:
        for boss_bar in self.boss_bars:
            boss_bar.add_player(event.player)


class EndstoneTest(Plugin):
    prefix = "Test"
    api_version = "0.12"
    commands = {
        "test": {
            "description": "Run the tests",
            "usages": [
                "/test",
                "/test sender",
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
        self.extra_boss_bars: list[BossBar] = []
        self.boss_bar_ticks = 0
        self.driver = None

    def on_load(self) -> None:
        self.logger.info("on_load is called!")

    def on_enable(self) -> None:
        self.logger.info("on_enable is called!")
        self.logger.info(f"protocol version: {self.server.protocol_version}")

        self.extra_boss_bars = [
            self.server.create_boss_bar("Boss bar A", BarColor.RED, BarStyle.SOLID),
            self.server.create_boss_bar(
                "Boss bar B", BarColor.BLUE, BarStyle.SEGMENTED_6
            ),
        ]
        self.server.scheduler.run_task(self, self.update_boss_bars, delay=20, period=20)
        self.register_events(BossBarListener(self.extra_boss_bars))
        self.get_command("test").executor = TestCommandExecutor(self)

        address = os.environ.get(CONTROL_ENV)
        if address:
            self.start_harness(address)
        else:
            self.run_tests()

    def on_disable(self) -> None:
        self.logger.info("on_disable is called!")
        for boss_bar in self.extra_boss_bars:
            boss_bar.remove_all()

    def start_harness(self, address: str) -> None:
        setattr(sys, HARNESS_STARTS, getattr(sys, HARNESS_STARTS, 0) + 1)
        import endstone_test.scenarios  # noqa: F401
        from endstone_test.harness.api_manifest import manifest as api_manifest
        from endstone_test.harness.capture import Hub, event_classes
        from endstone_test.harness.engine import REGISTRY, Driver
        from endstone_test.harness.manifest import ABSTRACT, coverage

        hub = Hub(self)
        hub.register([cls for cls in event_classes() if cls.__name__ not in ABSTRACT])
        recorder = api_recorder.active_recorder()
        control = api_recorder.RecordingControl(address, recorder)
        control.send(
            {
                "type": "hello",
                "run_id": os.environ.get(RUN_ID_ENV),
                "scenarios": [s.name for s in REGISTRY.values() if s.enabled],
                "coverage": coverage([s for s in REGISTRY.values() if s.enabled]),
                "api": api_manifest(recorder) if recorder is not None else None,
            }
        )
        self.driver = Driver(self, control, hub)

    def update_boss_bars(self) -> None:
        self.boss_bar_ticks += 1
        colors = list(BarColor)
        for i, boss_bar in enumerate(self.extra_boss_bars):
            boss_bar.progress = ((self.boss_bar_ticks + i * 5) % 11) / 10
            boss_bar.title = f"Boss bar {'AB'[i]}: {boss_bar.progress:.0%}"

        bar_a, bar_b = self.extra_boss_bars
        if self.boss_bar_ticks % 5 == 0:
            bar_a.color = colors[(self.boss_bar_ticks // 5) % len(colors)]
        if self.boss_bar_ticks % 10 == 0:
            bar_b.is_visible = not bar_b.is_visible

    def on_command(
        self, sender: CommandSender, command: Command, args: list[str]
    ) -> bool:
        if command.name == "testoptmsg":
            self.last_testoptmsg_args = list(args)
            return True
        return False

    def run_tests(self, player: Player | None = None) -> int:
        if player is None:
            return run_pytest(self, "not player").exit_code
        return run_pytest(self, "player", player=player).exit_code
