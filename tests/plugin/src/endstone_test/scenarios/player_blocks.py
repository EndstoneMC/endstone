import functools
import json
import math

from endstone.block import Side
from endstone.event import (
    ActorSpawnEvent,
    BlockBreakEvent,
    BlockExplodeEvent,
    BlockPlaceEvent,
    CauldronLevelChangeEvent,
    PlayerBedEnterEvent,
    PlayerBedLeaveEvent,
    PlayerBucketEmptyEvent,
    PlayerBucketFillEvent,
    PlayerInteractEvent,
    PlayerOpenSignEvent,
)
from endstone.inventory import ItemStack
from endstone.level import Location

from endstone_test.harness import Context, scenario

BUDGET = 900
FACE_OFFSETS = {0: (0, -1, 0), 1: (0, 1, 0), 2: (0, 0, -1), 3: (0, 0, 1), 4: (-1, 0, 0), 5: (1, 0, 0)}
FACE_NAMES = {0: "DOWN", 1: "UP", 2: "NORTH", 3: "SOUTH", 4: "WEST", 5: "EAST"}


def trimmed(fn):
    @functools.wraps(fn)
    async def run(ctx: Context) -> None:
        try:
            await fn(ctx)
        finally:
            for capture in ctx.captures:
                for event in capture.events:
                    for view in (event, event.get("after") or {}):
                        if isinstance(view.get("block_list"), list):
                            view["block_list"] = view["block_list"][:5]

    return run


def flat(a, b) -> float:
    return math.hypot(a[0] - b[0], a[2] - b[2])


async def poll(ctx: Context, read, ok, timeout: int = 100, what: str = "a condition"):
    value = None
    for _ in range(timeout):
        value = await read()
        if ok(value):
            return value
        await ctx.ticks(1)
    raise TimeoutError(f"timed out after {timeout} polls waiting for {what}: last {value!r}")


async def join(ctx: Context, name: str, mode: str = "survival", dx: int = 4, dz: int = 8):
    bot = await ctx.bot(name)
    player = ctx.server.get_player(name)
    assert player is not None, f"{name} is not online"
    ctx.run(f"gamemode {mode} {name}")
    x, y, z = ctx.plot.pos(dx, 0, dz)
    spot = (x + 0.5, y, z + 0.5)
    ctx.run(f"tp {name} {spot[0]} {spot[1]} {spot[2]}")
    await ctx.until(
        lambda: flat((player.location.x, 0, player.location.z), spot) < 0.1,
        timeout=60,
        what=f"{name} to arrive in the plot",
    )
    await poll(
        ctx,
        lambda: bot.call("local_player.get_position"),
        lambda p: flat(p, spot) < 0.1,
        what=f"{name}'s client to see the teleport",
    )
    await ctx.ticks(10)
    return bot, player


def mine(name: str):
    return lambda e: e.player.name == name


async def mark(bot) -> int:
    return await bot.call("get_next_packet_index")


async def packets(bot, name: str, since: int) -> list[dict]:
    return await bot.call("get_received_packets", name, since=since)


def xyz(p: dict) -> tuple[int, int, int]:
    return int(p["x"]), int(p["y"]), int(p["z"])


def updates_at(found: list[dict], at: tuple[int, int, int]) -> list[int]:
    return [p["packet"]["runtime_id"] for p in found if xyz(p["packet"]["pos"]) == at and p["packet"]["layer"] == 0]


def runtime_at(ctx: Context, at: tuple[int, int, int]) -> int:
    return ctx.overworld.get_block_at(*at).data.runtime_id


def type_at(ctx: Context, at: tuple[int, int, int]) -> str:
    return str(ctx.overworld.get_block_at(*at).type)


async def sent_block(ctx: Context, bot, since: int, at: tuple[int, int, int], runtime_id: int, what: str) -> None:
    await poll(
        ctx,
        lambda: packets(bot, "UpdateBlockPacket", since),
        lambda found: runtime_id in updates_at(found, at),
        what=what,
    )


async def seen_by(ctx: Context, bot, at: tuple[int, int, int]) -> None:
    want = runtime_at(ctx, at)
    await poll(ctx, lambda: bot.call("get_block", list(at)), lambda r: r == want, what=f"the client to see the block at {at}")


async def client_slot(bot, slot: int) -> tuple[str, int]:
    held = (await bot.call("local_player.get_inventory_json"))["inventory"]["slots"][slot]
    return held["id"], held["count"]


async def give(ctx: Context, bot, name: str, item: str, amount: int = 1, slot: int = 0, data: int | None = None) -> None:
    ctx.run(f"replaceitem entity {name} slot.hotbar {slot} {item} {amount}" + ("" if data is None else f" {data}"))
    full = item if ":" in item else f"minecraft:{item}"
    await poll(
        ctx,
        lambda: client_slot(bot, slot),
        lambda s: s == (full, amount),
        what=f"{name}'s client to hold {amount} {full} in slot {slot}",
    )


def server_slot(player, slot: int) -> tuple[str, int]:
    item = player.inventory.get_item(slot)
    return ("minecraft:air", 0) if item is None else (str(item.type), item.amount)


def drops(ctx: Context) -> list:
    return ctx.capture(ActorSpawnEvent, lambda e: str(e.actor.type) == "minecraft:item")


@scenario(
    "blocks.break.creative.normal",
    covers={BlockBreakEvent: "normal"},
    plot=True,
    budget=BUDGET,
)
async def break_creative_normal(ctx: Context) -> None:
    bot, player = await join(ctx, "bkBreakC", mode="creative")
    at = ctx.plot.pos(6, 0, 8)
    ctx.run(f"setblock {at[0]} {at[1]} {at[2]} stone")
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:stone", timeout=20, what="the stone")
    await seen_by(ctx, bot, at)
    stone = runtime_at(ctx, at)
    hits = ctx.capture(BlockBreakEvent, mine("bkBreakC"))
    index = await mark(bot)
    await bot.call("game_mode.destroy_block", list(at), 4)
    await ctx.until(lambda: hits.count, timeout=40, what="BlockBreakEvent")
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:air", timeout=40, what="the server to remove the block")
    await ctx.ticks(5)
    assert hits.count == 1, hits.events
    block = hits.events[0]["block"]
    assert (block["x"], block["y"], block["z"]) == at, block
    assert block["type"] == "minecraft:stone", "the event named the block after it was already gone"
    air = runtime_at(ctx, at)
    assert air != stone
    await sent_block(ctx, bot, index, at, air, "the client to be told the block is gone")
    assert await bot.call("get_block", list(at)) == air


@scenario(
    "blocks.break.creative.cancel",
    covers={BlockBreakEvent: "cancel"},
    plot=True,
    budget=BUDGET,
)
async def break_creative_cancel(ctx: Context) -> None:
    bot, player = await join(ctx, "bkBreakCN", mode="creative")
    at = ctx.plot.pos(6, 0, 8)
    ctx.run(f"setblock {at[0]} {at[1]} {at[2]} stone")
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:stone", timeout=20, what="the stone")
    await seen_by(ctx, bot, at)
    stone = runtime_at(ctx, at)
    spawned = drops(ctx)
    hits = ctx.capture(BlockBreakEvent, mine("bkBreakCN"), cancel=True)
    index = await mark(bot)
    await bot.call("game_mode.destroy_block", list(at), 4)
    await ctx.until(lambda: hits.count, timeout=40, what="BlockBreakEvent")
    await ctx.ticks(20)
    assert type_at(ctx, at) == "minecraft:stone", "a cancelled break still removed the block"
    assert spawned.count == 0
    await sent_block(ctx, bot, index, at, stone, "the client to be given the block back")
    assert await bot.call("get_block", list(at)) == stone
    ctx.release(hits)
    del hits.events[3:]


@scenario(
    "blocks.break.survival.normal",
    covers={BlockBreakEvent: "normal", PlayerInteractEvent: "normal"},
    plot=True,
    budget=BUDGET,
)
async def break_survival_normal(ctx: Context) -> None:
    bot, player = await join(ctx, "bkBreakS")
    at = ctx.plot.pos(6, 0, 8)
    ctx.run(f"setblock {at[0]} {at[1]} {at[2]} dirt")
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:dirt", timeout=20, what="the dirt")
    await seen_by(ctx, bot, at)
    clicks = ctx.capture(PlayerInteractEvent, mine("bkBreakS"))
    hits = ctx.capture(BlockBreakEvent, mine("bkBreakS"))
    spawned = drops(ctx)
    index = await mark(bot)
    await bot.call("game_mode.start_destroy_block", list(at), 4)
    await ctx.until(lambda: clicks.count, timeout=40, what="PlayerInteractEvent")
    assert hits.count == 0, "the block broke in the same tick the player started"
    assert type_at(ctx, at) == "minecraft:dirt"
    await ctx.until(lambda: hits.count, timeout=200, what="BlockBreakEvent")
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:air", timeout=40, what="the server to remove the block")
    await ctx.until(lambda: spawned.count, timeout=40, what="the dirt drop")
    assert hits.count == 1, hits.events
    block = hits.events[0]["block"]
    assert (block["x"], block["y"], block["z"]) == at and block["type"] == "minecraft:dirt", block
    air = runtime_at(ctx, at)
    await sent_block(ctx, bot, index, at, air, "the client to be told the block is gone")
    assert await bot.call("get_block", list(at)) == air
    left = [e for e in clicks.events if e["action"] == "LEFT_CLICK_BLOCK"]
    assert len(left) == 1, clicks.events


@scenario(
    "blocks.break.survival.cancel",
    covers={BlockBreakEvent: "cancel"},
    plot=True,
    budget=BUDGET,
)
async def break_survival_cancel(ctx: Context) -> None:
    bot, player = await join(ctx, "bkBreakSN")
    at = ctx.plot.pos(6, 0, 8)
    ctx.run(f"setblock {at[0]} {at[1]} {at[2]} dirt")
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:dirt", timeout=20, what="the dirt")
    await seen_by(ctx, bot, at)
    dirt = runtime_at(ctx, at)
    spawned = drops(ctx)
    hits = ctx.capture(BlockBreakEvent, mine("bkBreakSN"), cancel=True)
    await mark(bot)
    await bot.call("game_mode.start_destroy_block", list(at), 4)
    await ctx.until(lambda: hits.count, timeout=200, what="BlockBreakEvent")
    await ctx.ticks(20)
    assert type_at(ctx, at) == "minecraft:dirt", "a cancelled break still removed the block"
    assert spawned.count == 0, "a cancelled break still dropped the block"
    assert await bot.call("get_block", list(at)) == dirt
    assert runtime_at(ctx, at) == dirt
    ctx.release(hits)
    del hits.events[3:]


@scenario("blocks.break.survival.cancel_predicted", covers={BlockBreakEvent: "cancel"}, plot=True, budget=BUDGET)
async def break_survival_cancel_predicted(ctx: Context) -> None:
    bot, player = await join(ctx, "bkBreakSP")
    at = ctx.plot.pos(6, 0, 8)
    ctx.run(f"setblock {at[0]} {at[1]} {at[2]} dirt")
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:dirt", timeout=20, what="the dirt")
    await seen_by(ctx, bot, at)
    dirt = runtime_at(ctx, at)
    spawned = drops(ctx)
    hits = ctx.capture(BlockBreakEvent, mine("bkBreakSP"), cancel=True)
    index = await mark(bot)
    await bot.call("game_mode.destroy_block", list(at), 4)
    await ctx.until(lambda: hits.count, timeout=200, what="BlockBreakEvent")
    await ctx.ticks(20)
    assert type_at(ctx, at) == "minecraft:dirt", "a cancelled break still removed the block"
    assert spawned.count == 0, "a cancelled break still dropped the block"
    await sent_block(ctx, bot, index, at, dirt, "the client to be given the block back")
    assert await bot.call("get_block", list(at)) == dirt
    ctx.release(hits)
    del hits.events[3:]


