import threading

from endstone.event import ChunkLoadEvent, ChunkUnloadEvent

from endstone_test.harness import Context, scenario

AREA = "endstone_test_far_chunk"


def chunk_at(cx: int, cz: int):
    return lambda e: (
        (e.chunk.x, e.chunk.z) == (cx, cz)
        and e.chunk.dimension.id == "minecraft:overworld"
    )


@scenario(
    "chunk.ticking_area",
    covers={ChunkLoadEvent: "normal", ChunkUnloadEvent: "normal"},
    budget=2400,
)
async def ticking_area(ctx: Context) -> None:
    cx, cz = 2000, 2000
    x, z = cx * 16, cz * 16
    probe = f"{x} -61 {z}"
    assert not ctx.overworld.is_chunk_loaded(cx, cz)
    load = ctx.capture(ChunkLoadEvent, chunk_at(cx, cz))
    unload = ctx.capture(ChunkUnloadEvent, chunk_at(cx, cz))

    ctx.run(f"tickingarea add {x} 0 {z} {x + 15} 0 {z + 15} {AREA} true")
    await ctx.until(
        lambda: load.count,
        timeout=600,
        what="ChunkLoadEvent for the ticking area's chunk",
    )
    await ctx.until(
        lambda: ctx.block_is(probe, "grass_block"),
        timeout=200,
        what="the chunk to answer /testforblock",
    )
    assert load.count == 1, load.events
    assert load.events[0]["chunk"] == {
        "x": cx,
        "z": cz,
        "dimension": "minecraft:overworld",
    }
    assert load.events[0]["dimension"] == {"id": "minecraft:overworld"}
    assert unload.count == 0
    assert ctx.overworld.is_chunk_generated(cx, cz) is True
    assert ctx.overworld.unload_chunk(cx, cz) is False, (
        "unload_chunk let go of a chunk a ticking area holds"
    )
    assert ctx.block_is(probe, "grass_block") and unload.count == 0

    ctx.run(f"tickingarea remove {AREA}")
    await ctx.until(
        lambda: unload.count,
        timeout=1200,
        what="ChunkUnloadEvent after the ticking area went away",
    )
    assert unload.events[0]["chunk"] == {
        "x": cx,
        "z": cz,
        "dimension": "minecraft:overworld",
    }
    await ctx.until(
        lambda: not ctx.overworld.is_chunk_loaded(cx, cz),
        timeout=200,
        what="the chunk to unload",
    )
    assert (
        "outside of the world"
        in ctx.command(f"testforblock {probe} grass_block").output
    )
    assert load.count == 1 and unload.count == 1


@scenario("chunk.load_chunk", covers={ChunkLoadEvent: "normal"}, budget=600)
async def load_chunk(ctx: Context) -> None:
    cx, cz = 1875, -1875
    dimension = ctx.overworld
    assert not dimension.is_chunk_loaded(cx, cz)
    load = ctx.capture(ChunkLoadEvent, chunk_at(cx, cz))
    assert dimension.load_chunk(cx, cz) is True
    await ctx.until(
        lambda: dimension.is_chunk_loaded(cx, cz),
        timeout=200,
        what="load_chunk to load the chunk",
    )
    assert load.count == 1, load.events
    for nx, nz in (
        (cx - 1, cz - 1),
        (cx + 1, cz + 1),
        (cx - 1, cz + 1),
        (cx + 1, cz - 1),
    ):
        await ctx.until(
            lambda: dimension.is_chunk_loaded(nx, nz),
            timeout=400,
            what=f"load_chunk to hold the neighbouring chunk {nx},{nz}",
        )
    await ctx.ticks(2)
    floor = dimension.get_block_at(cx * 16, -61, cz * 16)
    assert str(floor.type) == "minecraft:grass_block", floor
    assert dimension.is_chunk_generated(cx, cz)
    unload = ctx.capture(ChunkUnloadEvent, chunk_at(cx, cz))
    released = dimension.unload_chunk(cx, cz)
    ctx.info["unload_chunk_returned"] = released
    ctx.info["unload_events_before_return"] = unload.count
    ctx.info["loaded_after_return"] = dimension.is_chunk_loaded(cx, cz)
    await ctx.ticks(20)
    ctx.info["unload_events_after"] = unload.count
    assert released is True, (
        f"unload_chunk returned {released} for a chunk only load_chunk held: {ctx.info}"
    )
    assert ctx.info["unload_events_before_return"] == 1, ctx.info
    assert ctx.info["loaded_after_return"] is False, ctx.info
    assert unload.count == 1, ctx.info
    assert dimension.unload_chunk(cx, cz) is True, "unload_chunk on an unloaded chunk"


