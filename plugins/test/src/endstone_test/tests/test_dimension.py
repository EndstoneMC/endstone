from endstone import Server
from endstone.level import Dimension


def test_dimension_constants() -> None:
    """Test that Dimension class exposes the correct identifier constants."""
    assert Dimension.OVERWORLD == "minecraft:overworld"
    assert Dimension.NETHER == "minecraft:nether"
    assert Dimension.THE_END == "minecraft:the_end"


def test_get_dimension_by_id(server: Server) -> None:
    """Test looking up dimensions by their DimensionId."""
    level = server.level
    overworld = level.get_dimension(Dimension.OVERWORLD)
    assert overworld is not None

    nether = level.get_dimension(Dimension.NETHER)
    assert nether is not None

    the_end = level.get_dimension(Dimension.THE_END)
    assert the_end is not None


def test_get_dimension_invalid_id(server: Server) -> None:
    """Test that looking up a non-existent dimension returns None."""
    dim = server.level.get_dimension("minecraft:nonexistent")
    assert dim is None


def test_dimension_id_property(server: Server) -> None:
    """Test that each dimension reports the correct id."""
    level = server.level
    overworld = level.get_dimension(Dimension.OVERWORLD)
    assert overworld.id == "minecraft:overworld"

    nether = level.get_dimension(Dimension.NETHER)
    assert nether.id == "minecraft:nether"

    the_end = level.get_dimension(Dimension.THE_END)
    assert the_end.id == "minecraft:the_end"


def test_dimension_translation_key(server: Server) -> None:
    """Test that each dimension has a translation key."""
    for dim_id in [Dimension.OVERWORLD, Dimension.NETHER, Dimension.THE_END]:
        dim = server.level.get_dimension(dim_id)
        assert dim is not None
        assert isinstance(dim.translation_key, str)
        assert len(dim.translation_key) > 0


def test_dimension_level_reference(server: Server) -> None:
    """Test that dimension.level refers back to the correct level."""
    level = server.level
    for dim_id in [Dimension.OVERWORLD, Dimension.NETHER, Dimension.THE_END]:
        dim = level.get_dimension(dim_id)
        assert dim is not None
        assert dim.level is level


def test_dimensions_list(server: Server) -> None:
    """Test that level.dimensions returns all vanilla dimensions."""
    dims = server.level.dimensions
    assert len(dims) >= 3
    dim_ids = {dim.id for dim in dims}
    assert "minecraft:overworld" in dim_ids
    assert "minecraft:nether" in dim_ids
    assert "minecraft:the_end" in dim_ids
