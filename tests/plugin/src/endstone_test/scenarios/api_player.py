from contextlib import contextmanager

import numpy as np
from endstone.event import (
    Cancellable,
    InventoryInteractEvent,
    PlayerBucketEvent,
    PlayerChatEvent,
    PlayerInputEvent,
    PlayerMoveEvent,
)
from endstone.level import Location
from endstone.map import MapCursor, MapRenderer

from endstone_test.harness import Context, scenario
from endstone_test.harness.capture import Capture
from endstone_test.harness.engine import REGISTRY
from endstone_test.scenarios import (  # noqa: F401
    player_actors,
    player_blocks,
    player_inventory,
    player_lifecycle,
    player_movement,
)
from endstone_test.scenarios.player_apis import got, map_item, wait_for
from endstone_test.scenarios.player_apis import join as apis_join
from endstone_test.scenarios.player_apis import mark as apis_mark
from endstone_test.scenarios.player_inventory import InventoryCapture
from endstone_test.scenarios.player_lifecycle import barrier, named, three
from endstone_test.scenarios.player_lifecycle import mark as chat_mark
from endstone_test.scenarios.player_movement import (
    bot_position,
    flat,
    join,
    mine,
    poll,
    where,
)

BUDGET = 900
SNEAKING = 1
SPRINTING = 3


def abgr(r: int, g: int, b: int, a: int = 255) -> int:
    return a << 24 | b << 16 | g << 8 | r


class Recycled:
    def __init__(self):
        self.cycles = 0
        self.problems: list[BaseException] = []


def recycle(event, state: Recycled) -> None:
    event.is_cancelled = False
    assert event.is_cancelled is False, "is_cancelled = False left the event cancelled"
    for base in (PlayerBucketEvent, InventoryInteractEvent):
        if isinstance(event, base):
            base.is_cancelled.fset(event, False)
            assert base.is_cancelled.fget(event) is False, f"{base.__name__}.is_cancelled = False did not stick"
            base.cancel(event)
            assert base.is_cancelled.fget(event) is True, f"{base.__name__}.cancel did not cancel"
            base.is_cancelled.fset(event, False)
            base.is_cancelled.fset(event, True)
            assert base.is_cancelled.fget(event) is True, f"{base.__name__}.is_cancelled = True did not stick"
    event.is_cancelled = True
    assert event.is_cancelled is True, "is_cancelled = True did not cancel the event"
    state.cycles += 1


@contextmanager
def cancelling_through_the_setter():
    state = Recycled()
    originals = {cls: cls.offer for cls in (Capture, InventoryCapture)}

    def wrap(original):
        def offer(self, event):
            before = len(self.events)
            original(self, event)
            if len(self.events) > before and isinstance(event, Cancellable) and event.is_cancelled:
                try:
                    recycle(event, state)
                except BaseException as e:  # noqa: BLE001
                    state.problems.append(e)

        return offer

    for cls, original in originals.items():
        cls.offer = wrap(original)
    try:
        yield state
    finally:
        for cls, original in originals.items():
            cls.offer = original


def through_the_setter(source: str):
    original = REGISTRY[source]

    async def run(ctx: Context) -> None:
        with cancelling_through_the_setter() as state:
            await original.fn(ctx)
        assert not state.problems, state.problems
        assert state.cycles, f"{source} never cancelled an event"

    return original, run


for name, source in {
    "api.player.cancel.craft": "inventory.craft.cancel",
    "api.player.cancel.enchant_apply": "inventory.enchant.apply_cancel",
    "api.player.cancel.enchant_prepare": "inventory.enchant.prepare_cancel",
    "api.player.cancel.open": "inventory.open.cancel",
    "api.player.cancel.book": "inventory.book.cancel",
    "api.player.cancel.consume": "inventory.consume.cancel",
    "api.player.cancel.arrow": "inventory.pickup.arrow_cancel",
    "api.player.cancel.swing": "movement.swing.cancel",
    "api.player.cancel.bed": "blocks.bed.enter_cancel",
    "api.player.cancel.interact": "blocks.interact.right_click_air",
    "api.player.cancel.bucket_fill": "blocks.bucket.fill.cancel",
}.items():
    base, run = through_the_setter(source)
    scenario(name, budget=base.budget, plot=base.plot)(run)


