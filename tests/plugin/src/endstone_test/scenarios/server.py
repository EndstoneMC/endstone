import json
import os
import sys
import threading

from endstone.command import Command
from endstone.event import (
    BroadcastMessageEvent,
    MapInitializeEvent,
    PluginEnableEvent,
    ScriptMessageEvent,
    ServerCommandEvent,
    ServerListPingEvent,
    ServerLoadEvent,
    UnknownCommandEvent,
)

from endstone_test.harness import Context, scenario
from endstone_test.plugin import HARNESS_STARTS

UNKNOWN = "endstone_test_no_such_command"
NETHERNET = os.environ.get("ENDSTONE_TEST_TRANSPORT") == "nethernet"


@scenario("command.unknown.dispatch")
async def unknown_dispatch(ctx: Context) -> None:
    unknown = ctx.capture(UnknownCommandEvent)
    result = ctx.command(f"{UNKNOWN} first second")
    assert not result.success
    assert unknown.count == 0, unknown.events
    assert result.errors == [] and result.messages == [], result.output


@scenario("command.unknown.console", covers={UnknownCommandEvent: "normal"})
async def unknown_console(ctx: Context) -> None:
    unknown = ctx.capture(
        UnknownCommandEvent, where=lambda e: e.command_line.startswith(UNKNOWN)
    )
    typed = ctx.capture(ServerCommandEvent)
    await ctx.console(f"{UNKNOWN} from the console")
    await ctx.until(lambda: unknown.count, what="UnknownCommandEvent")
    assert unknown.count == 1, unknown.events
    assert [e["command"] for e in typed.events] == [f"/{UNKNOWN} from the console"], (
        typed.events
    )
    assert unknown.events[0]["command_line"] == f"{UNKNOWN} from the console"
    assert unknown.events[0]["sender"]["class"] == "ConsoleCommandSender"


@scenario("command.unknown.mutate", covers={UnknownCommandEvent: "mutate"})
async def unknown_mutate(ctx: Context) -> None:
    unknown = ctx.capture(
        UnknownCommandEvent,
        where=lambda e: e.command_line.startswith(UNKNOWN),
        mutate=lambda e: setattr(e, "message", "Rewritten by endstone-test."),
    )
    await ctx.console(UNKNOWN)
    await ctx.until(lambda: unknown.count, what="UnknownCommandEvent")
    assert unknown.events[0]["after"]["message"] == "Rewritten by endstone-test."


@scenario("command.unknown.suppress", covers={UnknownCommandEvent: "mutate"})
async def unknown_suppress(ctx: Context) -> None:
    unknown = ctx.capture(
        UnknownCommandEvent,
        where=lambda e: e.command_line.startswith(UNKNOWN),
        mutate=lambda e: setattr(e, "message", None),
    )
    await ctx.console(UNKNOWN)
    await ctx.until(lambda: unknown.count, what="UnknownCommandEvent")
    assert unknown.events[0]["after"]["message"] is None


@scenario("command.console.normal", covers={ServerCommandEvent: "normal"}, plot=True)
async def console_normal(ctx: Context) -> None:
    at = ctx.plot.at(*ctx.plot.middle)
    typed = ctx.capture(ServerCommandEvent)
    await ctx.console(f"setblock {at} gold_block")
    await ctx.until(lambda: typed.count, what="ServerCommandEvent")
    await ctx.until(
        lambda: ctx.block_is(at, "gold_block"), timeout=20, what="the gold block"
    )
    assert typed.count == 1, typed.events
    assert typed.events[0]["command"] == f"/setblock {at} gold_block", typed.events
    assert typed.events[0]["sender"]["class"] == "ConsoleCommandSender"


@scenario("command.console.cancel", covers={ServerCommandEvent: "cancel"}, plot=True)
async def console_cancel(ctx: Context) -> None:
    at = ctx.plot.at(*ctx.plot.middle)
    typed = ctx.capture(ServerCommandEvent, cancel=True)
    await ctx.console(f"setblock {at} gold_block")
    await ctx.until(lambda: typed.count, what="ServerCommandEvent")
    await ctx.ticks(20)
    assert ctx.block_is(at, "air"), "a cancelled console command still ran"


@scenario("command.console.mutate", covers={ServerCommandEvent: "mutate"}, plot=True)
async def console_mutate(ctx: Context) -> None:
    at = ctx.plot.at(*ctx.plot.middle)
    typed = ctx.capture(
        ServerCommandEvent,
        mutate=lambda e: setattr(e, "command", f"setblock {at} diamond_block"),
    )
    await ctx.console(f"setblock {at} gold_block")
    await ctx.until(lambda: typed.count, what="ServerCommandEvent")
    await ctx.until(
        lambda: ctx.block_is(at, "diamond_block"),
        timeout=20,
        what="the rewritten command's block",
    )
    assert not ctx.block_is(at, "gold_block")


def delivered(lines: list[str], text: str) -> bool:
    return any(text in line for line in lines)


