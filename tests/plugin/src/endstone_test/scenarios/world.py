import math

from endstone.event import (
    ActorChangeBlockEvent,
    ActorCollideWithActorEvent,
    ActorDismountEvent,
    ActorKnockbackEvent,
    BlockFromToEvent,
    BlockPistonExtendEvent,
    BlockPistonRetractEvent,
    LeavesDecayEvent,
)
from endstone.util import Vector

from endstone_test.harness import Context, scenario
from endstone_test.scenarios.actor import actors, plot_filter


def at_pos(xyz: tuple[int, int, int]):
    return lambda e: (e.block.x, e.block.y, e.block.z) == xyz


def block_type(ctx: Context, dx: int, dy: int, dz: int) -> str:
    return str(ctx.overworld.get_block_at(*ctx.plot.pos(dx, dy, dz)).type)


# =============================================================================
# Leaves
# =============================================================================


async def loose_leaves(ctx: Context, capture) -> str:
    at = ctx.plot.at(8, 0, 8)
    ctx.run("gamerule randomtickspeed 4096")
    try:
        ctx.run(f'setblock {at} oak_leaves ["update_bit"=true,"persistent_bit"=false]')
        await ctx.until(lambda: capture.count, timeout=200, what="LeavesDecayEvent")
        await ctx.ticks(5)
    finally:
        ctx.run("gamerule randomtickspeed 1")
    return at


@scenario(
    "block.leaves_decay.normal",
    covers={LeavesDecayEvent: "normal"},
    plot=True,
    budget=400,
)
async def leaves_normal(ctx: Context) -> None:
    decay = ctx.capture(LeavesDecayEvent, at_pos(ctx.plot.pos(8, 0, 8)))
    at = await loose_leaves(ctx, decay)
    assert decay.count == 1, decay.events
    assert decay.events[0]["block"]["type"] == "minecraft:oak_leaves"
    assert ctx.block_is(at, "air"), "the leaves did not decay"


@scenario(
    "block.leaves_decay.cancel",
    covers={LeavesDecayEvent: "cancel"},
    plot=True,
    budget=400,
)
async def leaves_cancel(ctx: Context) -> None:
    decay = ctx.capture(LeavesDecayEvent, at_pos(ctx.plot.pos(8, 0, 8)), cancel=True)
    at = await loose_leaves(ctx, decay)
    assert ctx.block_is(at, "oak_leaves"), "cancelled leaves decayed anyway"


# =============================================================================
# Liquid flow
# =============================================================================


def in_plot_block(ctx: Context):
    plot = ctx.plot
    return lambda e: (
        plot.x <= e.block.x < plot.x + 16 and plot.z <= e.block.z < plot.z + 16
    )


async def water_source(ctx: Context, flow, every) -> None:
    ctx.run(f"setblock {ctx.plot.at(8, 0, 8)} water")
    ctx.run(f"setblock {ctx.plot.at(8, 1, 8)} stone")
    ctx.run(f"setblock {ctx.plot.at(8, 1, 8)} air")
    await ctx.ticks(40)
    ctx.info["flows"] = [(e["block"], e["to_block"]) for e in every.events][:10]
    ctx.info["neighbour"] = block_type(ctx, 9, 0, 8)


@scenario("block.from_to.normal", covers={BlockFromToEvent: "normal"}, plot=True)
async def from_to_normal(ctx: Context) -> None:
    every = ctx.capture(BlockFromToEvent, in_plot_block(ctx))
    flow = ctx.capture(BlockFromToEvent, at_pos(ctx.plot.pos(8, 0, 8)))
    await water_source(ctx, flow, every)
    targets = {
        (e["to_block"]["x"], e["to_block"]["y"], e["to_block"]["z"])
        for e in flow.events
    }
    assert ctx.plot.pos(9, 0, 8) in targets, ctx.info
    assert ctx.info["neighbour"] in ("minecraft:water", "minecraft:flowing_water"), (
        ctx.info
    )


