from endstone import Player
from endstone.block import Container
from endstone.level import Chunk


def test_the_players_chunk_is_loaded(player: Player) -> None:
    """Verify the chunk the player is standing in is resident."""
    x, z = player.location.block_x >> 4, player.location.block_z >> 4
    assert player.dimension.is_chunk_loaded(x, z) is True


def test_loaded_chunks(player: Player) -> None:
    """Verify the dimension reports its resident chunks."""
    chunks = player.dimension.loaded_chunks
    assert len(chunks) > 0
    for chunk in chunks:
        assert isinstance(chunk, Chunk)


def test_chunk_properties(player: Player) -> None:
    """Verify a chunk knows its coordinates, level and dimension."""
    x, z = player.location.block_x >> 4, player.location.block_z >> 4
    chunk = next(c for c in player.dimension.loaded_chunks if (c.x, c.z) == (x, z))
    assert chunk.level is player.server.level
    assert chunk.dimension.id == player.dimension.id
    assert str(chunk)
    assert repr(chunk)


def test_loaded_chunks_agree_with_is_chunk_loaded(player: Player) -> None:
    """Verify every chunk in loaded_chunks reports itself as loaded."""
    for chunk in player.dimension.loaded_chunks[:16]:
        assert player.dimension.is_chunk_loaded(chunk.x, chunk.z) is True


def test_chunk_entities_include_the_player(player: Player) -> None:
    x, z = player.location.block_x >> 4, player.location.block_z >> 4
    chunk = next(c for c in player.dimension.loaded_chunks if (c.x, c.z) == (x, z))
    assert player.id in {actor.id for actor in chunk.entities}
    assert chunk.is_generated is True


def test_block_actors_reports_a_placed_block_actor(player: Player) -> None:
    """Verify a chest placed near the player shows up in its chunk's block_actors at its own coordinates."""
    location = player.location
    x, y, z = location.block_x, location.block_y + 8, location.block_z
    server = player.server
    dimension = player.dimension
    try:
        server.dispatch_command(server.command_sender, f"setblock {x} {y} {z} chest")
        chunk = next(c for c in dimension.loaded_chunks if (c.x, c.z) == (x >> 4, z >> 4))
        placed = [s for s in chunk.block_actors if (s.x, s.y, s.z) == (x, y, z)]
        assert len(placed) == 1
        assert isinstance(placed[0], Container)
        assert placed[0].type == "minecraft:chest"
    finally:
        server.dispatch_command(server.command_sender, f"setblock {x} {y} {z} air")


def test_block_actors_drops_a_broken_block_actor(player: Player) -> None:
    """Verify a chest that has been broken no longer shows up in its chunk's block_actors."""
    location = player.location
    x, y, z = location.block_x, location.block_y + 8, location.block_z
    server = player.server
    dimension = player.dimension
    server.dispatch_command(server.command_sender, f"setblock {x} {y} {z} chest")
    server.dispatch_command(server.command_sender, f"setblock {x} {y} {z} air")
    chunk = next(c for c in dimension.loaded_chunks if (c.x, c.z) == (x >> 4, z >> 4))
    assert [s for s in chunk.block_actors if (s.x, s.y, s.z) == (x, y, z)] == []
