import pytest
from endstone import Identifier, Server
from endstone.block import Biome


def test_get_a_known_biome(server: Server) -> None:
    """Verify a biome can be looked up by its namespaced name."""
    plains = server.get_registry(Biome).get_or_throw("minecraft:plains")
    assert plains is not None
    assert plains.id == "minecraft:plains"


def test_get_returns_none_for_an_unknown_biome(server: Server) -> None:
    """Verify an unknown biome name yields None rather than raising."""
    assert server.get_registry(Biome).get("minecraft:not_a_biome") is None


def test_id_is_an_identifier(server: Server) -> None:
    """Verify Biome.id is an Identifier."""
    plains = server.get_registry(Biome).get_or_throw("minecraft:plains")
    assert isinstance(plains.id, Identifier)
    assert plains.id.namespace == "minecraft"
    assert plains.id.key == "plains"


def test_translation_key_raises(server: Server) -> None:
    """Verify asking a biome for a translation key raises, as Bedrock has none for biomes."""
    plains = server.get_registry(Biome).get_or_throw("minecraft:plains")
    with pytest.raises(RuntimeError):
        _ = plains.translation_key


def test_str_and_repr(server: Server) -> None:
    """Verify str() is the plain id and repr() wraps it in the class name."""
    plains = server.get_registry(Biome).get_or_throw("minecraft:plains")
    assert str(plains) == "minecraft:plains"
    assert repr(plains) == "Biome(minecraft:plains)"


def test_equality_and_hash(server: Server) -> None:
    """Verify a biome compares and hashes like its string form."""
    registry = server.get_registry(Biome)
    plains = registry.get_or_throw("minecraft:plains")
    assert plains == "minecraft:plains"
    assert plains != "minecraft:desert"
    assert hash(plains) == hash("minecraft:plains")
    assert registry.get("minecraft:plains") is plains


def test_registry_is_iterable(server: Server) -> None:
    """Verify the biome registry supports len, iteration and membership."""
    registry = server.get_registry(Biome)
    assert len(registry) > 0
    assert "minecraft:plains" in registry
    assert "minecraft:not_a_biome" not in registry
    for biome in registry:
        assert ":" in str(biome.id)
