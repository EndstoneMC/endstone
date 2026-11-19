import json
import sys

from endstone.event import DimensionLoadEvent
from endstone.level import Dimension, DimensionCreator, Location

from endstone_test.dimensions import (
    CUSTOM_REALM,
    ENABLE_DIMENSION,
    HARNESS_REALM,
    PERSIST_REALM,
)
from endstone_test.harness import Context, scenario
from endstone_test.plugin import HARNESS_STARTS
from endstone_test.scenarios.commands import available
from endstone_test.scenarios.player_apis import join

RUNTIME_REALM = "endstone_test:runtime_realm"
PERSIST_MARKER = "persist_realm.json"


@scenario("level.dimension_load", covers={DimensionLoadEvent: "normal"}, budget=100)
async def dimension_load(ctx: Context) -> None:
    at_startup = [
        str(e["dimension"]["id"])
        for e in ctx.driver.hub.startup["DimensionLoadEvent"].events
    ]
    assert not set(at_startup) & {
        Dimension.OVERWORLD,
        Dimension.NETHER,
        Dimension.THE_END,
    }, at_startup
    assert HARNESS_REALM not in at_startup, at_startup
    assert ctx.level.get_dimension(HARNESS_REALM) is not None
    if getattr(sys, HARNESS_STARTS) == 1:
        assert all(d is None for d in ctx.plugin.load_dimensions.values()), (
            ctx.plugin.load_dimensions
        )


@scenario("level.dimension_execute_in", budget=100)
async def dimension_execute_in(ctx: Context) -> None:
    dimension = ctx.level.get_dimension(CUSTOM_REALM)
    assert dimension is not None, f"{CUSTOM_REALM} was not loaded at startup"
    result = ctx.command(f"execute in {CUSTOM_REALM} run say hello")
    assert result.success, result.output


@scenario(
    "level.create_dimension_at_runtime",
    covers={DimensionLoadEvent: "normal"},
    budget=2000,
)
async def create_dimension_at_runtime(ctx: Context) -> None:
    load = ctx.capture(
        DimensionLoadEvent, lambda e: str(e.dimension.id) == RUNTIME_REALM
    )
    created = ctx.server.create_dimension(DimensionCreator(RUNTIME_REALM))
    assert created is not None, "create_dimension returned None after startup"
    assert str(created.id) == RUNTIME_REALM
    assert ctx.level.get_dimension(RUNTIME_REALM) is created
    assert ctx.server.create_dimension(DimensionCreator(RUNTIME_REALM)) is created
    assert ctx.server.create_dimension(RUNTIME_REALM) is created
    result = ctx.command(f"execute in {RUNTIME_REALM} run say hello")
    assert result.success, result.output
    await ctx.until(
        lambda: load.count, timeout=200, what=f"DimensionLoadEvent for {RUNTIME_REALM}"
    )
    assert load.count == 1, load.events
    ctx.release(load)

    for invalid in (
        "minecraft:runtime_realm",
        "MINECRAFT:runtime_realm",
        "endstone_test:Runtime Realm",
        "endstone_test:",
    ):
        refused = None
        try:
            ctx.server.create_dimension(DimensionCreator(invalid))
        except ValueError as e:
            refused = e
        assert refused is not None, f"{invalid!r} was accepted"

    def floor() -> bool:
        created.load_chunk(0, 0)
        if not created.is_chunk_loaded(0, 0):
            return False
        for x in range(3):
            for z in range(3):
                created.get_block_at(x, 63, z).set_type("minecraft:glass")
        return str(created.get_block_at(1, 63, 1).type.id) == "minecraft:glass"

    await ctx.until(floor, timeout=400, what=f"a floor in {RUNTIME_REALM}")
    bot, player = await join(ctx, "LvRuntimeDim", op=True)
    player.teleport(Location(created, 1.5, 64, 1.5))
    await ctx.until(
        lambda: str(player.location.dimension.id) == RUNTIME_REALM,
        timeout=400,
        what=f"the player in {RUNTIME_REALM}",
    )
    for _ in range(600):
        if await bot.call("local_player.is_changing_dimension") is False:
            break
        await ctx.ticks(1)
    else:
        raise AssertionError("the client never finished the dimension change")

    packet = await available(ctx, bot, player)
    dimensions = next(
        (
            [packet["enum_values"][v] for v in e["values"]]
            for e in packet["enums"]
            if e["name"] == "Dimension"
        ),
        [],
    )
    ctx.info["dimension_enum"] = dimensions
    assert RUNTIME_REALM in dimensions, dimensions

    result = ctx.command(f"execute in {RUNTIME_REALM} run setblock 0 64 0 stone")
    assert result.success, result.output
    assert str(created.get_block_at(0, 64, 0).type.id) == "minecraft:stone"

    await bot.call(
        "local_player.run_command",
        f"/execute in {RUNTIME_REALM} run setblock 2 64 2 stone",
    )
    await ctx.until(
        lambda: str(created.get_block_at(2, 64, 2).type.id) == "minecraft:stone",
        timeout=100,
        what=f"the block the player placed through execute in {RUNTIME_REALM}",
    )


@scenario("level.create_dimension_reload_noop", budget=1500)
async def create_dimension_reload_noop(ctx: Context) -> None:
    stash = ctx.stash
    if not stash:
        stash["before"] = sorted(str(d.id) for d in ctx.level.dimensions)
        await ctx.console("reload")
        await ctx.ticks(1200)
        raise AssertionError("the plugin was not reloaded")

    assert sorted(str(d.id) for d in ctx.level.dimensions) == stash["before"]
    for dimension_id, dimension in ctx.plugin.load_dimensions.items():
        assert dimension is ctx.level.get_dimension(dimension_id), dimension_id
        assert dimension is not None, dimension_id
    assert ctx.plugin.enable_dimension is ctx.level.get_dimension(ENABLE_DIMENSION)
    assert ctx.plugin.enable_dimension is not None
    assert ctx.server.create_dimension(
        DimensionCreator(CUSTOM_REALM)
    ) is ctx.level.get_dimension(CUSTOM_REALM)


@scenario("level.dimension_persists_restart", budget=200)
async def dimension_persists_restart(ctx: Context) -> None:
    marker = ctx.plugin.data_folder / PERSIST_MARKER
    dimension = ctx.level.get_dimension(PERSIST_REALM)
    assert dimension is not None, f"{PERSIST_REALM} was not loaded at startup"
    assert dimension.load_chunk(0, 0), "the chunk of the custom dimension did not load"
    await ctx.ticks(5)

    if marker.exists():
        saved = json.loads(marker.read_text())
        ctx.info["phase"] = "after restart"
        assert dimension.get_block_at(0, 64, 0).type.id == saved["block"], saved
        return

    ctx.run(f"execute in {PERSIST_REALM} run setblock 0 64 0 stone")
    assert dimension.get_block_at(0, 64, 0).type.id == "minecraft:stone"
    marker.parent.mkdir(parents=True, exist_ok=True)
    marker.write_text(json.dumps({"block": "minecraft:stone"}))
    ctx.info["phase"] = "before restart"
