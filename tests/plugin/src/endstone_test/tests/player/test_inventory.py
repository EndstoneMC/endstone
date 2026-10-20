import pytest
from endstone import Identifier, Player, Server
from endstone.inventory import Inventory, ItemStack, PlayerInventory

# =============================================================================
# Fixtures
# =============================================================================


@pytest.fixture(autouse=True)
def restore_inventory(player: Player):
    """Put the player's inventory and ender chest back the way each test found them."""
    inventory = list(player.inventory.contents)
    ender_chest = list(player.ender_chest.contents)

    yield

    player.inventory.contents = inventory
    player.ender_chest.contents = ender_chest


def test_inventory_sizes(player: Player):
    assert player.inventory.size == 36
    assert player.inventory.max_stack_size == 254
    assert player.ender_chest.size == 27
    assert player.ender_chest.max_stack_size == 254


def test_get_item(player: Player, server: Server):
    player.inventory.clear()
    assert player.inventory.is_empty

    server.dispatch_command(
        server.command_sender, f'give "{player.name}" minecraft:clock'
    )
    assert not player.inventory.is_empty

    item1 = player.inventory.get_item(0)
    item2 = player.inventory.contents[0]
    assert item1.type == "minecraft:clock"
    assert item1.amount == 1
    assert item1.type == item2.type
    assert item1.amount == item2.amount

    player.inventory.clear()
    assert player.inventory.is_empty


def test_add_item(player: Player):
    player.inventory.add_item(ItemStack("minecraft:clock", 7))
    item = player.inventory.get_item(0)
    assert item.type == "minecraft:clock"
    assert item.amount == 7

    player.inventory.add_item(ItemStack("minecraft:cobblestone", 2))
    item = player.inventory.get_item(1)
    assert item.type == "minecraft:cobblestone"
    assert item.amount == 2


def test_set_item(player: Player):
    player.inventory.set_item(35, ItemStack("minecraft:diamond", 7))

    item = player.inventory.get_item(35)
    assert item.type == "minecraft:diamond"
    assert item.amount == 7

    player.inventory.set_item(35, item)
    item = player.inventory.get_item(35)
    assert item.type == "minecraft:diamond"
    assert item.amount == 7

    assert player.inventory.first(item) == 35


def test_set_empty_item(player: Player):
    player.inventory.set_item(35, None)
    assert player.inventory.get_item(35) is None


def test_update_item_meta(player: Player, server: Server):
    player.inventory.clear()
    server.dispatch_command(
        server.command_sender, f'give "{player.name}" minecraft:clock'
    )

    # Set the lore
    item = player.inventory.contents[0]
    meta = item.item_meta
    assert not meta.lore
    lore = ["This is a test lore", "And another line of lore:)"]
    meta.lore = lore
    assert meta.lore == lore
    item.set_item_meta(meta)
    assert item.item_meta.lore == lore

    # Update the inventory
    player.inventory.set_item(0, item)
    item = player.inventory.get_item(0)
    assert item.item_meta.lore == lore

    # Remove the lore
    meta = item.item_meta
    meta.lore = None
    item.set_item_meta(meta)
    assert not item.item_meta.lore


def test_add_item_with_meta(player: Player):
    item = ItemStack("minecraft:diamond_sword", 1)
    meta = item.item_meta
    assert not meta.lore
    lore = ["A powerful blade", "of destiny"]
    meta.display_name = "Excalibur"
    meta.lore = lore
    meta.damage = 500
    assert not meta.add_enchant("sharpness", 66, force=False)
    assert meta.add_enchant("sharpness", 66, force=True)

    assert item.set_item_meta(meta)

    player.inventory.set_item(1, item)
    item = player.inventory.get_item(1)
    assert item.item_meta.display_name == "Excalibur"
    assert item.item_meta.lore == lore
    assert item.item_meta.damage == 500
    assert item.item_meta.has_enchant("sharpness")
    assert item.item_meta.get_enchant_level("sharpness") == 66
    assert not item.item_meta.has_enchant("protection")


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


# =============================================================================
# Section: Identifier overloads new in v0.12
# =============================================================================


@pytest.fixture
def stocked(player: Player) -> Player:
    """Give the player a known set of items to query."""
    player.inventory.clear()
    player.inventory.set_item(0, ItemStack("minecraft:diamond", 5))
    player.inventory.set_item(1, ItemStack("minecraft:emerald", 2))
    return player


@pytest.mark.parametrize(
    "item_type",
    ["minecraft:diamond", Identifier("minecraft", "diamond"), "diamond"],
    ids=["qualified_str", "identifier", "bare_str"],
)
def test_contains_by_type(stocked: Player, item_type) -> None:
    """Verify contains accepts an id in every form the caster allows."""
    assert stocked.inventory.contains(item_type) is True


def test_contains_by_item_stack(stocked: Player) -> None:
    """Verify the ItemStack overload of contains matches on type and amount."""
    assert stocked.inventory.contains(ItemStack("minecraft:diamond", 5)) is True
    assert stocked.inventory.contains(ItemStack("minecraft:gold_ingot", 1)) is False


def test_contains_at_least(stocked: Player) -> None:
    """Verify contains_at_least counts across the inventory."""
    assert stocked.inventory.contains_at_least("minecraft:diamond", 5) is True
    assert stocked.inventory.contains_at_least("minecraft:diamond", 6) is False


def test_all_by_type(stocked: Player) -> None:
    """Verify all() maps every matching slot to its stack."""
    matches = stocked.inventory.all("minecraft:diamond")
    assert list(matches) == [0]
    assert matches[0].type == "minecraft:diamond"