@scenario(
    "blocks.interact.left_click",
    covers={PlayerInteractEvent: ("normal", "cancel")},
    plot=True,
    budget=BUDGET,
)
async def interact_left_click(ctx: Context) -> None:
    bot, player = await join(ctx, "bkLeft")
    at = ctx.plot.pos(6, 0, 8)
    ctx.run(f"setblock {at[0]} {at[1]} {at[2]} dirt")
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:dirt", timeout=20, what="the dirt")
    await seen_by(ctx, bot, at)
    await give(ctx, bot, "bkLeft", "stick", 1, slot=0)
    gate = {"on": False}
    clicks = ctx.capture(
        PlayerInteractEvent,
        mine("bkLeft"),
        mutate=lambda e: e.cancel() if gate["on"] else None,
    )
    hits = ctx.capture(BlockBreakEvent, mine("bkLeft"))
    await bot.call("game_mode.start_destroy_block", list(at), 4)
    await ctx.until(lambda: clicks.count, timeout=40, what="PlayerInteractEvent")
    first = clicks.events[0]
    assert first["action"] == "LEFT_CLICK_BLOCK", first
    assert (first["block"]["x"], first["block"]["y"], first["block"]["z"]) == at, first["block"]
    assert first["block"]["type"] == "minecraft:dirt"
    assert first["block_face"] == "WEST", first["block_face"]
    assert first["has_item"] and first["item"] == {"type": "minecraft:stick", "amount": 1}, first["item"]
    ctx.info["clicked_position"] = first["clicked_position"]
    await ctx.until(lambda: hits.count, timeout=200, what="the dirt to break")
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:air", timeout=40, what="the break")
    other = ctx.plot.pos(6, 0, 10)
    ctx.run(f"setblock {other[0]} {other[1]} {other[2]} dirt")
    await ctx.until(lambda: type_at(ctx, other) == "minecraft:dirt", timeout=20, what="the second dirt")
    await seen_by(ctx, bot, other)
    gate["on"] = True
    broken = hits.count
    shown = clicks.count
    await bot.call("game_mode.start_destroy_block", list(other), 4)
    await ctx.until(lambda: clicks.count > shown, timeout=40, what="the cancelled LEFT_CLICK_BLOCK")
    await ctx.ticks(60)
    assert hits.count == broken, "a cancelled left click still started the break"
    assert type_at(ctx, other) == "minecraft:dirt"
    gate["on"] = False
    await bot.call("game_mode.stop_destroy_block")
    await ctx.ticks(5)
    await bot.call("game_mode.start_destroy_block", list(other), 4)
    await ctx.until(lambda: hits.count > broken, timeout=200, what="the break once the click is allowed again")


@scenario(
    "blocks.interact.right_click_block",
    covers={PlayerInteractEvent: ("normal", "cancel")},
    plot=True,
    budget=BUDGET,
)
async def interact_right_click_block(ctx: Context) -> None:
    bot, player = await join(ctx, "bkRight")
    x, y, z = ctx.plot.pos(6, 0, 8)
    ctx.run(f'setblock {x} {y} {z} wooden_door ["upper_block_bit"=false,"open_bit"=false]')
    await ctx.ticks(2)
    if type_at(ctx, (x, y + 1, z)) != "minecraft:wooden_door":
        ctx.run(f'setblock {x} {y + 1} {z} wooden_door ["upper_block_bit"=true]')
    low = (x, y, z)
    await ctx.until(lambda: type_at(ctx, low) == "minecraft:wooden_door", timeout=20, what="the door")
    closed = runtime_at(ctx, low)
    assert ctx.overworld.get_block_at(*low).data.block_states["open_bit"] is False
    gate = {"on": False}
    clicks = ctx.capture(
        PlayerInteractEvent,
        mine("bkRight"),
        mutate=lambda e: e.cancel() if gate["on"] else None,
    )
    await poll(ctx, lambda: bot.call("get_block", list(low)), lambda r: r == closed, what="the client to see the door")
    gate["on"] = True
    index = await mark(bot)
    await bot.call("game_mode.use_item_on", list(low), 4)
    await ctx.until(lambda: clicks.count, timeout=40, what="PlayerInteractEvent")
    await ctx.ticks(20)
    ctx.info["cancelled_packets"] = await interesting(bot, index)
    assert clicks.count == 1, clicks.events
    assert ctx.overworld.get_block_at(*low).data.block_states["open_bit"] is False, "a cancelled click opened the door"
    assert runtime_at(ctx, low) == closed
    assert await bot.call("get_block", list(low)) == closed
    gate["on"] = False
    index = await mark(bot)
    await bot.call("game_mode.use_item_on", list(low), 4)
    await ctx.until(lambda: clicks.count == 2, timeout=40, what="the second PlayerInteractEvent")
    await ctx.until(
        lambda: ctx.overworld.get_block_at(*low).data.block_states["open_bit"] is True,
        timeout=40,
        what="the door to open",
    )
    event = clicks.events[1]
    assert event["action"] == "RIGHT_CLICK_BLOCK", event
    assert (event["block"]["x"], event["block"]["y"], event["block"]["z"]) == low, event["block"]
    assert event["block"]["type"] == "minecraft:wooden_door"
    assert event["block_face"] == "WEST", event["block_face"]
    assert event["has_item"] is False and event["item"] is None, event["item"]
    ctx.info["clicked_position"] = event["clicked_position"]
    opened = runtime_at(ctx, low)
    assert opened != closed
    await poll(
        ctx,
        lambda: bot.call("get_block", list(low)),
        lambda r: r == opened,
        what="the client to see the door open",
    )
    assert clicks.count == 2, "the right click raised more than one PlayerInteractEvent"


@scenario(
    "blocks.interact.right_click_air",
    covers={PlayerInteractEvent: ("normal", "cancel")},
    plot=True,
    budget=BUDGET,
)
async def interact_right_click_air(ctx: Context) -> None:
    bot, player = await join(ctx, "bkAir")
    await give(ctx, bot, "bkAir", "snowball", 8)
    gate = {"on": False}
    clicks = ctx.capture(
        PlayerInteractEvent,
        mine("bkAir"),
        mutate=lambda e: e.cancel() if gate["on"] else None,
    )
    thrown = ctx.capture(ActorSpawnEvent, lambda e: str(e.actor.type) == "minecraft:snowball")
    gate["on"] = True
    await bot.call("game_mode.use_item")
    await ctx.until(lambda: clicks.count, timeout=40, what="the cancelled PlayerInteractEvent")
    await ctx.ticks(30)
    assert thrown.count == 0, "a cancelled right click still threw the snowball"
    assert server_slot(player, 0) == ("minecraft:snowball", 8)
    gate["on"] = False
    await bot.call("game_mode.use_item")
    await ctx.until(lambda: thrown.count, timeout=60, what="the snowball once the click is allowed")
    assert clicks.count == 2, clicks.events
    for event in clicks.events:
        assert event["action"] == "RIGHT_CLICK_AIR", event
        assert event["block"] is None and event["has_block"] is False, event["block"]
        assert event["has_item"] and event["item"]["type"] == "minecraft:snowball", event["item"]
        assert event["clicked_position"] is None, event["clicked_position"]
    ctx.info["block_face"] = clicks.events[0]["block_face"]
    await ctx.until(lambda: server_slot(player, 0) == ("minecraft:snowball", 7), timeout=40, what="the snowball to be spent")


@scenario("blocks.interact.left_click_position", plot=True, budget=BUDGET)
async def interact_left_click_position(ctx: Context) -> None:
    bot, player = await join(ctx, "bkLeftPos")
    at = ctx.plot.pos(6, 0, 8)
    ctx.run(f"setblock {at[0]} {at[1]} {at[2]} dirt")
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:dirt", timeout=20, what="the dirt")
    await seen_by(ctx, bot, at)
    clicks = ctx.capture(PlayerInteractEvent, mine("bkLeftPos"))
    await bot.call("game_mode.start_destroy_block", list(at), 4)
    await ctx.until(lambda: clicks.count, timeout=40, what="PlayerInteractEvent")
    event = clicks.events[0]
    assert event["action"] == "LEFT_CLICK_BLOCK", event
    assert event["clicked_position"] is None, (
        f"clicked_position is documented as None outside RIGHT_CLICK_BLOCK but was {event['clicked_position']}"
    )


@scenario("blocks.interact.right_click_position", plot=True, budget=BUDGET)
async def interact_right_click_position(ctx: Context) -> None:
    bot, player = await join(ctx, "bkRightPos")
    at = ctx.plot.pos(6, 0, 8)
    ctx.run(f"setblock {at[0]} {at[1]} {at[2]} stone")
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:stone", timeout=20, what="the stone")
    await seen_by(ctx, bot, at)
    clicks = ctx.capture(PlayerInteractEvent, mine("bkRightPos"))
    await bot.call("game_mode.use_item_on", list(at), 4, [0.0, 0.25, 0.75])
    await ctx.until(lambda: clicks.count, timeout=40, what="PlayerInteractEvent")
    event = clicks.events[0]
    assert event["action"] == "RIGHT_CLICK_BLOCK", event
    assert event["clicked_position"] == [0.0, 0.25, 0.75], (
        f"clicked_position is documented as the position within the block (0.0 to 1.0) but was {event['clicked_position']}"
    )


NOISE = (
    "BossEventPacket",
    "SubChunkPacket",
    "LevelChunkPacket",
    "CurrentStructureFeaturePacket",
    "NetworkChunkPublisherUpdatePacket",
    "ChunkRadiusUpdatedPacket",
)


async def interesting(bot, since: int) -> list[str]:
    return [p["name"] for p in await bot.call("get_received_packets", since=since) if p["name"] not in NOISE]


def coords(block: dict) -> tuple[int, int, int]:
    return block["x"], block["y"], block["z"]


@scenario("blocks.place.normal", covers={BlockPlaceEvent: "normal"}, plot=True, budget=BUDGET)
async def place_normal(ctx: Context) -> None:
    bot, player = await join(ctx, "bkPlace")
    await give(ctx, bot, "bkPlace", "stone", 5)
    floor = ctx.plot.pos(6, -1, 8)
    at = (floor[0], floor[1] + 1, floor[2])
    assert type_at(ctx, at) == "minecraft:air"
    placed = ctx.capture(BlockPlaceEvent, mine("bkPlace"))
    index = await mark(bot)
    await bot.call("game_mode.use_item_on", list(floor), 1)
    await ctx.until(lambda: placed.count, timeout=40, what="BlockPlaceEvent")
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:stone", timeout=40, what="the stone to appear")
    await ctx.ticks(5)
    assert placed.count == 1, placed.events
    event = placed.events[0]
    assert coords(event["block_placed"]) == at and event["block_placed"]["type"] == "minecraft:stone", event
    assert coords(event["block"]) == at, event["block"]
    assert coords(event["block_against"]) == floor and event["block_against"]["type"] == "minecraft:stone", event
    assert coords(event["block_replaced_state"]) == at and event["block_replaced_state"]["type"] == "minecraft:air"
    await sent_block(ctx, bot, index, at, runtime_at(ctx, at), "the client to be told about the stone")
    assert await bot.call("get_block", list(at)) == runtime_at(ctx, at)
    assert server_slot(player, 0) == ("minecraft:stone", 4)
    await poll(ctx, lambda: client_slot(bot, 0), lambda s: s == ("minecraft:stone", 4), what="the client's slot to show 4")


