import gc
import os
import socket
import struct
import sys
import threading
import time
import weakref

from endstone.actor import ActorType
from endstone.block import BlockType, Side, Sign
from endstone.boss import BarColor, BarStyle
from endstone.command import CommandExecutor
from endstone.event import (
    ActorRemoveEvent,
    ActorSpawnEvent,
    BlockFromToEvent,
    ChunkLoadEvent,
    InventoryOpenEvent,
    PlayerChatEvent,
    PlayerCommandEvent,
    PlayerJoinEvent,
    PlayerLoginEvent,
    PlayerMoveEvent,
    PlayerQuitEvent,
)
from endstone.inventory import ItemStack, ItemType
from endstone.level import Location
from endstone.map import MapRenderer
from endstone.metrics import Metrics
from endstone.potion import EffectType
from endstone.scoreboard import Criteria

from endstone_test.harness import Context, scenario
from endstone_test.harness.engine import HarnessError
from endstone_test.harness.plot import AREA as PLOT_AREA

FIXTURE = "test_fixture"
GRANTED = "endstone_test.granted"
SHUTDOWN_ENV = "ENDSTONE_TEST_LIFETIME_SHUTDOWN"
RELOAD_STASH = "_endstone_test_lifetime_reload"
RELOAD_TASK_STASH = "_endstone_test_lifetime_reload_task"
KEPT_AT_SHUTDOWN = []


def touch(action) -> tuple[bool, str]:
    try:
        value = action()
    except Exception as e:
        return True, f"raised {type(e).__name__}: {e}"
    return False, f"returned {value!r}"


def probe(ctx: Context, label: str, actions: dict) -> dict[str, str]:
    seen = {}
    for name, action in actions.items():
        raised, what = touch(action)
        seen[name] = what
    ctx.info[label] = seen
    return seen


def unraised(ctx: Context, label: str, actions: dict) -> dict[str, str]:
    seen = probe(ctx, label, actions)
    return {name: what for name, what in seen.items() if not what.startswith("raised ")}


def must_raise(ctx: Context, label: str, actions: dict) -> None:
    quiet = unraised(ctx, label, actions)
    assert not quiet, f"{label}: touching a dead object did not raise: {quiet}"


def fixture(ctx: Context):
    plugin = ctx.server.plugin_manager.get_plugin(FIXTURE)
    assert plugin is not None, f"the {FIXTURE} plugin is not loaded"
    return plugin


def in_plot(ctx: Context, actor) -> bool:
    location = actor.location
    plot = ctx.plot
    return plot.x <= location.x < plot.x + 16 and plot.z <= location.z < plot.z + 16


async def summon(ctx: Context, kind: str, dx: int = 8, dz: int = 8):
    ctx.run(f"summon {kind} {ctx.plot.at(dx, 0, dz)}")
    found = await ctx.until(
        lambda: [a for a in ctx.level.actors if str(a.type) == f"minecraft:{kind}" and in_plot(ctx, a)],
        timeout=20,
        what=f"the summoned {kind}",
    )
    return found[0]


async def gone(ctx: Context, actor, timeout: int = 200) -> None:
    await ctx.until(lambda: touch(lambda: actor.location)[0], timeout=timeout, what="the entity to be destroyed")


async def join(ctx: Context, name: str):
    bot = await ctx.bot(name)
    player = await ctx.until(lambda: ctx.server.get_player(name), timeout=60, what=f"{name} online")
    return bot, player


async def leave(ctx: Context, name: str) -> None:
    await ctx.call("bot.close", bot=name)
    await ctx.until(lambda: ctx.server.get_player(name) is None, timeout=200, what=f"{name} offline")


async def into_plot(ctx: Context, bot, player, dx: int, dz: int) -> tuple[float, float, float]:
    x, y, z = ctx.plot.pos(dx, 0, dz)
    spot = (x + 0.5, y, z + 0.5)
    ctx.run(f"tp {player.name} {spot[0]} {spot[1]} {spot[2]}")
    await ctx.until(
        lambda: abs(player.location.x - spot[0]) < 0.1 and abs(player.location.z - spot[2]) < 0.1,
        timeout=60,
        what=f"{player.name} to arrive in the plot",
    )
    for _ in range(200):
        position = await bot.call("local_player.get_position")
        if abs(position[0] - spot[0]) < 0.1 and abs(position[2] - spot[2]) < 0.1:
            break
        await ctx.ticks(1)
    await ctx.ticks(10)
    return spot


def named(name: str):
    return lambda e: e.player.name == name


@scenario("lifetime.actor.killed", plot=True, budget=400)
async def actor_killed(ctx: Context) -> None:
    pig = await summon(ctx, "pig")
    runtime_id = pig.runtime_id
    movement = pig.get_attribute("minecraft:movement")
    location = pig.location
    removed = ctx.capture(ActorRemoveEvent, lambda e: e.actor.runtime_id == runtime_id)
    ctx.run(f"kill @e[type=pig,{ctx.plot.volume}]")
    await ctx.until(lambda: removed.count, timeout=100, what="ActorRemoveEvent for the killed pig")
    await gone(ctx, pig)
    await ctx.ticks(5)
    assert pig.is_valid is False
    must_raise(
        ctx,
        "pig",
        {
            "name": lambda: pig.name,
            "location": lambda: pig.location,
            "health": lambda: pig.health,
            "dimension": lambda: pig.dimension,
            "teleport": lambda: pig.teleport(location),
            "add_scoreboard_tag": lambda: pig.add_scoreboard_tag("lifetime"),
            "remove": lambda: pig.remove(),
            "get_attribute": lambda: pig.get_attribute("minecraft:movement"),
            "active_effects": lambda: pig.active_effects,
        },
    )
    must_raise(
        ctx,
        "movement_attribute",
        {
            "value": lambda: movement.value,
            "base_value": lambda: movement.base_value,
            "modifiers": lambda: movement.modifiers,
        },
    )


@scenario("lifetime.actor.chunk_unloaded", budget=3600)
async def actor_chunk_unloaded(ctx: Context) -> None:
    cx, cz = -2100, -2100
    x0, z0 = cx * 16, cz * 16
    area = "endstone_test_lifetime_unload"
    ctx.command(f"tickingarea remove {area}")
    ctx.run(f"tickingarea add {x0} 0 {z0} {x0 + 15} 0 {z0 + 15} {area} true")
    await ctx.until(lambda: ctx.overworld.is_chunk_loaded(cx, cz), timeout=600, what="the far chunk to load")
    await ctx.until(
        lambda: ctx.block_is(f"{x0 + 8} -61 {z0 + 8}", "grass_block"), timeout=200, what="the far chunk to answer"
    )
    ctx.run(f"summon pig {x0 + 8} -60 {z0 + 8}")

    def far_pigs():
        return [
            a
            for a in ctx.level.actors
            if str(a.type) == "minecraft:pig" and x0 <= a.location.x < x0 + 16 and z0 <= a.location.z < z0 + 16
        ]

    found = await ctx.until(far_pigs, timeout=40, what="the pig in the far chunk")
    pig = found[0]
    unique_id = pig.id
    movement = pig.get_attribute("minecraft:movement")
    ctx.run(f"tickingarea remove {area}")
    await ctx.until(lambda: not ctx.overworld.is_chunk_loaded(cx, cz), timeout=2400, what="the far chunk to unload")
    await ctx.ticks(5)
    ctx.info["is_valid_after_unload"] = pig.is_valid
    assert pig.is_valid is False, "an actor whose chunk unloaded still reports itself valid"
    must_raise(
        ctx,
        "pig_after_unload",
        {
            "name": lambda: pig.name,
            "location": lambda: pig.location,
            "health": lambda: pig.health,
            "movement.value": lambda: movement.value,
        },
    )

    ctx.run(f"tickingarea add {x0} 0 {z0} {x0 + 15} 0 {z0 + 15} {area} true")
    try:
        again = await ctx.until(far_pigs, timeout=600, what="the pig to come back with its chunk")
        ctx.info["reloaded"] = {
            "same_id": again[0].id == unique_id,
            "old_handle_valid": pig.is_valid,
            "new_handle_valid": again[0].is_valid,
        }
        assert pig.is_valid is False, "the old handle came back to life when the chunk reloaded"
        ctx.command(f"kill @e[type=pig,x={x0},y=-64,z={z0},dx=15,dy=40,dz=15]")
    finally:
        ctx.command(f"tickingarea remove {area}")


