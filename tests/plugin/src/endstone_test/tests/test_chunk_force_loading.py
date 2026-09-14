from concurrent.futures import ThreadPoolExecutor

import pytest
from endstone import Server
from endstone.level import Chunk, Dimension
from endstone.plugin import Plugin

COORDINATES = (-28_000, 28_000)


@pytest.fixture(params=[Dimension.OVERWORLD, Dimension.NETHER, Dimension.THE_END])
def dimension(request, server: Server) -> Dimension:
    return server.level.get_dimension(request.param)


@pytest.fixture
def chunk(dimension: Dimension, plugin: Plugin):
    assert not dimension.is_chunk_generated(*COORDINATES)
    assert not dimension.is_chunk_force_loaded(*COORDINATES)
    assert dimension.add_plugin_chunk_ticket(*COORDINATES, plugin)
    try:
        chunk = next(
            c
            for c in dimension.plugin_chunk_tickets[plugin]
            if (c.x, c.z) == COORDINATES
        )
        assert dimension.remove_plugin_chunk_ticket(*COORDINATES, plugin)
        yield chunk
    finally:
        dimension.set_chunk_force_loaded(*COORDINATES, False)
        dimension.remove_plugin_chunk_ticket(*COORDINATES, plugin)
        dimension.unload_chunk_request(*COORDINATES)


def test_force_loading_round_trip(chunk: Chunk, dimension: Dimension) -> None:
    assert chunk.is_force_loaded is False
    chunk.is_force_loaded = True
    assert chunk.is_force_loaded is True
    assert dimension.is_chunk_force_loaded(*COORDINATES) is True
    assert chunk.is_loaded is False
    matches = [c for c in dimension.force_loaded_chunks if (c.x, c.z) == COORDINATES]
    assert len(matches) == 1
    assert matches[0].dimension == dimension
    assert matches[0].is_force_loaded is True
    matches[0].is_force_loaded = False
    assert chunk.is_force_loaded is False
    assert dimension.is_chunk_force_loaded(*COORDINATES) is False
    assert COORDINATES not in {(c.x, c.z) for c in dimension.force_loaded_chunks}


def test_force_loading_is_idempotent(chunk: Chunk, dimension: Dimension) -> None:
    for _ in range(3):
        dimension.set_chunk_force_loaded(*COORDINATES, forced=True)
    assert sum((c.x, c.z) == COORDINATES for c in dimension.force_loaded_chunks) == 1
    for _ in range(3):
        chunk.is_force_loaded = False
    assert chunk.is_force_loaded is False
    assert not dimension.load_chunk(*COORDINATES, generate=False)


def test_force_loaded_chunks_is_a_snapshot(chunk: Chunk, dimension: Dimension) -> None:
    before = dimension.force_loaded_chunks
    assert isinstance(before, list)
    chunk.is_force_loaded = True
    during = dimension.force_loaded_chunks
    assert COORDINATES not in {(c.x, c.z) for c in before}
    assert COORDINATES in {(c.x, c.z) for c in during}
    chunk.is_force_loaded = False
    assert COORDINATES in {(c.x, c.z) for c in during}
    assert COORDINATES not in {(c.x, c.z) for c in dimension.force_loaded_chunks}


def test_reading_and_clearing_force_loading_do_not_load_a_chunk(
    chunk: Chunk, dimension: Dimension
) -> None:
    assert chunk.is_force_loaded is False
    assert dimension.is_chunk_force_loaded(*COORDINATES) is False
    assert COORDINATES not in {(c.x, c.z) for c in dimension.force_loaded_chunks}
    chunk.is_force_loaded = False
    assert chunk.is_loaded is False
    assert chunk.is_generated is False
    assert not dimension.load_chunk(*COORDINATES, generate=False)


@pytest.mark.parametrize("release", ["unload_chunk", "unload_chunk_request"])
def test_unloading_does_not_clear_force_loading(
    chunk: Chunk, dimension: Dimension, release: str
) -> None:
    chunk.is_force_loaded = True
    assert dimension.load_chunk(*COORDINATES)
    getattr(dimension, release)(*COORDINATES)
    assert chunk.is_force_loaded is True
    assert COORDINATES in {(c.x, c.z) for c in dimension.force_loaded_chunks}
    chunk.is_force_loaded = False
    assert not dimension.load_chunk(*COORDINATES, generate=False)