@scenario("blocks.place.cancel", covers={BlockPlaceEvent: "cancel"}, plot=True, budget=BUDGET)
async def place_cancel(ctx: Context) -> None:
    bot, player = await join(ctx, "bkPlaceN")
    await give(ctx, bot, "bkPlaceN", "stone", 5)
    floor = ctx.plot.pos(6, -1, 8)
    at = (floor[0], floor[1] + 1, floor[2])
    air = runtime_at(ctx, at)
    placed = ctx.capture(BlockPlaceEvent, mine("bkPlaceN"), cancel=True)
    index = await mark(bot)
    await bot.call("game_mode.use_item_on", list(floor), 1)
    await ctx.until(lambda: placed.count, timeout=40, what="BlockPlaceEvent")
    await ctx.ticks(20)
    assert type_at(ctx, at) == "minecraft:air", "a cancelled placement still placed the block"
    assert server_slot(player, 0) == ("minecraft:stone", 5), "a cancelled placement still spent the item"
    await sent_block(ctx, bot, index, at, air, "the client to be told the block was not placed")
    assert await bot.call("get_block", list(at)) == air
    assert await client_slot(bot, 0) == ("minecraft:stone", 5)
    ctx.info["packets"] = await interesting(bot, index)


@scenario("blocks.place.faces", covers={BlockPlaceEvent: "normal"}, plot=True, budget=BUDGET)
async def place_faces(ctx: Context) -> None:
    bot, player = await join(ctx, "bkFaces", mode="creative")
    await give(ctx, bot, "bkFaces", "stone", 1)
    cx, cy, cz = ctx.plot.pos(7, 2, 8)
    ctx.run(f"setblock {cx} {cy} {cz} glass")
    await ctx.until(lambda: type_at(ctx, (cx, cy, cz)) == "minecraft:glass", timeout=20, what="the glass")
    await seen_by(ctx, bot, (cx, cy, cz))
    placed = ctx.capture(BlockPlaceEvent, mine("bkFaces"))
    for face, (dx, dy, dz) in FACE_OFFSETS.items():
        target = (cx + dx, cy + dy, cz + dz)
        before = placed.count
        await bot.call("game_mode.use_item_on", [cx, cy, cz], face)
        await ctx.until(lambda: placed.count > before, timeout=40, what=f"BlockPlaceEvent for face {FACE_NAMES[face]}")
        await ctx.until(lambda: type_at(ctx, target) == "minecraft:stone", timeout=40, what="the stone")
        assert placed.count == before + 1, placed.events[before:]
        event = placed.events[before]
        assert coords(event["block_placed"]) == target, (FACE_NAMES[face], event["block_placed"], target)
        assert coords(event["block_against"]) == (cx, cy, cz), (FACE_NAMES[face], event["block_against"])
        assert event["block_against"]["type"] == "minecraft:glass"
        ctx.run(f"setblock {target[0]} {target[1]} {target[2]} air")
        await ctx.until(lambda: type_at(ctx, target) == "minecraft:air", timeout=20, what="the stone to go")
    assert placed.count == len(FACE_OFFSETS)


@scenario("blocks.place.replaced_state", covers={BlockPlaceEvent: "normal"}, plot=True, budget=BUDGET)
async def place_replaced_state(ctx: Context) -> None:
    bot, player = await join(ctx, "bkReplaced")
    await give(ctx, bot, "bkReplaced", "stone", 5)
    at = ctx.plot.pos(6, 0, 8)
    ctx.run(f"setblock {at[0]} {at[1]} {at[2]} short_grass")
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:short_grass", timeout=20, what="the grass")
    await seen_by(ctx, bot, at)
    seen = {}

    def inspect(event) -> None:
        state = event.block_replaced_state
        seen["before"] = (str(state.type), state.data.runtime_id)
        ctx.overworld.get_block_at(*at).set_type("minecraft:air")
        seen["after"] = (str(state.type), state.data.runtime_id)

    placed = ctx.capture(BlockPlaceEvent, mine("bkReplaced"), mutate=inspect)
    await bot.call("game_mode.use_item_on", list(at), 1)
    await ctx.until(lambda: placed.count, timeout=40, what="BlockPlaceEvent")
    ctx.info["seen"] = seen
    event = placed.events[0]
    assert event["block_replaced_state"]["type"] == "minecraft:short_grass", event["block_replaced_state"]
    assert coords(event["block_placed"]) == at, event["block_placed"]
    assert seen["before"][0] == "minecraft:short_grass"
    assert seen["after"] == seen["before"], (
        f"block_replaced_state follows the world instead of being a snapshot: {seen}"
    )


LIQUIDS = {"water": "minecraft:water_bucket", "lava": "minecraft:lava_bucket"}


def bucket_id(event) -> dict:
    return {"bucket_id": str(event.bucket.id)}


async def clear_liquid(ctx: Context, at: tuple[int, int, int]) -> None:
    x, y, z = at
    ctx.command(f"fill {x - 2} {y} {z - 2} {x + 2} {y + 1} {z + 2} air")
    await ctx.until(
        lambda: all(
            type_at(ctx, (x + dx, y, z + dz)) == "minecraft:air" for dx in range(-2, 3) for dz in range(-2, 3)
        ),
        timeout=40,
        what="the liquid to clear",
    )


async def with_liquid(ctx: Context, bot, name: str, liquid: str, count: int = 1) -> tuple[int, int, int]:
    at = ctx.plot.pos(6, 0, 8)
    await clear_liquid(ctx, at)
    ctx.run(f"setblock {at[0]} {at[1]} {at[2]} {liquid}")
    await ctx.until(lambda: type_at(ctx, at) == f"minecraft:{liquid}", timeout=20, what=f"the {liquid}")
    await seen_by(ctx, bot, at)
    await give(ctx, bot, name, "bucket", count)
    return at


@scenario(
    "blocks.bucket.fill.normal",
    covers={PlayerBucketFillEvent: "normal"},
    plot=True,
    budget=BUDGET,
)
async def bucket_fill_normal(ctx: Context) -> None:
    bot, player = await join(ctx, "bkFill")
    filled = ctx.capture(PlayerBucketFillEvent, mine("bkFill"), extra=bucket_id)
    for liquid, result in LIQUIDS.items():
        at = await with_liquid(ctx, bot, "bkFill", liquid)
        before = filled.count
        index = await mark(bot)
        await bot.call("game_mode.use_item_on", list(at), 1)
        await ctx.until(lambda: filled.count > before, timeout=40, what=f"PlayerBucketFillEvent for {liquid}")
        await ctx.until(lambda: server_slot(player, 0) == (result, 1), timeout=40, what=f"the {result}")
        await ctx.ticks(5)
        assert filled.count == before + 1, filled.events[before:]
        event = filled.events[before]
        assert coords(event["block"]) == at and event["block"]["type"] == f"minecraft:{liquid}", event["block"]
        assert coords(event["block_clicked"]) == at, event["block_clicked"]
        assert event["block_face"] == "UP", event["block_face"]
        assert event["bucket_id"] == "minecraft:bucket", event["bucket_id"]
        assert event["hand"] == "HAND", event["hand"]
        assert event["item_stack"] == {"type": result, "amount": 1}, event["item_stack"]
        assert type_at(ctx, at) == "minecraft:air", f"the {liquid} source is still there"
        await sent_block(ctx, bot, index, at, runtime_at(ctx, at), f"the client to be told the {liquid} is gone")
        await poll(ctx, lambda: client_slot(bot, 0), lambda s: s == (result, 1), what=f"the client to hold the {result}")


def carried(player, item: str) -> int:
    return sum(i.amount for i in player.inventory.contents if i is not None and str(i.type) == item)


@scenario(
    "blocks.bucket.fill.creative",
    covers={PlayerBucketFillEvent: "normal"},
    plot=True,
    budget=BUDGET,
)
async def bucket_fill_creative(ctx: Context) -> None:
    bot, player = await join(ctx, "bkFillC", mode="creative")
    filled = ctx.capture(PlayerBucketFillEvent, mine("bkFillC"))
    at = await with_liquid(ctx, bot, "bkFillC", "water")
    counts = []
    for attempt in range(2):
        if attempt:
            ctx.run(f"setblock {at[0]} {at[1]} {at[2]} water")
            await ctx.until(lambda: type_at(ctx, at) == "minecraft:water", timeout=20, what="the water")
            await seen_by(ctx, bot, at)
        before = filled.count
        await bot.call("game_mode.use_item_on", list(at), 1)
        await ctx.until(lambda: filled.count > before, timeout=40, what="PlayerBucketFillEvent")
        await ctx.until(lambda: type_at(ctx, at) == "minecraft:air", timeout=40, what="the water to be taken")
        await ctx.ticks(10)
        counts.append(carried(player, "minecraft:water_bucket"))
        assert server_slot(player, 0) == ("minecraft:bucket", 1), server_slot(player, 0)
    assert counts == [1, 1], f"a creative player's water buckets after each fill: {counts}"


@scenario(
    "blocks.cauldron.bucket_fill_creative",
    covers={CauldronLevelChangeEvent: "normal", PlayerBucketFillEvent: "normal"},
    plot=True,
    budget=BUDGET,
)
async def cauldron_bucket_fill_creative(ctx: Context) -> None:
    bot, player = await join(ctx, "bkCauFC", mode="creative")
    changed = ctx.capture(CauldronLevelChangeEvent)
    at = await with_cauldron(ctx, bot, "bkCauFC", "bucket", level=6)
    counts = []
    for attempt in range(2):
        if attempt:
            ctx.run(f'setblock {at[0]} {at[1]} {at[2]} cauldron ["fill_level"=6,"cauldron_liquid"="water"]')
            await ctx.until(lambda: cauldron_state(ctx, at)[0] == 6, timeout=20, what="the refilled cauldron")
            await seen_by(ctx, bot, at)
        before = changed.count
        await bot.call("game_mode.use_item_on", list(at), 1)
        await ctx.until(lambda: changed.count > before, timeout=40, what="CauldronLevelChangeEvent")
        await ctx.until(lambda: cauldron_state(ctx, at)[0] == 0, timeout=40, what="the cauldron to empty")
        await ctx.ticks(10)
        counts.append(carried(player, "minecraft:water_bucket"))
        assert server_slot(player, 0) == ("minecraft:bucket", 1), server_slot(player, 0)
    assert counts == [1, 1], f"a creative player's water buckets after each cauldron fill: {counts}"


@scenario(
    "blocks.bucket.fill.cancel",
    covers={PlayerBucketFillEvent: "cancel"},
    plot=True,
    budget=BUDGET,
)
async def bucket_fill_cancel(ctx: Context) -> None:
    bot, player = await join(ctx, "bkFillN")
    filled = ctx.capture(PlayerBucketFillEvent, mine("bkFillN"), cancel=True)
    summary = {}
    for liquid in LIQUIDS:
        at = await with_liquid(ctx, bot, "bkFillN", liquid)
        source = runtime_at(ctx, at)
        before = filled.count
        index = await mark(bot)
        await bot.call("game_mode.use_item_on", list(at), 1)
        await ctx.until(lambda: filled.count > before, timeout=40, what=f"PlayerBucketFillEvent for {liquid}")
        await ctx.ticks(20)
        assert type_at(ctx, at) == f"minecraft:{liquid}", f"a cancelled fill still removed the {liquid}"
        assert runtime_at(ctx, at) == source
        assert server_slot(player, 0) == ("minecraft:bucket", 1), "a cancelled fill still changed the bucket"
        assert await client_slot(bot, 0) == ("minecraft:bucket", 1)
        found = [p for p in await bot.call("get_received_packets", since=index) if p["name"] not in NOISE]
        summary[liquid] = sorted({p["name"] for p in found})
        assert source in updates_at([p for p in found if p["name"] == "UpdateBlockPacket"], at), (
            f"the client was not given the {liquid} back; it was sent {summary[liquid]}"
        )
    ctx.info["packets"] = summary