@scenario("block.from_to.cancel", covers={BlockFromToEvent: "cancel"}, plot=True)
async def from_to_cancel(ctx: Context) -> None:
    every = ctx.capture(BlockFromToEvent, in_plot_block(ctx))
    flow = ctx.capture(BlockFromToEvent, at_pos(ctx.plot.pos(8, 0, 8)), cancel=True)
    await water_source(ctx, flow, every)
    assert flow.count, ctx.info
    around = {
        block_type(ctx, 8 + dx, 0, 8 + dz)
        for dx, dz in ((1, 0), (-1, 0), (0, 1), (0, -1))
    }
    assert around == {"minecraft:air"}, (
        f"water flowed out of a source whose flow was cancelled: {around}"
    )


# =============================================================================
# Pistons
# =============================================================================


async def piston_with_stone(ctx: Context) -> None:
    ctx.run(f'setblock {ctx.plot.at(8, 0, 8)} sticky_piston ["facing_direction"=1]')
    ctx.run(f"setblock {ctx.plot.at(8, 1, 8)} stone")


async def power(ctx: Context, on: bool) -> None:
    ctx.run(f"setblock {ctx.plot.at(9, 0, 8)} {'redstone_block' if on else 'air'}")


@scenario(
    "block.piston.normal",
    covers={BlockPistonExtendEvent: "normal", BlockPistonRetractEvent: "normal"},
    plot=True,
)
async def piston_normal(ctx: Context) -> None:
    extend = ctx.capture(BlockPistonExtendEvent, at_pos(ctx.plot.pos(8, 0, 8)))
    retract = ctx.capture(BlockPistonRetractEvent, at_pos(ctx.plot.pos(8, 0, 8)))
    await piston_with_stone(ctx)
    await power(ctx, True)
    await ctx.until(
        lambda: ctx.block_is(ctx.plot.at(8, 2, 8), "stone"),
        timeout=40,
        what="the stone to be pushed up",
    )
    assert extend.count == 1, extend.events
    assert extend.events[0]["direction"] == "UP", extend.events[0]
    await power(ctx, False)
    await ctx.until(
        lambda: ctx.block_is(ctx.plot.at(8, 1, 8), "stone"),
        timeout=40,
        what="the stone to be pulled back",
    )
    assert retract.count == 1, retract.events
    assert retract.events[0]["direction"] == "DOWN", retract.events[0]


@scenario(
    "block.piston.cancel_extend", covers={BlockPistonExtendEvent: "cancel"}, plot=True
)
async def piston_cancel_extend(ctx: Context) -> None:
    extend = ctx.capture(
        BlockPistonExtendEvent, at_pos(ctx.plot.pos(8, 0, 8)), cancel=True
    )
    await piston_with_stone(ctx)
    await power(ctx, True)
    await ctx.until(lambda: extend.count, timeout=40, what="BlockPistonExtendEvent")
    await ctx.ticks(20)
    assert ctx.block_is(ctx.plot.at(8, 1, 8), "stone"), (
        "a cancelled extension moved the stone"
    )


@scenario(
    "block.piston.cancel_retract", covers={BlockPistonRetractEvent: "cancel"}, plot=True
)
async def piston_cancel_retract(ctx: Context) -> None:
    await piston_with_stone(ctx)
    await power(ctx, True)
    await ctx.until(
        lambda: ctx.block_is(ctx.plot.at(8, 2, 8), "stone"),
        timeout=40,
        what="the stone to be pushed up",
    )
    retract = ctx.capture(
        BlockPistonRetractEvent, at_pos(ctx.plot.pos(8, 0, 8)), cancel=True
    )
    await power(ctx, False)
    await ctx.until(lambda: retract.count, timeout=40, what="BlockPistonRetractEvent")
    await ctx.ticks(20)
    assert ctx.block_is(ctx.plot.at(8, 2, 8), "stone"), (
        "a cancelled retraction pulled the stone back"
    )


# =============================================================================
# Actors pushing and hitting each other
# =============================================================================


