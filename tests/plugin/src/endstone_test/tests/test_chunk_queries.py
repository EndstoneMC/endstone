import pytest
from endstone import Server
from endstone.actor import Actor
from endstone.level import Chunk, Dimension, Location
from endstone.plugin import Plugin


@pytest.fixture
def resident_chunk(server: Server) -> Chunk:
    chunks = server.level.get_dimension(Dimension.OVERWORLD).loaded_chunks
    if not chunks:
        pytest.skip("no resident chunk")
    return next((chunk for chunk in chunks if (chunk.x, chunk.z) == (0, 0)), chunks[0])


@pytest.fixture
def unloaded_chunk(server: Server, plugin: Plugin) -> Chunk:
    dimension = server.level.get_dimension(Dimension.OVERWORLD)
    coordinates = (29_000, -29_000)
    assert not dimension.is_chunk_generated(*coordinates)
    assert dimension.add_plugin_chunk_ticket(*coordinates, plugin)
    try:
        chunk = next(c for c in dimension.plugin_chunk_tickets[plugin] if (c.x, c.z) == coordinates)
    finally:
        dimension.remove_plugin_chunk_ticket(*coordinates, plugin)
    assert not chunk.is_loaded
    return chunk


def test_entities_match_positions_in_the_dimension(resident_chunk: Chunk) -> None:
    expected = {
        actor.id
        for actor in resident_chunk.dimension.actors
        if (actor.location.block_x >> 4, actor.location.block_z >> 4) == (resident_chunk.x, resident_chunk.z)
    }
    entities = resident_chunk.entities
    assert isinstance(entities, list)
    assert all(isinstance(actor, Actor) for actor in entities)
    assert {actor.id for actor in entities} == expected
    assert len(entities) == len(expected)


@pytest.mark.parametrize("axis", ["x", "z"])
@pytest.mark.parametrize("boundary", [-16, 0, 16])
def test_entities_follow_positions_across_chunk_boundaries(server: Server, axis: str, boundary: int) -> None:
    dimension = server.level.get_dimension(Dimension.OVERWORLD)
    before = Location(dimension, 0.5, 64, 0.5)
    after = Location(dimension, 0.5, 64, 0.5)
    setattr(before, axis, boundary - 0.25)
    setattr(after, axis, boundary + 0.25)
    chunks = {(c.x, c.z): c for c in dimension.loaded_chunks}
    source = chunks.get((before.block_x >> 4, before.block_z >> 4))
    target = chunks.get((after.block_x >> 4, after.block_z >> 4))
    if source is None or target is None:
        pytest.skip("chunks on both sides of the boundary must be loaded")
    actor = dimension.spawn_actor(before, "minecraft:chicken")
    assert actor is not None
    try:
        snapshot = source.entities
        assert actor.id in {entity.id for entity in snapshot}
        assert actor.id not in {entity.id for entity in target.entities}
        actor.teleport(after)
        assert actor.id not in {entity.id for entity in source.entities}
        assert actor.id in {entity.id for entity in target.entities}
        assert actor.id in {entity.id for entity in snapshot}
    finally:
        actor.remove()


def test_entities_exclude_other_dimensions(resident_chunk: Chunk, server: Server) -> None:
    nether = server.level.get_dimension(Dimension.NETHER)
    if not nether.is_chunk_loaded(resident_chunk.x, resident_chunk.z):
        pytest.skip("matching Nether chunk must be loaded")
    location = Location(nether, resident_chunk.x * 16 + 0.5, 64, resident_chunk.z * 16 + 0.5)
    actor = nether.spawn_actor(location, "minecraft:chicken")
    assert actor is not None
    try:
        assert actor.id in {entity.id for entity in nether.actors}
        assert actor.id not in {entity.id for entity in resident_chunk.entities}
    finally:
        actor.remove()


def test_entities_do_not_load_an_unloaded_chunk(unloaded_chunk: Chunk) -> None:
    assert unloaded_chunk.entities == []
    assert not unloaded_chunk.is_loaded
    assert not unloaded_chunk.dimension.is_chunk_generated(unloaded_chunk.x, unloaded_chunk.z)


def test_is_generated_for_a_resident_chunk(resident_chunk: Chunk) -> None:
    assert resident_chunk.is_generated is True
    assert resident_chunk.is_generated is resident_chunk.dimension.is_chunk_generated(resident_chunk.x, resident_chunk.z)


def test_is_generated_does_not_generate_a_chunk(unloaded_chunk: Chunk) -> None:
    assert unloaded_chunk.is_generated is False
    assert not unloaded_chunk.is_loaded
    assert not unloaded_chunk.dimension.is_chunk_generated(unloaded_chunk.x, unloaded_chunk.z)


@pytest.mark.parametrize("dimension_id", [Dimension.OVERWORLD, Dimension.NETHER, Dimension.THE_END])
@pytest.mark.parametrize("x,z,expected", [(0, 0, False), (3, 1, True), (-61, -59, True), (-61, -60, False)])
def test_is_slime_chunk_uses_the_bedrock_pattern(
    server: Server, plugin: Plugin, dimension_id: str, x: int, z: int, expected: bool
) -> None:
    dimension = server.level.get_dimension(dimension_id)
    added_ticket = dimension.add_plugin_chunk_ticket(x, z, plugin)
    try:
        chunk = next(c for c in dimension.plugin_chunk_tickets[plugin] if (c.x, c.z) == (x, z))
    finally:
        if added_ticket:
            dimension.remove_plugin_chunk_ticket(x, z, plugin)
    loaded = chunk.is_loaded
    generated = chunk.is_generated
    assert chunk.is_slime_chunk is expected
    assert chunk.is_slime_chunk is expected
    assert chunk.is_loaded is loaded
    assert chunk.is_generated is generated