@scenario("lifetime.location.unset_dimension", plot=True, budget=200)
async def location_unset_dimension(ctx: Context) -> None:
    pig = await summon(ctx, "pig")
    x, y, z = ctx.plot.pos(2, 0, 2)
    nowhere = Location(None, x + 0.5, y, z + 0.5)
    assert nowhere.dimension is None
    assert nowhere.is_dimension_loaded is False
    cleared = Location(ctx.overworld, x + 0.5, y, z + 0.5)
    cleared.dimension = None
    seen = probe(
        ctx,
        "teleport",
        {
            "Location(None)": lambda: pig.teleport(nowhere),
            "dimension cleared": lambda: pig.teleport(cleared),
        },
    )
    await ctx.ticks(2)
    moved = abs(pig.location.x - (x + 0.5)) < 0.01 and abs(pig.location.z - (z + 0.5)) < 0.01
    ctx.info["moved"] = moved
    bad = {k: v for k, v in seen.items() if not (v.startswith("raised ") or v == "returned False")}
    assert not bad, f"teleporting to a location without a dimension neither raised nor refused: {bad}"


@scenario("lifetime.block_state.replaced", plot=True, budget=200)
async def block_state_replaced(ctx: Context) -> None:
    x, y, z = ctx.plot.pos(4, 0, 4)
    ctx.run(f"setblock {x} {y} {z} chest")
    block = ctx.overworld.get_block_at(x, y, z)
    live = block.capture_state(use_snapshot=False)
    live.inventory.set_item(0, ItemStack("minecraft:diamond", 5))
    snapshot = block.capture_state()
    live_inventory = live.inventory
    snapshot_inventory = snapshot.snapshot_inventory
    assert live.is_snapshot is False and snapshot.is_snapshot is True

    ctx.run(f"setblock {x} {y} {z} barrel")
    await ctx.ticks(1)
    must_raise(
        ctx,
        "live_chest_now_barrel",
        {
            "inventory.size": lambda: live_inventory.size,
            "inventory.get_item": lambda: live_inventory.get_item(0),
            "inventory.set_item": lambda: live_inventory.set_item(1, ItemStack("minecraft:stick", 1)),
        },
    )
    kept = snapshot_inventory.get_item(0)
    ctx.info["snapshot_item"] = None if kept is None else (str(kept.type), kept.amount)
    assert kept is not None and str(kept.type) == "minecraft:diamond" and kept.amount == 5
    assert snapshot.update() is False, "a chest snapshot updated a block that is now a barrel"
    assert ctx.block_is(f"{x} {y} {z}", "barrel")

    sx, sy, sz = ctx.plot.pos(6, 0, 4)
    ctx.run(f"setblock {sx} {sy} {sz} standing_sign")
    sign = ctx.overworld.get_block_at(sx, sy, sz).capture_state(use_snapshot=False)
    assert isinstance(sign, Sign), type(sign)
    front = sign.get_side(Side.FRONT)
    front.set_line(0, "lifetime")
    ctx.run(f"setblock {sx} {sy} {sz} chest")
    await ctx.ticks(1)
    must_raise(
        ctx,
        "live_sign_now_chest",
        {
            "lines": lambda: front.lines,
            "set_line": lambda: front.set_line(1, "x"),
            "waxed": lambda: sign.waxed,
        },
    )

    ctx.run(f"setblock {x} {y} {z} air")
    await ctx.ticks(1)
    must_raise(ctx, "live_chest_now_air", {"inventory.get_item": lambda: live_inventory.get_item(0)})


@scenario("lifetime.block_state.chunk_unloaded", budget=3600)
async def block_state_chunk_unloaded(ctx: Context) -> None:
    cx, cz = -2110, -2100
    x0, z0 = cx * 16, cz * 16
    area = "endstone_test_lifetime_chest"
    ctx.command(f"tickingarea remove {area}")
    ctx.run(f"tickingarea add {x0} 0 {z0} {x0 + 15} 0 {z0 + 15} {area} true")
    await ctx.until(lambda: ctx.overworld.is_chunk_loaded(cx, cz), timeout=600, what="the far chunk to load")
    await ctx.until(
        lambda: ctx.block_is(f"{x0 + 8} -61 {z0 + 8}", "grass_block"), timeout=200, what="the far chunk to answer"
    )
    try:
        ctx.run(f"setblock {x0 + 8} -60 {z0 + 8} chest")
        block = ctx.overworld.get_block_at(x0 + 8, -60, z0 + 8)
        live = block.capture_state(use_snapshot=False)
        inventory = live.inventory
        inventory.set_item(0, ItemStack("minecraft:emerald", 2))
        ctx.run(f"tickingarea remove {area}")
        await ctx.until(lambda: not ctx.overworld.is_chunk_loaded(cx, cz), timeout=2400, what="the far chunk to unload")
        await ctx.ticks(5)
        probe(
            ctx,
            "unloaded",
            {
                "block.type": lambda: str(block.type),
                "inventory.get_item": lambda: inventory.get_item(0),
                "inventory.size": lambda: inventory.size,
                "capture_state": lambda: type(block.capture_state()).__name__,
            },
        )
        must_raise(ctx, "live_chest_unloaded", {"inventory.get_item": lambda: inventory.get_item(0)})
    finally:
        ctx.command(f"tickingarea remove {area}")