@scenario(
    "blocks.bucket.fill.mutate",
    covers={PlayerBucketFillEvent: "mutate"},
    plot=True,
    budget=BUDGET,
)
async def bucket_fill_mutate(ctx: Context) -> None:
    bot, player = await join(ctx, "bkFillM")
    filled = ctx.capture(
        PlayerBucketFillEvent,
        mine("bkFillM"),
        mutate=lambda e: setattr(e, "item_stack", ItemStack("minecraft:diamond", 2)),
    )
    at = await with_liquid(ctx, bot, "bkFillM", "water")
    await bot.call("game_mode.use_item_on", list(at), 1)
    await ctx.until(lambda: filled.count, timeout=40, what="PlayerBucketFillEvent")
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:air", timeout=40, what="the water to go")
    await ctx.until(lambda: server_slot(player, 0) == ("minecraft:diamond", 2), timeout=40, what="the diamonds")
    assert filled.events[0]["item_stack"] == {"type": "minecraft:water_bucket", "amount": 1}
    assert filled.events[0]["after"]["item_stack"] == {"type": "minecraft:diamond", "amount": 2}
    await poll(ctx, lambda: client_slot(bot, 0), lambda s: s == ("minecraft:diamond", 2), what="the client to hold the diamonds")
    others = [server_slot(player, i) for i in range(1, 36)]
    assert all(s == ("minecraft:air", 0) for s in others), f"the filled bucket ended up in another slot too: {others}"


@scenario(
    "blocks.bucket.fill.stack",
    covers={PlayerBucketFillEvent: "normal"},
    plot=True,
    budget=BUDGET,
)
async def bucket_fill_stack(ctx: Context) -> None:
    bot, player = await join(ctx, "bkFillS")
    filled = ctx.capture(PlayerBucketFillEvent, mine("bkFillS"))
    at = await with_liquid(ctx, bot, "bkFillS", "water", count=5)
    await bot.call("game_mode.use_item_on", list(at), 1)
    await ctx.until(lambda: filled.count, timeout=40, what="PlayerBucketFillEvent")
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:air", timeout=40, what="the water to go")
    await ctx.ticks(10)
    held = {(str(i.type), i.amount) for i in map(player.inventory.get_item, range(36)) if i is not None}
    ctx.info["inventory"] = sorted(held)
    totals = {}
    for kind, amount in held:
        totals[kind] = totals.get(kind, 0) + amount
    assert totals == {"minecraft:bucket": 4, "minecraft:water_bucket": 1}, held


@scenario(
    "blocks.bucket.fill.stack_mutate",
    covers={PlayerBucketFillEvent: "mutate"},
    plot=True,
    budget=BUDGET,
)
async def bucket_fill_stack_mutate(ctx: Context) -> None:
    bot, player = await join(ctx, "bkFillSM")
    filled = ctx.capture(
        PlayerBucketFillEvent,
        mine("bkFillSM"),
        mutate=lambda e: setattr(e, "item_stack", ItemStack("minecraft:diamond", 2)),
    )
    at = await with_liquid(ctx, bot, "bkFillSM", "water", count=5)
    await bot.call("game_mode.use_item_on", list(at), 1)
    await ctx.until(lambda: filled.count, timeout=40, what="PlayerBucketFillEvent")
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:air", timeout=40, what="the water to go")
    await ctx.ticks(10)
    held = [(str(i.type), i.amount) for i in map(player.inventory.get_item, range(36)) if i is not None]
    ctx.info["inventory"] = held
    totals = {}
    for kind, amount in held:
        totals[kind] = totals.get(kind, 0) + amount
    assert totals == {"minecraft:bucket": 4, "minecraft:diamond": 2}, (
        f"the four buckets that were not filled were lost to the replaced item: {held}"
    )


@scenario(
    "blocks.bucket.empty.normal",
    covers={PlayerBucketEmptyEvent: "normal"},
    plot=True,
    budget=BUDGET,
)
async def bucket_empty_normal(ctx: Context) -> None:
    bot, player = await join(ctx, "bkEmpty")
    emptied = ctx.capture(PlayerBucketEmptyEvent, mine("bkEmpty"), extra=bucket_id)
    floor = ctx.plot.pos(6, -1, 8)
    at = (floor[0], floor[1] + 1, floor[2])
    blocks = {}
    for liquid, held in LIQUIDS.items():
        await clear_liquid(ctx, at)
        await give(ctx, bot, "bkEmpty", held.removeprefix("minecraft:"), 1)
        before = emptied.count
        index = await mark(bot)
        await bot.call("game_mode.use_item_on", list(floor), 1)
        await ctx.until(lambda: emptied.count > before, timeout=40, what=f"PlayerBucketEmptyEvent for {liquid}")
        await ctx.until(lambda: type_at(ctx, at) == f"minecraft:{liquid}", timeout=40, what=f"the {liquid}")
        await ctx.until(lambda: server_slot(player, 0) == ("minecraft:bucket", 1), timeout=40, what="the empty bucket")
        assert emptied.count == before + 1, emptied.events[before:]
        event = emptied.events[before]
        blocks[liquid] = event["block"]
        assert coords(event["block_clicked"]) == floor and event["block_clicked"]["type"] == "minecraft:stone"
        assert event["block_face"] == "UP", event["block_face"]
        assert event["bucket_id"] == held, event["bucket_id"]
        assert event["hand"] == "HAND", event["hand"]
        assert event["item_stack"] == {"type": "minecraft:bucket", "amount": 1}, event["item_stack"]
        await sent_block(ctx, bot, index, at, runtime_at(ctx, at), f"the client to be told about the {liquid}")
        await poll(ctx, lambda: client_slot(bot, 0), lambda s: s == ("minecraft:bucket", 1), what="the client's empty bucket")
    ctx.info["blocks"] = blocks
    wrong = {k: v for k, v in blocks.items() if coords(v) != at or v["type"] != "minecraft:air"}
    assert not wrong, f"block should be the air the liquid flows into at {at}, but was {wrong}"


@scenario(
    "blocks.bucket.empty.cancel",
    covers={PlayerBucketEmptyEvent: "cancel"},
    plot=True,
    budget=BUDGET,
)
async def bucket_empty_cancel(ctx: Context) -> None:
    bot, player = await join(ctx, "bkEmptyN")
    emptied = ctx.capture(PlayerBucketEmptyEvent, mine("bkEmptyN"), cancel=True)
    floor = ctx.plot.pos(6, -1, 8)
    at = (floor[0], floor[1] + 1, floor[2])
    summary = {}
    for liquid, held in LIQUIDS.items():
        await clear_liquid(ctx, at)
        await give(ctx, bot, "bkEmptyN", held.removeprefix("minecraft:"), 1)
        air = runtime_at(ctx, at)
        before = emptied.count
        index = await mark(bot)
        await bot.call("game_mode.use_item_on", list(floor), 1)
        await ctx.until(lambda: emptied.count > before, timeout=40, what=f"PlayerBucketEmptyEvent for {liquid}")
        await ctx.ticks(20)
        assert type_at(ctx, at) == "minecraft:air", f"a cancelled empty still placed the {liquid}"
        assert server_slot(player, 0) == (held, 1), "a cancelled empty still changed the bucket"
        assert await client_slot(bot, 0) == (held, 1)
        found = [p for p in await bot.call("get_received_packets", since=index) if p["name"] not in NOISE]
        summary[liquid] = sorted({p["name"] for p in found})
        assert air in updates_at([p for p in found if p["name"] == "UpdateBlockPacket"], at), (
            f"the client was not told the {liquid} was not placed; it was sent {summary[liquid]}"
        )
    ctx.info["packets"] = summary


@scenario(
    "blocks.bucket.empty.mutate",
    covers={PlayerBucketEmptyEvent: "mutate"},
    plot=True,
    budget=BUDGET,
)
async def bucket_empty_mutate(ctx: Context) -> None:
    bot, player = await join(ctx, "bkEmptyM")
    emptied = ctx.capture(
        PlayerBucketEmptyEvent,
        mine("bkEmptyM"),
        mutate=lambda e: setattr(e, "item_stack", ItemStack("minecraft:diamond", 3)),
    )
    floor = ctx.plot.pos(6, -1, 8)
    at = (floor[0], floor[1] + 1, floor[2])
    await give(ctx, bot, "bkEmptyM", "water_bucket", 1)
    await bot.call("game_mode.use_item_on", list(floor), 1)
    await ctx.until(lambda: emptied.count, timeout=40, what="PlayerBucketEmptyEvent")
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:water", timeout=40, what="the water")
    await ctx.until(lambda: server_slot(player, 0) == ("minecraft:diamond", 3), timeout=40, what="the diamonds")
    assert emptied.events[0]["item_stack"] == {"type": "minecraft:bucket", "amount": 1}
    await poll(ctx, lambda: client_slot(bot, 0), lambda s: s == ("minecraft:diamond", 3), what="the client to hold the diamonds")


@scenario("blocks.bucket.empty.replaceable", covers={PlayerBucketEmptyEvent: "normal"}, plot=True, budget=BUDGET)
async def bucket_empty_replaceable(ctx: Context) -> None:
    bot, player = await join(ctx, "bkEmptyR")
    emptied = ctx.capture(PlayerBucketEmptyEvent, mine("bkEmptyR"))
    floor = ctx.plot.pos(6, -1, 8)
    grass = (floor[0], floor[1] + 1, floor[2])
    above = (grass[0], grass[1] + 1, grass[2])
    await clear_liquid(ctx, grass)
    ctx.run(f"setblock {grass[0]} {grass[1]} {grass[2]} short_grass")
    await ctx.until(lambda: type_at(ctx, grass) == "minecraft:short_grass", timeout=20, what="the grass")
    await seen_by(ctx, bot, grass)
    await give(ctx, bot, "bkEmptyR", "water_bucket", 1)
    await bot.call("game_mode.use_item_on", list(grass), 1)
    await ctx.until(lambda: emptied.count, timeout=40, what="PlayerBucketEmptyEvent")
    await ctx.until(lambda: type_at(ctx, above) == "minecraft:water", timeout=40, what="the water above the grass")
    event = emptied.events[0]
    assert coords(event["block"]) == above and event["block"]["type"] == "minecraft:air", event["block"]
    assert coords(event["block_clicked"]) == grass, event["block_clicked"]
    assert event["block_clicked"]["type"] == "minecraft:short_grass", event["block_clicked"]
    assert event["block_face"] == "UP", event["block_face"]


