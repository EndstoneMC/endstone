from endstone.event import DimensionLoadEvent
from endstone.level import Dimension, DimensionCreator

from endstone_test.harness import Context, scenario

CUSTOM = "endstone_test:harness_realm"


@scenario("level.dimension_load", covers={DimensionLoadEvent: "normal"}, budget=400)
async def dimension_load(ctx: Context) -> None:
    at_startup = {
        e["dimension"]["id"]
        for e in ctx.driver.hub.startup["DimensionLoadEvent"].events
    }
    assert not at_startup & {
        Dimension.OVERWORLD,
        Dimension.NETHER,
        Dimension.THE_END,
    }, at_startup

    load = ctx.capture(DimensionLoadEvent, lambda e: str(e.dimension.id) == CUSTOM)
    created = ctx.level.create_dimension(DimensionCreator(CUSTOM))
    ctx.info["created"] = None if created is None else str(created.id)
    assert created is not None, "create_dimension returned None for a new id"
    assert str(created.id) == CUSTOM
    await ctx.until(
        lambda: load.count, timeout=200, what=f"DimensionLoadEvent for {CUSTOM}"
    )
    assert load.count == 1, load.events
    assert ctx.level.get_dimension(CUSTOM) is not None