@scenario("lifetime.event.chunk_retained", budget=3600)
async def event_chunk_retained(ctx: Context) -> None:
    cx, cz = -2130, -2130
    x0, z0 = cx * 16, cz * 16
    area = "endstone_test_lifetime_chunks"
    kept = []

    def keep(event) -> bool:
        chunk = event.chunk
        if cx - 1 <= chunk.x <= cx + 1 and cz - 1 <= chunk.z <= cz + 1:
            kept.append((chunk, chunk.x, chunk.z))
            return True
        return False

    ctx.capture(ChunkLoadEvent, keep)
    ctx.command(f"tickingarea remove {area}")
    ctx.run(f"tickingarea add {x0 - 16} 0 {z0 - 16} {x0 + 31} 0 {z0 + 31} {area} true")
    try:
        await ctx.until(lambda: len(kept) >= 9, timeout=1200, what="ChunkLoadEvent for the 3x3 far chunks")
        await ctx.ticks(40)
        ctx.run(f"tickingarea remove {area}")
        await ctx.ticks(40)
        gc.collect()
        mismatched = []
        for chunk, x, z in kept:
            raised, what = touch(lambda c=chunk: (c.x, c.z))
            if not raised and what != f"returned {(x, z)!r}":
                mismatched.append(f"chunk ({x}, {z}) now {what}")
        ctx.info["retained"] = len(kept)
        ctx.info["mismatched"] = mismatched
        assert not mismatched, f"a ChunkLoadEvent chunk kept past its handler reads as another chunk: {mismatched[:5]}"
    finally:
        ctx.command(f"tickingarea remove {area}")


@scenario("lifetime.event.retained_event_object", plot=True, budget=200)
async def event_retained_event_object(ctx: Context) -> None:
    kept = []

    def keep(event) -> bool:
        if str(event.actor.type) == "minecraft:pig" and in_plot(ctx, event.actor):
            kept.append((event, event.event_name, event.actor.runtime_id))
            return True
        return False

    spawned = ctx.capture(ActorSpawnEvent, keep)
    ctx.run(f"summon pig {ctx.plot.at(8, 0, 8)}")
    await ctx.until(lambda: spawned.count, timeout=20, what="ActorSpawnEvent for the pig")
    ctx.run(f"summon chicken {ctx.plot.at(4, 0, 4)}")
    await ctx.ticks(20)
    gc.collect()
    event, event_name, runtime_id = kept[0]
    seen = probe(
        ctx,
        "after_dispatch",
        {
            "event_name": lambda: event.event_name,
            "is_cancelled": lambda: event.is_cancelled,
            "actor.runtime_id": lambda: event.actor.runtime_id,
        },
    )
    expected = {"event_name": f"returned {event_name!r}", "actor.runtime_id": f"returned {runtime_id!r}"}
    wrong = {k: seen[k] for k, v in expected.items() if seen[k] != v and not seen[k].startswith("raised ")}
    assert not wrong, f"an event kept past its dispatch reads garbage: {wrong}"


@scenario("lifetime.event.block_from_to_retained", plot=True, budget=300)
async def event_block_from_to_retained(ctx: Context) -> None:
    kept = []

    def keep(event) -> bool:
        block = event.to_block
        if not (ctx.plot.x <= block.x < ctx.plot.x + 16 and ctx.plot.z <= block.z < ctx.plot.z + 16):
            return False
        if len(kept) < 40:
            kept.append((block, (block.x, block.y, block.z)))
        return True

    flows = ctx.capture(BlockFromToEvent, keep)
    x, y, z = ctx.plot.pos(8, 0, 8)
    ctx.run(f"setblock {x} {y} {z} flowing_water")
    try:
        await ctx.until(lambda: flows.count >= 10, timeout=200, what="BlockFromToEvent from the water")
        await ctx.ticks(40)
        gc.collect()
        mismatched = []
        for block, where in kept:
            raised, what = touch(lambda b=block: (b.x, b.y, b.z))
            if not raised and what != f"returned {where!r}":
                mismatched.append(f"to_block at {where} now {what}")
        ctx.info["retained"] = len(kept)
        ctx.info["mismatched"] = mismatched
        assert not mismatched, f"a BlockFromToEvent.to_block kept past its handler moved: {mismatched[:5]}"
    finally:
        ctx.command(f"fill {ctx.plot.at(0, 0, 0)} {ctx.plot.at(15, 3, 15)} air")


@scenario("lifetime.scoreboard.removed", budget=200)
async def scoreboard_removed(ctx: Context) -> None:
    board = ctx.server.scoreboard
    for name in ("lt_obj", "lt_vanilla"):
        if board.get_objective(name) is not None:
            board.get_objective(name).unregister()
    objective = board.add_objective("lt_obj", Criteria.DUMMY, "Lifetime")
    score = objective.get_score("lt_entry")
    score.value = 5
    borrowed = objective.get_score("lt_entry").objective
    gc.collect()
    seen = probe(ctx, "score.objective", {"name": lambda: borrowed.name})
    assert seen["name"] == "returned 'lt_obj'", (
        f"Score.objective of a registered objective went bad once its Score was dropped: {seen}"
    )

    objective.unregister()
    assert board.get_objective("lt_obj") is None
    must_raise(
        ctx,
        "after_unregister",
        {
            "objective.name": lambda: objective.name,
            "objective.display_name": lambda: objective.display_name,
            "score.value": lambda: score.value,
            "score.value=": lambda: setattr(score, "value", 3),
            "score.is_score_set": lambda: score.is_score_set,
            "objective.get_score": lambda: objective.get_score("lt_entry").value,
        },
    )

    ctx.run("scoreboard objectives add lt_vanilla dummy")
    vanilla = board.get_objective("lt_vanilla")
    assert vanilla is not None
    vanilla_score = vanilla.get_score("lt_entry")
    vanilla_score.value = 7
    ctx.run("scoreboard objectives remove lt_vanilla")
    must_raise(
        ctx,
        "after_vanilla_remove",
        {
            "objective.name": lambda: vanilla.name,
            "score.value": lambda: vanilla_score.value,
        },
    )

    extra = ctx.server.create_scoreboard()
    orphan = extra.add_objective("lt_orphan", Criteria.DUMMY)
    orphan_score = orphan.get_score("lt_entry")
    orphan_score.value = 1
    del extra
    gc.collect()
    probe(
        ctx,
        "created_board_dropped",
        {
            "objective.name": lambda: orphan.name,
            "score.value": lambda: orphan_score.value,
            "objective.scoreboard": lambda: orphan.scoreboard is not None,
        },
    )


@scenario("lifetime.scheduler.pending_task_after_run", budget=200)
async def scheduler_pending_task_after_run(ctx: Context) -> None:
    scheduler = ctx.server.scheduler
    before = {t.task_id for t in scheduler.get_pending_tasks()}
    fired = []
    scheduler.run_task(ctx.plugin, lambda: fired.append(1), delay=5)
    fresh = [t for t in scheduler.get_pending_tasks() if t.task_id not in before]
    assert len(fresh) == 1, [t.task_id for t in fresh]
    held = fresh[0]
    task_id = held.task_id
    del fresh
    await ctx.until(lambda: fired, timeout=40, what="the one-shot task to run")
    await ctx.ticks(10)
    assert not scheduler.is_queued(task_id)
    gc.collect()
    seen = probe(
        ctx,
        "finished_task",
        {
            "task_id": lambda: held.task_id,
            "is_sync": lambda: held.is_sync,
            "is_cancelled": lambda: held.is_cancelled,
            "owner.name": lambda: held.owner.name,
        },
    )
    expected = {
        "task_id": f"returned {task_id!r}",
        "is_sync": "returned True",
        "owner.name": f"returned {ctx.plugin.name!r}",
    }
    wrong = {k: seen[k] for k, v in expected.items() if seen[k] != v and not seen[k].startswith("raised ")}
    assert not wrong, f"a task from get_pending_tasks() reads garbage once the task finished: {wrong}"


