import pytest

from endstone import Identifier
from endstone.level import Dimension, DimensionCreator


def test_construction_from_a_namespaced_string() -> None:
    """Verify a namespaced id is kept as given."""
    creator = DimensionCreator("myplugin:void_realm")
    assert creator.id == "myplugin:void_realm"


def test_construction_from_a_bare_key() -> None:
    """Verify a key with no namespace is placed in the minecraft namespace."""
    creator = DimensionCreator("void_realm")
    assert creator.id == "minecraft:void_realm"


def test_construction_from_an_identifier() -> None:
    """Verify an Identifier is accepted as the id."""
    creator = DimensionCreator(Identifier("myplugin", "void_realm"))
    assert creator.id == "myplugin:void_realm"


def test_construction_from_a_vanilla_dimension_id() -> None:
    """Verify the vanilla dimension constants are accepted as the id."""
    creator = DimensionCreator(Dimension.OVERWORLD)
    assert creator.id == "minecraft:overworld"


def test_id_is_an_identifier() -> None:
    """Verify the id is handed back as an Identifier, not a plain string."""
    creator = DimensionCreator("myplugin:void_realm")
    assert isinstance(creator.id, Identifier)
    assert creator.id.namespace == "myplugin"
    assert creator.id.key == "void_realm"


def test_id_is_read_only() -> None:
    """Verify the id cannot be reassigned after construction."""
    creator = DimensionCreator("myplugin:void_realm")
    with pytest.raises(AttributeError):
        creator.id = "myplugin:other_realm"