@scenario("api.player.input_value", plot=True, budget=BUDGET)
async def input_value(ctx: Context) -> None:
    bot, player, start = await join(ctx, "apiInput")
    seen = []

    def keep(event) -> dict:
        seen.append(event.input)
        return {}

    inputs = ctx.capture(PlayerInputEvent, mine("apiInput"), extra=keep)

    async def press(label: str, method: str, *args) -> None:
        before = inputs.count
        await bot.call(method, *args)
        await ctx.until(lambda: inputs.count > before, timeout=60, what=f"PlayerInputEvent for {label}")
        await ctx.ticks(6)

    await press("forward", "local_player.set_move_vector", 0.0, 1.0)
    await press("release", "local_player.stop_moving")
    await press("forward again", "local_player.set_move_vector", 0.0, 1.0)
    await press("release again", "local_player.stop_moving")
    await press("left", "local_player.set_move_vector", 1.0, 0.0)
    await press("release left", "local_player.stop_moving")
    assert inputs.error is None, inputs.error
    forward, idle, forward_again, idle_again, left, idle_left = seen[-6:]
    assert forward.is_forward and not forward.is_left and not forward.is_jump
    assert not idle.is_forward and not idle.is_left
    assert left.is_left and not left.is_forward
    assert forward == forward and not forward != forward
    assert forward == forward_again and not forward != forward_again
    assert idle == idle_again == idle_left
    assert forward != idle and not forward == idle
    assert forward != left and left != idle
    assert (forward == 1) is False and (forward != 1) is True
    assert (forward == None) is False and (forward != None) is True  # noqa: E711
    assert hash(forward) == hash(forward_again)
    assert hash(idle) == hash(idle_again)
    assert len({hash(forward), hash(idle), hash(left)}) == 3
    assert len({forward, forward_again, idle, idle_again, left}) == 3
    table = {forward: "w", left: "a"}
    assert table[forward_again] == "w" and table[left] == "a" and idle not in table
    assert repr(forward) == (
        "Input(forward=True, backward=False, left=False, right=False, jump=False, sneak=False, sprint=False)"
    ), repr(forward)
    assert repr(left) == (
        "Input(forward=False, backward=False, left=True, right=False, jump=False, sneak=False, sprint=False)"
    ), repr(left)


def actor_flags(found: list[dict], runtime_id: int, bit: int) -> list[bool]:
    flags = []
    for entry in found:
        packet = entry["packet"]
        if packet["id"] != runtime_id:
            continue
        for item in packet["packed_items"]["data"]:
            if item["id"] in (0, "RESERVED_0"):
                flags.append(bool(item["payload"]["value"] >> bit & 1))
    return flags


async def flag_seen(ctx: Context, bot, since: int, runtime_id: int, bit: int, want: bool, what: str) -> None:
    found: list[dict] = []
    for _ in range(80):
        found = await bot.call("get_received_packets", "SetActorDataPacket", since)
        flags = actor_flags(found, runtime_id, bit)
        if want and True in flags:
            return
        if not want and True in flags and False in flags[flags.index(True) :]:
            return
        await ctx.ticks(1)
    raise AssertionError(f"{bot.name} never saw {what}; SetActorDataPacket: {found[-5:]}")


@scenario("api.player.sneak_set", plot=True, budget=BUDGET)
async def sneak_set(ctx: Context) -> None:
    bot, player, start = await join(ctx, "apiSneakSet")
    watcher, other, _ = await join(ctx, "apiSneakWatch", dx=10)
    rid = player.runtime_id
    own, theirs = await apis_mark(bot), await apis_mark(watcher)
    player.is_sneaking = True
    assert player.is_sneaking is True
    await flag_seen(ctx, watcher, theirs, rid, SNEAKING, True, "the SNEAKING flag set")
    await flag_seen(ctx, bot, own, rid, SNEAKING, True, "its own SNEAKING flag set")
    player.is_sneaking = False
    assert player.is_sneaking is False
    await flag_seen(ctx, watcher, theirs, rid, SNEAKING, False, "the SNEAKING flag cleared")
    await flag_seen(ctx, bot, own, rid, SNEAKING, False, "its own SNEAKING flag cleared")
    assert not other.is_sneaking


