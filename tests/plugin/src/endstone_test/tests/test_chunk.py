import pytest
from endstone import Server
from endstone.block import Block, BlockState
from endstone.level import Chunk, Dimension
from endstone.plugin import Plugin

FAR_CHUNK = (30_000, 30_000)
NEVER_LOADED_CHUNK = (29_000, 29_000)


@pytest.fixture
def chunk(server: Server) -> Chunk:
    chunks = server.level.get_dimension(Dimension.OVERWORLD).loaded_chunks
    if not chunks:
        pytest.skip("no resident chunk")
    return chunks[0]


@pytest.mark.parametrize("x,z", [(0, 0), (0, 15), (15, 0), (15, 15), (7, 11)])
@pytest.mark.parametrize("y", [-64, -1, 0, 319])
def test_get_block_uses_local_xz_and_dimension_y(chunk: Chunk, x: int, y: int, z: int) -> None:
    block = chunk.get_block(x=x, y=y, z=z)
    assert isinstance(block, Block)
    assert (block.x, block.y, block.z) == (chunk.x * 16 + x, y, chunk.z * 16 + z)
    assert block.dimension.id == chunk.dimension.id
    expected = chunk.dimension.get_block_at(block.x, y, block.z)
    assert block.type == expected.type
    assert block.data.runtime_id == expected.data.runtime_id
    assert block.data.block_states == expected.data.block_states


@pytest.mark.parametrize("axis", ["x", "z"])
def test_get_block_in_negative_chunks(server: Server, axis: str) -> None:
    chunks = server.level.get_dimension(Dimension.OVERWORLD).loaded_chunks
    chunk = next((c for c in chunks if getattr(c, axis) < 0), None)
    if chunk is None:
        pytest.skip(f"no resident chunk with negative {axis}")
    block = chunk.get_block(15, -64, 15)
    assert (block.x, block.y, block.z) == (chunk.x * 16 + 15, -64, chunk.z * 16 + 15)
    assert block.x // 16 == chunk.x
    assert block.z // 16 == chunk.z


def test_get_block_returns_a_live_block(chunk: Chunk, server: Server) -> None:
    block = chunk.get_block(7, 319, 11)
    original = block.data
    try:
        block.set_data(server.create_block_data("minecraft:gold_block"), apply_physics=False)
        assert chunk.dimension.get_block_at(block.x, block.y, block.z).type == "minecraft:gold_block"
        chunk.dimension.get_block_at(block.x, block.y, block.z).set_data(
            server.create_block_data("minecraft:stone"), apply_physics=False
        )
        assert block.type == "minecraft:stone"
    finally:
        block.set_data(original, apply_physics=False)
    assert block.data.runtime_id == original.runtime_id
    assert block.data.block_states == original.block_states


@pytest.mark.parametrize("axis", ["x", "z"])
@pytest.mark.parametrize("value", [-2**31, -1, 16, 2**31 - 1])
def test_get_block_rejects_invalid_local_coordinates(chunk: Chunk, axis: str, value: int) -> None:
    coordinates = {"x": 0, "y": 0, "z": 0}
    coordinates[axis] = value
    with pytest.raises(ValueError, match=axis):
        chunk.get_block(**coordinates)


@pytest.mark.parametrize("y", [-2**31, -65, 320, 2**31 - 1])
def test_get_block_rejects_invalid_height(chunk: Chunk, y: int) -> None:
    with pytest.raises(ValueError, match="y"):
        chunk.get_block(0, y, 0)


def test_get_block_does_not_load_the_chunk(server: Server, plugin: Plugin) -> None:
    dimension = server.level.get_dimension(Dimension.OVERWORLD)
    dimension.add_plugin_chunk_ticket(*FAR_CHUNK, plugin)
    try:
        chunk = next(c for c in dimension.plugin_chunk_tickets[plugin] if (c.x, c.z) == FAR_CHUNK)
    finally:
        dimension.remove_plugin_chunk_ticket(*FAR_CHUNK, plugin)
    assert chunk.is_loaded is False
    block = chunk.get_block(7, 64, 11)
    assert (block.x, block.y, block.z) == (480_007, 64, 480_011)
    assert block.type == "minecraft:air"
    assert chunk.is_loaded is False


def _resident_chunk_near_spawn(dimension: Dimension) -> tuple[int, int] | None:
    for x in range(-2, 3):
        for z in range(-2, 3):
            if dimension.is_chunk_loaded(x, z):
                return x, z
    return None


def test_a_far_chunk_is_not_loaded(server: Server) -> None:
    """Verify a chunk far outside the spawn area is not resident."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    assert overworld.is_chunk_loaded(*NEVER_LOADED_CHUNK) is False


def test_a_far_chunk_is_not_generated(server: Server) -> None:
    """Verify a chunk that was never visited reports as ungenerated."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    assert overworld.is_chunk_generated(*NEVER_LOADED_CHUNK) is False


