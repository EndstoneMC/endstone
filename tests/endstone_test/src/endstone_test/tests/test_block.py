from endstone import Server


def test_create_block_data_default_block_states(server: Server) -> None:
    block_data = server.create_block_data("minecraft:standing_sign")
    assert block_data.type == "minecraft:standing_sign"
    assert "ground_sign_direction" in block_data.block_states
    assert block_data.block_states["ground_sign_direction"] == 0
    assert "block_light_level" not in block_data.block_states


def test_create_block_data_custom_block_states(server: Server) -> None:
    block_data = server.create_block_data(
        "minecraft:standing_sign", {"ground_sign_direction": 8}
    )
    assert block_data.type == "minecraft:standing_sign"
    assert "ground_sign_direction" in block_data.block_states
    assert block_data.block_states["ground_sign_direction"] == 8
    assert "block_light_level" not in block_data.block_states
    assert block_data.runtime_id == 2761757297


def test_get_block_outside_world_boundaries(server: Server) -> None:
    block = server.level.dimensions[0].get_block_at(x=0, y=-256, z=0)
    assert block.type == "minecraft:air"
