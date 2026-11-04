import itertools
import json
import uuid

from endstone import GameMode
from endstone.event import (
    PlayerChatEvent,
    PlayerCommandEvent,
    PlayerDeathEvent,
    PlayerDimensionChangeEvent,
    PlayerExpChangeEvent,
    PlayerGameModeChangeEvent,
    PlayerJoinEvent,
    PlayerKickEvent,
    PlayerLevelChangeEvent,
    PlayerLoginEvent,
    PlayerPickupExperienceEvent,
    PlayerQuitEvent,
    PlayerRespawnEvent,
    PlayerSetSpawnEvent,
    PlayerTeleportEvent,
    UnknownCommandEvent,
)
from endstone.level import Location

from endstone_test.harness import Context, scenario
from endstone_test.harness.engine import HarnessError

SECTION = "§"
JOINED = f"{SECTION}e%multiplayer.player.joined"
LEFT = f"{SECTION}e%multiplayer.player.left"
ORDER = itertools.count()


def parse(body: dict) -> tuple:
    kind = body["type"]
    if kind == "CHAT":
        return "chat", body["author"].removesuffix(f"{SECTION}r"), body["message"]
    if kind == "RAW":
        return "raw", body["message"]
    if kind == "TEXT_OBJECT":
        part = json.loads(body["message"])["rawtext"][0]
        params = [t["text"] for t in part.get("with", {}).get("rawtext", [])]
        return "obj", part["translate"], params
    return kind.lower(), body.get("message")


async def mark(bot) -> int:
    return await bot.call("get_next_packet_index")


async def heard(bot, since: int) -> list[tuple]:
    packets = await bot.call("get_received_packets", "TextPacket", since)
    return [parse(p["packet"]["body"]) for p in packets]


async def hear(ctx: Context, bot, since: int, predicate, what: str, polls: int = 80):
    got: list[tuple] = []
    for _ in range(polls):
        got = await heard(bot, since)
        if predicate(got):
            return got
        await ctx.ticks(1)
    raise AssertionError(f"{bot.name} never heard {what}; heard {got}")


async def barrier(ctx: Context, since: dict) -> dict:
    token = ("raw", f"@@barrier {uuid.uuid4().hex[:8]}")
    ctx.server.broadcast_message(token[1])
    result = {}
    for bot, start in since.items():
        got = await hear(
            ctx, bot, start, lambda g: token in g, f"the barrier {token[1]}"
        )
        result[bot.name] = [line for line in got if line != token]
    return result


async def join(ctx: Context, name: str, op: bool = False):
    bot = await ctx.bot(name)
    player = await ctx.until(
        lambda: ctx.server.get_player(name), timeout=60, what=f"{name} online"
    )
    if op:
        player.is_op = True
    return bot, player


def named(name: str):
    return lambda e: e.player.name == name


def order(event) -> dict:
    return {"order": next(ORDER)}


async def disconnect_reason(ctx: Context, bot) -> str:
    for _ in range(80):
        reason = await bot.call("get_disconnect_reason")
        if reason:
            return reason
        await ctx.ticks(1)
    raise AssertionError(f"{bot.name} was never disconnected")


async def wait_gone(ctx: Context, name: str) -> None:
    await ctx.until(
        lambda: ctx.server.get_player(name) is None, timeout=80, what=f"{name} offline"
    )


@scenario(
    "lifecycle.login.normal",
    covers={PlayerLoginEvent: "normal", PlayerJoinEvent: "normal"},
    budget=1500,
)
async def login_normal(ctx: Context) -> None:
    login = ctx.capture(PlayerLoginEvent, named("LcLoginN"), extra=order)
    joined = ctx.capture(PlayerJoinEvent, named("LcLoginN"), extra=order)
    observer, _ = await join(ctx, "LcLoginO")
    since = await mark(observer)
    bot, player = await join(ctx, "LcLoginN")
    assert login.count == 1 and joined.count == 1, (login.events, joined.events)
    assert login.events[0]["order"] < joined.events[0]["order"]
    assert login.events[0]["kick_message"] == ""
    assert joined.events[0]["join_message"] == {
        "translate": JOINED,
        "params": ["LcLoginN"],
    }
    assert player.name == "LcLoginN" and await bot.call("is_connected")
    await hear(
        ctx,
        observer,
        since,
        lambda g: ("obj", JOINED, ["LcLoginN"]) in g,
        "the join message",
    )


@scenario("lifecycle.login.cancel", covers={PlayerLoginEvent: "cancel"}, budget=1500)
async def login_cancel(ctx: Context) -> None:
    login = ctx.capture(PlayerLoginEvent, named("LcLoginC"), cancel=True)
    joined = ctx.capture(PlayerJoinEvent, named("LcLoginC"))
    kicked = ctx.capture(PlayerKickEvent, named("LcLoginC"))
    left = ctx.capture(PlayerQuitEvent, named("LcLoginC"))
    observer, _ = await join(ctx, "LcLoginCO")
    since = await mark(observer)
    try:
        await ctx.bot("LcLoginC")
    except HarnessError as e:
        failure = str(e)
    else:
        raise AssertionError("a bot whose login was cancelled finished joining")
    assert "join failed" in failure, failure
    ctx.info["client_saw"] = failure
    assert login.count == 1, login.events
    await ctx.ticks(10)
    assert ctx.server.get_player("LcLoginC") is None, "the cancelled player is online"
    assert joined.count == 0, joined.events
    assert kicked.count == 0, "a login refusal ran through PlayerKickEvent"
    after = await barrier(ctx, {observer: since})
    assert left.count == 0, f"PlayerQuitEvent fired for a player who never joined: {left.events}"
    assert after["LcLoginCO"] == [], f"the observer heard a quit message for a player who never joined: {after}"


@scenario("lifecycle.login.mutate", covers={PlayerLoginEvent: "mutate"}, budget=1500)
async def login_mutate(ctx: Context) -> None:
    ctx.capture(
        PlayerLoginEvent,
        named("LcLoginM"),
        cancel=True,
        mutate=lambda e: setattr(e, "kick_message", "QA closed for maintenance"),
    )
    try:
        await ctx.bot("LcLoginM")
    except HarnessError as e:
        failure = str(e)
    else:
        raise AssertionError("a bot whose login was cancelled finished joining")
    assert "QA closed for maintenance" in failure, failure


