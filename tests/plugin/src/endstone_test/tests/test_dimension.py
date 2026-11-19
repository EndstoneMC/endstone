import pytest
from endstone import Server
from endstone.level import Dimension, DimensionCreator

from endstone_test.dimensions import CUSTOM_REALM

CUSTOM_DIMENSION_ID = CUSTOM_REALM


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


def test_startup_dimension_is_loaded(server: Server) -> None:
    """Test that a dimension created in on_load is loaded and reports the requested id."""
    dimension = server.level.get_dimension(CUSTOM_DIMENSION_ID)
    assert dimension is not None
    assert dimension.id == CUSTOM_DIMENSION_ID
    assert dimension.is_valid is True
    assert dimension.level is server.level


def test_startup_dimension_is_listed_once(server: Server) -> None:
    """Test that a startup dimension is retrievable and listed exactly once."""
    level = server.level
    listed = [dim for dim in level.dimensions if dim.id == CUSTOM_DIMENSION_ID]
    assert len(listed) == 1


def test_create_dimension_for_a_registered_id_returns_it(server: Server) -> None:
    """Test that creating a registered dimension again returns it without adding another."""
    level = server.level
    before = len(level.dimensions)
    existing = level.get_dimension(CUSTOM_DIMENSION_ID)
    assert server.create_dimension(DimensionCreator(CUSTOM_DIMENSION_ID)) is existing
    assert len(level.dimensions) == before


def test_create_dimension_accepts_an_id(server: Server) -> None:
    """Test that an id is accepted in place of a DimensionCreator."""
    existing = server.level.get_dimension(CUSTOM_DIMENSION_ID)
    assert server.create_dimension(CUSTOM_DIMENSION_ID) is existing


@pytest.mark.parametrize(
    "dimension_id",
    [
        "minecraft:void_realm",
        "MINECRAFT:void_realm",
        Dimension.OVERWORLD,
        "endstone_test:Bad Realm",
        "endstone_test:bad\trealm",
        "endstone_test:",
        ":realm",
        "endstone_test:a:b",
    ],
)
def test_create_dimension_rejects_an_invalid_id(
    server: Server, dimension_id: str
) -> None:
    """Test that an id with a bad namespace or a forbidden character is refused."""
    with pytest.raises(ValueError):
        server.create_dimension(DimensionCreator(dimension_id))


def test_create_dimension_accepts_an_uppercase_id(server: Server) -> None:
    """Test that an uppercase id passes validation and creates a dimension."""
    dimension = server.create_dimension(DimensionCreator("Endstone_Test:Upper_Realm"))
    assert dimension is not None
    assert str(dimension.id).lower() == "endstone_test:upper_realm"


def test_startup_dimension_has_a_translation_key(server: Server) -> None:
    """Test that a custom dimension exposes a translation key."""
    dimension = server.level.get_dimension(CUSTOM_DIMENSION_ID)
    assert isinstance(dimension.translation_key, str)
    assert len(dimension.translation_key) > 0