@scenario("blocks.bucket.empty.slab", covers={PlayerBucketEmptyEvent: "normal"}, plot=True, budget=BUDGET)
async def bucket_empty_slab(ctx: Context) -> None:
    bot, player = await join(ctx, "bkEmptyW")
    emptied = ctx.capture(PlayerBucketEmptyEvent, mine("bkEmptyW"))
    floor = ctx.plot.pos(6, -1, 8)
    slab = (floor[0], floor[1] + 1, floor[2])
    above = (slab[0], slab[1] + 1, slab[2])
    await clear_liquid(ctx, slab)
    ctx.run(f"setblock {slab[0]} {slab[1]} {slab[2]} oak_slab")
    await ctx.until(lambda: type_at(ctx, slab) == "minecraft:oak_slab", timeout=20, what="the slab")
    await seen_by(ctx, bot, slab)
    await give(ctx, bot, "bkEmptyW", "water_bucket", 1)
    await bot.call("game_mode.use_item_on", list(slab), 1)
    await ctx.until(lambda: emptied.count, timeout=40, what="PlayerBucketEmptyEvent")
    await ctx.until(lambda: server_slot(player, 0) == ("minecraft:bucket", 1), timeout=40, what="the empty bucket")
    event = emptied.events[0]
    poured_above = type_at(ctx, above) == "minecraft:water"
    ctx.info["poured_above"] = poured_above
    ctx.info["block_type"] = event["block"]["type"]
    assert type_at(ctx, slab) == "minecraft:oak_slab", "the slab was replaced"
    assert coords(event["block_clicked"]) == slab, event["block_clicked"]
    want = above if poured_above else slab
    assert coords(event["block"]) == want, (
        f"the water went {'above' if poured_above else 'into'} the slab but block was {coords(event['block'])}"
    )


@scenario(
    "blocks.bucket.empty.cauldron_pass",
    covers={PlayerBucketEmptyEvent: ("cancel", "normal")},
    plot=True,
    budget=BUDGET,
)
async def bucket_empty_cauldron_pass(ctx: Context) -> None:
    bot, player = await join(ctx, "bkEmptyP")

    def at_cauldron(event) -> bool:
        return event.player.name == "bkEmptyP" and str(event.block.type) == "minecraft:cauldron"

    def beside_cauldron(event) -> bool:
        return event.player.name == "bkEmptyP" and str(event.block.type) != "minecraft:cauldron"

    cancelled = ctx.capture(PlayerBucketEmptyEvent, at_cauldron, cancel=True)
    passed = ctx.capture(PlayerBucketEmptyEvent, beside_cauldron)
    changed = ctx.capture(CauldronLevelChangeEvent)
    at = await with_cauldron(ctx, bot, "bkEmptyP", "water_bucket", level=3)
    above = (at[0], at[1] + 1, at[2])
    await bot.call("game_mode.use_item_on", list(at), 1)
    await ctx.until(lambda: cancelled.count, timeout=40, what="the cancelled PlayerBucketEmptyEvent")
    await ctx.until(lambda: passed.count, timeout=40, what="the PlayerBucketEmptyEvent for the block above")
    await ctx.until(lambda: type_at(ctx, above) == "minecraft:water", timeout=40, what="the water above the cauldron")
    assert changed.count == 0, "a cancelled bucket event still changed the cauldron"
    assert cauldron_state(ctx, at) == (3, "water"), cauldron_state(ctx, at)
    first, second = cancelled.events[0], passed.events[0]
    assert coords(first["block"]) == at and coords(first["block_clicked"]) == at, first
    assert coords(second["block"]) == above and second["block"]["type"] == "minecraft:air", second["block"]
    assert coords(second["block_clicked"]) == at and second["block_clicked"]["type"] == "minecraft:cauldron", second
    assert second["block_face"] == "UP", second["block_face"]
    await ctx.until(lambda: server_slot(player, 0) == ("minecraft:bucket", 1), timeout=40, what="the empty bucket")


@scenario(
    "blocks.bucket.milk",
    covers={PlayerBucketFillEvent: ("normal", "cancel", "mutate")},
    plot=True,
    budget=BUDGET,
)
async def bucket_milk(ctx: Context) -> None:
    bot, player = await join(ctx, "bkMilk")
    gate = {"mode": "pass"}

    def decide(event) -> None:
        if gate["mode"] == "cancel":
            event.cancel()
        elif gate["mode"] == "mutate":
            event.item_stack = ItemStack("minecraft:diamond", 2)

    filled = ctx.capture(PlayerBucketFillEvent, mine("bkMilk"), mutate=decide, extra=bucket_id)
    x, y, z = ctx.plot.pos(6, 0, 8)
    ctx.run(f"summon cow {x + 0.5} {y} {z + 0.5}")
    ctx.run("effect @e[type=cow,c=1] slowness 120 255 true")
    cows = await poll(
        ctx, lambda: bot.call("get_actors", "minecraft:cow"), lambda found: found, what="the client to see the cow"
    )
    cow = cows[0]["runtime_id"]
    await give(ctx, bot, "bkMilk", "bucket", 1)
    gate["mode"] = "cancel"
    index = await mark(bot)
    await bot.call("game_mode.interact", cow)
    await ctx.until(lambda: filled.count, timeout=60, what="the cancelled PlayerBucketFillEvent")
    await ctx.ticks(20)
    assert server_slot(player, 0) == ("minecraft:bucket", 1), "a cancelled milking still filled the bucket"
    assert await client_slot(bot, 0) == ("minecraft:bucket", 1)
    resent = {p["name"] for p in await bot.call("get_received_packets", since=index)} & {
        "InventorySlotPacket",
        "InventoryContentPacket",
    }
    ctx.info["resent"] = sorted(resent)
    assert resent, "the client was not sent its inventory back after the cancelled milking"
    gate["mode"] = "pass"
    await ctx.ticks(30)
    before = filled.count
    await bot.call("game_mode.interact", cow)
    await ctx.until(lambda: filled.count > before, timeout=60, what="the PlayerBucketFillEvent for milk")
    await ctx.until(lambda: server_slot(player, 0) == ("minecraft:milk_bucket", 1), timeout=60, what="the milk")
    event = filled.events[before]
    assert event["block_face"] == "SELF", event["block_face"]
    assert event["hand"] == "HAND" and event["bucket_id"] == "minecraft:bucket", event
    assert event["item_stack"] == {"type": "minecraft:milk_bucket", "amount": 1}
    assert event["block"]["x"] == x and event["block"]["z"] == z, event["block"]
    await poll(ctx, lambda: client_slot(bot, 0), lambda s: s == ("minecraft:milk_bucket", 1), what="the client's milk")
    await give(ctx, bot, "bkMilk", "bucket", 1)
    await ctx.ticks(30)
    gate["mode"] = "mutate"
    before = filled.count
    await bot.call("game_mode.interact", cow)
    await ctx.until(lambda: filled.count > before, timeout=60, what="the mutated PlayerBucketFillEvent")
    await ctx.until(lambda: server_slot(player, 0) == ("minecraft:diamond", 2), timeout=60, what="the diamonds")
    await poll(ctx, lambda: client_slot(bot, 0), lambda s: s == ("minecraft:diamond", 2), what="the client's diamonds")


async def with_cauldron(ctx: Context, bot, name: str, item: str, count: int = 1, level: int = 0, liquid: str = "water"):
    at = ctx.plot.pos(6, 0, 8)
    ctx.command(f"setblock {at[0]} {at[1]} {at[2]} air")
    ctx.run(f'setblock {at[0]} {at[1]} {at[2]} cauldron ["fill_level"={level},"cauldron_liquid"="{liquid}"]')
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:cauldron", timeout=20, what="the cauldron")
    await seen_by(ctx, bot, at)
    await give(ctx, bot, name, item, count, data=0 if item == "potion" else None)
    return at


def cauldron_state(ctx: Context, at) -> tuple[int, str]:
    states = ctx.overworld.get_block_at(*at).data.block_states
    return states["fill_level"], states["cauldron_liquid"]


def new_state(event) -> dict:
    states = event.new_state.data.block_states
    return {
        "new_level": states["fill_level"],
        "new_liquid": states["cauldron_liquid"],
        "who": None if event.actor is None else event.actor.name,
    }


def stamped(order: list, label: str, extra=None):
    def run(event) -> dict:
        order.append(label)
        return {} if extra is None else extra(event)

    return run


@scenario(
    "blocks.cauldron.bucket_empty",
    covers={CauldronLevelChangeEvent: "normal", PlayerBucketEmptyEvent: "normal"},
    plot=True,
    budget=BUDGET,
)
async def cauldron_bucket_empty(ctx: Context) -> None:
    bot, player = await join(ctx, "bkCauE")
    order = []
    emptied = ctx.capture(PlayerBucketEmptyEvent, mine("bkCauE"), extra=stamped(order, "bucket", bucket_id))
    changed = ctx.capture(CauldronLevelChangeEvent, extra=stamped(order, "cauldron", new_state))
    at = await with_cauldron(ctx, bot, "bkCauE", "water_bucket")
    index = await mark(bot)
    await bot.call("game_mode.use_item_on", list(at), 1)
    await ctx.until(lambda: changed.count, timeout=40, what="CauldronLevelChangeEvent")
    await ctx.until(lambda: cauldron_state(ctx, at) == (6, "water"), timeout=40, what="the cauldron to fill")
    await ctx.until(lambda: server_slot(player, 0) == ("minecraft:bucket", 1), timeout=40, what="the empty bucket")
    assert order[0] == "bucket" and set(order[1:]) == {"cauldron"}, order
    assert emptied.count == 1, emptied.events
    bucket, change = emptied.events[0], changed.events[-1]
    assert coords(bucket["block"]) == at and coords(bucket["block_clicked"]) == at
    assert bucket["block"]["type"] == "minecraft:cauldron"
    assert bucket["block_face"] == "UP" and bucket["hand"] == "HAND", bucket
    assert bucket["bucket_id"] == "minecraft:water_bucket"
    assert bucket["item_stack"] == {"type": "minecraft:bucket", "amount": 1}
    assert change["reason"] == "BUCKET_EMPTY", change["reason"]
    assert change["who"] == "bkCauE", change["who"]
    assert coords(change["block"]) == at
    assert (change["new_level"], change["new_liquid"]) == (6, "water"), change
    await sent_block(ctx, bot, index, at, runtime_at(ctx, at), "the client to be told the cauldron filled")
    await poll(ctx, lambda: client_slot(bot, 0), lambda s: s == ("minecraft:bucket", 1), what="the client's empty bucket")


@scenario(
    "blocks.cauldron.bucket_fill",
    covers={CauldronLevelChangeEvent: "normal", PlayerBucketFillEvent: "normal"},
    plot=True,
    budget=BUDGET,
)
async def cauldron_bucket_fill(ctx: Context) -> None:
    bot, player = await join(ctx, "bkCauF")
    order = []
    filled = ctx.capture(PlayerBucketFillEvent, mine("bkCauF"), extra=stamped(order, "bucket", bucket_id))
    changed = ctx.capture(CauldronLevelChangeEvent, extra=stamped(order, "cauldron", new_state))
    at = await with_cauldron(ctx, bot, "bkCauF", "bucket", level=6)
    index = await mark(bot)
    await bot.call("game_mode.use_item_on", list(at), 1)
    await ctx.until(lambda: changed.count, timeout=40, what="CauldronLevelChangeEvent")
    await ctx.until(lambda: cauldron_state(ctx, at)[0] == 0, timeout=40, what="the cauldron to empty")
    await ctx.until(lambda: server_slot(player, 0) == ("minecraft:water_bucket", 1), timeout=40, what="the water bucket")
    assert order == ["bucket", "cauldron"], order
    bucket, change = filled.events[0], changed.events[0]
    assert coords(bucket["block"]) == at and bucket["block"]["type"] == "minecraft:cauldron"
    assert bucket["block_face"] == "UP" and bucket["hand"] == "HAND", bucket
    assert bucket["bucket_id"] == "minecraft:bucket"
    assert bucket["item_stack"] == {"type": "minecraft:water_bucket", "amount": 1}
    assert change["reason"] == "BUCKET_FILL" and change["who"] == "bkCauF", change
    assert change["new_level"] == 0, change
    await sent_block(ctx, bot, index, at, runtime_at(ctx, at), "the client to be told the cauldron emptied")
    await poll(ctx, lambda: client_slot(bot, 0), lambda s: s == ("minecraft:water_bucket", 1), what="the client's water bucket")