async def still_pigs(ctx: Context, count: int) -> list:
    """Summon pigs 0.3 blocks apart in the middle of the plot, too slow to walk."""
    x, y, z = ctx.plot.pos(8, 0, 8)
    for i in range(count):
        ctx.run(f"summon pig {x + 0.5 + 0.3 * i} {y} {z + 0.5}")
    pigs = await ctx.until(
        lambda: (
            len(actors(ctx, "minecraft:pig")) == count and actors(ctx, "minecraft:pig")
        ),
        timeout=20,
    )
    ctx.run(f"effect @e[type=pig,{ctx.plot.volume}] slowness 60 255 true")
    return pigs


def distance(a, b) -> float:
    return math.dist((a.location.x, a.location.z), (b.location.x, b.location.z))


@scenario(
    "actor.collide.normal", covers={ActorCollideWithActorEvent: "normal"}, plot=True
)
async def collide_normal(ctx: Context) -> None:
    collide = ctx.capture(
        ActorCollideWithActorEvent,
        lambda e: all(str(a.type) == "minecraft:pig" for a in e.actors),
    )
    first, second = await still_pigs(ctx, 2)
    await ctx.until(
        lambda: distance(first, second) > 0.6,
        timeout=60,
        what="the pigs to push each other apart",
    )
    assert collide.count >= 1


@scenario(
    "actor.collide.cancel", covers={ActorCollideWithActorEvent: "cancel"}, plot=True
)
async def collide_cancel(ctx: Context) -> None:
    collide = ctx.capture(
        ActorCollideWithActorEvent,
        lambda e: all(str(a.type) == "minecraft:pig" for a in e.actors),
        cancel=True,
    )
    first, second = await still_pigs(ctx, 2)
    await ctx.until(
        lambda: collide.count, timeout=40, what="ActorCollideWithActorEvent"
    )
    await ctx.ticks(40)
    assert distance(first, second) < 0.4, (
        f"cancelled collisions still pushed the pigs {distance(first, second):.2f} apart"
    )


async def knock(ctx: Context, capture):
    (target,) = await still_pigs(ctx, 1)
    ctx.run(f"summon sheep {ctx.plot.at(6, 0, 8)}")
    start = target.location
    result = ctx.command(
        f"damage @e[type=pig,{ctx.plot.volume}] 1 entity_attack entity @e[type=sheep,{ctx.plot.volume},c=1]"
    )
    ctx.info["damage"] = result.output
    await ctx.until(lambda: capture.count, timeout=20, what="ActorKnockbackEvent")
    await ctx.ticks(15)
    end = target.location
    return end.x - start.x, end.y - start.y, end.z - start.z


@scenario("actor.knockback.normal", covers={ActorKnockbackEvent: "normal"}, plot=True)
async def knockback_normal(ctx: Context) -> None:
    knockback = ctx.capture(ActorKnockbackEvent, plot_filter(ctx, "minecraft:pig"))
    moved = await knock(ctx, knockback)
    assert knockback.count == 1, knockback.events
    assert knockback.events[0]["source"]["type"] == "minecraft:sheep", knockback.events[
        0
    ]
    assert moved[0] > 0.3, f"the pig was not knocked away from the sheep: {moved}"


@scenario("actor.knockback.cancel", covers={ActorKnockbackEvent: "cancel"}, plot=True)
async def knockback_cancel(ctx: Context) -> None:
    knockback = ctx.capture(
        ActorKnockbackEvent, plot_filter(ctx, "minecraft:pig"), cancel=True
    )
    moved = await knock(ctx, knockback)
    assert abs(moved[0]) < 0.05 and abs(moved[2]) < 0.05, (
        f"a cancelled knockback still moved the pig: {moved}"
    )


@scenario("actor.knockback.mutate", covers={ActorKnockbackEvent: "mutate"}, plot=True)
async def knockback_mutate(ctx: Context) -> None:
    knockback = ctx.capture(
        ActorKnockbackEvent,
        plot_filter(ctx, "minecraft:pig"),
        mutate=lambda e: setattr(e, "knockback", Vector(0, 0, 1)),
    )
    moved = await knock(ctx, knockback)
    assert moved[2] > 0.3 and abs(moved[0]) < 0.1, (
        f"the pig did not follow the knockback the listener wrote: {moved}"
    )


# =============================================================================
# Riding
# =============================================================================