@scenario("api.player.sprint_set", plot=True, budget=BUDGET)
async def sprint_set(ctx: Context) -> None:
    bot, player, start = await join(ctx, "apiSprintSet")
    watcher, other, _ = await join(ctx, "apiSprintWatch", dx=10)
    rid = player.runtime_id
    own, theirs = await apis_mark(bot), await apis_mark(watcher)
    player.is_sprinting = True
    assert player.is_sprinting is True
    await flag_seen(ctx, watcher, theirs, rid, SPRINTING, True, "the SPRINTING flag set")
    await flag_seen(ctx, bot, own, rid, SPRINTING, True, "its own SPRINTING flag set")
    player.is_sprinting = False
    assert player.is_sprinting is False
    await flag_seen(ctx, watcher, theirs, rid, SPRINTING, False, "the SPRINTING flag cleared")
    await flag_seen(ctx, bot, own, rid, SPRINTING, False, "its own SPRINTING flag cleared")
    assert not other.is_sprinting


@scenario("api.player.sounds", budget=BUDGET)
async def sounds(ctx: Context) -> None:
    bot, player = await apis_join(ctx, "apiSound")
    watcher, other = await apis_join(ctx, "apiSoundW")
    await ctx.ticks(20)
    since, seen = await apis_mark(bot), await apis_mark(watcher)
    player.stop_sound("random.pop")
    player.stop_all_sounds()
    player.stop_sound("note.harp")
    found = await wait_for(ctx, bot, "StopSoundPacket", since, lambda f: len(f) == 3, "three StopSoundPackets")
    ctx.info["stop_sound_packets"] = found
    keys = set(found[0])
    name_key = next(k for k in keys if "name" in k)
    all_key = next(k for k in keys if "all" in k)
    assert [(p[name_key], p[all_key]) for p in found] == [
        ("random.pop", False),
        ("", True),
        ("note.harp", False),
    ], found
    other.send_message("@@sound barrier")
    await wait_for(
        ctx,
        watcher,
        "TextPacket",
        seen,
        lambda f: any(t["body"].get("message") == "@@sound barrier" for t in f),
        "the barrier",
    )
    assert not await got(watcher, "StopSoundPacket", seen)


class DrawRenderer(MapRenderer):
    def __init__(self, record: dict):
        MapRenderer.__init__(self, is_contextual=False)
        self.record = record

    def render(self, view, canvas, player):
        if self.record.get("rendered"):
            return
        record = self.record
        record["rendered"] = True
        record["view_id"] = (canvas.map_view.id, view.id)
        canvas.set_pixel(0, 0, 0xFF0A0B0C)
        record["get_pixel"] = canvas.get_pixel(0, 0)
        record["pixel_color"] = tuple(canvas.get_pixel_color(0, 0))
        canvas.set_pixel(-1, 5, 0xFF111111)
        canvas.set_pixel(5, -1, 0xFF111111)
        canvas.set_pixel(128, 5, 0xFF111111)
        canvas.set_pixel(5, 128, 0xFF111111)
        record["outside"] = (
            canvas.get_pixel(-1, 5),
            canvas.get_pixel(5, -1),
            canvas.get_pixel(128, 5),
            canvas.get_pixel(5, 128),
        )
        record["untouched"] = canvas.get_pixel(64, 64)
        image = np.zeros((4, 3, 4), np.uint8)
        image[..., :] = (1, 2, 3, 255)
        canvas.draw_image(10, 20, image)
        corner = np.zeros((4, 4, 4), np.uint8)
        corner[..., :] = (4, 5, 6, 255)
        canvas.draw_image(126, 126, corner)
        canvas.draw_image(-2, 60, corner)
        canvas.cursors = [
            MapCursor(10, -20, 4, MapCursor.Type.RED_MARKER, True, "hello"),
            MapCursor(-30, 40, 8, MapCursor.Type.BLUE_MARKER, True),
        ]
        record["cursors"] = [(c.x, c.y, c.direction, c.type.name, c.is_visible, c.caption) for c in canvas.cursors]


