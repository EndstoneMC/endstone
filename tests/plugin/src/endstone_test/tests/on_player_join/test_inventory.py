from endstone import Player, Server
from endstone.inventory import ItemStack, MapMeta
from endstone.plugin import Plugin
from PIL import Image

from endstone_test.image_renderer import ImageRenderer


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


def test_add_map(player: Player, server: Server, plugin: Plugin):
    item = ItemStack("minecraft:filled_map", 1)
    meta = item.item_meta
    assert isinstance(meta, MapMeta), "Item is not a map"

    view = server.create_map(player.dimension)
    assert view is not None, "Failed to create map"
    assert view.id != -1, "Invalid map ID"

    assert len(view.renderers) == 1, "Map view should have the vanilla renderer"
    for renderer in view.renderers:
        assert view.remove_renderer(renderer) is True, "Failed to remove renderer"

    assert len(view.renderers) == 0, "Map view should have no renderers"
    view.add_renderer(ImageRenderer(Image.open(plugin.data_folder / "lena.png")))
    assert len(view.renderers) == 1, "Map view should have one custom renderer"

    meta.map_view = view
    assert item.set_item_meta(meta)

    player.inventory.set_item(2, item)
    item = player.inventory.get_item(2)

    meta = item.item_meta
    assert item.type == "minecraft:filled_map", "Item is not a map"
    assert isinstance(meta, MapMeta), "Item is not a map"
    assert meta.map_id != -1, "Invalid map ID"
    assert meta.map_id == view.id, "Map ID does not match"
    assert meta.map_view is not None, "Map view is not set"
    assert meta.map_view.id == view.id, "Map ID does not match"