@scenario("lifecycle.join.mutate", covers={PlayerJoinEvent: "mutate"}, budget=1500)
async def join_mutate(ctx: Context) -> None:
    ctx.capture(
        PlayerJoinEvent,
        named("LcJoinM1"),
        mutate=lambda e: setattr(e, "join_message", "QA hello LcJoinM1"),
    )
    ctx.capture(
        PlayerJoinEvent,
        named("LcJoinM2"),
        mutate=lambda e: setattr(e, "join_message", None),
    )
    observer, _ = await join(ctx, "LcJoinMO")
    since = await mark(observer)
    first, _ = await join(ctx, "LcJoinM1")
    await hear(
        ctx,
        observer,
        since,
        lambda g: ("raw", "QA hello LcJoinM1") in g,
        "the custom join message",
    )
    own = await heard(first, 0)
    assert ("raw", "QA hello LcJoinM1") in own, own
    mid = await mark(observer)
    await join(ctx, "LcJoinM2")
    after = await barrier(ctx, {observer: mid})
    assert after["LcJoinMO"] == [], after
    everything = await heard(observer, since)
    assert ("obj", JOINED, ["LcJoinM1"]) not in everything, everything


@scenario("lifecycle.quit.normal", covers={PlayerQuitEvent: "normal"}, budget=1500)
async def quit_normal(ctx: Context) -> None:
    quit_ = ctx.capture(PlayerQuitEvent, named("LcQuitN"))
    observer, _ = await join(ctx, "LcQuitNO")
    await join(ctx, "LcQuitN")
    since = await mark(observer)
    await ctx.call("bot.close", bot="LcQuitN")
    await ctx.until(lambda: quit_.count, timeout=80, what="PlayerQuitEvent")
    await ctx.ticks(5)
    assert quit_.count == 1, quit_.events
    assert quit_.events[0]["quit_message"] == {"translate": LEFT, "params": ["LcQuitN"]}
    assert ctx.server.get_player("LcQuitN") is None
    await hear(
        ctx,
        observer,
        since,
        lambda g: ("obj", LEFT, ["LcQuitN"]) in g,
        "the quit message",
    )


@scenario("lifecycle.quit.mutate", covers={PlayerQuitEvent: "mutate"}, budget=1500)
async def quit_mutate(ctx: Context) -> None:
    ctx.capture(
        PlayerQuitEvent,
        named("LcQuitM1"),
        mutate=lambda e: setattr(e, "quit_message", "QA farewell LcQuitM1"),
    )
    ctx.capture(
        PlayerQuitEvent,
        named("LcQuitM2"),
        mutate=lambda e: setattr(e, "quit_message", None),
    )
    observer, _ = await join(ctx, "LcQuitMO")
    await join(ctx, "LcQuitM1")
    await join(ctx, "LcQuitM2")
    since = await mark(observer)
    await ctx.call("bot.close", bot="LcQuitM1")
    await hear(
        ctx,
        observer,
        since,
        lambda g: ("raw", "QA farewell LcQuitM1") in g,
        "the custom quit message",
    )
    mid = await mark(observer)
    await ctx.call("bot.close", bot="LcQuitM2")
    await wait_gone(ctx, "LcQuitM2")
    after = await barrier(ctx, {observer: mid})
    assert after["LcQuitMO"] == [], after
    everything = await heard(observer, since)
    assert ("obj", LEFT, ["LcQuitM1"]) not in everything, everything


@scenario("lifecycle.kick.normal", covers={PlayerKickEvent: "normal"}, budget=1500)
async def kick_normal(ctx: Context) -> None:
    api = ctx.capture(PlayerKickEvent, named("LcKickN1"))
    command = ctx.capture(PlayerKickEvent, named("LcKickN2"))
    await join(ctx, "LcKickNO")
    first, player = await join(ctx, "LcKickN1")
    second, _ = await join(ctx, "LcKickN2")
    player.kick("QA api kick")
    await ctx.until(lambda: api.count, timeout=40, what="PlayerKickEvent from the API")
    assert api.events[0]["reason"] == "QA api kick"
    assert "QA api kick" in await disconnect_reason(ctx, first)
    await wait_gone(ctx, "LcKickN1")
    ctx.run("kick LcKickN2 QA command kick")
    await ctx.until(
        lambda: command.count, timeout=40, what="PlayerKickEvent from /kick"
    )
    assert command.events[0]["reason"] == "QA command kick"
    assert "QA command kick" in await disconnect_reason(ctx, second)
    await wait_gone(ctx, "LcKickN2")
    assert api.count == 1 and command.count == 1


@scenario("lifecycle.kick.cancel", covers={PlayerKickEvent: "cancel"}, budget=1500)
async def kick_cancel(ctx: Context) -> None:
    kicked = ctx.capture(PlayerKickEvent, named("LcKickC"), cancel=True)
    bot, player = await join(ctx, "LcKickC")
    since = await mark(bot)
    player.kick("QA never leaves")
    await ctx.until(lambda: kicked.count, timeout=40, what="PlayerKickEvent")
    await barrier(ctx, {bot: since})
    assert await bot.call("is_connected"), "a cancelled kick disconnected the bot"
    assert await bot.call("get_disconnect_reason") is None
    assert ctx.server.get_player("LcKickC") is not None
    assert kicked.count == 1, kicked.events


@scenario("lifecycle.kick.mutate", covers={PlayerKickEvent: "mutate"}, budget=1500)
async def kick_mutate(ctx: Context) -> None:
    ctx.capture(
        PlayerKickEvent,
        named("LcKickM"),
        mutate=lambda e: setattr(e, "reason", "QA mutated reason"),
    )
    bot, player = await join(ctx, "LcKickM")
    player.kick("QA original reason")
    reason = await disconnect_reason(ctx, bot)
    assert "QA mutated reason" in reason, reason
    assert "QA original reason" not in reason, reason


async def three(ctx: Context, prefix: str):
    sender, _ = await join(ctx, f"{prefix}A")
    second, _ = await join(ctx, f"{prefix}B")
    third, _ = await join(ctx, f"{prefix}C")
    return sender, second, third


@scenario("lifecycle.chat.normal", covers={PlayerChatEvent: "normal"}, budget=1500)
async def chat_normal(ctx: Context) -> None:
    chat = ctx.capture(
        PlayerChatEvent,
        named("LcChatNA"),
        extra=lambda e: {"recipient_names": sorted(p.name for p in e.recipients)},
    )
    a, b, c = await three(ctx, "LcChatN")
    since = {bot: await mark(bot) for bot in (a, b, c)}
    await a.call("local_player.chat", "hello lifecycle")
    expected = ("chat", "LcChatNA", "hello lifecycle")
    for bot in (a, b, c):
        await hear(ctx, bot, since[bot], lambda g: expected in g, "the chat line")
    assert chat.count == 1, chat.events
    event = chat.events[0]
    assert event["message"] == "hello lifecycle"
    assert event["format"] == "<{0}> {1}"
    assert event["recipient_names"] == ["LcChatNA", "LcChatNB", "LcChatNC"]
    assert event["player"]["name"] == "LcChatNA"