@scenario(
    "blocks.cauldron.bucket_cancel",
    covers={PlayerBucketEmptyEvent: "cancel", PlayerBucketFillEvent: "cancel"},
    plot=True,
    budget=BUDGET,
)
async def cauldron_bucket_cancel(ctx: Context) -> None:
    bot, player = await join(ctx, "bkCauN")
    emptied = ctx.capture(PlayerBucketEmptyEvent, mine("bkCauN"), cancel=True)
    filled = ctx.capture(PlayerBucketFillEvent, mine("bkCauN"), cancel=True)
    changed = ctx.capture(CauldronLevelChangeEvent)
    at = await with_cauldron(ctx, bot, "bkCauN", "water_bucket")
    index = await mark(bot)
    await bot.call("game_mode.use_item_on", list(at), 1)
    await ctx.until(lambda: emptied.count, timeout=40, what="PlayerBucketEmptyEvent")
    await ctx.ticks(20)
    assert cauldron_state(ctx, at)[0] == 0, "a cancelled empty still filled the cauldron"
    assert server_slot(player, 0) == ("minecraft:water_bucket", 1)
    assert await client_slot(bot, 0) == ("minecraft:water_bucket", 1)
    ctx.info["empty_packets"] = await interesting(bot, index)
    at = await with_cauldron(ctx, bot, "bkCauN", "bucket", level=6)
    index = await mark(bot)
    await bot.call("game_mode.use_item_on", list(at), 1)
    await ctx.until(lambda: filled.count, timeout=40, what="PlayerBucketFillEvent")
    await ctx.ticks(20)
    assert cauldron_state(ctx, at)[0] == 6, "a cancelled fill still emptied the cauldron"
    assert server_slot(player, 0) == ("minecraft:bucket", 1)
    assert await client_slot(bot, 0) == ("minecraft:bucket", 1)
    ctx.info["fill_packets"] = await interesting(bot, index)
    assert changed.count == 0, "a cancelled bucket event still changed the cauldron"


@scenario(
    "blocks.cauldron.bucket_mutate",
    covers={PlayerBucketEmptyEvent: "mutate", PlayerBucketFillEvent: "mutate"},
    plot=True,
    budget=BUDGET,
)
async def cauldron_bucket_mutate(ctx: Context) -> None:
    bot, player = await join(ctx, "bkCauM")

    def swap(event) -> None:
        event.item_stack = ItemStack("minecraft:diamond", 2)

    emptied = ctx.capture(PlayerBucketEmptyEvent, mine("bkCauM"), mutate=swap)
    filled = ctx.capture(PlayerBucketFillEvent, mine("bkCauM"), mutate=swap)
    at = await with_cauldron(ctx, bot, "bkCauM", "water_bucket")
    await bot.call("game_mode.use_item_on", list(at), 1)
    await ctx.until(lambda: emptied.count, timeout=40, what="PlayerBucketEmptyEvent")
    await ctx.until(lambda: cauldron_state(ctx, at) == (6, "water"), timeout=40, what="the cauldron to fill")
    await ctx.until(lambda: server_slot(player, 0) == ("minecraft:diamond", 2), timeout=40, what="the diamonds")
    await poll(ctx, lambda: client_slot(bot, 0), lambda s: s == ("minecraft:diamond", 2), what="the client's diamonds")
    at = await with_cauldron(ctx, bot, "bkCauM", "bucket", level=6)
    await bot.call("game_mode.use_item_on", list(at), 1)
    await ctx.until(lambda: filled.count, timeout=40, what="PlayerBucketFillEvent")
    await ctx.until(lambda: cauldron_state(ctx, at)[0] == 0, timeout=40, what="the cauldron to empty")
    await ctx.until(lambda: server_slot(player, 0) == ("minecraft:diamond", 2), timeout=40, what="the diamonds")
    await poll(ctx, lambda: client_slot(bot, 0), lambda s: s == ("minecraft:diamond", 2), what="the client's diamonds")


@scenario("blocks.cauldron.level_cancel", covers={CauldronLevelChangeEvent: "cancel"}, plot=True, budget=BUDGET)
async def cauldron_level_cancel(ctx: Context) -> None:
    bot, player = await join(ctx, "bkCauLN")
    changed = ctx.capture(CauldronLevelChangeEvent, cancel=True)
    at = await with_cauldron(ctx, bot, "bkCauLN", "water_bucket")
    index = await mark(bot)
    await bot.call("game_mode.use_item_on", list(at), 1)
    await ctx.until(lambda: changed.count, timeout=40, what="CauldronLevelChangeEvent")
    await ctx.ticks(20)
    assert cauldron_state(ctx, at)[0] == 0, "a cancelled change still filled the cauldron"
    assert server_slot(player, 0) == ("minecraft:water_bucket", 1), (
        f"the cauldron stayed empty but the player's bucket became {server_slot(player, 0)}"
    )
    assert await client_slot(bot, 0) == ("minecraft:water_bucket", 1)
    ctx.info["packets"] = await interesting(bot, index)


@scenario("blocks.cauldron.level_mutate", covers={CauldronLevelChangeEvent: "mutate"}, plot=True, budget=BUDGET)
async def cauldron_level_mutate(ctx: Context) -> None:
    bot, player = await join(ctx, "bkCauLM")

    def shrink(event) -> None:
        event.new_state.data = ctx.server.create_block_data(
            "minecraft:cauldron", {"fill_level": 3, "cauldron_liquid": "water"}
        )

    changed = ctx.capture(CauldronLevelChangeEvent, mutate=shrink, extra=new_state)
    at = await with_cauldron(ctx, bot, "bkCauLM", "water_bucket")
    await bot.call("game_mode.use_item_on", list(at), 1)
    await ctx.until(lambda: changed.count, timeout=40, what="CauldronLevelChangeEvent")
    await ctx.until(lambda: cauldron_state(ctx, at) != (0, "water"), timeout=40, what="the cauldron to change")
    await ctx.ticks(10)
    assert changed.events[-1]["new_level"] == 6
    assert cauldron_state(ctx, at) == (3, "water"), cauldron_state(ctx, at)
    await seen_by(ctx, bot, at)


@scenario("blocks.cauldron.single_event", covers={CauldronLevelChangeEvent: "normal"}, plot=True, budget=BUDGET)
async def cauldron_single_event(ctx: Context) -> None:
    bot, player = await join(ctx, "bkCauS")
    changed = ctx.capture(CauldronLevelChangeEvent, extra=new_state)
    at = await with_cauldron(ctx, bot, "bkCauS", "water_bucket")
    await bot.call("game_mode.use_item_on", list(at), 1)
    await ctx.until(lambda: cauldron_state(ctx, at) == (6, "water"), timeout=40, what="the cauldron to fill")
    await ctx.ticks(10)
    levels = [e["new_level"] for e in changed.events]
    assert levels == [6], f"one bucket raised {len(levels)} CauldronLevelChangeEvents, reporting the levels {levels}"


async def click_cauldron(ctx: Context, bot, at, ticks: int = 15) -> None:
    await bot.call("game_mode.use_item_on", list(at), 1)
    await ctx.ticks(ticks)


@scenario("blocks.cauldron.bottle_fill", covers={CauldronLevelChangeEvent: "normal"}, plot=True, budget=BUDGET)
async def cauldron_bottle_fill(ctx: Context) -> None:
    bot, player = await join(ctx, "bkBotF")
    changed = ctx.capture(CauldronLevelChangeEvent, extra=new_state)
    at = await with_cauldron(ctx, bot, "bkBotF", "glass_bottle", level=6)
    await click_cauldron(ctx, bot, at)
    assert changed.count == 1, changed.events
    event = changed.events[0]
    assert event["reason"] == "BOTTLE_FILL" and event["who"] == "bkBotF", event
    level = cauldron_state(ctx, at)[0]
    assert level < 6 and event["new_level"] == level, (level, event["new_level"])
    assert server_slot(player, 0) == ("minecraft:potion", 1), server_slot(player, 0)
    await poll(ctx, lambda: client_slot(bot, 0), lambda s: s == ("minecraft:potion", 1), what="the client's potion")


@scenario("blocks.cauldron.bottle_empty", covers={CauldronLevelChangeEvent: "normal"}, plot=True, budget=BUDGET)
async def cauldron_bottle_empty(ctx: Context) -> None:
    bot, player = await join(ctx, "bkBotE")
    changed = ctx.capture(CauldronLevelChangeEvent, extra=new_state)
    at = await with_cauldron(ctx, bot, "bkBotE", "potion", level=0)
    await click_cauldron(ctx, bot, at)
    assert changed.count == 1, changed.events
    event = changed.events[0]
    assert event["reason"] == "BOTTLE_EMPTY" and event["who"] == "bkBotE", event
    level = cauldron_state(ctx, at)[0]
    assert level > 0 and event["new_level"] == level, (level, event["new_level"])
    assert server_slot(player, 0) == ("minecraft:glass_bottle", 1), server_slot(player, 0)


@scenario("blocks.cauldron.shulker_wash", covers={CauldronLevelChangeEvent: "normal"}, plot=True, budget=BUDGET)
async def cauldron_shulker_wash(ctx: Context) -> None:
    bot, player = await join(ctx, "bkWash")
    changed = ctx.capture(CauldronLevelChangeEvent, extra=new_state)
    at = await with_cauldron(ctx, bot, "bkWash", "red_shulker_box", level=6)
    await click_cauldron(ctx, bot, at)
    assert changed.count == 1, changed.events
    event = changed.events[0]
    assert event["reason"] == "SHULKER_WASH" and event["who"] == "bkWash", event
    level = cauldron_state(ctx, at)[0]
    assert level < 6 and event["new_level"] == level, (level, event["new_level"])
    assert server_slot(player, 0) == ("minecraft:undyed_shulker_box", 1), server_slot(player, 0)


@scenario("blocks.cauldron.no_change", covers={CauldronLevelChangeEvent: "normal"}, plot=True, budget=BUDGET)
async def cauldron_no_change(ctx: Context) -> None:
    bot, player = await join(ctx, "bkNoCau")
    changed = ctx.capture(CauldronLevelChangeEvent, extra=new_state)
    emptied = ctx.capture(PlayerBucketEmptyEvent, mine("bkNoCau"))
    filled = ctx.capture(PlayerBucketFillEvent, mine("bkNoCau"))
    for item, level in (("bucket", 0), ("red_shulker_box", 0), ("glass_bottle", 0)):
        at = await with_cauldron(ctx, bot, "bkNoCau", item, level=level)
        await click_cauldron(ctx, bot, at, ticks=20)
        assert (changed.count, emptied.count, filled.count) == (0, 0, 0), (
            f"{item} on a cauldron at level {level} changes nothing, yet "
            f"{changed.events} {emptied.events} {filled.events}"
        )
        assert cauldron_state(ctx, at)[0] == level
    at = await with_cauldron(ctx, bot, "bkNoCau", "water_bucket")
    await click_cauldron(ctx, bot, at)
    assert changed.count >= 1 and emptied.count == 1, "the control click raised nothing, so the quiet clicks prove nothing"


