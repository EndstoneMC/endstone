from endstone import Server
from endstone.actor import Mob
from endstone.level import Location


def test_blocks_can_be_set_in_the_plot(server: Server, plot) -> None:
    """The harness plot is loaded and ticking, so blocks set there stick."""
    overworld = server.level.get_dimension("overworld")
    x, y, z = plot.pos(1, 0, 1)
    block = overworld.get_block_at(x, y, z)
    assert str(block.type) == "minecraft:air"
    block.set_type("minecraft:gold_block")
    assert str(overworld.get_block_at(x, y, z).type) == "minecraft:gold_block"
    assert str(overworld.get_block_at(x, y - 1, z).type) == "minecraft:stone"


def test_mobs_can_be_spawned_in_the_plot(server: Server, plot) -> None:
    """spawn_actor returns a live mob inside the plot."""
    overworld = server.level.get_dimension("overworld")
    x, y, z = plot.pos(*plot.middle)
    pig = overworld.spawn_actor(
        Location(overworld, x + 0.5, y, z + 0.5), "minecraft:pig"
    )
    try:
        assert isinstance(pig, Mob)
        assert pig.is_valid
        assert pig.get_attribute("minecraft:movement") is not None
    finally:
        if pig is not None:
            pig.remove()