@scenario("lifetime.scheduler.disabled_plugin", budget=400)
async def scheduler_disabled_plugin(ctx: Context) -> None:
    manager = ctx.server.plugin_manager
    scheduler = ctx.server.scheduler
    plugin = fixture(ctx)
    counts = {"repeat": 0, "later": 0, "storm": 0}

    def bump(key):
        return lambda: counts.__setitem__(key, counts[key] + 1)

    try:
        repeating = scheduler.run_task(plugin, bump("repeat"), delay=1, period=1)
        later = scheduler.run_task(plugin, bump("later"), delay=400)
        await ctx.until(lambda: counts["repeat"] >= 3, timeout=40, what="the fixture's repeating task to run")
        manager.disable_plugin(plugin)
        frozen = dict(counts)
        await ctx.ticks(10)
        assert counts == frozen, f"tasks of a disabled plugin kept running: {frozen} -> {counts}"
        assert repeating.is_cancelled and later.is_cancelled
        assert not scheduler.is_queued(repeating.task_id) and not scheduler.is_queued(later.task_id)
        assert repeating.owner.name == FIXTURE
        raised, what = touch(lambda: scheduler.run_task(plugin, bump("later"), delay=1))
        ctx.info["run_task_while_disabled"] = what
        assert raised or what == "returned None", f"run_task queued a task for a disabled plugin: {what}"
        await ctx.ticks(3)
        assert counts == frozen, f"a task scheduled while its plugin was disabled ran: {counts}"

        manager.enable_plugin(plugin)
        tasks = []

        def disable_self() -> None:
            try:
                manager.disable_plugin(plugin)
                for task in tasks:
                    task.cancel()
            except Exception as e:
                ctx.info["disable_inside_task"] = f"{type(e).__name__}: {e}"

        for _ in range(30):
            tasks.append(scheduler.run_task(plugin, bump("storm"), delay=2, period=1))
        tasks.insert(0, scheduler.run_task(plugin, disable_self, delay=2))
        for _ in range(30):
            tasks.append(scheduler.run_task(plugin, bump("storm"), delay=2, period=1))
        await ctx.until(lambda: not plugin.is_enabled, timeout=40, what="the fixture to disable itself from a task")
        storm = counts["storm"]
        await ctx.ticks(10)
        ctx.info["storm_runs"] = {"at_disable": storm, "after": counts["storm"]}
        assert counts["storm"] == storm, "tasks kept running after their plugin disabled itself"
        assert all(t.is_cancelled for t in tasks)
    finally:
        if not plugin.is_enabled:
            manager.enable_plugin(plugin)


class KeepSender(CommandExecutor):
    def __init__(self):
        CommandExecutor.__init__(self)
        self.kept = []

    def on_command(self, sender, command, args) -> bool:
        self.kept.append((sender, type(sender).__name__, sender.name))
        return True


@scenario("lifetime.command.proxied_sender", plot=True, budget=200)
async def command_proxied_sender(ctx: Context) -> None:
    await summon(ctx, "pig")
    command = ctx.plugin.get_command("testoptmsg")
    keep = KeepSender()
    command.executor = keep
    try:
        result = ctx.command(f"execute as @e[type=pig,{ctx.plot.volume}] run testoptmsg lifetime")
    finally:
        command.executor = None
    ctx.info["execute"] = result.output
    assert keep.kept, f"the /execute-proxied command never reached the executor: {result.output}"
    sender, kind, name = keep.kept[0]
    ctx.info["sender"] = {"class": kind, "name_during_command": name}
    await ctx.ticks(2)
    probe(
        ctx,
        "after_command",
        {
            "name": lambda: sender.name,
            "caller.name": lambda: sender.caller.name,
            "callee.name": lambda: sender.callee.name,
            "has_permission": lambda: sender.has_permission(GRANTED),
            "send_message": lambda: sender.send_message("lifetime"),
        },
    )
    seen = ctx.info["after_command"]
    wrong = {k: v for k, v in seen.items() if k == "name" and v != f"returned {name!r}" and not v.startswith("raised ")}
    assert not wrong, f"a sender kept past its command reads garbage: {wrong}"


@scenario("lifetime.permission.disabled_plugin", budget=200)
async def permission_disabled_plugin(ctx: Context) -> None:
    manager = ctx.server.plugin_manager
    plugin = fixture(ctx)
    console = ctx.server.command_sender
    attachment = console.add_attachment(plugin, GRANTED, True)
    try:
        assert console.has_permission(GRANTED)
        manager.disable_plugin(plugin)
        assert attachment.plugin.name == FIXTURE
        ctx.info["granted_while_disabled"] = console.has_permission(GRANTED)
        raised, what = touch(lambda: console.add_attachment(plugin, GRANTED, True))
        ctx.info["add_attachment_while_disabled"] = what
        assert raised and "ValueError" in what, what
        attachment.set_permission(GRANTED, False)
        assert not console.has_permission(GRANTED)
        assert attachment.remove() is True
        assert attachment.remove() is False
    finally:
        if not plugin.is_enabled:
            manager.enable_plugin(plugin)
        attachment.remove()


def retained_registry_entries() -> dict:
    return {
        "item_type": (ItemType.get("minecraft:diamond"), "minecraft:diamond"),
        "block_type": (BlockType.get("minecraft:stone"), "minecraft:stone"),
        "actor_type": (ActorType.get("minecraft:pig"), "minecraft:pig"),
        "effect_type": (EffectType.get("minecraft:speed"), "minecraft:speed"),
    }


async def across_reload(ctx: Context, key: str, arm, check) -> None:
    stash = getattr(sys, key, None)
    if stash is None:
        setattr(sys, key, arm(ctx))
        await release_plot_area(ctx)
        await ctx.console("reload")
        await ctx.ticks(1200)
        raise AssertionError("the plugin was not reloaded")
    delattr(sys, key)
    gc.collect()
    await ctx.ticks(5)
    check(ctx, stash)


def arm_attachment(ctx: Context) -> dict:
    return {"attachment": ctx.server.command_sender.add_attachment(fixture(ctx), GRANTED, True)}


def check_attachment(ctx: Context, stash: dict) -> None:
    attachment = stash["attachment"]
    seen = probe(
        ctx,
        "attachment_after_reload",
        {
            "permissions": lambda: attachment.permissions,
            "console_granted": lambda: ctx.server.command_sender.has_permission(GRANTED),
            "plugin.name": lambda: attachment.plugin.name,
        },
    )
    attachment.remove()
    name = seen["plugin.name"]
    assert name == f"returned {FIXTURE!r}" or name.startswith("raised "), (
        f"PermissionAttachment.plugin kept across /reload reads garbage: {name}"
    )


def arm_task(ctx: Context) -> dict:
    return {"task": ctx.server.scheduler.run_task(fixture(ctx), lambda: None, delay=100000)}


def check_task(ctx: Context, stash: dict) -> None:
    task = stash["task"]
    seen = probe(
        ctx,
        "task_after_reload",
        {
            "is_cancelled": lambda: task.is_cancelled,
            "owner.name": lambda: task.owner.name,
        },
    )
    assert seen["is_cancelled"] == "returned True", seen
    owner = seen["owner.name"]
    assert owner == f"returned {FIXTURE!r}" or owner.startswith("raised "), (
        f"Task.owner of a task kept across /reload reads garbage: {owner}"
    )