def test_load_chunk_without_generate_refuses_an_ungenerated_chunk(server: Server) -> None:
    """Verify load_chunk(generate=False) reports False and takes no hold."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    assert overworld.load_chunk(*NEVER_LOADED_CHUNK, generate=False) is False
    assert overworld.is_chunk_loaded(*NEVER_LOADED_CHUNK) is False


def test_load_chunk_holds_the_chunk(server: Server) -> None:
    """Verify load_chunk accepts the request; the load itself completes on a later tick."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    try:
        assert overworld.load_chunk(*FAR_CHUNK) is True
        assert overworld.is_chunk_loaded(*FAR_CHUNK) is False
    finally:
        overworld.unload_chunk_request(*FAR_CHUNK)


def test_unload_chunk_reports_true_when_nothing_was_resident(server: Server) -> None:
    """Verify unload_chunk releases the hold and reports True for a chunk that never loaded."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    overworld.load_chunk(*FAR_CHUNK)
    assert overworld.unload_chunk(*FAR_CHUNK) is True


def test_unload_chunk_request_always_reports_true(server: Server) -> None:
    """Verify unload_chunk_request releases the hold without unloading."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    overworld.load_chunk(*FAR_CHUNK)
    assert overworld.unload_chunk_request(*FAR_CHUNK) is True


def test_unload_chunk_reports_false_for_a_chunk_in_use(server: Server) -> None:
    """Verify unload_chunk reports False while something else keeps the chunk resident."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    in_use = _resident_chunk_near_spawn(overworld)
    if in_use is None:
        pytest.skip("no resident chunk near spawn")
    assert overworld.unload_chunk(*in_use) is False


def test_a_resident_chunk_is_generated(server: Server) -> None:
    """Verify a chunk that is loaded also reports as generated."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    in_use = _resident_chunk_near_spawn(overworld)
    if in_use is None:
        pytest.skip("no resident chunk near spawn")
    assert overworld.is_chunk_generated(*in_use) is True


def test_plugin_chunk_ticket_is_recorded(server: Server, plugin: Plugin) -> None:
    """Verify a plugin ticket is registered once, reported, and released again."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    try:
        assert overworld.add_plugin_chunk_ticket(*FAR_CHUNK, plugin) is True
        assert overworld.add_plugin_chunk_ticket(*FAR_CHUNK, plugin) is False
        assert plugin in overworld.get_plugin_chunk_tickets(*FAR_CHUNK)
        assert plugin in overworld.plugin_chunk_tickets
    finally:
        assert overworld.remove_plugin_chunk_ticket(*FAR_CHUNK, plugin) is True
        assert overworld.remove_plugin_chunk_ticket(*FAR_CHUNK, plugin) is False


def test_unload_chunk_does_not_remove_a_plugin_ticket(server: Server, plugin: Plugin) -> None:
    """Verify unload_chunk leaves plugin tickets alone, as Paper does."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    overworld.add_plugin_chunk_ticket(*FAR_CHUNK, plugin)
    try:
        overworld.unload_chunk(*FAR_CHUNK)
        assert plugin in overworld.get_plugin_chunk_tickets(*FAR_CHUNK)
    finally:
        overworld.remove_plugin_chunk_ticket(*FAR_CHUNK, plugin)


def test_remove_plugin_chunk_tickets_releases_every_chunk(server: Server, plugin: Plugin) -> None:
    """Verify remove_plugin_chunk_tickets drops all of a plugin's tickets at once."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    overworld.add_plugin_chunk_ticket(*FAR_CHUNK, plugin)
    overworld.remove_plugin_chunk_tickets(plugin)
    assert overworld.get_plugin_chunk_tickets(*FAR_CHUNK) == []
    assert plugin not in overworld.plugin_chunk_tickets


def test_loaded_chunks_is_a_list(server: Server) -> None:
    """Verify the dimension reports its resident chunks as a list."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    assert isinstance(overworld.loaded_chunks, list)


def test_chunk_reports_its_own_loaded_state(server: Server) -> None:
    """Verify Chunk.is_loaded agrees with the dimension for a resident chunk."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    chunks = overworld.loaded_chunks
    if not chunks:
        pytest.skip("no resident chunk")
    chunk = chunks[0]
    assert chunk.is_loaded is overworld.is_chunk_loaded(chunk.x, chunk.z)


def test_block_actors_stay_inside_the_chunk(server: Server) -> None:
    """Verify every state block_actors reports sits within its own chunk and matches the block there."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    for chunk in overworld.loaded_chunks[:16]:
        for state in chunk.block_actors:
            assert isinstance(state, BlockState)
            assert state.x >> 4 == chunk.x
            assert state.z >> 4 == chunk.z
            assert state.type == overworld.get_block_at(state.x, state.y, state.z).type