def test_clearing_force_loading_preserves_a_normal_load(
    chunk: Chunk, dimension: Dimension
) -> None:
    assert dimension.load_chunk(*COORDINATES)
    assert chunk.is_force_loaded is False
    assert COORDINATES not in {(c.x, c.z) for c in dimension.force_loaded_chunks}
    chunk.is_force_loaded = True
    chunk.is_force_loaded = False
    assert dimension.load_chunk(*COORDINATES, generate=False)


def test_clearing_force_loading_preserves_a_plugin_ticket(
    chunk: Chunk, dimension: Dimension, plugin: Plugin
) -> None:
    assert chunk.add_plugin_chunk_ticket(plugin)
    assert chunk.is_force_loaded is False
    assert COORDINATES not in {(c.x, c.z) for c in dimension.force_loaded_chunks}
    chunk.is_force_loaded = True
    chunk.is_force_loaded = False
    assert plugin in chunk.plugin_chunk_tickets
    assert not chunk.add_plugin_chunk_ticket(plugin)
    assert dimension.load_chunk(*COORDINATES, generate=False)


@pytest.mark.parametrize("bulk", [False, True])
def test_removing_plugin_tickets_preserves_force_loading(
    chunk: Chunk, dimension: Dimension, plugin: Plugin, bulk: bool
) -> None:
    original_tickets = list(dimension.plugin_chunk_tickets.get(plugin, []))
    try:
        chunk.is_force_loaded = True
        assert chunk.add_plugin_chunk_ticket(plugin)
        if bulk:
            dimension.remove_plugin_chunk_tickets(plugin)
        else:
            assert chunk.remove_plugin_chunk_ticket(plugin)
        assert plugin not in chunk.plugin_chunk_tickets
        assert chunk.is_force_loaded is True
        assert COORDINATES in {(c.x, c.z) for c in dimension.force_loaded_chunks}
    finally:
        for original in original_tickets:
            original.add_plugin_chunk_ticket(plugin)


def test_force_loaded_chunks_are_dimension_specific(
    chunk: Chunk, dimension: Dimension, server: Server
) -> None:
    other_dimensions = [d for d in server.level.dimensions if d != dimension]
    before = {d.id: d.is_chunk_force_loaded(*COORDINATES) for d in other_dimensions}
    chunk.is_force_loaded = True
    assert all(c.dimension == dimension for c in dimension.force_loaded_chunks)
    assert all(
        d.is_chunk_force_loaded(*COORDINATES) is before[d.id] for d in other_dimensions
    )


@pytest.mark.parametrize(
    "operation",
    ["get_chunk", "set_chunk", "get_dimension", "set_dimension", "list_dimension"],
)
def test_force_loading_requires_the_server_thread(
    chunk: Chunk, dimension: Dimension, operation: str
) -> None:
    def access_api() -> None:
        if operation == "get_chunk":
            _ = chunk.is_force_loaded
        elif operation == "set_chunk":
            chunk.is_force_loaded = True
        elif operation == "get_dimension":
            dimension.is_chunk_force_loaded(*COORDINATES)
        elif operation == "set_dimension":
            dimension.set_chunk_force_loaded(*COORDINATES, forced=True)
        else:
            _ = dimension.force_loaded_chunks

    with (
        ThreadPoolExecutor(max_workers=1) as executor,
        pytest.raises(RuntimeError, match="server thread"),
    ):
        executor.submit(access_api).result(timeout=5)
    assert chunk.is_force_loaded is False


@pytest.mark.parametrize("coordinates", [(2**31 - 1, 0), (0, -(2**31))])
def test_force_loading_rejects_coordinates_outside_the_world(
    dimension: Dimension, coordinates: tuple[int, int]
) -> None:
    with pytest.raises(RuntimeError, match="Unable to force load chunk"):
        dimension.set_chunk_force_loaded(*coordinates, forced=True)
    assert dimension.is_chunk_force_loaded(*coordinates) is False
    assert coordinates not in {(c.x, c.z) for c in dimension.force_loaded_chunks}