def arm_registry(ctx: Context) -> dict:
    return retained_registry_entries()


def check_registry(ctx: Context, stash: dict) -> None:
    seen = probe(ctx, "registry_after_reload", {name: lambda e=entry: str(e.id) for name, (entry, _) in stash.items()})
    wrong = {
        name: what
        for name, what in seen.items()
        if what != f"returned {stash[name][1]!r}" and not what.startswith("raised ")
    }
    assert not wrong, f"registry entries kept across /reload read garbage: {wrong}"


@scenario("lifetime.reload.attachment_plugin", budget=1500)
async def reload_attachment_plugin(ctx: Context) -> None:
    await across_reload(ctx, RELOAD_STASH + "_attachment", arm_attachment, check_attachment)


def arm_command(ctx: Context) -> dict:
    command = fixture(ctx).get_command("fixturecmd")
    assert command is not None, "the fixture plugin has no fixturecmd"
    return {"command": command}


def check_command(ctx: Context, stash: dict) -> None:
    command = stash["command"]
    seen = probe(ctx, "command_after_reload", {"executor": lambda: command.executor, "plugin": lambda: command.plugin})
    assert seen["executor"] == "returned None", seen
    assert seen["plugin"] == "returned None", seen


@scenario("lifetime.reload.command_executor", budget=1500)
async def reload_command_executor(ctx: Context) -> None:
    await across_reload(ctx, RELOAD_STASH + "_command", arm_command, check_command)


@scenario("lifetime.reload.task_owner", budget=1500)
async def reload_task_owner(ctx: Context) -> None:
    await across_reload(ctx, RELOAD_STASH + "_task", arm_task, check_task)


def arm_self_kept(ctx: Context) -> dict:
    plugin = fixture(ctx)
    plugin.kept_attachment = ctx.server.command_sender.add_attachment(plugin, GRANTED, True)
    plugin.kept_task = ctx.server.scheduler.run_task(plugin, lambda: plugin.name, delay=1, period=20)
    return {"plugin": weakref.ref(plugin), "attachment": plugin.kept_attachment}


def check_self_kept(ctx: Context, stash: dict) -> None:
    attachment = stash["attachment"]
    seen = probe(ctx, "self_kept_after_reload", {"attachment.plugin": lambda: attachment.plugin})
    attachment.remove()
    assert seen["attachment.plugin"] == "returned None", seen
    assert stash["plugin"]() is None, "the replaced plugin is still alive after /reload"


@scenario("lifetime.reload.self_kept", budget=1500)
async def reload_self_kept(ctx: Context) -> None:
    await across_reload(ctx, RELOAD_STASH + "_self_kept", arm_self_kept, check_self_kept)


def arm_metrics(ctx: Context) -> dict:
    plugin = fixture(ctx)
    metrics = Metrics(plugin, 1)
    metrics.shutdown()
    plugin.kept_metrics = metrics
    return {"plugin": weakref.ref(plugin), "metrics": weakref.ref(metrics)}


def check_metrics(ctx: Context, stash: dict) -> None:
    assert stash["metrics"]() is None, "the Metrics of the replaced plugin is still alive after /reload"
    assert stash["plugin"]() is None, "the replaced plugin that created a Metrics is still alive after /reload"


@scenario("lifetime.reload.metrics", budget=1500)
async def reload_metrics(ctx: Context) -> None:
    await across_reload(ctx, RELOAD_STASH + "_metrics", arm_metrics, check_metrics)


@scenario("lifetime.reload.registry_entries", budget=1500)
async def reload_registry_entries(ctx: Context) -> None:
    await across_reload(ctx, RELOAD_STASH + "_registry", arm_registry, check_registry)


def probe_one(action) -> str:
    return touch(action)[1]


async def release_plot_area(ctx: Context) -> None:
    ctx.command(f"tickingarea remove {PLOT_AREA}")
    await ctx.ticks(5)


@scenario("lifetime.reload.from_task", budget=1500)
async def reload_from_task(ctx: Context) -> None:
    if getattr(sys, RELOAD_TASK_STASH, None) is None:
        setattr(sys, RELOAD_TASK_STASH, {"armed": True})
        await release_plot_area(ctx)
        ctx.server.reload()
        await ctx.ticks(1200)
        raise AssertionError("Server.reload() from a task returned but the plugin was never re-enabled")
    delattr(sys, RELOAD_TASK_STASH)
    assert fixture(ctx).is_enabled
    assert ctx.plugin.is_enabled


RAKNET_MAGIC = bytes.fromhex("00ffff00fefefefefdfdfdfd12345678")


def ping_storm(port: int, stop: threading.Event) -> None:
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        sock.setblocking(False)
        while not stop.is_set():
            ping = b"\x01" + struct.pack(">q", int(time.monotonic() * 1000)) + RAKNET_MAGIC + struct.pack(">q", 1)
            try:
                sock.sendto(ping, ("127.0.0.1", port))
                sock.recvfrom(2048)
            except (BlockingIOError, ConnectionResetError):
                pass


def signaling_ping_storm(port: int, stop: threading.Event) -> None:
    request = b"GET /v1/join HTTP/1.1\r\nHost: 127.0.0.1\r\nConnection: close\r\n\r\n"
    while not stop.is_set():
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=1) as sock:
                sock.sendall(request)
                while sock.recv(4096):
                    pass
        except OSError:
            stop.wait(0.05)


@scenario("lifetime.event.ping_during_disable", budget=1500)
async def ping_during_disable(ctx: Context) -> None:
    manager = ctx.server.plugin_manager
    plugin = fixture(ctx)
    stop = threading.Event()
    storms = [
        threading.Thread(target=target, args=(ctx.server.port, stop), daemon=True)
        for target in (ping_storm, signaling_ping_storm)
    ]
    for storm in storms:
        storm.start()
    try:
        await ctx.until(lambda: plugin.pings > 0, timeout=100, what="the fixture to see a ping")
        for _ in range(50):
            manager.disable_plugin(plugin)
            manager.enable_plugin(plugin)
            await ctx.ticks(1)
        before = plugin.pings
        await ctx.until(lambda: plugin.pings > before, timeout=100, what="pings after the last re-enable")
    finally:
        stop.set()
        for storm in storms:
            storm.join(timeout=5)
    assert plugin.is_enabled