@scenario("command.register.failed_usage_enum")
async def failed_usage_enum(ctx: Context) -> None:
    command_map = ctx.server.command_map
    results = {}
    failing = Command(
        "endstone_test_enum_fail",
        usages=["/endstone_test_enum_fail (a|b)<pick: EndstoneTestReusedEnum> <count: no_such_type>"],
    )
    await ctx.log_of(lambda: results.__setitem__("failing", command_map.register_command(failing)))
    assert results["failing"] is False, "a command with no valid usage registered"
    reusing = Command(
        "endstone_test_enum_reuse",
        usages=["/endstone_test_enum_reuse (c|d)<pick: EndstoneTestReusedEnum>"],
    )
    lines = await ctx.log_of(lambda: results.__setitem__("reusing", command_map.register_command(reusing)))
    assert results["reusing"] is True
    assert not any("EndstoneTestReusedEnum" in line and "already exists" in line for line in lines), lines


@scenario("broadcast.normal", covers={BroadcastMessageEvent: "normal"})
async def broadcast_normal(ctx: Context) -> None:
    broadcast = ctx.capture(BroadcastMessageEvent)
    lines = await ctx.log_of(
        lambda: ctx.server.broadcast_message("endstone-test broadcast normal")
    )
    assert broadcast.count == 1
    event = broadcast.events[0]
    assert event["message"] == "endstone-test broadcast normal"
    assert event["is_asynchronous"] is False
    assert {
        "sender": ctx.server.command_sender.name,
        "class": "ConsoleCommandSender",
    } in event["recipients"]
    assert delivered(lines, "endstone-test broadcast normal"), lines


@scenario("broadcast.cancel", covers={BroadcastMessageEvent: "cancel"})
async def broadcast_cancel(ctx: Context) -> None:
    broadcast = ctx.capture(BroadcastMessageEvent, cancel=True)
    lines = await ctx.log_of(
        lambda: ctx.server.broadcast_message("endstone-test broadcast cancel")
    )
    assert broadcast.count == 1
    assert not delivered(lines, "endstone-test broadcast cancel"), lines


@scenario("broadcast.mutate", covers={BroadcastMessageEvent: "mutate"})
async def broadcast_mutate(ctx: Context) -> None:
    broadcast = ctx.capture(
        BroadcastMessageEvent,
        mutate=lambda e: setattr(e, "message", "endstone-test rewritten"),
    )
    lines = await ctx.log_of(
        lambda: ctx.server.broadcast_message("endstone-test broadcast mutate")
    )
    assert broadcast.count == 1
    assert delivered(lines, "endstone-test rewritten"), lines
    assert not delivered(lines, "endstone-test broadcast mutate"), lines


@scenario("broadcast.async", covers={BroadcastMessageEvent: "normal"})
async def broadcast_async(ctx: Context) -> None:
    broadcast = ctx.capture(BroadcastMessageEvent)
    worker = threading.Thread(
        target=ctx.server.broadcast_message, args=("endstone-test broadcast async",)
    )
    worker.start()
    await ctx.until(
        lambda: not worker.is_alive(), timeout=100, what="the broadcasting thread"
    )
    assert broadcast.count == 1
    assert broadcast.events[0]["is_asynchronous"] is True


@scenario(
    "server.startup", covers={ServerLoadEvent: "normal", PluginEnableEvent: "normal"}
)
async def startup(ctx: Context) -> None:
    loads = ctx.driver.hub.startup["ServerLoadEvent"].events
    expected = "STARTUP" if getattr(sys, HARNESS_STARTS) == 1 else "RELOAD"
    assert [e["type"] for e in loads] == [expected], loads
    enabled = [
        e["plugin"]["plugin"]
        for e in ctx.driver.hub.startup["PluginEnableEvent"].events
    ]
    assert ctx.plugin.name in enabled, enabled
    assert len(enabled) == len(set(enabled)), enabled
    assert set(enabled) <= {p.name for p in ctx.server.plugin_manager.plugins}, enabled


async def script_setblock(ctx: Context, message) -> str:
    at = ctx.plot.at(*ctx.plot.middle)
    result = ctx.command(f"scriptevent endstone_test:setblock {at} gold_block")
    ctx.info["output"] = result.output
    await ctx.until(lambda: message.count, timeout=20, what="ScriptMessageEvent")
    return at


@scenario(
    "server.script_message.normal", covers={ScriptMessageEvent: "normal"}, plot=True
)
async def script_message_normal(ctx: Context) -> None:
    message = ctx.capture(ScriptMessageEvent)
    at = await script_setblock(ctx, message)
    await ctx.until(
        lambda: ctx.block_is(at, "gold_block"),
        timeout=20,
        what="the behavior pack's script to set the block",
    )
    assert message.count == 1, message.events
    event = message.events[0]
    assert event["message_id"] == "endstone_test:setblock"
    assert event["message"] == f"{at} gold_block"
    assert event["sender"]["class"] == "ConsoleCommandSender", event["sender"]


@scenario(
    "server.script_message.cancel", covers={ScriptMessageEvent: "cancel"}, plot=True
)
async def script_message_cancel(ctx: Context) -> None:
    message = ctx.capture(ScriptMessageEvent, cancel=True)
    at = await script_setblock(ctx, message)
    await ctx.ticks(20)
    assert ctx.block_is(at, "air"), (
        "a script received a ScriptMessageEvent that was cancelled"
    )