@scenario("chunk.load_chunk_expires", covers={ChunkUnloadEvent: "normal"}, budget=3000)
async def load_chunk_expires(ctx: Context) -> None:
    cx, cz = 1925, 1925
    dimension = ctx.overworld
    assert not dimension.is_chunk_loaded(cx, cz)
    unload = ctx.capture(ChunkUnloadEvent, chunk_at(cx, cz))
    assert dimension.load_chunk(cx, cz) is True
    await ctx.until(
        lambda: dimension.is_chunk_loaded(cx, cz),
        timeout=200,
        what="load_chunk to load the chunk",
    )
    await ctx.ticks(400)
    assert dimension.is_chunk_loaded(cx, cz), "the hold expired before 600 ticks"
    assert dimension.load_chunk(cx, cz) is True
    await ctx.ticks(400)
    assert dimension.is_chunk_loaded(cx, cz), "load_chunk did not restart the timeout"
    await ctx.until(
        lambda: unload.count,
        timeout=1500,
        what="ChunkUnloadEvent after the hold expired",
    )
    assert not dimension.is_chunk_loaded(cx, cz)
    assert unload.count == 1, unload.events
    for nx, nz in ((cx - 1, cz - 1), (cx + 1, cz + 1)):
        await ctx.until(
            lambda: not dimension.is_chunk_loaded(nx, nz),
            timeout=600,
            what=f"the neighbouring chunk {nx},{nz} to unload",
        )


@scenario("chunk.held_at_stop", budget=600)
async def held_at_stop(ctx: Context) -> None:
    cx, cz = -1875, 1875
    dimension = ctx.overworld
    assert dimension.load_chunk(cx, cz) is True
    await ctx.until(
        lambda: dimension.is_chunk_loaded(cx, cz),
        timeout=200,
        what="load_chunk to load the chunk",
    )
    ctx.info["held"] = [cx, cz]