@scenario("lifetime.player.after_quit", budget=1500)
async def player_after_quit(ctx: Context) -> None:
    name = "ltQuit"
    board = ctx.server.scoreboard
    if board.get_objective("lt_quit") is not None:
        board.get_objective("lt_quit").unregister()
    bot, player = await join(ctx, name)
    inventory = player.inventory
    ender_chest = player.ender_chest
    health = player.get_attribute("minecraft:health")
    inventory.set_item(0, ItemStack("minecraft:diamond", 3))
    ender_chest.set_item(0, ItemStack("minecraft:emerald", 2))
    attachment = player.add_attachment(ctx.plugin, GRANTED, True)
    assert player.has_permission(GRANTED)
    bar = ctx.server.create_boss_bar("lifetime", BarColor.RED, BarStyle.SOLID)
    bar.add_player(player)
    objective = board.add_objective("lt_quit", Criteria.DUMMY)
    objective.get_score(player).value = 3
    quit = ctx.capture(PlayerQuitEvent, named(name))
    try:
        await leave(ctx, name)
        assert quit.count == 1, quit.events
        await ctx.ticks(20)

        assert player.is_valid is False
        failures = {}
        failures["player_held"] = unraised(
            ctx,
            "player_held",
            {
                "name": lambda: player.name,
                "location": lambda: player.location,
                "kick": lambda: player.kick("lifetime"),
                "inventory.size": lambda: inventory.size,
                "inventory.get_item": lambda: inventory.get_item(0),
                "ender_chest.size": lambda: ender_chest.size,
                "ender_chest.get_item": lambda: ender_chest.get_item(0),
                "health.value": lambda: health.value,
                "get_attribute": lambda: player.get_attribute("minecraft:health"),
            },
        )
        probe(
            ctx,
            "player_held_tolerant",
            {
                "send_message": lambda: player.send_message("lifetime"),
                "is_op": lambda: player.is_op,
                "has_permission": lambda: player.has_permission(GRANTED),
                "attachment.permissible": lambda: attachment.permissible is not None,
                "score": lambda: objective.get_score(player).value,
                "reset_scores": lambda: board.reset_scores(player),
                "bar.add_player": lambda: bar.add_player(player),
                "bar.progress": lambda: setattr(bar, "progress", 0.5),
                "bar.players": lambda: [p.is_valid for p in bar.players],
                "bar.remove_player": lambda: bar.remove_player(player),
            },
        )
        assert bar.players == [], "a boss bar still lists a player who left"

        player = None
        quit.events.clear()
        gc.collect()
        await ctx.ticks(10)
        ctx.info["attachment.permissible_after_drop"] = probe_one(lambda: attachment.permissible)
        ctx.info["attachment.remove_after_drop"] = probe_one(lambda: attachment.remove())
        seen = probe(
            ctx,
            "player_dropped",
            {
                "inventory.size": lambda: inventory.size,
                "inventory.get_item": lambda: inventory.get_item(0),
                "ender_chest.size": lambda: ender_chest.size,
                "ender_chest.get_item": lambda: ender_chest.get_item(0),
            },
        )
        failures["player_dropped"] = {k: v for k, v in seen.items() if "no longer valid" not in v}
        failures = {k: v for k, v in failures.items() if v}
        assert not failures, f"touching a departed player's objects did not raise: {failures}"
    finally:
        bar.remove_all()
        if board.get_objective("lt_quit") is not None:
            board.get_objective("lt_quit").unregister()


def kicker(ctx: Context, label: str):
    def kick(event) -> None:
        if label in ctx.info.setdefault("kicked_in", {}):
            return
        ctx.info["kicked_in"][label] = probe_one(lambda: event.player.kick(f"lifetime {label}"))

    return kick


@scenario("lifetime.player.kick_in_handler", budget=3000)
async def player_kick_in_handler(ctx: Context) -> None:
    outcomes = {}

    ctx.capture(PlayerLoginEvent, named("ltKickLogin"), mutate=kicker(ctx, "login"))
    try:
        await ctx.bot("ltKickLogin")
        outcomes["login"] = "joined"
    except HarnessError as e:
        outcomes["login"] = f"join failed: {e}"
    await ctx.ticks(20)
    assert ctx.server.get_player("ltKickLogin") is None or outcomes["login"] == "joined"

    ctx.capture(PlayerJoinEvent, named("ltKickJoin"), mutate=kicker(ctx, "join"))
    try:
        await ctx.bot("ltKickJoin")
        outcomes["join"] = "joined"
    except HarnessError as e:
        outcomes["join"] = f"join failed: {e}"
    await ctx.until(lambda: ctx.server.get_player("ltKickJoin") is None, timeout=100, what="ltKickJoin kicked")

    for label, event, act in (
        ("chat", PlayerChatEvent, ("local_player.chat", "lifetime")),
        ("command", PlayerCommandEvent, ("local_player.run_command", "/me lifetime")),
        ("move", PlayerMoveEvent, ("local_player.move_to", None)),
    ):
        name = f"ltKick{label.capitalize()}"
        bot, player = await join(ctx, name)
        ctx.capture(event, named(name), mutate=kicker(ctx, label))
        method, argument = act
        if argument is None:
            location = player.location
            argument = [location.x + 3, location.y, location.z]
        await bot.call(method, argument)
        await ctx.until(lambda n=name: ctx.server.get_player(n) is None, timeout=200, what=f"{name} kicked")
        outcomes[label] = await bot.call("get_disconnect_reason")
        player = None

    name = "ltKickQuit"
    bot, player = await join(ctx, name)

    def during_quit(event) -> None:
        quitting = event.player
        probe(
            ctx,
            "during_quit",
            {
                "kick": lambda: quitting.kick("lifetime again"),
                "send_message": lambda: quitting.send_message("bye"),
                "inventory.get_item": lambda: quitting.inventory.get_item(0),
                "location": lambda: quitting.location,
                "add_attachment": lambda: quitting.add_attachment(ctx.plugin, GRANTED, True).remove(),
                "teleport": lambda: quitting.teleport(quitting.location),
            },
        )

    ctx.capture(PlayerQuitEvent, named(name), mutate=during_quit)
    await leave(ctx, name)
    player = None
    await ctx.ticks(20)
    ctx.info["outcomes"] = outcomes
    for label in ("chat", "command", "move"):
        assert ctx.info.get("kicked_in", {}).get(label) == "returned None", ctx.info.get("kicked_in")


@scenario("lifetime.visibility.holds", plot=True, budget=2000)
async def visibility_holds(ctx: Context) -> None:
    manager = ctx.server.plugin_manager
    plugin = fixture(ctx)
    bot, viewer = await join(ctx, "ltHider")
    other_bot, other = await join(ctx, "ltHidden")
    pig = await summon(ctx, "pig")
    try:
        viewer.hide_actor(plugin, pig)
        viewer.hide_actor(plugin, other)
        assert not viewer.can_see(pig) and not viewer.can_see(other)
        manager.disable_plugin(plugin)
        assert not viewer.can_see(pig), "a disabled plugin lost its hold"
        raised, what = touch(lambda: viewer.hide_actor(plugin, pig))
        ctx.info["hide_while_disabled"] = what
        ctx.info["show_while_disabled"] = probe_one(lambda: viewer.show_actor(plugin, pig))
        ctx.info["can_see_after_show_while_disabled"] = viewer.can_see(pig)
        manager.enable_plugin(plugin)
        viewer.hide_actor(plugin, pig)

        ctx.run(f"kill @e[type=pig,{ctx.plot.volume}]")
        await gone(ctx, pig)
        await ctx.ticks(5)
        probe(
            ctx,
            "hidden_pig_dead",
            {
                "can_see": lambda: viewer.can_see(pig),
                "show_actor": lambda: viewer.show_actor(plugin, pig),
                "hide_actor": lambda: viewer.hide_actor(plugin, pig),
            },
        )

        await leave(ctx, "ltHidden")
        await ctx.ticks(10)
        probe(
            ctx,
            "hidden_player_left",
            {
                "can_see": lambda: viewer.can_see(other),
                "show_actor": lambda: viewer.show_actor(plugin, other),
            },
        )
        await leave(ctx, "ltHider")
        await ctx.ticks(10)
        probe(
            ctx,
            "viewer_left",
            {
                "hide_actor": lambda: viewer.hide_actor(plugin, other),
                "can_see": lambda: viewer.can_see(other),
            },
        )
    finally:
        if not plugin.is_enabled:
            manager.enable_plugin(plugin)