@scenario("lifecycle.chat.cancel", covers={PlayerChatEvent: "cancel"}, budget=1500)
async def chat_cancel(ctx: Context) -> None:
    chat = ctx.capture(PlayerChatEvent, named("LcChatCA"), cancel=True)
    a, b, c = await three(ctx, "LcChatC")
    since = {bot: await mark(bot) for bot in (a, b, c)}
    await a.call("local_player.chat", "nobody hears this")
    await ctx.until(lambda: chat.count, timeout=40, what="PlayerChatEvent")
    after = await barrier(ctx, since)
    assert after == {"LcChatCA": [], "LcChatCB": [], "LcChatCC": []}, after


@scenario("lifecycle.chat.mutate_message", covers={PlayerChatEvent: "mutate"}, budget=1500)
async def chat_mutate_message(ctx: Context) -> None:
    chat = ctx.capture(
        PlayerChatEvent,
        named("LcChatMA"),
        mutate=lambda e: setattr(e, "message", "rewritten by a plugin"),
    )
    a, b, c = await three(ctx, "LcChatM")
    since = {bot: await mark(bot) for bot in (a, b, c)}
    await a.call("local_player.chat", "original words")
    await ctx.until(lambda: chat.count, timeout=40, what="PlayerChatEvent")
    expected = ("chat", "LcChatMA", "rewritten by a plugin")
    after = await barrier(ctx, since)
    for name, lines in after.items():
        assert lines == [expected], (name, lines)


@scenario("lifecycle.chat.mutate_format", covers={PlayerChatEvent: "mutate"}, budget=1500)
async def chat_mutate_format(ctx: Context) -> None:
    chat = ctx.capture(
        PlayerChatEvent,
        named("LcChatFA"),
        mutate=lambda e: setattr(e, "format", "[{0}] says {1}"),
    )
    a, b, c = await three(ctx, "LcChatF")
    since = {bot: await mark(bot) for bot in (a, b, c)}
    await a.call("local_player.chat", "formatted")
    await ctx.until(lambda: chat.count, timeout=40, what="PlayerChatEvent")
    expected = ("raw", "[LcChatFA] says formatted")
    after = await barrier(ctx, since)
    for name, lines in after.items():
        assert lines == [expected], (name, lines)


@scenario("lifecycle.chat.mutate_player", covers={PlayerChatEvent: "mutate"}, budget=1500)
async def chat_mutate_player(ctx: Context) -> None:
    def impersonate(e) -> None:
        e.player = ctx.server.get_player("LcChatPC")

    chat = ctx.capture(PlayerChatEvent, named("LcChatPA"), mutate=impersonate)
    a, b, c = await three(ctx, "LcChatP")
    since = await mark(b)
    await a.call("local_player.chat", "who said this")
    await ctx.until(lambda: chat.count, timeout=40, what="PlayerChatEvent")
    await barrier(ctx, {b: since})
    lines = await heard(b, since)
    assert ("chat", "LcChatPC", "who said this") in lines, lines


@scenario("lifecycle.chat.recipients", budget=1500)
async def chat_recipients(ctx: Context) -> None:
    def trim(e) -> None:
        e.recipients.clear()

    chat = ctx.capture(PlayerChatEvent, named("LcChatRA"), mutate=trim)
    a, b, c = await three(ctx, "LcChatR")
    since = {bot: await mark(bot) for bot in (a, b, c)}
    await a.call("local_player.chat", "to nobody")
    await ctx.until(lambda: chat.count, timeout=40, what="PlayerChatEvent")
    after = await barrier(ctx, since)
    assert after == {"LcChatRA": [], "LcChatRB": [], "LcChatRC": []}, after
    assert sorted(p["name"] for p in chat.events[0]["recipients"]) == ["LcChatRA", "LcChatRB", "LcChatRC"]
    assert chat.events[0]["after"]["recipients"] == [], chat.events[0]["after"]


@scenario("lifecycle.command.normal", covers={PlayerCommandEvent: "normal"}, budget=1500)
async def command_normal(ctx: Context) -> None:
    command = ctx.capture(PlayerCommandEvent, named("LcCmdN"))
    bot, player = await join(ctx, "LcCmdN", op=True)
    await bot.call("local_player.run_command", "/gamemode creative")
    await ctx.until(
        lambda: player.game_mode == GameMode.CREATIVE, timeout=40, what="creative"
    )
    assert command.count == 1, command.events
    assert command.events[0]["command"] == "/gamemode creative"


@scenario("lifecycle.command.cancel", covers={PlayerCommandEvent: "cancel"}, budget=1500)
async def command_cancel(ctx: Context) -> None:
    command = ctx.capture(PlayerCommandEvent, named("LcCmdC"), cancel=True)
    changed = ctx.capture(PlayerGameModeChangeEvent, named("LcCmdC"))
    bot, player = await join(ctx, "LcCmdC", op=True)
    since = await mark(bot)
    await bot.call("local_player.run_command", "/gamemode creative")
    await ctx.until(lambda: command.count, timeout=40, what="PlayerCommandEvent")
    await barrier(ctx, {bot: since})
    assert player.game_mode == GameMode.SURVIVAL, player.game_mode
    assert changed.count == 0, changed.events
    lines = await heard(bot, since)
    assert not any("gamemode" in str(line) for line in lines), lines


@scenario("lifecycle.command.mutate", covers={PlayerCommandEvent: "mutate"}, budget=1500)
async def command_mutate(ctx: Context) -> None:
    ctx.capture(
        PlayerCommandEvent,
        named("LcCmdM"),
        mutate=lambda e: setattr(e, "command", "/gamemode adventure"),
    )
    bot, player = await join(ctx, "LcCmdM", op=True)
    await bot.call("local_player.run_command", "/gamemode creative")
    await ctx.until(
        lambda: player.game_mode == GameMode.ADVENTURE, timeout=40, what="adventure"
    )


@scenario(
    "lifecycle.unknown_command.normal",
    covers={UnknownCommandEvent: "normal"},
    budget=1500,
)
async def unknown_command_normal(ctx: Context) -> None:
    unknown = ctx.capture(UnknownCommandEvent, lambda e: "lcunknownn" in e.command_line)
    bot, _ = await join(ctx, "LcUnkN", op=True)
    since = await mark(bot)
    await bot.call("local_player.run_command", "/lcunknownn first second")
    got = await hear(
        ctx,
        bot,
        since,
        lambda g: any("commands.generic.unknown" in str(x) for x in g),
        "the unknown command error",
    )
    assert unknown.count == 1, unknown.events
    event = unknown.events[0]
    assert event["command_line"] == "lcunknownn first second"
    assert event["sender"]["name"] == "LcUnkN"
    assert event["message"] == {
        "translate": "commands.generic.unknown",
        "params": ["lcunknownn"],
    }
    assert ("obj", f"{SECTION}c%commands.generic.unknown", ["lcunknownn"]) in got
    console = ctx.command("lcunknownn from the console")
    assert not console.success
    assert unknown.count == 2, unknown.events
    assert unknown.events[1]["sender"]["sender"] == "Server", unknown.events[1]


