import pytest
from endstone import ColorFormat
from endstone.boss import BarColor, BarStyle, BossBar
from endstone.event import Event
from endstone.plugin import Plugin, PluginLoadOrder

from endstone_test.command_executor import TestCommandExecutor
from endstone_test.listeners import (
    ActorEventListener,
    BlockEventListener,
    LevelEventListener,
    PlayerEventListener,
    ServerEventListener,
    WeatherEventListener,
)
from endstone_test.test_helper import FixtureInjection


class EndstoneTest(Plugin):
    prefix = "Test"
    api_version = "0.11"
    load = PluginLoadOrder.POSTWORLD

    commands = {
        "test": {
            "description": "Run the test command",
            "usages": [
                "/test form <message|action|modal>",
                "/test sender",
                "/test player <toast|title|kick|particle|sound>",
                "/test block <block: block> [blockStates: block_states]",
                "/test broadcast",
                "/test inv <mainhand|offhand|meta>",
                "/test spawn <entity: entity_type>",
            ],
            "permissions": ["endstone_test.command.test"],
        }
    }

    permissions = {
        "endstone_test.command.test": {
            "description": "Allow users to use the /test command.",
            "default": "op",
        }
    }

    def __init__(self):
        super().__init__()
        self.tracked_events: dict[str, int] = {}
        self.boss_bar: BossBar | None = None

    def on_load(self) -> None:
        self.logger.info("on_load is called!")
        self.save_resources("lena.png", replace=True)

    def on_enable(self) -> None:
        self.logger.info("on_enable is called!")
        self.logger.info(f"protocol version: {self.server.protocol_version}")
        self.boss_bar = self.server.create_boss_bar(
            "", BarColor.GREEN, BarStyle.SEGMENTED_10
        )
        self.register_events(ActorEventListener(self))
        self.register_events(BlockEventListener(self))
        self.register_events(LevelEventListener(self))
        self.register_events(PlayerEventListener(self))
        self.register_events(ServerEventListener(self))
        self.register_events(WeatherEventListener(self))
        self.register_events(self)
        self.run_tests("on_enable")
        self.get_command("test").executor = TestCommandExecutor(self)

    def on_disable(self) -> None:
        self.logger.info("on_disable is called!")
        self.boss_bar.remove_all()

    def run_tests(self, name: str, **kwargs):
        return pytest.main(
            ["-s", "--pyargs", f"endstone_test.tests.{name}"],
            plugins=[FixtureInjection(server=self.server, plugin=self, **kwargs)],
        )

    def track_event(self, name: str):
        self.tracked_events.setdefault(name, 0)
        self.boss_bar.title = f"Events: 0/{len(self.tracked_events)}"
        self.boss_bar.progress = 0

    def on_event_triggered(self, event: Event, message: str, always_log: bool = False):
        event_name = event.__class__.__name__
        if self.tracked_events[event_name] == 0 or always_log:
            self.logger.info(
                ColorFormat.GREEN
                + f"Event {event_name} triggered! "
                + ColorFormat.RESET
                + message
            )

        self.tracked_events[event_name] += 1
        triggered_count = 0
        next_to_trigger = None
        for name, count in self.tracked_events.items():
            if count == 0:
                next_to_trigger = name
            else:
                triggered_count += 1

        self.boss_bar.progress = triggered_count / len(self.tracked_events)
        if next_to_trigger:
            self.boss_bar.title = f"Events: {triggered_count}/{len(self.tracked_events)}, Next: {next_to_trigger}"
        else:
            self.boss_bar.title = "All events triggered!"