@scenario("lifetime.event.inventory_open_retained", plot=True, budget=1500)
async def event_inventory_open_retained(ctx: Context) -> None:
    name = "ltChest"
    kept = []

    def keep(event) -> bool:
        if event.player.name != name:
            return False
        item = event.inventory.get_item(0)
        kept.append((event.inventory, None if item is None else (str(item.type), item.amount)))
        return True

    opened = ctx.capture(InventoryOpenEvent, keep)
    x, y, z = ctx.plot.pos(8, 0, 10)
    ctx.run(f"setblock {x} {y} {z} chest")
    ctx.overworld.get_block_at(x, y, z).capture_state(use_snapshot=False).inventory.set_item(
        0, ItemStack("minecraft:diamond", 4)
    )
    bot, player = await join(ctx, name)
    await into_plot(ctx, bot, player, 8, 8)
    await bot.call("local_player.look_at", [x + 0.5, y + 0.5, z + 0.5])
    await ctx.ticks(2)
    await bot.call("game_mode.use_item_on", [x, y, z], 1)
    await ctx.until(lambda: opened.count, timeout=100, what="InventoryOpenEvent for the chest")
    inventory, during = kept[0]
    kept.clear()
    ctx.info["during_handler"] = during
    assert during == ("minecraft:diamond", 4), during
    await ctx.ticks(2)
    gc.collect()
    seen = probe(ctx, "after_dispatch", {"size": lambda: inventory.size})
    assert seen["size"] in ("returned 27",) or seen["size"].startswith("raised "), (
        f"InventoryOpenEvent.inventory kept past its handler reads garbage while the chest still exists: {seen}"
    )
    await bot.call("local_player.close_container_screen")
    await ctx.ticks(10)
    ctx.run(f"setblock {x} {y} {z} air")
    await ctx.ticks(5)
    must_raise(ctx, "after_chest_removed", {"size": lambda: inventory.size})


class CountingRenderer(MapRenderer):
    def __init__(self):
        MapRenderer.__init__(self, False)
        self.calls = 0

    def render(self, view, canvas, player) -> None:
        self.calls += 1
        canvas.set_pixel_color(1, 1, (255, 0, 0, 255))


class OneShotRenderer(CountingRenderer):
    def __init__(self, replacement: CountingRenderer, errors: list):
        CountingRenderer.__init__(self)
        self.replacement = replacement
        self.errors = errors

    def render(self, view, canvas, player) -> None:
        CountingRenderer.render(self, view, canvas, player)
        if self.calls == 1:
            try:
                view.remove_renderer(self)
                view.add_renderer(self.replacement)
            except Exception as e:
                self.errors.append(f"{type(e).__name__}: {e}")


@scenario("lifetime.map.renderer_removes_itself", budget=1500)
async def map_renderer_removes_itself(ctx: Context) -> None:
    name = "ltMap"
    bot, player = await join(ctx, name)
    view = ctx.server.create_map(ctx.overworld)
    for renderer in view.renderers:
        view.remove_renderer(renderer)
    errors = []
    replacement = CountingRenderer()
    one_shot = OneShotRenderer(replacement, errors)
    view.add_renderer(one_shot)
    item = ItemStack("minecraft:filled_map")
    meta = item.item_meta
    meta.map_view = view
    item.set_item_meta(meta)
    player.inventory.set_item(0, item)
    await bot.call("local_player.select_slot", 0)
    since = await bot.call("get_next_packet_index")
    try:
        await ctx.until(lambda: replacement.calls >= 2, timeout=600, what="the map to render after the swap")
    finally:
        received = await bot.call("get_received_count", "ClientboundMapItemDataPacket")
        ctx.info["map"] = {
            "one_shot_calls": one_shot.calls,
            "replacement_calls": replacement.calls,
            "renderers": len(view.renderers),
            "map_packets": received,
            "since": since,
            "errors": errors,
        }
    assert one_shot.calls == 1, f"a renderer that removed itself rendered again: {one_shot.calls}"
    assert not errors, errors


@scenario("lifetime.reload.map_renderer", budget=2000)
async def reload_map_renderer(ctx: Context) -> None:
    key = RELOAD_STASH + "_map"
    name = "ltMapReload"
    stash = getattr(sys, key, None)
    if stash is None:
        await ctx.call("bot.spawn", bot=name, timeout=1200)
        player = await ctx.until(lambda: ctx.server.get_player(name), timeout=60, what=f"{name} online")
        view = ctx.server.create_map(ctx.overworld)
        for renderer in view.renderers:
            view.remove_renderer(renderer)
        renderer = CountingRenderer()
        view.add_renderer(renderer)
        item = ItemStack("minecraft:filled_map")
        meta = item.item_meta
        meta.map_view = view
        item.set_item_meta(meta)
        player.inventory.set_item(0, item)
        await ctx.call("bot.call", bot=name, method="local_player.select_slot", args=[0], kwargs={})
        await ctx.until(lambda: renderer.calls >= 2, timeout=600, what="the map to render before the reload")
        setattr(sys, key, {"renderer": renderer, "view_id": view.id})
        await release_plot_area(ctx)
        await ctx.console("reload")
        await ctx.ticks(1200)
        raise AssertionError("the plugin was not reloaded")
    delattr(sys, key)
    try:
        renderer = stash["renderer"]
        before = renderer.calls
        gc.collect()
        await ctx.until(lambda: renderer.calls > before, timeout=600, what="the old renderer to keep rendering")
        view = ctx.server.get_map(stash["view_id"])
        ctx.info["after_reload"] = {
            "calls_before": before,
            "calls_after": renderer.calls,
            "renderers": probe_one(lambda: len(view.renderers)),
        }
    finally:
        await ctx.call("bot.close", bot=name)


@scenario("lifetime.shutdown.bots_online", enabled=os.environ.get(SHUTDOWN_ENV) == "1", budget=1200)
async def shutdown_bots_online(ctx: Context) -> None:
    for name in ("ltStop1", "ltStop2", "ltStop3"):
        await ctx.call("bot.spawn", bot=name, timeout=1200)
    await ctx.until(lambda: len(ctx.server.online_players) >= 3, timeout=100, what="three bots online")
    kept = list(ctx.server.online_players)
    KEPT_AT_SHUTDOWN.extend(kept)
    KEPT_AT_SHUTDOWN.extend(p.inventory for p in kept)
    bar = ctx.server.create_boss_bar("shutdown", BarColor.BLUE, BarStyle.SOLID)
    for player in kept:
        bar.add_player(player)
    ctx.server.shutdown()
    await ctx.ticks(1200)


RELOAD_SUMMON_STASH = "_endstone_test_lifetime_reload_summon"
SUMMON_KINDS = ("pig", "cow", "zombie", "sheep")


def plot_actors(ctx: Context) -> dict:
    return {a.id: str(a.type) for a in ctx.level.actors if in_plot(ctx, a)}