@scenario(
    "lifecycle.unknown_command.mutate",
    covers={UnknownCommandEvent: "mutate"},
    budget=1500,
)
async def unknown_command_mutate(ctx: Context) -> None:
    ctx.capture(
        UnknownCommandEvent,
        lambda e: "lcunknownmsg" in e.command_line,
        mutate=lambda e: setattr(e, "message", "QA no such command"),
    )
    ctx.capture(
        UnknownCommandEvent,
        lambda e: "lcunknownsilent" in e.command_line,
        mutate=lambda e: setattr(e, "message", None),
    )
    bot, _ = await join(ctx, "LcUnkM", op=True)
    since = await mark(bot)
    await bot.call("local_player.run_command", "/lcunknownmsg")
    await hear(
        ctx,
        bot,
        since,
        lambda g: ("raw", f"{SECTION}cQA no such command") in g,
        "the custom unknown command error",
    )
    mid = await mark(bot)
    await bot.call("local_player.run_command", "/lcunknownsilent")
    after = await barrier(ctx, {bot: mid})
    assert after["LcUnkM"] == [], after


@scenario("lifecycle.command.proxied", budget=1500)
async def command_proxied(ctx: Context) -> None:
    command = ctx.capture(PlayerCommandEvent, named("LcProxy"))
    bot, _ = await join(ctx, "LcProxy", op=True)
    since = await mark(bot)
    await bot.call("local_player.run_command", "/execute as @s run test sender")
    got = await hear(
        ctx,
        bot,
        since,
        lambda g: any("ProxiedCommandSender" in str(x) for x in g),
        "the proxied sender report",
    )
    assert command.count == 1, command.events
    assert not any("You are a player" in str(x) for x in got), got
    result = ctx.command("execute as LcProxy run test sender")
    assert not result.success
    assert any("ProxiedCommandSender" in line for line in result.errors), result.errors


@scenario(
    "lifecycle.gamemode.normal",
    covers={PlayerGameModeChangeEvent: "normal"},
    budget=1500,
)
async def gamemode_normal(ctx: Context) -> None:
    changed = ctx.capture(PlayerGameModeChangeEvent, named("LcGmN"))
    bot, player = await join(ctx, "LcGmN")
    ctx.run("gamemode creative LcGmN")
    await ctx.until(lambda: changed.count, timeout=40, what="the command change")
    assert changed.events[0]["new_game_mode"] == "CREATIVE"
    assert player.game_mode == GameMode.CREATIVE
    since = await mark(bot)
    player.game_mode = GameMode.ADVENTURE
    await ctx.until(lambda: changed.count == 2, timeout=40, what="the API change")
    assert changed.events[1]["new_game_mode"] == "ADVENTURE"
    assert player.game_mode == GameMode.ADVENTURE
    packets = await bot.call("get_received_packets", None, since)
    names = {p["name"] for p in packets}
    ctx.info["game_type_packets"] = sorted(n for n in names if "GameType" in n)
    assert ctx.info["game_type_packets"], names


@scenario(
    "lifecycle.gamemode.cancel",
    covers={PlayerGameModeChangeEvent: "cancel"},
    budget=1500,
)
async def gamemode_cancel(ctx: Context) -> None:
    changed = ctx.capture(PlayerGameModeChangeEvent, named("LcGmC"), cancel=True)
    bot, player = await join(ctx, "LcGmC")
    ctx.run("gamemode creative LcGmC")
    await ctx.until(lambda: changed.count, timeout=40, what="the command change")
    await ctx.ticks(5)
    assert player.game_mode == GameMode.SURVIVAL, player.game_mode
    player.game_mode = GameMode.ADVENTURE
    await ctx.until(lambda: changed.count == 2, timeout=40, what="the API change")
    await ctx.ticks(5)
    assert player.game_mode == GameMode.SURVIVAL, player.game_mode
    assert changed.events[0]["new_game_mode"] == "CREATIVE"
    assert changed.events[1]["new_game_mode"] == "ADVENTURE"


def near(actual, expected, tolerance: float = 1.0) -> bool:
    return all(abs(a - e) <= tolerance for a, e in zip(actual, expected))


async def bot_position(bot) -> list[float]:
    return await bot.call("local_player.get_position")


async def bot_arrives(ctx: Context, bot, expected, what: str, tolerance: float = 1.0):
    position: list[float] = []
    for _ in range(100):
        position = await bot_position(bot)
        if near(position, expected, tolerance):
            return position
        await ctx.ticks(1)
    raise AssertionError(f"{bot.name} never reached {what} {expected}; at {position}")


def xyz(player) -> list[float]:
    return [player.location.x, player.location.y, player.location.z]


async def stand_in_plot(ctx: Context, name: str, dx: int = 8, dz: int = 8):
    bot, player = await join(ctx, name, op=True)
    ctx.run(f"tp {name} {ctx.plot.at(dx, 0, dz)}")
    x, y, z = ctx.plot.pos(dx, 0, dz)
    where = [x + 0.5, float(y), z + 0.5]
    await bot_arrives(ctx, bot, where, "the plot", 0.1)
    return bot, player, where


async def wait_dead(ctx: Context, player) -> None:
    await ctx.until(lambda: player.is_dead, timeout=60, what=f"{player.name} dead")


@scenario("lifecycle.death.normal", covers={PlayerDeathEvent: "normal"}, budget=1500)
async def death_normal(ctx: Context) -> None:
    death = ctx.capture(PlayerDeathEvent, named("LcDeathN"))
    observer, _ = await join(ctx, "LcDeathNO")
    victim, player = await join(ctx, "LcDeathN")
    seen_by_observer = await mark(observer)
    seen_by_victim = await mark(victim)
    ctx.run("kill LcDeathN")
    await ctx.until(lambda: death.count, timeout=40, what="PlayerDeathEvent")
    await ctx.ticks(2)
    assert death.count == 1, death.events
    event = death.events[0]
    key = event["death_message"]["translate"]
    assert event["death_message"]["params"][0] == "LcDeathN", event["death_message"]
    await hear(
        ctx,
        observer,
        seen_by_observer,
        lambda g: any(x[0] == "obj" and x[1].endswith(key.lstrip("%")) for x in g),
        f"the death message {key}",
    )
    info = await victim.call("get_received_packets", "DeathInfoPacket", seen_by_victim)
    assert info, "the victim received no DeathInfoPacket"
    assert key.lstrip("%") in json.dumps(info[0]["packet"]), info[0]
    assert player.is_dead


