from endstone import Player


def test_get_block_at_xyz(player: Player) -> None:
    location = player.location
    block = player.dimension.get_block_at(location.block_x, -64, location.block_z)
    assert block.type == "minecraft:bedrock"
    block = player.dimension.get_block_at(location.block_x, 320, location.block_z)
    assert block.type == "minecraft:air"


def test_get_block_at_location(player: Player) -> None:
    location = player.location
    block = player.dimension.get_block_at(location)
    assert block.data.type == block.type


def test_get_highest_block_y_at(player: Player) -> None:
    location = player.location
    y = player.dimension.get_highest_block_y_at(location.block_x, location.block_z)
    assert y != 0


def test_get_highest_block_at_xz(player: Player) -> None:
    location = player.location
    block = player.dimension.get_highest_block_at(location.block_x, location.block_z)
    assert block.type != "minecraft:air"


def test_get_highest_block_at_location(player: Player) -> None:
    location = player.location
    block = player.dimension.get_highest_block_at(location)
    assert block.type != "minecraft:air"