@scenario("blocks.cauldron.full_bucket", covers={CauldronLevelChangeEvent: "normal"}, plot=True, budget=BUDGET)
async def cauldron_full_bucket(ctx: Context) -> None:
    bot, player = await join(ctx, "bkCauFull")
    changed = ctx.capture(CauldronLevelChangeEvent, extra=new_state)
    emptied = ctx.capture(PlayerBucketEmptyEvent, mine("bkCauFull"))
    at = await with_cauldron(ctx, bot, "bkCauFull", "water_bucket", level=6)
    await click_cauldron(ctx, bot, at, ticks=20)
    assert cauldron_state(ctx, at) == (6, "water")
    levels = [e["new_level"] for e in changed.events]
    assert (changed.count, emptied.count) == (0, 0), (
        f"a water bucket on a full cauldron changes nothing, yet it raised {changed.count} "
        f"CauldronLevelChangeEvents (levels {levels}) and {emptied.count} PlayerBucketEmptyEvents"
    )


def sign_lines(ctx: Context, at, side: str = "FRONT") -> list[str]:
    state = ctx.overworld.get_block_at(*at).capture_state()
    return list(state.get_side(Side[side]).lines)


async def sign_packets(bot, since: int) -> list[dict]:
    return [p for p in await bot.call("get_received_packets", since=since) if "Sign" in p["name"]]


@scenario("blocks.sign.place", covers={PlayerOpenSignEvent: "normal"}, plot=True, budget=BUDGET)
async def sign_place(ctx: Context) -> None:
    bot, player = await join(ctx, "bkSignP")
    await give(ctx, bot, "bkSignP", "oak_sign", 3)
    opened = ctx.capture(PlayerOpenSignEvent, mine("bkSignP"))
    floor = ctx.plot.pos(6, -1, 8)
    at = (floor[0], floor[1] + 1, floor[2])
    index = await mark(bot)
    await bot.call("game_mode.use_item_on", list(floor), 1)
    await ctx.until(lambda: opened.count, timeout=60, what="PlayerOpenSignEvent")
    await ctx.ticks(10)
    assert opened.count == 1, opened.events
    event = opened.events[0]
    assert event["cause"] == "PLACE", event["cause"]
    assert event["side"] == "FRONT", event["side"]
    assert coords(event["sign"]) == at, event["sign"]
    assert "sign" in type_at(ctx, at), type_at(ctx, at)
    shown = await sign_packets(bot, index)
    ctx.info["sign_packets"] = sorted({p["name"] for p in shown})
    assert shown, "the client was never told to open the sign editor"
    await bot.call("local_player.set_sign_text", list(at), "hello", "behind")
    await ctx.until(lambda: sign_lines(ctx, at)[0] == "hello", timeout=60, what="the server to take the typed text")
    assert sign_lines(ctx, at)[1:] == ["", "", ""], sign_lines(ctx, at)
    assert sign_lines(ctx, at, "BACK")[0] == "behind", sign_lines(ctx, at, "BACK")


@scenario("blocks.sign.place_cancel", covers={PlayerOpenSignEvent: "cancel"}, plot=True, budget=BUDGET)
async def sign_place_cancel(ctx: Context) -> None:
    bot, player = await join(ctx, "bkSignPN")
    await give(ctx, bot, "bkSignPN", "oak_sign", 3)
    gate = {"on": True}
    opened = ctx.capture(PlayerOpenSignEvent, mine("bkSignPN"), mutate=lambda e: e.cancel() if gate["on"] else None)
    floor = ctx.plot.pos(6, -1, 8)
    at = (floor[0], floor[1] + 1, floor[2])
    index = await mark(bot)
    await bot.call("game_mode.use_item_on", list(floor), 1)
    await ctx.until(lambda: opened.count, timeout=60, what="the cancelled PlayerOpenSignEvent")
    await ctx.ticks(20)
    assert "sign" in type_at(ctx, at), "cancelling the editor should not unplace the sign"
    assert not await sign_packets(bot, index), "the client was told to open the editor of a cancelled event"
    gate["on"] = False
    other = ctx.plot.pos(6, 0, 10)
    index = await mark(bot)
    await bot.call("game_mode.use_item_on", [other[0], other[1] - 1, other[2]], 1)
    await ctx.until(lambda: opened.count == 2, timeout=60, what="the control PlayerOpenSignEvent")
    await poll(
        ctx, lambda: sign_packets(bot, index), lambda found: found, what="the editor of the uncancelled sign to open"
    )


@scenario("blocks.sign.interact", covers={PlayerOpenSignEvent: ("normal", "cancel")}, plot=True, budget=BUDGET)
async def sign_interact(ctx: Context) -> None:
    bot, player = await join(ctx, "bkSignI")
    at = ctx.plot.pos(6, 0, 8)
    ctx.run(f"setblock {at[0]} {at[1]} {at[2]} standing_sign")
    await ctx.until(lambda: "sign" in type_at(ctx, at), timeout=20, what="the sign")
    await seen_by(ctx, bot, at)
    await give(ctx, bot, "bkSignI", "stick", 1)
    gate = {"on": True}
    opened = ctx.capture(PlayerOpenSignEvent, mine("bkSignI"), mutate=lambda e: e.cancel() if gate["on"] else None)
    index = await mark(bot)
    await bot.call("game_mode.use_item_on", list(at), 4)
    await ctx.until(lambda: opened.count, timeout=60, what="the cancelled PlayerOpenSignEvent")
    await ctx.ticks(20)
    assert not await sign_packets(bot, index), "the client was told to open the editor of a cancelled event"
    gate["on"] = False
    index = await mark(bot)
    await bot.call("game_mode.use_item_on", list(at), 4)
    await ctx.until(lambda: opened.count == 2, timeout=60, what="the PlayerOpenSignEvent")
    await poll(ctx, lambda: sign_packets(bot, index), lambda found: found, what="the sign editor to open")
    event = opened.events[1]
    assert event["cause"] == "INTERACT", event["cause"]
    assert event["side"] in ("FRONT", "BACK")
    assert coords(event["sign"]) == at
    await bot.call("local_player.set_sign_text", list(at), "line", "")
    await ctx.until(lambda: sign_lines(ctx, at)[0] == "line", timeout=60, what="the typed text")


@scenario("blocks.sign.stale_cause", covers={PlayerOpenSignEvent: "normal"}, plot=True, budget=BUDGET)
async def sign_stale_cause(ctx: Context) -> None:
    bot, player = await join(ctx, "bkSignW")
    at = ctx.plot.pos(6, 0, 8)
    ctx.run(f"setblock {at[0]} {at[1]} {at[2]} standing_sign")
    await ctx.until(lambda: "sign" in type_at(ctx, at), timeout=20, what="the sign")
    await seen_by(ctx, bot, at)
    state = ctx.overworld.get_block_at(*at).capture_state()
    state.waxed = True
    state.update()
    await give(ctx, bot, "bkSignW", "stick", 1)
    opened = ctx.capture(PlayerOpenSignEvent, mine("bkSignW"))
    index = await mark(bot)
    await bot.call("game_mode.use_item_on", list(at), 4)
    await ctx.ticks(30)
    assert opened.count == 0 and not await sign_packets(bot, index), "a waxed sign opened its editor"
    state = ctx.overworld.get_block_at(*at).capture_state()
    state.waxed = False
    state.update()
    player.open_sign(ctx.overworld.get_block_at(*at).capture_state(), Side.FRONT)
    await ctx.until(lambda: opened.count, timeout=60, what="PlayerOpenSignEvent for the plugin call")
    assert opened.events[0]["cause"] == "PLUGIN", (
        f"open_sign after a waxed-sign click reported cause {opened.events[0]['cause']}"
    )


def bed_halves(ctx: Context, near: tuple[int, int, int]) -> list[tuple[int, int, int]]:
    x, y, z = near
    return [
        (x + dx, y, z + dz)
        for dx in range(-1, 2)
        for dz in range(-1, 2)
        if type_at(ctx, (x + dx, y, z + dz)) == "minecraft:bed"
    ]


async def made_bed(ctx: Context, bot, player, name: str):
    await give(ctx, bot, name, "bed", 1)
    floor = ctx.plot.pos(6, -1, 8)
    at = (floor[0], floor[1] + 1, floor[2])
    await bot.call("game_mode.use_item_on", list(floor), 1)
    halves = await ctx.until(lambda: len(bed_halves(ctx, at)) == 2 and bed_halves(ctx, at), timeout=60, what="the bed")
    for half in halves:
        await seen_by(ctx, bot, half)
    return halves


@scenario(
    "blocks.bed.enter_leave",
    covers={PlayerBedEnterEvent: "normal", PlayerBedLeaveEvent: "normal"},
    plot=True,
    budget=BUDGET,
)
async def bed_enter_leave(ctx: Context) -> None:
    bot, player = await join(ctx, "bkBed")
    ctx.run("time set midnight")
    try:
        halves = await made_bed(ctx, bot, player, "bkBed")
        entered = ctx.capture(PlayerBedEnterEvent, mine("bkBed"))
        left = ctx.capture(PlayerBedLeaveEvent, mine("bkBed"))
        await bot.call("game_mode.use_item_on", list(halves[0]), 1)
        await ctx.until(lambda: entered.count, timeout=60, what="PlayerBedEnterEvent")
        await ctx.ticks(20)
        assert entered.count == 1 and left.count == 0, (entered.events, left.events)
        assert coords(entered.events[0]["bed"]) in halves, (entered.events[0]["bed"], halves)
        assert entered.events[0]["bed"]["type"] == "minecraft:bed"
        await bot.call("local_player.stop_sleeping")
        await ctx.until(lambda: left.count, timeout=60, what="PlayerBedLeaveEvent")
        assert left.count == 1, left.events
        assert coords(left.events[0]["bed"]) in halves, (left.events[0]["bed"], halves)
        assert left.events[0]["bed"]["type"] == "minecraft:bed"
    finally:
        ctx.run("time set day")


@scenario("blocks.bed.enter_cancel", covers={PlayerBedEnterEvent: "cancel"}, plot=True, budget=BUDGET)
async def bed_enter_cancel(ctx: Context) -> None:
    bot, player = await join(ctx, "bkBedN")
    ctx.run("time set midnight")
    try:
        halves = await made_bed(ctx, bot, player, "bkBedN")
        gate = {"on": True}
        entered = ctx.capture(PlayerBedEnterEvent, mine("bkBedN"), mutate=lambda e: e.cancel() if gate["on"] else None)
        left = ctx.capture(PlayerBedLeaveEvent, mine("bkBedN"))
        await bot.call("game_mode.use_item_on", list(halves[0]), 1)
        await ctx.until(lambda: entered.count, timeout=60, what="the cancelled PlayerBedEnterEvent")
        await ctx.ticks(30)
        await bot.call("local_player.stop_sleeping")
        await ctx.ticks(20)
        assert left.count == 0, "a cancelled bed enter still put the player to bed"
        gate["on"] = False
        await bot.call("game_mode.use_item_on", list(halves[0]), 1)
        await ctx.until(lambda: entered.count == 2, timeout=60, what="the second PlayerBedEnterEvent")
        await ctx.ticks(20)
        await bot.call("local_player.stop_sleeping")
        await ctx.until(lambda: left.count == 1, timeout=60, what="the player, who slept this time, to leave the bed")
    finally:
        ctx.run("time set day")