def test_first_by_type(stocked: Player) -> None:
    """Verify first() reports the slot index, or -1 when absent."""
    assert stocked.inventory.first("minecraft:emerald") == 1
    assert stocked.inventory.first("minecraft:gold_ingot") == -1


def test_remove_by_type(stocked: Player) -> None:
    """Verify remove() strips every stack of the given type."""
    stocked.inventory.remove("minecraft:diamond")
    assert stocked.inventory.contains("minecraft:diamond") is False
    assert stocked.inventory.contains("minecraft:emerald") is True


def test_contains_dunder(stocked: Player) -> None:
    """Verify the in operator accepts both an id and a stack."""
    assert "minecraft:diamond" in stocked.inventory
    assert "minecraft:gold_ingot" not in stocked.inventory
    assert ItemStack("minecraft:diamond", 5) in stocked.inventory


# =============================================================================
# Section: The rest of the Inventory surface
# =============================================================================


def test_first_empty(player: Player) -> None:
    """Verify first_empty finds a free slot, and reports -1 when full."""
    player.inventory.clear()
    assert player.inventory.first_empty == 0

    player.inventory.contents = [
        ItemStack("minecraft:stone", 1)
    ] * player.inventory.size
    assert player.inventory.first_empty == -1


def test_add_item_reports_what_did_not_fit(player: Player) -> None:
    """Verify add_item returns the stacks it could not place."""
    player.inventory.clear()
    assert player.inventory.add_item(ItemStack("minecraft:stone", 1)) == {}

    player.inventory.contents = [
        ItemStack("minecraft:dirt", 64)
    ] * player.inventory.size
    leftover = player.inventory.add_item(ItemStack("minecraft:stone", 1))
    assert leftover != {}


def test_remove_item_reports_what_was_not_removed(stocked: Player) -> None:
    """Verify remove_item returns the part of the request it could not satisfy."""
    assert stocked.inventory.remove_item(ItemStack("minecraft:diamond", 2)) == {}
    assert stocked.inventory.remove_item(ItemStack("minecraft:gold_ingot", 1)) != {}


def test_len_and_indexing(stocked: Player) -> None:
    """Verify the inventory behaves as a sequence."""
    inventory = stocked.inventory
    assert len(inventory) == inventory.size
    assert inventory[0].type == "minecraft:diamond"

    inventory[2] = ItemStack("minecraft:gold_ingot", 1)
    assert inventory.get_item(2).type == "minecraft:gold_ingot"

    inventory[2] = None
    assert inventory[2] is None


def test_clear_one_slot(stocked: Player) -> None:
    """Verify clear(index) empties a single slot and clear() empties everything."""
    stocked.inventory.clear(0)
    assert stocked.inventory.get_item(0) is None
    assert stocked.inventory.get_item(1) is not None

    stocked.inventory.clear()
    assert stocked.inventory.is_empty is True


@pytest.mark.parametrize(
    "slot,item_type",
    [
        ("helmet", "minecraft:diamond_helmet"),
        ("chestplate", "minecraft:diamond_chestplate"),
        ("leggings", "minecraft:diamond_leggings"),
        ("boots", "minecraft:diamond_boots"),
    ],
)
def test_armour_slots(player: Player, slot: str, item_type: str) -> None:
    """Verify each armour slot round-trips."""
    setattr(player.inventory, slot, ItemStack(item_type))
    assert getattr(player.inventory, slot).type == item_type

    setattr(player.inventory, slot, None)
    assert getattr(player.inventory, slot) is None


def test_hand_slots(player: Player) -> None:
    """Verify the main and off hand slots round-trip."""
    player.inventory.item_in_main_hand = ItemStack("minecraft:diamond_sword")
    assert player.inventory.item_in_main_hand.type == "minecraft:diamond_sword"

    player.inventory.item_in_off_hand = ItemStack("minecraft:shield")
    assert player.inventory.item_in_off_hand.type == "minecraft:shield"


def test_held_item_slot(player: Player) -> None:
    """Verify the held hotbar slot is readable and within the hotbar."""
    assert 0 <= player.inventory.held_item_slot <= 8


# =============================================================================
# Section: PlayerInventory versus a plain Inventory
# =============================================================================


def test_player_inventory_type(player: Player) -> None:
    """Verify only the player's own inventory is a PlayerInventory."""
    assert isinstance(player.inventory, PlayerInventory)
    assert isinstance(player.ender_chest, Inventory)
    assert not isinstance(player.ender_chest, PlayerInventory)


@pytest.mark.parametrize(
    "member", ["item_in_main_hand", "item_in_off_hand", "held_item_slot"]
)
def test_ender_chest_lacks_the_player_only_members(player: Player, member: str) -> None:
    """Verify the hand and hotbar members belong to PlayerInventory alone."""
    assert hasattr(player.inventory, member)
    assert not hasattr(player.ender_chest, member)


def test_iterating_an_inventory_visits_every_slot(player: Player):
    player.inventory.clear()
    player.inventory.set_item(3, ItemStack("minecraft:diamond", 2))
    items = list(player.inventory)
    assert len(items) == player.inventory.size
    assert items[3] is not None and items[3].type == "minecraft:diamond"
    assert items[0] is None


def test_indexing_past_the_end_raises_index_error(player: Player):
    with pytest.raises(IndexError):
        player.inventory[player.inventory.size]
    with pytest.raises(IndexError):
        player.inventory[-1] = ItemStack("minecraft:stone")