@scenario("lifecycle.death.mutate", covers={PlayerDeathEvent: "mutate"}, budget=1500)
async def death_mutate(ctx: Context) -> None:
    ctx.capture(
        PlayerDeathEvent,
        named("LcDeathM1"),
        mutate=lambda e: setattr(e, "death_message", "QA LcDeathM1 met a dramatic end"),
    )
    ctx.capture(
        PlayerDeathEvent,
        named("LcDeathM2"),
        mutate=lambda e: setattr(e, "death_message", None),
    )
    observer, _ = await join(ctx, "LcDeathMO")
    first, player = await join(ctx, "LcDeathM1")
    second, other = await join(ctx, "LcDeathM2")
    since = await mark(observer)
    seen_by_victim = await mark(first)
    ctx.run("kill LcDeathM1")
    await hear(
        ctx,
        observer,
        since,
        lambda g: ("raw", "QA LcDeathM1 met a dramatic end") in g,
        "the custom death message",
    )
    info = await first.call("get_received_packets", "DeathInfoPacket", seen_by_victim)
    assert "QA LcDeathM1 met a dramatic end" in json.dumps(info), info
    mid = await mark(observer)
    ctx.run("kill LcDeathM2")
    await wait_dead(ctx, other)
    after = await barrier(ctx, {observer: mid})
    assert after["LcDeathMO"] == [], after


@scenario("lifecycle.respawn.normal", covers={PlayerRespawnEvent: "normal"}, plot=True, budget=1500)
async def respawn_normal(ctx: Context) -> None:
    respawn = ctx.capture(PlayerRespawnEvent, named("LcRespawn"))
    death = ctx.capture(PlayerDeathEvent, named("LcRespawn"))
    bot, player, _ = await stand_in_plot(ctx, "LcRespawn", 8, 8)
    ctx.run(f"spawnpoint LcRespawn {ctx.plot.at(2, 0, 2)}")
    home = [float(c) + 0.5 for c in ctx.plot.pos(2, 0, 2)]
    ctx.run("kill LcRespawn")
    await ctx.until(lambda: death.count, timeout=40, what="PlayerDeathEvent")
    await wait_dead(ctx, player)
    assert respawn.count == 0, "respawned before the client asked to"
    for _ in range(60):
        if await bot.call("local_player.is_dead"):
            break
        await ctx.ticks(1)
    else:
        raise AssertionError("the client never saw its death")
    await bot.call("local_player.respawn")
    await ctx.until(lambda: respawn.count, timeout=100, what="PlayerRespawnEvent")
    assert respawn.count == 1, respawn.events
    assert respawn.events[0]["respawn_reason"] == "DEATH", respawn.events
    await ctx.until(lambda: not player.is_dead, timeout=60, what="the player alive")
    assert player.health > 0
    assert near(xyz(player), home, 0.6), (xyz(player), home)
    await bot_arrives(ctx, bot, home, "the spawn point", 0.6)
    assert not await bot.call("local_player.is_dead"), "the client still sees itself dead"


async def spawn_packets(ctx: Context, bot, since: int) -> list[dict]:
    for _ in range(60):
        packets = await bot.call("get_received_packets", "SetSpawnPositionPacket", since)
        if packets:
            return packets
        await ctx.ticks(1)
    return []


@scenario("lifecycle.setspawn.normal", covers={PlayerSetSpawnEvent: "normal"}, plot=True, budget=1500)
async def setspawn_normal(ctx: Context) -> None:
    spawn = ctx.capture(PlayerSetSpawnEvent, named("LcSpawnN"))
    bot, player = await join(ctx, "LcSpawnN", op=True)
    cell = ctx.plot.pos(4, 0, 4)
    since = await mark(bot)
    result = ctx.run(f"spawnpoint LcSpawnN {ctx.plot.at(4, 0, 4)}")
    await ctx.until(lambda: spawn.count, timeout=40, what="the command spawn")
    first = spawn.events[0]
    assert first["cause"] == "COMMAND", first
    assert [first["location"][k] for k in "xyz"] == [float(c) for c in cell], first
    assert first["location"]["dimension"] == "minecraft:overworld"
    stored = player.respawn_location
    assert stored is not None and [int(stored.x), int(stored.y), int(stored.z)] == list(cell)
    assert result.success
    packets = await spawn_packets(ctx, bot, since)
    assert packets, "the client was not told about the new spawn"
    other = ctx.plot.pos(9, 0, 9)
    player.respawn_location = Location(ctx.overworld, *[float(c) for c in other])
    await ctx.until(lambda: spawn.count == 2, timeout=40, what="the plugin spawn")
    assert spawn.events[1]["cause"] == "PLUGIN", spawn.events[1]
    stored = player.respawn_location
    assert [int(stored.x), int(stored.y), int(stored.z)] == list(other)


@scenario("lifecycle.setspawn.cancel", covers={PlayerSetSpawnEvent: "cancel"}, plot=True, budget=1500)
async def setspawn_cancel(ctx: Context) -> None:
    spawn = ctx.capture(PlayerSetSpawnEvent, named("LcSpawnC"), cancel=True)
    bot, player = await join(ctx, "LcSpawnC", op=True)
    before = player.respawn_location
    since = await mark(bot)
    result = ctx.command(f"spawnpoint LcSpawnC {ctx.plot.at(4, 0, 4)}")
    await ctx.until(lambda: spawn.count, timeout=40, what="the command spawn")
    await barrier(ctx, {bot: since})
    assert result.success, result.output
    assert player.respawn_location == before, (before, player.respawn_location)
    assert not await bot.call("get_received_packets", "SetSpawnPositionPacket", since)
    player.respawn_location = Location(ctx.overworld, *[float(c) for c in ctx.plot.pos(9, 0, 9)])
    await ctx.until(lambda: spawn.count == 2, timeout=40, what="the plugin spawn")
    assert player.respawn_location == before, (before, player.respawn_location)


@scenario("lifecycle.setspawn.mutate", covers={PlayerSetSpawnEvent: "mutate"}, plot=True, budget=1500)
async def setspawn_mutate(ctx: Context) -> None:
    target = [float(c) for c in ctx.plot.pos(12, 0, 12)]
    spawn = ctx.capture(
        PlayerSetSpawnEvent,
        named("LcSpawnM"),
        mutate=lambda e: setattr(e, "location", Location(ctx.overworld, *target)),
    )
    bot, player = await join(ctx, "LcSpawnM", op=True)
    since = await mark(bot)
    ctx.run(f"spawnpoint LcSpawnM {ctx.plot.at(4, 0, 4)}")
    await ctx.until(lambda: spawn.count, timeout=40, what="the command spawn")
    stored = player.respawn_location
    assert [int(stored.x), int(stored.y), int(stored.z)] == [int(c) for c in target], stored
    packets = await spawn_packets(ctx, bot, since)
    assert packets, "the client was not told about the new spawn"
    assert packets[-1]["packet"]["pos"] == {"x": int(target[0]), "y": int(target[1]), "z": int(target[2])}, packets


