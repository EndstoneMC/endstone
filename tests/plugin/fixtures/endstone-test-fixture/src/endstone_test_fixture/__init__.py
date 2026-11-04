from endstone.command import Command, CommandSender
from endstone.event import ServerListPingEvent, event_handler
from endstone.plugin import Plugin


class TestFixture(Plugin):
    api_version = "0.12"
    load = "startup"

    commands = {
        "fixturecmd": {
            "description": "A command registered by a plugin enabled at STARTUP",
            "usages": ["/fixturecmd <actor: entity_type>", "/fixturecmd place <block: block>"],
            "permissions": ["test_fixture.command.fixturecmd"],
        },
        "status": {
            "description": "A command named like one of Endstone's own, which must stay Endstone's",
            "usages": ["/status"],
        },
    }

    permissions = {
        "test_fixture.command.fixturecmd": {
            "description": "Allow users to use the /fixturecmd command.",
            "default": "op",
        },
    }

    def __init__(self):
        super().__init__()
        self.enabled_times = 0
        self.disabled_times = 0
        self.received: list[list[str]] = []
        self.startup_commands: dict | None = None
        self.pings = 0
        self.load_clear: str | None = None
        self.load_reload: str | None = None
        self.addon_report: dict | None = None

    def on_load(self) -> None:
        try:
            self.server.command_map.clear_commands()
            self.load_clear = "ok"
        except Exception as e:
            self.load_clear = repr(e)
        try:
            self.server.level
        except RuntimeError:
            try:
                self.server.reload()
                self.load_reload = "returned"
            except Exception as e:
                self.load_reload = repr(e)

    def on_enable(self) -> None:
        self.enabled_times += 1
        self.register_events(self)
        if self.startup_commands is None:
            self.startup_commands = self._use_commands()
        if self.addon_report is None:
            self.addon_report = self._enable_addon()

    def on_disable(self) -> None:
        self.disabled_times += 1

    @event_handler
    def on_server_list_ping(self, event: ServerListPingEvent) -> None:
        self.pings += 1

    def on_command(self, sender: CommandSender, command: Command, args: list[str]) -> bool:
        self.received.append(list(args))
        return True

    def _use_commands(self) -> dict:
        report = {}
        try:
            report["get_command"] = self.get_command("fixturecmd") is not None
            report["dispatch"] = self.server.dispatch_command(self.server.command_sender, "fixturecmd zombie")
            report["received"] = [list(args) for args in self.received]
        except Exception as e:
            report["error"] = repr(e)
        return report

    def _enable_addon(self) -> dict:
        report = {}
        try:
            manager = self.server.plugin_manager
            addon = manager.get_plugin("test_addon")
            report["loaded"] = addon is not None
            manager.enable_plugin(addon)
            report["enabled"] = addon.is_enabled
            report["get_command"] = self.server.get_plugin_command("addoncmd") is not None
            report["dispatch"] = self.server.dispatch_command(self.server.command_sender, "addoncmd early")
            report["received"] = [list(args) for args in addon.received]
        except Exception as e:
            report["error"] = repr(e)
        return report


__all__ = ["TestFixture"]
