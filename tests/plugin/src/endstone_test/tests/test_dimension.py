from endstone import Server
from endstone.level import Dimension, DimensionCreator

CUSTOM_DIMENSION_ID = "endstone_test:custom_realm"


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


def test_create_dimension(server: Server) -> None:
    """Test that a custom dimension is created and reports the requested id."""
    dimension = server.level.create_dimension(DimensionCreator(CUSTOM_DIMENSION_ID))
    assert dimension is not None
    assert dimension.id == CUSTOM_DIMENSION_ID
    assert dimension.is_valid is True
    assert dimension.level is server.level


def test_create_dimension_registers_it_on_the_level(server: Server) -> None:
    """Test that a created dimension is retrievable and listed exactly once."""
    level = server.level
    level.create_dimension(DimensionCreator(CUSTOM_DIMENSION_ID))

    assert level.get_dimension(CUSTOM_DIMENSION_ID) is not None
    listed = [dim for dim in level.dimensions if dim.id == CUSTOM_DIMENSION_ID]
    assert len(listed) == 1


def test_create_dimension_is_idempotent(server: Server) -> None:
    """Test that creating the same dimension twice returns it instead of adding another."""
    level = server.level
    first = level.create_dimension(DimensionCreator(CUSTOM_DIMENSION_ID))
    before = len(level.dimensions)

    second = level.create_dimension(DimensionCreator(CUSTOM_DIMENSION_ID))
    assert second is not None
    assert second.id == first.id
    assert len(level.dimensions) == before


def test_create_dimension_accepts_an_id(server: Server) -> None:
    """Test that an id is accepted in place of a DimensionCreator."""
    dimension = server.level.create_dimension(CUSTOM_DIMENSION_ID)
    assert dimension is not None
    assert dimension.id == CUSTOM_DIMENSION_ID


def test_create_dimension_returns_an_existing_vanilla_dimension(server: Server) -> None:
    """Test that an existing dimension id yields that dimension rather than a new one."""
    level = server.level
    before = len(level.dimensions)

    overworld = level.create_dimension(DimensionCreator(Dimension.OVERWORLD))
    assert overworld is not None
    assert overworld.id == Dimension.OVERWORLD
    assert len(level.dimensions) == before


def test_created_dimension_has_a_translation_key(server: Server) -> None:
    """Test that a custom dimension exposes a translation key."""
    dimension = server.level.create_dimension(DimensionCreator(CUSTOM_DIMENSION_ID))
    assert isinstance(dimension.translation_key, str)
    assert len(dimension.translation_key) > 0