async def pig_in_boat(ctx: Context) -> None:
    ctx.run(f"summon boat {ctx.plot.at(8, 0, 8)}")
    ctx.run(f"summon pig {ctx.plot.at(8, 0, 9)}")
    ctx.run(
        f"ride @e[type=pig,{ctx.plot.volume}] start_riding @e[type=boat,{ctx.plot.volume},c=1] teleport_rider"
    )


def evict(ctx: Context):
    return ctx.command(f"ride @e[type=boat,{ctx.plot.volume}] evict_riders")


@scenario("actor.dismount.normal", covers={ActorDismountEvent: "normal"}, plot=True)
async def dismount_normal(ctx: Context) -> None:
    await pig_in_boat(ctx)
    dismount = ctx.capture(ActorDismountEvent, plot_filter(ctx, "minecraft:pig"))
    ctx.run(f"ride @e[type=pig,{ctx.plot.volume}] stop_riding")
    await ctx.until(lambda: dismount.count, timeout=20, what="ActorDismountEvent")
    assert dismount.events[0]["dismounted"]["type"] == "minecraft:boat"
    ctx.release(dismount)
    after = evict(ctx)
    ctx.info["evict"] = after.output
    assert "Pig" not in after.output, f"the boat still had the pig: {after.output}"


@scenario("actor.dismount.cancel", covers={ActorDismountEvent: "cancel"}, plot=True)
async def dismount_cancel(ctx: Context) -> None:
    await pig_in_boat(ctx)
    dismount = ctx.capture(
        ActorDismountEvent, plot_filter(ctx, "minecraft:pig"), cancel=True
    )
    ctx.command(f"ride @e[type=pig,{ctx.plot.volume}] stop_riding")
    await ctx.until(lambda: dismount.count, timeout=20, what="ActorDismountEvent")
    ctx.release(dismount)
    after = evict(ctx)
    ctx.info["evict"] = after.output
    assert after.success, (
        f"the pig left the boat although the dismount was cancelled: {after.output}"
    )


# =============================================================================
# Mobs changing blocks: a mob trampling farmland
# =============================================================================

FARMLAND = (8, 0, 8)


def changes_at(ctx: Context, kind: str, xyz: tuple[int, int, int]):
    return lambda e: (
        str(e.actor.type) == kind and (e.block.x, e.block.y, e.block.z) == xyz
    )


async def pig_falls_on_farmland(ctx: Context, change) -> str:
    at = ctx.plot.at(*FARMLAND)
    ctx.run(f'setblock {at} farmland ["moisturized_amount"=7]')
    ctx.run(f"summon pig {ctx.plot.at(8, 4, 8)}")
    try:
        await ctx.until(
            lambda: change.count,
            timeout=60,
            what="ActorChangeBlockEvent for the farmland",
        )
    finally:
        ctx.info["farmland"] = block_type(ctx, *FARMLAND)
    await ctx.ticks(5)
    return at


@scenario(
    "actor.change_block.trample", covers={ActorChangeBlockEvent: "normal"}, plot=True
)
async def trample_normal(ctx: Context) -> None:
    change = ctx.capture(
        ActorChangeBlockEvent, changes_at(ctx, "minecraft:pig", ctx.plot.pos(*FARMLAND))
    )
    at = await pig_falls_on_farmland(ctx, change)
    assert change.count == 1, change.events
    assert change.events[0]["block"]["type"] == "minecraft:farmland", change.events[0]
    assert ctx.block_is(at, "dirt"), (
        f"the trampled farmland is {block_type(ctx, *FARMLAND)}"
    )


@scenario(
    "actor.change_block.trample_cancel",
    covers={ActorChangeBlockEvent: "cancel"},
    plot=True,
)
async def trample_cancel(ctx: Context) -> None:
    change = ctx.capture(
        ActorChangeBlockEvent,
        changes_at(ctx, "minecraft:pig", ctx.plot.pos(*FARMLAND)),
        cancel=True,
    )
    at = await pig_falls_on_farmland(ctx, change)
    assert ctx.block_is(at, "farmland"), (
        f"cancelled trampling left {block_type(ctx, *FARMLAND)}"
    )
