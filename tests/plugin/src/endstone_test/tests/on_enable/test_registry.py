import typing

import pytest
from endstone import Server
from endstone.enchantments import Enchantment
from endstone.inventory import ItemType

T = typing.TypeVar("T")


@pytest.mark.parametrize(
    "registry,key,expected",
    [
        (
            Enchantment,
            "protection",
            {"start_level": 1, "max_level": 4, "id": Enchantment.PROTECTION},
        ),
        (
            Enchantment,
            Enchantment.SHARPNESS,
            {"start_level": 1, "max_level": 5, "id": Enchantment.SHARPNESS},
        ),
        (
            ItemType,
            "minecraft:diamond",
            {"max_stack_size": 64, "max_durability": 0},
        ),
        (
            ItemType,
            "minecraft:diamond_sword",
            {"max_stack_size": 1, "max_durability": 1561},
        ),
    ],
)
def test_get_valid(server: Server, registry: typing.Type[T], key: str, expected: dict):
    reg = server.get_registry(registry)
    assert reg.get(key) is not None
    assert reg[key] is not None
    assert key in reg
    assert reg.get_or_throw(key) is not None
    for attr, expected_value in expected.items():
        assert getattr(reg.get(key), attr) == expected_value


@pytest.mark.parametrize(
    "registry,key",
    [
        (Enchantment, "not_an_enchant"),
        (Enchantment, "bogus_enchantment"),
        (ItemType, "not_an_item"),
        (ItemType, "bogus_item"),
    ],
)
def test_get_invalid(server: Server, registry: typing.Type[T], key: str):
    reg = server.get_registry(registry)
    assert reg.get(key) is None
    assert key not in reg
    with pytest.raises(KeyError):
        reg.get_or_throw(key)
    with pytest.raises(KeyError):
        _ = reg[key]