@scenario("blocks.bed.broken_while_sleeping", covers={PlayerBedLeaveEvent: "normal"}, plot=True, budget=BUDGET)
async def bed_broken_while_sleeping(ctx: Context) -> None:
    bot, player = await join(ctx, "bkBedB")
    ctx.run("time set midnight")
    try:
        halves = await made_bed(ctx, bot, player, "bkBedB")
        entered = ctx.capture(PlayerBedEnterEvent, mine("bkBedB"))
        reported = ctx.info.setdefault("bed_type_at_leave", [])
        left = ctx.capture(PlayerBedLeaveEvent, lambda e: reported.append(str(e.bed.type)) or True)
        await bot.call("game_mode.use_item_on", list(halves[0]), 1)
        await ctx.until(lambda: entered.count, timeout=60, what="PlayerBedEnterEvent")
        await ctx.ticks(20)
        for half in halves:
            ctx.command(f"setblock {half[0]} {half[1]} {half[2]} air")
        await ctx.until(lambda: left.count, timeout=100, what="PlayerBedLeaveEvent after the bed was broken")
        assert left.count == 1, left.events
        bed = left.events[0]["bed"]
        assert (bed["x"], bed["y"], bed["z"]) in [tuple(half) for half in halves], bed
        assert bed["type"] == "minecraft:air", bed
    finally:
        ctx.run("time set day")


async def blast_site(ctx: Context, bot, name: str):
    anchor = ctx.plot.pos(8, 0, 8)
    kept = (anchor[0], anchor[1], anchor[2] + 1)
    lost = (anchor[0], anchor[1], anchor[2] - 1)
    ctx.run(f"setblock {kept[0]} {kept[1]} {kept[2]} dirt")
    ctx.run(f"setblock {lost[0]} {lost[1]} {lost[2]} dirt")
    ctx.run(f'setblock {anchor[0]} {anchor[1]} {anchor[2]} respawn_anchor ["respawn_anchor_charge"=1]')
    await ctx.until(
        lambda: type_at(ctx, anchor) == "minecraft:respawn_anchor" and type_at(ctx, kept) == type_at(ctx, lost),
        timeout=20,
        what="the anchor and its dirt",
    )
    await seen_by(ctx, bot, anchor)
    await give(ctx, bot, name, "stick", 1)
    return anchor, kept, lost


@scenario("blocks.explode.anchor_normal", covers={BlockExplodeEvent: "normal"}, plot=True, budget=BUDGET)
@trimmed
async def explode_anchor_normal(ctx: Context) -> None:
    bot, player = await join(ctx, "bkBlast", mode="creative")
    anchor, kept, lost = await blast_site(ctx, bot, "bkBlast")
    blasts = ctx.capture(BlockExplodeEvent)
    await bot.call("game_mode.use_item_on", list(anchor), 4)
    await ctx.until(lambda: blasts.count, timeout=60, what="BlockExplodeEvent")
    await ctx.until(lambda: type_at(ctx, lost) == "minecraft:air", timeout=40, what="the dirt to be blown away")
    assert blasts.count == 1, blasts.events
    event = blasts.events[0]
    assert coords(event["block"]) == anchor, event["block"]
    listed = {coords(b) for b in event["block_list"]}
    assert kept in listed and lost in listed, (listed, kept, lost)
    assert type_at(ctx, kept) == "minecraft:air"
    await seen_by(ctx, bot, lost)
    assert await bot.call("get_block", list(lost)) == runtime_at(ctx, lost)


@scenario("blocks.explode.anchor_cancel", covers={BlockExplodeEvent: "cancel"}, plot=True, budget=BUDGET)
@trimmed
async def explode_anchor_cancel(ctx: Context) -> None:
    bot, player = await join(ctx, "bkBlastN", mode="creative")
    anchor, kept, lost = await blast_site(ctx, bot, "bkBlastN")
    gate = {"on": True}
    blasts = ctx.capture(BlockExplodeEvent, mutate=lambda e: e.cancel() if gate["on"] else None)
    await bot.call("game_mode.use_item_on", list(anchor), 4)
    await ctx.until(lambda: blasts.count, timeout=60, what="BlockExplodeEvent")
    await ctx.ticks(30)
    assert type_at(ctx, lost) == "minecraft:dirt" and type_at(ctx, kept) == "minecraft:dirt", (
        "a cancelled explosion still destroyed blocks"
    )
    assert await bot.call("get_block", list(lost)) == runtime_at(ctx, lost)
    gate["on"] = False
    ctx.command(f"setblock {anchor[0]} {anchor[1]} {anchor[2]} air")
    ctx.run(f'setblock {anchor[0]} {anchor[1]} {anchor[2]} respawn_anchor ["respawn_anchor_charge"=1]')
    await seen_by(ctx, bot, anchor)
    await bot.call("game_mode.use_item_on", list(anchor), 4)
    await ctx.until(lambda: type_at(ctx, lost) == "minecraft:air", timeout=60, what="the blast once it is allowed")
    assert blasts.count == 2, blasts.events


@scenario("blocks.explode.anchor_mutate", covers={BlockExplodeEvent: "mutate"}, plot=True, budget=BUDGET)
@trimmed
async def explode_anchor_mutate(ctx: Context) -> None:
    bot, player = await join(ctx, "bkBlastM", mode="creative")
    anchor, kept, lost = await blast_site(ctx, bot, "bkBlastM")

    def spare(event) -> None:
        event.block_list = [b for b in event.block_list if (b.x, b.y, b.z) != kept]

    blasts = ctx.capture(BlockExplodeEvent, mutate=spare)
    await bot.call("game_mode.use_item_on", list(anchor), 4)
    await ctx.until(lambda: blasts.count, timeout=60, what="BlockExplodeEvent")
    await ctx.until(lambda: type_at(ctx, lost) == "minecraft:air", timeout=40, what="the other dirt to be blown away")
    await ctx.ticks(10)
    assert type_at(ctx, kept) == "minecraft:dirt", "the block removed from block_list was destroyed anyway"
    after = {coords(b) for b in blasts.events[0]["after"]["block_list"]}
    assert kept not in after and lost in after
    await seen_by(ctx, bot, kept)


@scenario("blocks.explode.nether_bed", covers={BlockExplodeEvent: "normal"}, plot=True, budget=1500)
@trimmed
async def explode_nether_bed(ctx: Context) -> None:
    bot, player = await join(ctx, "bkNethBed", mode="creative")
    nether = ctx.server.level.get_dimension("nether")
    ctx.run("execute in nether run tp bkNethBed 1000.5 100 1000.5")
    await ctx.until(
        lambda: str(player.location.dimension.id) == "minecraft:nether", timeout=200, what="the player to reach the nether"
    )
    await poll(
        ctx,
        lambda: bot.call("local_player.is_changing_dimension"),
        lambda changing: changing is False,
        timeout=300,
        what="the client to finish the dimension change",
    )
    await ctx.until(
        lambda: all(nether.is_chunk_loaded(cx, cz) for cx in (61, 62, 63) for cz in (61, 62, 63)),
        timeout=300,
        what="the nether chunks around the platform",
    )
    for px in range(998, 1005):
        for pz in range(998, 1005):
            for py in range(99, 103):
                nether.get_block_at(px, py, pz).set_type("minecraft:air")
            nether.get_block_at(px, 98, pz).set_type("minecraft:glass")
    await ctx.until(
        lambda: str(nether.get_block_at(1000, 98, 1000).type) == "minecraft:glass", timeout=100, what="the platform"
    )
    await ctx.ticks(10)
    await give(ctx, bot, "bkNethBed", "bed", 1)
    blasts = ctx.capture(BlockExplodeEvent)
    await seen_by_in(ctx, bot, nether, (1002, 98, 1000))
    await ctx.ticks(40)
    await bot.call("game_mode.use_item_on", [1002, 98, 1000], 1)
    halves = await ctx.until(
        lambda: [
            (x, 99, z)
            for x in range(1001, 1005)
            for z in range(999, 1004)
            if str(nether.get_block_at(x, 99, z).type) == "minecraft:bed"
        ],
        timeout=200,
        what="the bed",
    )
    assert len(halves) == 2, halves
    await seen_by_in(ctx, bot, nether, halves[0])
    await bot.call("game_mode.use_item_on", list(halves[0]), 1)
    await ctx.until(lambda: blasts.count, timeout=100, what="BlockExplodeEvent")
    assert blasts.count == 1, blasts.events
    event = blasts.events[0]
    assert (event["block"]["x"], event["block"]["y"], event["block"]["z"]) in halves, (event["block"], halves)
    assert event["block"]["dimension"] == "minecraft:nether", event["block"]
    await ctx.until(
        lambda: all(str(nether.get_block_at(*h).type) == "minecraft:air" for h in halves), timeout=60, what="the bed to go"
    )


async def seen_by_in(ctx: Context, bot, dimension, at) -> None:
    want = dimension.get_block_at(*at).data.runtime_id
    await poll(ctx, lambda: bot.call("get_block", list(at)), lambda r: r == want, what=f"the client to see {at}")


@scenario("blocks.send.block_change", plot=True, budget=BUDGET)
async def send_block_change(ctx: Context) -> None:
    bot, player = await join(ctx, "bkSendC", dx=4)
    watcher, seen_from = await join(ctx, "bkSendW", dx=4, dz=11)
    at = ctx.plot.pos(6, 0, 9)
    air = runtime_at(ctx, at)
    glass = ctx.server.create_block_data("minecraft:glass")
    await poll(ctx, lambda: bot.call("get_block", list(at)), lambda r: r == air, what="the client to know the air")
    await poll(ctx, lambda: watcher.call("get_block", list(at)), lambda r: r == air, what="the watcher to know the air")
    index = await mark(bot)
    player.send_block_change(Location(ctx.overworld, *at), glass)
    await poll(
        ctx, lambda: bot.call("get_block", list(at)), lambda r: r == glass.runtime_id, what="the client to see the glass"
    )
    assert glass.runtime_id != air
    assert type_at(ctx, at) == "minecraft:air", "send_block_change changed the world"
    await ctx.ticks(20)
    assert await watcher.call("get_block", list(at)) == air, "another player was shown the fake block"
    sent = [p for p in await packets(bot, "UpdateBlockPacket", index) if xyz(p["packet"]["pos"]) == at]
    assert [p["packet"]["runtime_id"] for p in sent] == [glass.runtime_id], sent


@scenario("blocks.send.block_update", plot=True, budget=BUDGET)
async def send_block_update(ctx: Context) -> None:
    bot, player = await join(ctx, "bkSendU", dx=4)
    at = ctx.plot.pos(6, 0, 8)
    ctx.run(f"setblock {at[0]} {at[1]} {at[2]} standing_sign")
    await ctx.until(lambda: "sign" in type_at(ctx, at), timeout=20, what="the sign")
    await seen_by(ctx, bot, at)
    state = ctx.overworld.get_block_at(*at).capture_state()
    state.get_side(Side.FRONT).set_line(0, "fakeline")
    index = await mark(bot)
    player.send_block_update(Location(ctx.overworld, *at), state)
    found = await poll(
        ctx,
        lambda: packets(bot, "BlockActorDataPacket", index),
        lambda got: any("fakeline" in json.dumps(p["packet"]) for p in got),
        what="the client to be sent the fake text",
    )
    assert sign_lines(ctx, at)[0] == "", "send_block_update changed the world's sign"
    assert any(xyz(p["packet"]["pos"]) == at for p in found)


@scenario("blocks.sign.set_line_newline", plot=True, budget=BUDGET)
async def sign_set_line_newline(ctx: Context) -> None:
    at = ctx.plot.pos(6, 0, 8)
    ctx.run(f"setblock {at[0]} {at[1]} {at[2]} standing_sign")
    await ctx.until(lambda: "sign" in type_at(ctx, at), timeout=20, what="the sign")
    state = ctx.overworld.get_block_at(*at).capture_state()
    side = state.get_side(Side.FRONT)
    side.set_line(0, "a\nb")
    side.set_line(2, "c")
    assert len(side.lines) == 4, f"a line containing a newline made the sign hold {len(side.lines)} lines: {side.lines}"
    assert side.lines[1] == "" and side.lines[2] == "c", side.lines
