from endstone.event import PluginDisableEvent, PluginEnableEvent

from endstone_test.harness import Context, scenario

FIXTURE = "test_fixture"
ADDON = "test_addon"


def fixture_events(ctx: Context, event):
    return ctx.capture(event, lambda e: e.plugin.name == FIXTURE)


@scenario(
    "plugin.disable_enable",
    covers={PluginDisableEvent: "normal", PluginEnableEvent: "normal"},
)
async def disable_enable(ctx: Context) -> None:
    manager = ctx.server.plugin_manager
    fixture = manager.get_plugin(FIXTURE)
    assert fixture is not None, f"the {FIXTURE} plugin is not loaded"
    assert fixture.is_enabled
    disabled_before = fixture.disabled_times
    disable = fixture_events(ctx, PluginDisableEvent)
    enable = fixture_events(ctx, PluginEnableEvent)

    manager.disable_plugin(fixture)
    assert disable.count == 1, disable.events
    assert disable.events[0]["plugin"] == {"plugin": FIXTURE}
    assert not fixture.is_enabled
    assert fixture.disabled_times == disabled_before + 1, "on_disable did not run"
    assert manager.is_plugin_enabled(FIXTURE) is False

    manager.disable_plugin(fixture)
    assert disable.count == 1, (
        "disabling a disabled plugin fired PluginDisableEvent again"
    )

    manager.enable_plugin(fixture)
    assert enable.count == 1, enable.events
    assert fixture.is_enabled
    assert manager.is_plugin_enabled(FIXTURE) is True


@scenario("plugin.startup_commands")
async def startup_commands(ctx: Context) -> None:
    fixture = ctx.server.plugin_manager.get_plugin(FIXTURE)
    assert fixture is not None, f"the {FIXTURE} plugin is not loaded"
    assert fixture.load_clear == "ok", f"CommandMap.clear_commands() in on_load: {fixture.load_clear}"
    report = fixture.startup_commands
    assert report == {"get_command": True, "dispatch": True, "received": [["zombie"]]}, report

    command = ctx.server.get_plugin_command("fixturecmd")
    assert command is not None and command.plugin.name == FIXTURE, command

    before = len(fixture.received)
    assert ctx.command("fixturecmd zombie").success
    assert ctx.command("fixturecmd place stone").success
    received = fixture.received[before:]
    assert len(received) == 2, received
    assert received[0][-1].endswith("zombie"), received
    assert received[1][0] == "place" and received[1][-1].endswith("stone"), received

    rejected = ctx.command("fixturecmd notanentity")
    assert not rejected.success and rejected.errors, rejected.output
    assert len(fixture.received) == before + 2, fixture.received[before:]

    assert ctx.server.get_plugin_command("status") is None, "a STARTUP plugin took the name of Endstone's /status"
    status = ctx.command("status")
    assert status.success, status.output
    assert len(fixture.received) == before + 2, f"/status reached the fixture plugin: {fixture.received[before:]}"
    prefixed = ctx.server.get_plugin_command(f"{FIXTURE}:status")
    assert prefixed is not None and prefixed.plugin.name == FIXTURE, prefixed
    assert ctx.command(f"{FIXTURE}:status").success
    assert len(fixture.received) == before + 3, fixture.received[before:]


@scenario("plugin.command_fallback_prefix")
async def command_fallback_prefix(ctx: Context) -> None:
    manager = ctx.server.plugin_manager
    plugins = {name: manager.get_plugin(name) for name in (FIXTURE, ADDON)}
    assert all(plugins.values()), plugins

    plain = ctx.server.get_plugin_command("sharedcmd")
    assert plain is not None, "neither plugin got the plain name"
    winner = plain.plugin.name
    assert winner in plugins, winner
    loser = ADDON if winner == FIXTURE else FIXTURE
    assert plain.name == "sharedcmd"

    for name in plugins:
        prefixed = ctx.server.get_plugin_command(f"{name}:sharedcmd")
        assert prefixed is not None and prefixed.plugin.name == name, (name, prefixed)
    assert (
        ctx.server.get_plugin_command(f"{loser}:sharedcmd").name == f"{loser}:sharedcmd"
    )

    for name, plugin in plugins.items():
        before = len(plugin.received)
        assert ctx.command(f"{name}:sharedcmd hello").success
        assert (
            len(plugin.received) == before + 1 and plugin.received[-1][-1] == "hello"
        ), plugin.received[before:]

    before = len(plugins[winner].received)
    assert ctx.command("sharedcmd plain").success
    assert (
        len(plugins[winner].received) == before + 1
        and plugins[winner].received[-1][-1] == "plain"
    )


@scenario("plugin.early_enable_commands")
async def early_enable_commands(ctx: Context) -> None:
    manager = ctx.server.plugin_manager
    fixture = manager.get_plugin(FIXTURE)
    assert fixture is not None, f"the {FIXTURE} plugin is not loaded"
    report = fixture.addon_report
    assert report == {
        "loaded": True,
        "enabled": True,
        "get_command": True,
        "dispatch": True,
        "received": [["early"]],
    }, report

    addon = manager.get_plugin(ADDON)
    command = ctx.server.get_plugin_command("addoncmd")
    assert command is not None and command.plugin.name == ADDON, command
    before = len(addon.received)
    assert ctx.command("addoncmd later").success
    assert len(addon.received) == before + 1 and addon.received[-1][-1] == "later", addon.received[before:]


@scenario("plugin.reload_before_level")
async def reload_before_level(ctx: Context) -> None:
    manager = ctx.server.plugin_manager
    fixture = manager.get_plugin(FIXTURE)
    assert fixture is not None, f"the {FIXTURE} plugin is not loaded"
    assert fixture.load_reload == "RuntimeError('Level has not been loaded yet.')", fixture.load_reload
    for name in (FIXTURE, ADDON, ctx.plugin.name):
        assert manager.is_plugin_enabled(name), f"{name} is not enabled after a reload() in on_load"