@scenario("server.map_initialize", covers={MapInitializeEvent: "normal"})
async def map_initialize(ctx: Context) -> None:
    initialized = ctx.capture(MapInitializeEvent)
    view = ctx.server.create_map(ctx.overworld)
    await ctx.until(lambda: initialized.count, timeout=20, what="MapInitializeEvent")
    await ctx.ticks(5)
    ids = [e["map"]["id"] for e in initialized.events]
    ctx.info["map"] = {"returned": view.id, "initialized": ids}
    assert ids.count(view.id) == 1, ctx.info
    assert all(ctx.server.get_map(map_id) is not None for map_id in ids), ctx.info


def advertisement(text: str | None) -> list[str]:
    assert text is not None, "the server did not answer the ping"
    return text.split(";")


@scenario("server.list_ping.normal", covers={ServerListPingEvent: "normal"})
async def list_ping_normal(ctx: Context) -> None:
    ping = ctx.capture(ServerListPingEvent)
    parts = advertisement(await ctx.call("ping"))
    assert ping.count == 1, ping.events
    event = ping.events[0]
    ctx.info["advertisement"] = parts
    assert parts[0] == "MCPE"
    assert parts[1] == event["motd"] and parts[7] == event["level_name"]
    assert (
        int(parts[2])
        == event["network_protocol_version"]
        == ctx.server.protocol_version
    )
    assert int(parts[4]) == event["num_players"] == len(ctx.server.online_players)
    assert int(parts[5]) == event["max_players"] == ctx.server.max_players
    assert parts[8] == event["game_mode"].capitalize(), (parts[8], event["game_mode"])


@scenario("server.list_ping.cancel", covers={ServerListPingEvent: "cancel"})
async def list_ping_cancel(ctx: Context) -> None:
    ping = ctx.capture(ServerListPingEvent, cancel=True)
    reply = await ctx.call("ping")
    assert ping.count == 1, ping.events
    assert reply is None, f"a cancelled ping was still answered: {reply}"


@scenario("server.list_ping.mutate", covers={ServerListPingEvent: "mutate"})
async def list_ping_mutate(ctx: Context) -> None:
    def rewrite(event) -> None:
        event.motd = "endstone-test motd"
        event.level_name = "endstone-test level"
        event.num_players = 7
        event.max_players = 42
        event.minecraft_version_network = "9.9.9"

    ping = ctx.capture(ServerListPingEvent, mutate=rewrite)
    parts = advertisement(await ctx.call("ping"))
    ctx.info["advertisement"] = parts
    assert ping.count == 1
    assert parts[1] == "endstone-test motd" and parts[7] == "endstone-test level", parts
    assert (parts[3], parts[4], parts[5]) == ("9.9.9", "7", "42"), parts


def signaling_host(event) -> dict:
    return {"host": event.address.hostname}


@scenario("server.signaling_ping.normal", enabled=NETHERNET)
async def signaling_ping_normal(ctx: Context) -> None:
    ping = ctx.capture(ServerListPingEvent, extra=signaling_host)
    reply = await ctx.call("signaling_ping")
    ctx.info["reply"] = reply
    assert reply is not None and reply["status"] == 200, reply
    metadata = json.loads(reply["body"])
    assert ping.count == 1, ping.events
    event = ping.events[0]
    assert event["host"] == "127.0.0.1", event
    assert metadata["name"] == event["motd"] and metadata["level"] == event["level_name"], metadata
    assert metadata["protocol"] == event["network_protocol_version"] == ctx.server.protocol_version
    assert metadata["players"] == event["num_players"] == len(ctx.server.online_players)
    assert metadata["maxPlayers"] == event["max_players"] == ctx.server.max_players


@scenario("server.signaling_ping.cancel", enabled=NETHERNET)
async def signaling_ping_cancel(ctx: Context) -> None:
    ping = ctx.capture(ServerListPingEvent, cancel=True)
    reply = await ctx.call("signaling_ping")
    assert ping.count == 1, ping.events
    assert reply is None, f"a cancelled signaling ping was still answered: {reply}"


@scenario("server.signaling_ping.mutate", enabled=NETHERNET)
async def signaling_ping_mutate(ctx: Context) -> None:
    def rewrite(event) -> None:
        event.motd = "endstone-test motd"
        event.level_name = "endstone-test level"
        event.num_players = 7
        event.max_players = 42
        event.minecraft_version_network = "9.9.9"

    ping = ctx.capture(ServerListPingEvent, mutate=rewrite)
    reply = await ctx.call("signaling_ping")
    assert ping.count == 1, ping.events
    assert reply is not None and reply["status"] == 200, reply
    metadata = json.loads(reply["body"])
    ctx.info["metadata"] = metadata
    assert (metadata["name"], metadata["level"]) == ("endstone-test motd", "endstone-test level"), metadata
    assert (metadata["version"], metadata["players"], metadata["maxPlayers"]) == ("9.9.9", 7, 42), metadata