@scenario("chunk.plugin_tickets", budget=600)
async def plugin_tickets(ctx: Context) -> None:
    dimension = ctx.overworld
    plugin = ctx.plugin
    x, z = 375, 375

    assert dimension.is_chunk_generated(x, z) is False
    assert dimension.load_chunk(x, z, generate=False) is False
    assert dimension.is_chunk_loaded(x, z) is False
    assert dimension.unload_chunk_request(x, z) is True

    assert dimension.add_plugin_chunk_ticket(x, z, plugin) is True
    assert dimension.add_plugin_chunk_ticket(x, z, plugin) is False
    assert plugin in dimension.get_plugin_chunk_tickets(x, z)
    assert any(
        (c.x, c.z) == (x, z) for c in dimension.plugin_chunk_tickets.get(plugin, [])
    )
    dimension.unload_chunk(x, z)
    assert plugin in dimension.get_plugin_chunk_tickets(x, z), (
        "unload_chunk dropped a plugin ticket"
    )
    assert dimension.remove_plugin_chunk_ticket(x, z, plugin) is True
    assert dimension.remove_plugin_chunk_ticket(x, z, plugin) is False
    assert dimension.get_plugin_chunk_tickets(x, z) == []

    dimension.add_plugin_chunk_ticket(x + 40, z + 40, plugin)
    dimension.remove_plugin_chunk_tickets(plugin)
    assert plugin not in dimension.plugin_chunk_tickets

    refused = []

    def off_thread() -> None:
        try:
            dimension.load_chunk(x + 80, z + 80)
        except RuntimeError:
            refused.append(True)

    worker = threading.Thread(target=off_thread)
    worker.start()
    await ctx.until(
        lambda: not worker.is_alive(), timeout=100, what="the off-thread load_chunk"
    )
    assert refused, "load_chunk from another thread was not refused"

    assert dimension.add_plugin_chunk_ticket(x, z, plugin) is True
    await ctx.until(
        lambda: dimension.is_chunk_loaded(x, z),
        timeout=200,
        what="the ticketed chunk to load",
    )
    assert dimension.unload_chunk(x, z) is False, "a ticketed chunk agreed to unload"

    resident = next(c for c in dimension.loaded_chunks if (c.x, c.z) != (x, z))
    assert resident.is_loaded is dimension.is_chunk_loaded(resident.x, resident.z)
    assert resident.add_plugin_chunk_ticket(plugin) is True
    assert plugin in resident.plugin_chunk_tickets
    assert resident.remove_plugin_chunk_ticket(plugin) is True
    dimension.remove_plugin_chunk_tickets(plugin)
    dimension.unload_chunk(x, z)


@scenario("chunk.block_actors.use_snapshot", plot=True)
async def block_actors_use_snapshot(ctx: Context) -> None:
    x, y, z = ctx.plot.pos(4, 1, 14)
    ctx.run(f"setblock {x} {y} {z} chest")
    await ctx.ticks(2)
    chunk = next(
        c for c in ctx.overworld.loaded_chunks if (c.x, c.z) == (x >> 4, z >> 4)
    )

    def at_chest(states):
        return [s for s in states if (s.x, s.y, s.z) == (x, y, z)]

    default = at_chest(chunk.block_actors)
    snapshots = at_chest(chunk.get_block_actors())
    live = at_chest(chunk.get_block_actors(use_snapshot=False))
    assert len(default) == len(snapshots) == len(live) == 1
    assert default[0].is_snapshot and snapshots[0].is_snapshot
    assert not live[0].is_snapshot

    seen = []
    filtered = chunk.get_block_actors(
        lambda block: seen.append(block.location) or block.x == x and block.z == z,
        False,
    )
    assert [(s.x, s.y, s.z) for s in filtered] == [(x, y, z)], filtered
    assert not filtered[0].is_snapshot
    assert len(seen) >= 1
    assert chunk.get_block_actors(lambda block: False) == []


@scenario("chunk.held_chunk_ticks", budget=900)
async def held_chunk_ticks(ctx: Context) -> None:
    dimension = ctx.overworld
    plugin = ctx.plugin
    for cx, cz, hold in (
        (1825, 1825, lambda cx, cz: dimension.load_chunk(cx, cz)),
        (-1825, 1825, lambda cx, cz: dimension.add_plugin_chunk_ticket(cx, cz, plugin)),
    ):
        x, z = cx * 16 + 8, cz * 16 + 8
        assert not dimension.is_chunk_loaded(cx, cz)
        assert hold(cx, cz) is True
        await ctx.until(
            lambda: dimension.is_chunk_loaded(cx, cz),
            timeout=200,
            what="the chunk to load",
        )
        await ctx.until(
            lambda: ctx.command(f"setblock {x} -60 {z} flowing_water").success,
            timeout=200,
            what="setblock to reach the held chunk",
        )
        await ctx.until(
            lambda: ctx.block_is(f"{x + 1} -60 {z}", "water"),
            timeout=200,
            what="the water to spread inside the held chunk with no player nearby",
        )
        if cx > 0:
            dimension.unload_chunk(cx, cz)
        else:
            dimension.remove_plugin_chunk_ticket(cx, cz, plugin)