@scenario("api.player.map_canvas", budget=1500)
async def map_canvas(ctx: Context) -> None:
    bot, player = await apis_join(ctx, "apiCanvas")
    await ctx.ticks(20)
    record: dict = {}
    view, item = map_item(ctx, player, DrawRenderer(record))
    since = await apis_mark(bot)
    player.inventory.add_item(item)
    found = await wait_for(
        ctx,
        bot,
        "ClientboundMapItemDataPacket",
        since,
        lambda f: any(len(p["map_pixels"] or []) == 128 * 128 and p["map_pixels"][0] for p in f),
        "the drawn map",
    )
    assert record["view_id"] == (view.id, view.id), record["view_id"]
    assert record["get_pixel"] == 0xFF0A0B0C, record
    assert record["pixel_color"] == (12, 11, 10, 255), record["pixel_color"]
    assert record["outside"] == (0, 0, 0, 0), record["outside"]
    assert record["untouched"] == 0
    assert record["cursors"] == [
        (10, -20, 4, "RED_MARKER", True, "hello"),
        (-30, 40, 8, "BLUE_MARKER", True, ""),
    ], record["cursors"]
    full = next(p for p in found if len(p["map_pixels"] or []) == 128 * 128 and p["map_pixels"][0])
    pixels = full["map_pixels"]

    def at(x: int, y: int) -> int:
        return pixels[y * 128 + x]

    assert at(0, 0) == 0xFF0A0B0C, hex(at(0, 0))
    inside = abgr(1, 2, 3)
    assert {at(x, y) for x in range(10, 13) for y in range(20, 24)} == {inside}
    assert at(13, 20) == 0 and at(9, 20) == 0 and at(10, 24) == 0 and at(10, 19) == 0
    clipped = abgr(4, 5, 6)
    assert {at(x, y) for x in (126, 127) for y in (126, 127)} == {clipped}
    assert {at(x, y) for x in (0, 1) for y in range(60, 64)} == {clipped}
    assert at(2, 60) == 0 and at(0, 59) == 0 and at(0, 64) == 0
    assert at(2, 2) == 0 and at(5, 0) == 0 and at(0, 5) == 0 and at(127, 5) == 0 and at(5, 127) == 0
    ctx.info["decorations"] = {k: v for k, v in full.items() if k != "map_pixels"}
    decorations = full.get("decorations") or []
    assert len(decorations) == 2, ctx.info["decorations"]


@scenario("api.player.send_map", budget=1500)
async def send_map(ctx: Context) -> None:
    bot, player = await apis_join(ctx, "apiSendMap")
    watcher, other = await apis_join(ctx, "apiSendMapW")
    await ctx.ticks(20)
    record: dict = {}
    view = ctx.server.create_map(player.dimension)
    for existing in view.renderers:
        view.remove_renderer(existing)
    view.add_renderer(DrawRenderer(record))
    since, seen = await apis_mark(bot), await apis_mark(watcher)
    player.send_map(view)
    found = await wait_for(
        ctx,
        bot,
        "ClientboundMapItemDataPacket",
        since,
        lambda f: any(p["map_id"] == view.id for p in f),
        "the map sent to the player",
    )
    ours = [p for p in found if p["map_id"] == view.id]
    ctx.info["send_map"] = [{k: v for k, v in p.items() if k != "map_pixels"} for p in ours]
    full = [p for p in ours if len(p["map_pixels"] or []) == 128 * 128]
    assert full, ctx.info["send_map"]
    assert full[-1]["width"] == 128 and full[-1]["height"] == 128 and full[-1]["dimension"] == 0
    assert full[-1]["map_pixels"][0] == 0xFF0A0B0C, "send_map sent the map without rendering it first"
    assert record.get("rendered"), "send_map never ran the renderers"
    other.send_message("@@send map barrier")
    await wait_for(
        ctx,
        watcher,
        "TextPacket",
        seen,
        lambda f: any(t["body"].get("message") == "@@send map barrier" for t in f),
        "the barrier",
    )
    assert not [p for p in await got(watcher, "ClientboundMapItemDataPacket", seen) if p["map_id"] == view.id]


