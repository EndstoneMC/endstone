import pytest
from endstone import Server
from endstone.level import Dimension
from endstone.plugin import Plugin

FAR_CHUNK = (30_000, 30_000)
NEVER_LOADED_CHUNK = (29_000, 29_000)


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