@scenario("lifecycle.setspawn.clear", plot=True, budget=1500)
async def setspawn_clear(ctx: Context) -> None:
    spawn = ctx.capture(PlayerSetSpawnEvent, named("LcSpawnX"))
    bot, player = await join(ctx, "LcSpawnX", op=True)
    ctx.run(f"spawnpoint LcSpawnX {ctx.plot.at(4, 0, 4)}")
    await ctx.until(lambda: spawn.count, timeout=40, what="the command spawn")
    assert player.respawn_location is not None
    ctx.run("clearspawnpoint LcSpawnX")
    await ctx.until(lambda: player.respawn_location is None, timeout=40, what="a cleared spawn")
    assert spawn.count == 1, f"/clearspawnpoint fired PlayerSetSpawnEvent: {spawn.events[1:]}"


@scenario("lifecycle.teleport.normal", covers={PlayerTeleportEvent: "normal"}, plot=True, budget=1500)
async def teleport_normal(ctx: Context) -> None:
    bot, player, start = await stand_in_plot(ctx, "LcTpN")
    teleport = ctx.capture(PlayerTeleportEvent, named("LcTpN"))
    ctx.run(f"tp LcTpN {ctx.plot.at(3, 0, 3)}")
    await ctx.until(lambda: teleport.count, timeout=40, what="the /tp event")
    first = teleport.events[0]
    goal = [float(c) + 0.5 for c in ctx.plot.pos(3, 0, 3)]
    goal[1] -= 0.5
    assert near([first["to_location"][k] for k in "xyz"], goal, 0.01), first
    assert near([first["from_location"][k] for k in "xyz"], start, 1.0), first
    assert near(xyz(player), goal, 0.5)
    await bot_arrives(ctx, bot, goal, "the /tp target")
    api = [c + 10.5 for c in goal]
    assert player.teleport(Location(ctx.overworld, *api))
    await ctx.until(lambda: teleport.count == 2, timeout=40, what="the API event")
    assert near([teleport.events[1]["to_location"][k] for k in "xyz"], api, 0.01)
    assert near(xyz(player), api, 0.5)
    await bot_arrives(ctx, bot, api, "the API target")


@scenario("lifecycle.teleport.cancel", covers={PlayerTeleportEvent: "cancel"}, plot=True, budget=1500)
async def teleport_cancel(ctx: Context) -> None:
    bot, player, start = await stand_in_plot(ctx, "LcTpC")
    teleport = ctx.capture(PlayerTeleportEvent, named("LcTpC"), cancel=True)
    since = await mark(bot)
    ctx.run(f"tp LcTpC {ctx.plot.at(3, 0, 3)}")
    await ctx.until(lambda: teleport.count, timeout=40, what="the /tp event")
    assert near(xyz(player), start, 0.5), xyz(player)
    player.teleport(Location(ctx.overworld, start[0] + 6.5, start[1], start[2] + 6.5))
    await ctx.until(lambda: teleport.count == 2, timeout=40, what="the API event")
    await barrier(ctx, {bot: since})
    assert near(xyz(player), start, 0.5), xyz(player)
    assert near(await bot_position(bot), start, 0.5), await bot_position(bot)


@scenario("lifecycle.teleport.mutate", covers={PlayerTeleportEvent: "mutate"}, plot=True, budget=1500)
async def teleport_mutate(ctx: Context) -> None:
    bot, player, _ = await stand_in_plot(ctx, "LcTpM")
    landing = [float(c) + 0.5 for c in ctx.plot.pos(13, 0, 13)]
    teleport = ctx.capture(
        PlayerTeleportEvent,
        named("LcTpM"),
        mutate=lambda e: setattr(e, "to_location", Location(ctx.overworld, *landing)),
    )
    ctx.run(f"tp LcTpM {ctx.plot.at(3, 0, 3)}")
    await ctx.until(lambda: teleport.count, timeout=40, what="the /tp event")
    await bot_arrives(ctx, bot, landing, "the mutated destination")
    assert near(xyz(player), landing, 0.5), xyz(player)


@scenario("lifecycle.dimension.normal", covers={PlayerDimensionChangeEvent: "normal"}, plot=True, budget=3000)
async def dimension_normal(ctx: Context) -> None:
    bot, player, _ = await stand_in_plot(ctx, "LcDim")
    changed = ctx.capture(PlayerDimensionChangeEvent, named("LcDim"))
    ctx.run("execute as LcDim in nether run tp @s 8 80 8")
    await ctx.until(lambda: changed.count, timeout=400, what="the nether change")
    assert changed.events[0]["from_dimension"] == {"id": "minecraft:overworld"}
    assert changed.events[0]["to_dimension"] == {"id": "minecraft:nether"}
    assert str(player.dimension.id) == "minecraft:nether"
    arrived = await bot_position(bot)
    ctx.info["nether_position"] = arrived
    ctx.run(f"execute as LcDim in overworld run tp @s {ctx.plot.at(8, 0, 8)}")
    await ctx.until(lambda: changed.count == 2, timeout=400, what="the return")
    assert changed.events[1]["from_dimension"] == {"id": "minecraft:nether"}
    assert changed.events[1]["to_dimension"] == {"id": "minecraft:overworld"}
    assert str(player.dimension.id) == "minecraft:overworld"
    goal = [float(c) for c in ctx.plot.pos(8, 0, 8)]
    await bot_arrives(ctx, bot, goal, "the overworld again", 1.5)


def orbs_near(ctx: Context, where) -> bool:
    return ctx.command(f"testfor @e[type=xp_orb,{ctx.plot.volume}]").success


def drop_orb(ctx: Context, where) -> None:
    ctx.run(f"summon xp_orb {where[0]} {where[1]} {where[2]}")


@scenario("lifecycle.exp.normal", covers={PlayerExpChangeEvent: "normal"}, plot=True, budget=1500)
async def exp_normal(ctx: Context) -> None:
    exp = ctx.capture(PlayerExpChangeEvent, named("LcExpN"))
    bot, player, _ = await stand_in_plot(ctx, "LcExpN")
    ctx.run("xp 5 LcExpN")
    await ctx.until(lambda: exp.count, timeout=40, what="the /xp event")
    assert exp.events[0]["amount"] == 5
    assert player.total_exp == 5, player.total_exp
    player.give_exp(7)
    await ctx.until(lambda: exp.count == 2, timeout=40, what="the API event")
    assert exp.events[1]["amount"] == 7
    assert player.total_exp == 12, player.total_exp
    refused = ctx.command("xp -5 LcExpN")
    assert not refused.success and exp.count == 2, (refused.output, exp.events)
    player.give_exp(-4)
    await ctx.until(lambda: exp.count == 3, timeout=40, what="the negative API event")
    assert exp.events[2]["amount"] == -4
    assert player.total_exp == 8, player.total_exp


