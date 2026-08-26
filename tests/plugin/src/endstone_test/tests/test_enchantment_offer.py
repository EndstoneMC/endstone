import pytest
from endstone import Server
from endstone.enchantments import Enchantment, EnchantmentOffer


def test_enchantment_offer(server: Server) -> None:
    registry = server.get_registry(Enchantment)
    sharpness = registry.get(Enchantment.SHARPNESS)
    unbreaking = registry.get(Enchantment.UNBREAKING)
    assert sharpness is not None
    assert unbreaking is not None

    offer = EnchantmentOffer({sharpness: 3}, 12)
    assert offer.enchants == {sharpness: 3}
    assert offer.cost == 12

    enchants = offer.enchants
    enchants[unbreaking] = 2
    assert offer.enchants == {sharpness: 3}

    offer.enchants = enchants
    offer.cost = 18
    assert offer.enchants == {sharpness: 3, unbreaking: 2}
    assert offer.cost == 18


@pytest.mark.parametrize(
    ("enchants", "cost"),
    [
        ({}, 1),
        ({"minecraft:sharpness": 0}, 1),
        ({"minecraft:sharpness": 1}, 0),
    ],
)
def test_enchantment_offer_rejects_invalid_values(
    server: Server, enchants: dict, cost: int
) -> None:
    registry = server.get_registry(Enchantment)
    resolved = {
        registry.get(enchantment): level for enchantment, level in enchants.items()
    }

    with pytest.raises(ValueError):
        EnchantmentOffer(resolved, cost)
