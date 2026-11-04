import typing

if typing.TYPE_CHECKING:
    from endstone_test.harness.engine import Context

AREA = "endstone_test_plots"
FIELD_X = 4096
FIELD_Z = 4096
COLUMNS = 5
ROWS = 5
PITCH = 32
SIZE = 16
HEIGHT = 24
FLOOR_Y = -61

GAMERULES = (
    ("domobspawning", "false"),
    ("doweathercycle", "false"),
    ("dodaylightcycle", "false"),
    ("dofiretick", "false"),
    ("randomtickspeed", "1"),
)


class Plot:
    """A SIZE x HEIGHT x SIZE box of air on a stone floor. Coordinates are relative to its lowest air corner."""

    def __init__(self, index: int):
        self.index = index
        self.x = FIELD_X + (index % COLUMNS) * PITCH
        self.z = FIELD_Z + (index // COLUMNS) * PITCH
        self.y = FLOOR_Y + 1

    def pos(self, dx: int = 0, dy: int = 0, dz: int = 0) -> tuple[int, int, int]:
        return self.x + dx, self.y + dy, self.z + dz

    def at(self, dx: int = 0, dy: int = 0, dz: int = 0) -> str:
        return "{} {} {}".format(*self.pos(dx, dy, dz))

    @property
    def middle(self) -> tuple[int, int, int]:
        return SIZE // 2, 0, SIZE // 2

    @property
    def volume(self) -> str:
        return f"x={self.x},y={self.y},z={self.z},dx={SIZE - 1},dy={HEIGHT - 1},dz={SIZE - 1}"

    def __repr__(self) -> str:
        return f"Plot({self.index} at {self.x} {self.y} {self.z})"


class PlotField:
    """One ticking area holding COLUMNS x ROWS plots, handed out in turn and cleared before each use."""

    def __init__(self):
        self.handed_out = 0

    def next(self) -> Plot:
        plot = Plot(self.handed_out % (COLUMNS * ROWS))
        self.handed_out += 1
        return plot

    async def setup(self, ctx: "Context") -> None:
        for rule, value in GAMERULES:
            ctx.run(f"gamerule {rule} {value}")
        x1, z1 = FIELD_X + COLUMNS * PITCH - 1, FIELD_Z + ROWS * PITCH - 1
        added = ctx.command(
            f"tickingarea add {FIELD_X} 0 {FIELD_Z} {x1} 0 {z1} {AREA} true"
        )
        if not added.success and "already exists" not in added.output:
            raise RuntimeError(f"could not add the {AREA} ticking area: {added.output}")
        chunks = [
            (cx, cz)
            for cx in range(FIELD_X >> 4, (x1 >> 4) + 1)
            for cz in range(FIELD_Z >> 4, (z1 >> 4) + 1)
        ]
        await ctx.until(
            lambda: all(ctx.overworld.is_chunk_loaded(cx, cz) for cx, cz in chunks),
            timeout=2400,
            what=f"the {len(chunks)} chunks of the plot field to load",
        )

    def release(self, ctx: "Context", plot: Plot) -> None:
        x0, z0 = plot.x, plot.z
        x1, z1 = x0 + SIZE - 1, z0 + SIZE - 1
        for actor in ctx.overworld.actors:
            where = actor.location
            if (
                str(actor.type) != "minecraft:player"
                and x0 - 1 <= where.x < x1 + 2
                and FLOOR_Y - 2 <= where.y < plot.y + HEIGHT + 1
                and z0 - 1 <= where.z < z1 + 2
            ):
                actor.remove()

    async def prepare(self, ctx: "Context", plot: Plot) -> None:
        """Clear the plot. Leftover actors are removed, not killed: a killed mob drops its loot a second later."""
        x0, z0 = plot.x, plot.z
        x1, z1 = x0 + SIZE - 1, z0 + SIZE - 1
        self.release(ctx, plot)
        fill(ctx, f"{x0} {FLOOR_Y - 2} {z0} {x1} {FLOOR_Y} {z1} stone")
        fill(ctx, f"{x0} {plot.y} {z0} {x1} {plot.y + HEIGHT - 1} {z1} air")
        ctx.command(f"kill @e[type=item,{plot.volume}]")

    def teardown(self, ctx: "Context") -> None:
        ctx.command(f"tickingarea remove {AREA}")


def fill(ctx: "Context", arguments: str) -> None:
    """Run /fill, accepting a region that already holds the block."""
    result = ctx.command(f"fill {arguments}")
    if not result.success and result.output not in (
        "0 blocks filled",
        "No blocks filled",
    ):
        raise RuntimeError(f"/fill {arguments} failed: {result.output}")