@scenario(
    "lifecycle.exp.pickup",
    covers={PlayerPickupExperienceEvent: "normal", PlayerExpChangeEvent: "normal"},
    plot=True,
    budget=1500,
)
async def exp_pickup(ctx: Context) -> None:
    pickup = ctx.capture(PlayerPickupExperienceEvent, named("LcExpP"))
    exp = ctx.capture(PlayerExpChangeEvent, named("LcExpP"), extra=order)
    bot, player, where = await stand_in_plot(ctx, "LcExpP")
    before = player.total_exp
    drop_orb(ctx, where)
    await ctx.until(lambda: pickup.count, timeout=200, what="the orb pickup")
    await ctx.until(lambda: exp.count, timeout=40, what="the experience change")
    amount = pickup.events[0]["amount"]
    assert amount > 0 and exp.events[0]["amount"] == amount, (pickup.events, exp.events)
    assert player.total_exp == before + amount, (before, amount, player.total_exp)
    assert not orbs_near(ctx, where), "the picked up orb is still there"


@scenario("lifecycle.exp.pickup_cancel", covers={PlayerPickupExperienceEvent: "cancel"}, plot=True, budget=1500)
async def exp_pickup_cancel(ctx: Context) -> None:
    pickup = ctx.capture(PlayerPickupExperienceEvent, named("LcExpC"), cancel=True)
    exp = ctx.capture(PlayerExpChangeEvent, named("LcExpC"))
    bot, player, where = await stand_in_plot(ctx, "LcExpC")
    drop_orb(ctx, where)
    await ctx.until(lambda: pickup.count, timeout=200, what="the pickup attempt")
    for _ in range(20):
        assert player.total_exp == 0, player.total_exp
        assert orbs_near(ctx, where), "a cancelled pickup consumed the orb"
        assert exp.count == 0, exp.events
        await ctx.ticks(1)


@scenario("lifecycle.exp.mutate", covers={PlayerExpChangeEvent: "mutate"}, plot=True, budget=1500)
async def exp_mutate(ctx: Context) -> None:
    exp = ctx.capture(
        PlayerExpChangeEvent,
        named("LcExpM"),
        mutate=lambda e: setattr(e, "amount", 9),
    )
    bot, player, where = await stand_in_plot(ctx, "LcExpM")
    ctx.run("xp 2 LcExpM")
    await ctx.until(lambda: exp.count, timeout=40, what="the /xp event")
    assert exp.events[0]["amount"] == 2 and exp.events[0]["after"]["amount"] == 9
    assert player.total_exp == 9, player.total_exp
    drop_orb(ctx, where)
    await ctx.until(lambda: exp.count == 2, timeout=200, what="the orb event")
    assert player.total_exp == 18, player.total_exp


@scenario("lifecycle.level.normal", covers={PlayerLevelChangeEvent: "normal"}, plot=True, budget=1500)
async def level_normal(ctx: Context) -> None:
    level = ctx.capture(PlayerLevelChangeEvent, named("LcLevel"))
    bot, player, _ = await stand_in_plot(ctx, "LcLevel")
    ctx.run("xp 3L LcLevel")
    await ctx.until(lambda: level.count, timeout=40, what="the /xp levels event")
    assert (level.events[-1]["old_level"], level.events[-1]["new_level"]) == (0, 3), level.events
    assert player.exp_level == 3
    seen = level.count
    player.give_exp(2000)
    await ctx.until(lambda: player.exp_level > 3, timeout=40, what="points turned to levels")
    gained = level.events[seen:]
    assert gained, "gaining levels through points fired no PlayerLevelChangeEvent"
    assert gained[0]["old_level"] == 3
    assert gained[-1]["new_level"] == player.exp_level
    for before, after in zip(gained, gained[1:]):
        assert before["new_level"] == after["old_level"], gained
    seen = level.count
    ctx.run("xp -2L LcLevel")
    await ctx.until(lambda: level.count > seen, timeout=40, what="the level loss")
    assert level.events[-1]["new_level"] == level.events[-1]["old_level"] - 2, level.events[-1]


@scenario("lifecycle.chat.recipients_discard", budget=1500)
async def chat_recipients_discard(ctx: Context) -> None:
    def drop(e) -> None:
        b = ctx.server.get_player("LcChatDB")
        assert b in e.recipients and len(e.recipients) == 3
        e.recipients.discard(b)
        e.recipients.discard(b)
        try:
            e.recipients.remove(b)
        except KeyError:
            pass
        else:
            raise AssertionError("remove() of a player no longer in the set did not raise KeyError")
        assert b not in e.recipients and len(e.recipients) == 2

    chat = ctx.capture(PlayerChatEvent, named("LcChatDA"), mutate=drop)
    a, b, c = await three(ctx, "LcChatD")
    since = {bot: await mark(bot) for bot in (a, b, c)}
    await a.call("local_player.chat", "not for b")
    await ctx.until(lambda: chat.count, timeout=40, what="PlayerChatEvent")
    after = await barrier(ctx, since)
    expected = ("chat", "LcChatDA", "not for b")
    assert after == {"LcChatDA": [expected], "LcChatDB": [], "LcChatDC": [expected]}, after
    assert chat.error is None, chat.error
    assert sorted(p["name"] for p in chat.events[0]["after"]["recipients"]) == ["LcChatDA", "LcChatDC"]


@scenario("lifecycle.chat.recipients_add", budget=1500)
async def chat_recipients_add(ctx: Context) -> None:
    def only_c(e) -> None:
        c = ctx.server.get_player("LcChatXC")
        e.recipients.clear()
        e.recipients.add(c)
        e.recipients.add(c)
        assert len(e.recipients) == 1 and [p.name for p in e.recipients] == ["LcChatXC"]

    chat = ctx.capture(PlayerChatEvent, named("LcChatXA"), mutate=only_c)
    a, b, c = await three(ctx, "LcChatX")
    since = {bot: await mark(bot) for bot in (a, b, c)}
    await a.call("local_player.chat", "only for c")
    await ctx.until(lambda: chat.count, timeout=40, what="PlayerChatEvent")
    after = await barrier(ctx, since)
    expected = ("chat", "LcChatXA", "only for c")
    assert after == {"LcChatXA": [], "LcChatXB": [], "LcChatXC": [expected]}, after
    assert chat.error is None, chat.error


@scenario("lifecycle.broadcast.recipients", budget=1500)
async def broadcast_recipients(ctx: Context) -> None:
    from endstone.event import BroadcastMessageEvent

    text = "endstone-test broadcast recipients"

    def drop(e) -> None:
        b = ctx.server.get_player("LcBcRB")
        assert b in e.recipients and ctx.server.command_sender in e.recipients
        e.recipients.discard(b)

    broadcast = ctx.capture(BroadcastMessageEvent, lambda e: e.message == text, mutate=drop)
    a, _ = await join(ctx, "LcBcRA")
    b, _ = await join(ctx, "LcBcRB")
    since = {bot: await mark(bot) for bot in (a, b)}
    lines = await ctx.log_of(lambda: ctx.server.broadcast_message(text))
    assert broadcast.count == 1, broadcast.events
    assert broadcast.error is None, broadcast.error
    assert any(text in line for line in lines), lines
    after = await barrier(ctx, since)
    assert after == {"LcBcRA": [("raw", text)], "LcBcRB": []}, after
    names = sorted(r["sender"] if "sender" in r else r["name"] for r in broadcast.events[0]["after"]["recipients"])
    assert "LcBcRB" not in names and "LcBcRA" in names, names



