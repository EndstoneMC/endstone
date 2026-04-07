import pytest
from endstone import Server
from endstone.actor import ActorType
from endstone.enchantments import Enchantment
from endstone.inventory import ItemType

# All registry types to test generically
REGISTRY_TYPES = [ActorType, Enchantment, ItemType]


def _get_enum_constants(cls):
    """Collect all static Identifier constants (strings containing ':') from a registry type class."""
    return {
        name: getattr(cls, name)
        for name in dir(cls)
        if not name.startswith("_") and isinstance(getattr(cls, name), str)
    }


# =============================================================================
# Per-registry generic tests
# =============================================================================


@pytest.mark.parametrize("registry_type", REGISTRY_TYPES, ids=lambda t: t.__name__)
def test_get(server: Server, registry_type):
    """registry.get() should return the same instance on repeated calls."""
    registry = server.get_registry(registry_type)
    for item in registry:
        assert registry.get(item.id) is item


@pytest.mark.parametrize("registry_type", REGISTRY_TYPES, ids=lambda t: t.__name__)
def test_get_invalid(server: Server, registry_type):
    """Invalid keys should return None for get(), and raise KeyError for get_or_throw() and []."""
    registry = server.get_registry(registry_type)
    assert registry.get("not_a_valid_key") is None
    assert "not_a_valid_key" not in registry
    with pytest.raises(KeyError):
        registry.get_or_throw("not_a_valid_key")
    with pytest.raises(KeyError):
        _ = registry["not_a_valid_key"]


@pytest.mark.parametrize("registry_type", REGISTRY_TYPES, ids=lambda t: t.__name__)
def test_iteration(server: Server, registry_type):
    """Iterating a registry should yield entries with valid id and translation_key."""
    registry = server.get_registry(registry_type)
    assert len(registry) > 0, f"{registry_type.__name__} registry is empty"
    for item in registry:
        assert item.id is not None
        assert ":" in str(item.id)
        assert isinstance(item.translation_key, str)
        assert len(item.translation_key) > 0


# =============================================================================
# Registry constants tests
# =============================================================================


@pytest.mark.parametrize("registry_type", REGISTRY_TYPES, ids=lambda t: t.__name__)
def test_excess_constants(server: Server, registry_type):
    """Every static Identifier constant on the class should resolve in the registry."""
    registry = server.get_registry(registry_type)
    constants = _get_enum_constants(registry_type)
    excess = [
        f"{name} ({value})"
        for name, value in constants.items()
        if registry.get(value) is None
    ]
    assert not excess, (
        f"{len(excess)} excess constant(s) in {registry_type.__name__} "
        f"that do not exist in the registry: {excess}"
    )


@pytest.mark.parametrize("registry_type", [ActorType], ids=lambda t: t.__name__)
def test_missing_constants(server: Server, registry_type):
    """Every registry entry should have a corresponding static constant on the class."""
    registry = server.get_registry(registry_type)
    constants = _get_enum_constants(registry_type)
    exported_ids = set(constants.values())
    missing = [str(item.id) for item in registry if str(item.id) not in exported_ids]
    assert not missing, (
        f"{len(missing)} registry id(s) in {registry_type.__name__} "
        f"not exported as constants: {missing}"
    )


# =============================================================================
# Type-specific attribute tests
# =============================================================================


@pytest.mark.parametrize(
    "key,expected",
    [
        ("protection", {"start_level": 1, "max_level": 4}),
        (Enchantment.SHARPNESS, {"start_level": 1, "max_level": 5}),
    ],
)
def test_enchantment_attributes(server: Server, key, expected: dict):
    registry = server.get_registry(Enchantment)
    entry = registry.get(key)
    assert entry is not None
    for attr, expected_value in expected.items():
        assert getattr(entry, attr) == expected_value


@pytest.mark.parametrize(
    "key,expected",
    [
        ("minecraft:diamond", {"max_stack_size": 64, "max_durability": 0}),
        ("minecraft:diamond_sword", {"max_stack_size": 1, "max_durability": 1561}),
    ],
)
def test_item_type_attributes(server: Server, key, expected: dict):
    registry = server.get_registry(ItemType)
    entry = registry.get(key)
    assert entry is not None
    for attr, expected_value in expected.items():
        assert getattr(entry, attr) == expected_value