@scenario("lifetime.reload.summon_entity_type", plot=True, budget=2000)
async def reload_summon_entity_type(ctx: Context) -> None:
    stash = getattr(sys, RELOAD_SUMMON_STASH, None)
    if stash is None:
        ctx.run(f"summon pig {ctx.plot.at(8, 0, 8)}")
        await ctx.until(lambda: "minecraft:pig" in plot_actors(ctx).values(), what="a pig before the reload")
        setattr(sys, RELOAD_SUMMON_STASH, True)
        await release_plot_area(ctx)
        await ctx.console("reload")
        await ctx.ticks(1200)
        raise AssertionError("the plugin was not reloaded")
    delattr(sys, RELOAD_SUMMON_STASH)
    wrong = {}
    for kind in SUMMON_KINDS:
        known = set(plot_actors(ctx))
        ctx.run(f"summon {kind} {ctx.plot.at(8, 0, 8)}")
        await ctx.ticks(10)
        spawned = sorted(t for i, t in plot_actors(ctx).items() if i not in known and t != "minecraft:item")
        if spawned != [f"minecraft:{kind}"]:
            wrong[kind] = spawned
    ctx.info["summoned_after_reload"] = wrong
    assert not wrong, f"/summon spawned the wrong entity after /reload: {wrong}"


async def broadcast_while_players_churn(ctx: Context, send, received, interval: float) -> int:
    stop = threading.Event()
    sent = [0]
    errors: list[str] = []

    def spam() -> None:
        while not stop.is_set():
            try:
                send()
            except Exception as e:
                errors.append(repr(e))
                return
            sent[0] += 1
            time.sleep(interval)

    worker = threading.Thread(target=spam, daemon=True)
    worker.start()
    try:
        for round in range(6):
            name = f"lpbc{round}"
            bot = await ctx.bot(name)
            since = await bot.call("get_next_packet_index")
            ctx.command(f"op {name}")
            await ctx.ticks(5)
            ctx.command(f"deop {name}")
            await ctx.ticks(5)
            await received(bot, since)
            ctx.command(f"kick {name}")
            await ctx.until(lambda: ctx.server.get_player(name) is None, timeout=200, what=f"{name} to leave")
    finally:
        stop.set()
        worker.join(5)
    assert not errors, errors
    assert sent[0] > 0, "the broadcasting thread sent nothing"
    return sent[0]


@scenario("lifetime.permission.subscription_race", budget=6000)
async def permission_subscription_race(ctx: Context) -> None:
    async def nothing(bot, since) -> None:
        pass

    ctx.info["broadcasts"] = await broadcast_while_players_churn(
        ctx, lambda: ctx.server.broadcast("endstone-test permission race", "endstone_test.race_probe"), nothing, 0.0005
    )


@scenario("lifetime.permission.broadcast_async", budget=6000)
async def permission_broadcast_async(ctx: Context) -> None:
    text = "endstone-test async broadcast"

    async def heard(bot, since) -> None:
        for _ in range(100):
            packets = [p["packet"] for p in await bot.call("get_received_packets", "TextPacket", since)]
            if any(text in str(p) for p in packets):
                return
            await ctx.ticks(1)
        raise AssertionError(f"{bot.name} never received the broadcast")

    ctx.info["broadcasts"] = await broadcast_while_players_churn(
        ctx, lambda: ctx.server.broadcast_message(text), heard, 0.005
    )


@scenario("lifetime.thread.player_api", budget=6000)
async def thread_player_api(ctx: Context) -> None:
    from endstone.form import MessageForm
    from endstone.level import Location

    tip = "endstone-test async tip"
    where = Location(None, 0.0, 64.0, 0.0)
    targets: list = []
    stop = threading.Event()
    sent = [0]
    errors: list[str] = []

    def spam() -> None:
        while not stop.is_set():
            for player in list(targets):
                try:
                    player.send_tip(tip)
                    player.send_title("endstone-test", "async title")
                    player.play_sound(where, "random.pop", 1.0, 1.0)
                    player.spawn_particle("minecraft:heart_particle", where)
                    player.send_form(MessageForm(title="endstone-test", content="async form", button1="a", button2="b"))
                except Exception as e:
                    errors.append(repr(e))
                    return
                sent[0] += 1
            time.sleep(0.005)

    worker = threading.Thread(target=spam, daemon=True)
    worker.start()
    try:
        for round in range(6):
            name = f"lptapi{round}"
            bot = await ctx.bot(name)
            since = await bot.call("get_next_packet_index")
            targets.append(ctx.server.get_player(name))
            heard = False
            for _ in range(100):
                packets = [p["packet"] for p in await bot.call("get_received_packets", "TextPacket", since)]
                if any(tip in str(p) for p in packets):
                    heard = True
                    break
                await ctx.ticks(1)
            assert heard, f"{name} never received the tip"
            ctx.command(f"kick {name}")
            await ctx.until(lambda: ctx.server.get_player(name) is None, timeout=200, what=f"{name} to leave")
    finally:
        stop.set()
        worker.join(5)
    ctx.info["sends"] = sent[0]
    assert not errors, errors


@scenario("lifetime.thread.kick")
async def thread_kick(ctx: Context) -> None:
    await ctx.bot("lptkick")
    player = ctx.server.get_player("lptkick")
    result: list[str] = []

    def kick() -> None:
        try:
            player.kick("from another thread")
            result.append("returned")
        except Exception as e:
            result.append(repr(e))

    worker = threading.Thread(target=kick)
    worker.start()
    await ctx.until(lambda: not worker.is_alive(), timeout=100, what="the kicking thread")
    await ctx.ticks(10)
    assert result == ["RuntimeError('Asynchronous player kick!')"], result
    assert ctx.server.get_player("lptkick") is not None, "a kick from another thread still disconnected the player"


@scenario("lifetime.thread.attachments", budget=6000)
async def thread_attachments(ctx: Context) -> None:
    plugin = ctx.plugin
    targets: list = []
    stop = threading.Event()
    rounds = [0]
    errors: list[str] = []

    def churn() -> None:
        while not stop.is_set():
            for player in list(targets):
                try:
                    attachment = player.add_attachment(plugin, "endstone_test.thread_perm", True)
                    attachment.set_permission("endstone_test.thread_other", True)
                    attachment.unset_permission("endstone_test.thread_other")
                    player.recalculate_permissions()
                    attachment.remove()
                except Exception as e:
                    errors.append(repr(e))
                    return
                rounds[0] += 1
            time.sleep(0.001)

    worker = threading.Thread(target=churn, daemon=True)
    worker.start()
    try:
        for round in range(6):
            name = f"lptatt{round}"
            await ctx.bot(name)
            player = ctx.server.get_player(name)
            targets.append(player)
            for _ in range(3):
                ctx.command(f"op {name}")
                await ctx.ticks(3)
                ctx.command(f"deop {name}")
                await ctx.ticks(3)
            ctx.command(f"kick {name}")
            await ctx.until(lambda: ctx.server.get_player(name) is None, timeout=200, what=f"{name} to leave")
    finally:
        stop.set()
        worker.join(5)
    ctx.info["rounds"] = rounds[0]
    assert not errors, errors
    assert rounds[0] > 0, "the attachment thread did nothing"
    for player in targets:
        assert not player.is_permission_set("endstone_test.thread_perm"), player.name
