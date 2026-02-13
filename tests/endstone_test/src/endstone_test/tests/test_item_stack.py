import pytest
from endstone import Server
from endstone.enchantments import Enchantment
from endstone.inventory import ItemMeta, ItemStack, MapMeta


@pytest.mark.parametrize(
    "type_id,data,max_durability,max_stack_size,name",
    [
        ("minecraft:diamond_sword", 0, 1561, 1, "钻石剑"),
        ("apple", 0, 0, 64, "苹果"),
        ("minecraft:apple", 0, 0, 64, "苹果"),
        ("minecraft:potion", 0, 0, 1, "水瓶"),
        ("minecraft:potion", 7, 0, 1, "隐身药水"),
    ],
)
def test_create_item(
    server: Server,
    type_id: str,
    data: int,
    max_durability: int,
    max_stack_size: int,
    name: str,
):
    item = ItemStack(type_id, data=data)
    assert item.type == type_id
    assert item.max_stack_size == max_stack_size
    assert server.language.translate(item.translation_key, locale="zh_CN") == name


def test_create_item_bad_type():
    with pytest.raises(RuntimeError) as err_info:
        _ = ItemStack("non_existent_item")

    assert "Unknown item type: minecraft:non_existent_item" == str(err_info.value)


@pytest.mark.parametrize("amount", [-1, 0, 256])
def test_create_item_bad_amount(amount):
    with pytest.raises(RuntimeError) as err_info:
        _ = ItemStack("minecraft:apple", amount)

    assert f"Item stack amount must be between 1 to 255, got {amount}." == str(
        err_info.value
    )


@pytest.mark.parametrize(
    "type,meta_cls",
    [
        ("minecraft:air", None),
        ("minecraft:apple", ItemMeta),
        ("minecraft:diamond_sword", ItemMeta),
        ("minecraft:filled_map", MapMeta),
    ],
)
def test_item_meta(type, meta_cls):
    item = ItemStack(type)
    meta = item.item_meta
    if meta is None:
        assert meta_cls is None
    else:
        assert isinstance(meta, meta_cls)


def test_set_lore():
    item = ItemStack("minecraft:diamond_sword")
    lore = ["A powerful blade", "of destiny"]

    meta = item.item_meta
    meta.lore = lore

    assert item.set_item_meta(meta)

    meta = item.item_meta
    assert meta.has_lore
    assert meta.lore == lore


def test_remove_lore():
    item = ItemStack("minecraft:diamond_sword")
    lore = ["A powerful blade", "of destiny"]

    meta = item.item_meta
    meta.lore = lore
    assert item.set_item_meta(meta)

    meta = item.item_meta
    meta.lore = []
    assert item.set_item_meta(meta)

    meta = item.item_meta
    assert not meta.has_lore
    assert not meta.lore


def test_clear_item_meta():
    item = ItemStack("minecraft:diamond_sword")
    lore = ["A powerful blade", "of destiny"]

    meta = item.item_meta
    meta.lore = lore
    assert item.set_item_meta(meta)

    assert item.set_item_meta(None)
    meta = item.item_meta
    assert not meta.has_lore


def test_enchantment_hash(server: Server):
    reg = server.get_registry(Enchantment)
    sharpness = reg.get("sharpness")
    assert hash(sharpness) == hash(sharpness)

    # two lookups of the same enchantment should have the same hash
    sharpness2 = reg.get(Enchantment.SHARPNESS)
    assert hash(sharpness) == hash(sharpness2)

    # different enchantments should (almost certainly) have different hashes
    protection = reg.get(Enchantment.PROTECTION)
    assert hash(sharpness) != hash(protection)


def test_enchantment_in_dict(server: Server):
    reg = server.get_registry(Enchantment)
    sharpness = reg.get(Enchantment.SHARPNESS)
    protection = reg.get(Enchantment.PROTECTION)

    # Enchantment objects can be used as dict keys
    d = {sharpness: 5, protection: 4}
    assert d[sharpness] == 5
    assert d[protection] == 4


def test_enchantment_in_set(server: Server):
    reg = server.get_registry(Enchantment)
    sharpness = reg.get(Enchantment.SHARPNESS)
    protection = reg.get(Enchantment.PROTECTION)

    s = {sharpness, protection, sharpness}
    assert len(s) == 2


def test_enchants_dict(server: Server):
    item = ItemStack("minecraft:diamond_sword")
    meta = item.item_meta

    assert not meta.has_enchants
    assert meta.enchants == {}

    meta.add_enchant(Enchantment.SHARPNESS, 5, True)
    meta.add_enchant(Enchantment.KNOCKBACK, 2, True)
    item.set_item_meta(meta)

    result_meta = item.item_meta
    enchants = result_meta.enchants
    assert len(enchants) == 2

    # verify enchantment keys are accessible
    for enchant, level in enchants.items():
        assert isinstance(level, int)
        assert level > 0