@scenario("lifecycle.chat.recipients_set_api", budget=1500)
async def chat_recipients_set_api(ctx: Context) -> None:
    import collections.abc

    seen = {}

    def probe(e) -> None:
        a = ctx.server.get_player("LcChatSA")
        b = ctx.server.get_player("LcChatSB")
        c = ctx.server.get_player("LcChatSC")
        r = e.recipients
        seen["mutable_set"] = isinstance(r, collections.abc.MutableSet)
        seen["equal"] = r == {a, b, c}
        seen["in_other"] = "LcChatSA" in r or None in r or 3 in r
        r.discard("nobody")
        r.discard(None)
        copy = r.copy()
        seen["copy"] = type(copy) is set and copy == {a, b, c}
        seen["minus"] = type(r - {b}) is set and (r - {b}) == {a, c}
        seen["or"] = (r | {a}) == {a, b, c}
        seen["and"] = (r & {a, b}) == {a, b}
        seen["xor"] = (r ^ {a}) == {b, c}
        seen["subset"] = r.issubset({a, b, c}) and r.issuperset({a}) and r <= {a, b, c} and not r < {a, b, c}
        seen["union"] = r.union([a]) == {a, b, c} and r.difference({c}) == {a, b} and r.intersection({c}) == {c}
        try:
            for player in r:
                r.discard(player)
        except RuntimeError as error:
            seen["iteration"] = str(error)
        r.update({a, b, c})
        seen["updated"] = len(r) == 3
        popped = r.pop()
        seen["pop"] = popped in {a, b, c} and len(r) == 2
        r.add(popped)
        r -= {b}
        seen["isub"] = r == {a, c}
        r |= {b}
        r &= {a, b}
        seen["iand"] = r == {a, b}
        r.difference_update([a])
        r.symmetric_difference_update({a, c})
        seen["updates"] = r == {a, b, c}
        r.intersection_update({a, c})
        seen["intersection_update"] = r == {a, c}
        try:
            hash(r)
        except TypeError:
            seen["unhashable"] = True

    chat = ctx.capture(PlayerChatEvent, named("LcChatSA"), mutate=probe)
    a, b, c = await three(ctx, "LcChatS")
    since = {bot: await mark(bot) for bot in (a, b, c)}
    await a.call("local_player.chat", "set api")
    await ctx.until(lambda: chat.count, timeout=40, what="PlayerChatEvent")
    after = await barrier(ctx, since)
    assert chat.error is None, chat.error
    ctx.info["seen"] = seen
    expected = ("chat", "LcChatSA", "set api")
    assert after == {"LcChatSA": [expected], "LcChatSB": [], "LcChatSC": [expected]}, after
    assert seen == {
        "mutable_set": True,
        "equal": True,
        "in_other": False,
        "copy": True,
        "minus": True,
        "or": True,
        "and": True,
        "xor": True,
        "subset": True,
        "union": True,
        "iteration": "Set changed size during iteration",
        "updated": True,
        "pop": True,
        "isub": True,
        "iand": True,
        "updates": True,
        "intersection_update": True,
        "unhashable": True,
    }, seen


@scenario("lifecycle.broadcast.recipients_set_api", budget=1500)
async def broadcast_recipients_set_api(ctx: Context) -> None:
    from endstone.event import BroadcastMessageEvent

    text = "endstone-test broadcast set api"
    seen = {}

    def probe(e) -> None:
        b = ctx.server.get_player("LcBcSB")
        staff = {b}
        rest = e.recipients - staff
        seen["minus"] = type(rest) is set and b not in rest and ctx.server.command_sender in rest
        seen["copy"] = e.recipients.copy() == set(e.recipients)
        recipients = e.recipients
        recipients -= staff

    broadcast = ctx.capture(BroadcastMessageEvent, lambda e: e.message == text, mutate=probe)
    a, _ = await join(ctx, "LcBcSA")
    b, _ = await join(ctx, "LcBcSB")
    since = {bot: await mark(bot) for bot in (a, b)}
    ctx.server.broadcast_message(text)
    after = await barrier(ctx, since)
    assert broadcast.error is None, broadcast.error
    assert seen == {"minus": True, "copy": True}, seen
    assert after == {"LcBcSA": [("raw", text)], "LcBcSB": []}, after



def foreign_element_errors(recipients) -> dict:
    errors = {}
    for label, call in (
        ("add", lambda: recipients.add("nobody")),
        ("update", lambda: recipients.update(["nobody"])),
        ("symmetric_difference_update", lambda: recipients.symmetric_difference_update(["nobody"])),
        ("ior", lambda: recipients.__ior__({"nobody"})),
    ):
        try:
            call()
            errors[label] = None
        except Exception as error:  # noqa: BLE001
            errors[label] = type(error).__name__
    return errors


FOREIGN_ELEMENT_ERRORS = {
    "add": "TypeError",
    "update": "TypeError",
    "symmetric_difference_update": "TypeError",
    "ior": "TypeError",
}


@scenario("lifecycle.chat.recipients_set_types", budget=1500)
async def chat_recipients_set_types(ctx: Context) -> None:
    seen = {}

    def probe(e) -> None:
        seen.update(foreign_element_errors(e.recipients))
        seen["kept"] = len(e.recipients)

    chat = ctx.capture(PlayerChatEvent, named("LcChatTA"), mutate=probe)
    a, b, c = await three(ctx, "LcChatT")
    await a.call("local_player.chat", "set types")
    await ctx.until(lambda: chat.count, timeout=40, what="PlayerChatEvent")
    assert chat.error is None, chat.error
    assert seen == {**FOREIGN_ELEMENT_ERRORS, "kept": 3}, seen


@scenario("lifecycle.broadcast.recipients_set_types", budget=1500)
async def broadcast_recipients_set_types(ctx: Context) -> None:
    from endstone.event import BroadcastMessageEvent

    text = "endstone-test broadcast set types"
    seen = {}

    def probe(e) -> None:
        seen.update(foreign_element_errors(e.recipients))

    broadcast = ctx.capture(BroadcastMessageEvent, lambda e: e.message == text, mutate=probe)
    a, _ = await join(ctx, "LcBcTA")
    ctx.server.broadcast_message(text)
    await ctx.until(lambda: broadcast.count, timeout=40, what="BroadcastMessageEvent")
    assert broadcast.error is None, broadcast.error
    assert seen == FOREIGN_ELEMENT_ERRORS, seen
