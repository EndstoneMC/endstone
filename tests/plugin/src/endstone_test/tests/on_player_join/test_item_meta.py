import pytest
from endstone import Player
from endstone.inventory import ItemStack


@pytest.fixture(params=["direct", "inventory"])
def item(request, player: Player):
    item_stack = ItemStack("minecraft:diamond_sword")
    if request.param == "direct":
        return item_stack
    else:
        player.inventory.set_item(0, item_stack)
        return player.inventory.get_item(0)


@pytest.mark.parametrize("name", ["§aEpic Sword", "§bTest Blade"])
def test_display_name(item: ItemStack, name: str):
    meta = item.item_meta
    assert meta is not None
    meta.display_name = name
    item.set_item_meta(meta)

    result = item.item_meta.display_name
    assert result == name


@pytest.mark.parametrize("lore", [["One", "Two"], ["Alpha", "Beta", "Gamma"]])
def test_lore(item: ItemStack, lore: list[str]):
    meta = item.item_meta
    meta.lore = lore
    item.set_item_meta(meta)

    result = item.item_meta.lore
    assert result == lore


@pytest.mark.parametrize(
    "enchantment",
    [("sharpness", 3), ("knockback", 2)],
)
def test_enchantments(item: ItemStack, enchantment: tuple[str, int]):
    enchantment_id, level = enchantment
    meta = item.item_meta
    assert meta.add_enchant(enchantment_id, level, True)
    item.set_item_meta(meta)

    result_meta = item.item_meta
    assert result_meta.has_enchant(enchantment_id)
    assert result_meta.get_enchant_level(enchantment_id) == level

    # removal
    result_meta.remove_enchant(enchantment_id)
    item.set_item_meta(result_meta)
    assert not item.item_meta.has_enchant(enchantment_id)


@pytest.mark.parametrize("damage", [0, 5, 100])
def test_damage(item: ItemStack, damage: int):
    meta = item.item_meta
    meta.damage = damage
    item.set_item_meta(meta)

    result_meta = item.item_meta
    assert result_meta.damage == damage