@scenario("api.player.move_from", plot=True, budget=BUDGET)
async def move_from(ctx: Context) -> None:
    bot, player, start = await join(ctx, "apiMoveFrom", dx=3, dz=3)
    x, y, z = ctx.plot.pos(12, 0, 12)
    away = Location(ctx.overworld, x + 0.5, y, z + 0.5, 0.0, 90.0)

    def redirect(event) -> None:
        event.from_location = away
        event.cancel()

    moves = ctx.capture(PlayerMoveEvent, mine("apiMoveFrom"), mutate=redirect)
    await bot.call("local_player.move_to", [start[0] + 6, start[1], start[2]])
    await ctx.until(lambda: moves.count >= 1, timeout=100, what="PlayerMoveEvent")
    original = moves.events[0]["from_location"]
    origin = (original["x"], original["y"], original["z"])
    assert moves.events[0]["after"]["from_location"]["x"] == away.x
    await ctx.until(
        lambda: flat(where(player), origin) < 0.5,
        timeout=40,
        what="the cancelled move to return the player to the original from_location",
    )
    await bot.call("local_player.stop_moving")
    assert flat(where(player), (away.x, away.y, away.z)) > 3.0
    seen = await poll(
        ctx,
        lambda: bot_position(bot),
        lambda p: flat(p, origin) < 0.5,
        what="the client to be moved back to the original from_location",
    )
    assert abs(seen[1] - origin[1]) < 0.1, seen


@scenario("api.player.playerset", budget=1500)
async def playerset(ctx: Context) -> None:
    def work(event) -> None:
        a, b, c = (ctx.server.get_player(f"ApiSet{n}") for n in "ABC")
        recipients = event.recipients
        everyone = {a, b, c}
        assert recipients >= everyone and recipients >= {a}
        assert not recipients > everyone and recipients > {a, b}
        assert recipients >= set() and recipients > set()
        assert not recipients.isdisjoint({a}) and recipients.isdisjoint(set())
        assert not recipients.isdisjoint({b, c})
        diff = recipients.symmetric_difference({b})
        assert diff == {a, c} and isinstance(diff, set)
        assert recipients.symmetric_difference(set()) == everyone
        assert len(recipients) == 3 and b in recipients
        for bad in (5, "abc"):
            for compare in (lambda r: r > bad, lambda r: r >= bad):
                try:
                    compare(recipients)
                except TypeError:
                    pass
                else:
                    raise AssertionError(f"comparing a PlayerSet with {bad!r} did not raise TypeError")
        recipients ^= {b}
        assert recipients is not None and b not in recipients and len(recipients) == 2
        assert event.recipients >= {a, c} and not event.recipients > {a, c} and event.recipients > {a}
        assert event.recipients.isdisjoint({b}) and not event.recipients.isdisjoint({b, c})
        assert event.recipients.symmetric_difference({b}) == everyone

    chat = ctx.capture(PlayerChatEvent, named("ApiSetA"), mutate=work)
    a, b, c = await three(ctx, "ApiSet")
    since = {bot: await chat_mark(bot) for bot in (a, b, c)}
    await a.call("local_player.chat", "not for b")
    await ctx.until(lambda: chat.count, timeout=40, what="PlayerChatEvent")
    after = await barrier(ctx, since)
    assert chat.error is None, chat.error
    expected = ("chat", "ApiSetA", "not for b")
    assert after == {"ApiSetA": [expected], "ApiSetB": [], "ApiSetC": [expected]}, after
    assert sorted(p["name"] for p in chat.events[0]["after"]["recipients"]) == ["ApiSetA", "ApiSetC"]
