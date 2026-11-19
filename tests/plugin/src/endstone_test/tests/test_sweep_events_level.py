import threading

from endstone import Server
from endstone.event import (
    DimensionLoadEvent,
    EnchantItemEvent,
    InventoryEvent,
    InventoryInteractEvent,
    event_handler,
)
from endstone.level import Dimension, DimensionCreator
from endstone.plugin import Plugin

from endstone_test.dimensions import SWEEP_REALM

CHUNK = (28_000, 28_000)
SECOND_CHUNK = (28_001, 28_001)
BEYOND_WORLD = (2_000_000_000, 2_000_000_000)
FRESH_DIMENSION = SWEEP_REALM
LATE_DIMENSION = "endstone_test:sweep_realm_late"
EVENT_DIMENSION = "endstone_test:sweep_realm_event"


class DimensionLoadListener:
    def __init__(self):
        self.ids: list[str] = []

    @event_handler
    def on_load(self, event: DimensionLoadEvent):
        self.ids.append(event.dimension.id)


def run_off_thread(fn):
    outcome = {}

    def target():
        try:
            outcome["value"] = fn()
        except BaseException as e:
            outcome["error"] = e

    thread = threading.Thread(target=target)
    thread.start()
    thread.join()
    return outcome


def test_create_dimension_registered_at_startup_is_listed(server: Server) -> None:
    """Verify a dimension created in on_load is loaded and the level then lists it."""
    level = server.level
    dimension = level.get_dimension(FRESH_DIMENSION)
    assert dimension is not None
    assert dimension.id == FRESH_DIMENSION
    assert FRESH_DIMENSION in {d.id for d in level.dimensions}


def test_create_dimension_registers_a_fresh_id(server: Server) -> None:
    """Verify create_dimension of an unregistered id after startup yields a dimension the level then lists."""
    level = server.level
    dimension = server.create_dimension(DimensionCreator(LATE_DIMENSION))
    assert dimension is not None
    assert dimension.id == LATE_DIMENSION
    assert level.get_dimension(LATE_DIMENSION) is dimension
    assert LATE_DIMENSION in {d.id for d in level.dimensions}


def test_create_dimension_fires_dimension_load_event(
    server: Server, plugin: Plugin
) -> None:
    """Verify DimensionLoadEvent is raised once for a dimension created after startup."""
    listener = DimensionLoadListener()
    plugin.register_events(listener)
    existed = server.level.get_dimension(EVENT_DIMENSION) is not None
    server.create_dimension(DimensionCreator(EVENT_DIMENSION))
    server.create_dimension(DimensionCreator(EVENT_DIMENSION))
    assert listener.ids.count(EVENT_DIMENSION) == (0 if existed else 1)


def test_create_dimension_twice_keeps_the_handle_and_tickets(
    server: Server, plugin: Plugin
) -> None:
    """Verify creating an existing dimension again neither replaces its handle nor drops its tickets."""
    level = server.level
    overworld = level.get_dimension(Dimension.OVERWORLD)
    try:
        assert overworld.add_plugin_chunk_ticket(*CHUNK, plugin) is True
        again = server.create_dimension(DimensionCreator(FRESH_DIMENSION))
        assert again is level.get_dimension(FRESH_DIMENSION)
        assert plugin in overworld.get_plugin_chunk_tickets(*CHUNK)
    finally:
        overworld.remove_plugin_chunk_ticket(*CHUNK, plugin)


def test_create_dimension_off_thread_raises(server: Server) -> None:
    """Verify create_dimension raises RuntimeError off the server thread."""
    outcome = run_off_thread(
        lambda: server.create_dimension(DimensionCreator(FRESH_DIMENSION))
    )
    assert isinstance(outcome.get("error"), RuntimeError), outcome


def test_get_dimension_twice_keeps_the_handle_and_tickets(
    server: Server, plugin: Plugin
) -> None:
    """Verify asking for an existing dimension again neither replaces its handle nor drops its tickets."""
    level = server.level
    overworld = level.get_dimension(Dimension.OVERWORLD)
    try:
        assert overworld.add_plugin_chunk_ticket(*CHUNK, plugin) is True
        again = level.get_dimension(Dimension.OVERWORLD)
        assert again is overworld
        assert plugin in overworld.get_plugin_chunk_tickets(*CHUNK)
    finally:
        overworld.remove_plugin_chunk_ticket(*CHUNK, plugin)


def test_get_mobs_are_a_subset_of_get_actors(server: Server) -> None:
    """Verify every mob a dimension reports is also among its actors."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    actor_ids = {a.runtime_id for a in overworld.actors}
    assert {m.runtime_id for m in overworld.mobs} <= actor_ids


def test_load_chunk_off_thread_raises(server: Server) -> None:
    """Verify load_chunk raises RuntimeError off the server thread when the chunk has no hold yet."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    outcome = run_off_thread(lambda: overworld.load_chunk(*CHUNK))
    assert isinstance(outcome.get("error"), RuntimeError), outcome
    assert overworld.unload_chunk_request(*CHUNK) is True


def test_load_chunk_off_thread_raises_when_a_hold_exists(server: Server) -> None:
    """Verify load_chunk raises RuntimeError off the server thread even for a chunk that is already held."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    assert overworld.load_chunk(*CHUNK) is True
    try:
        outcome = run_off_thread(lambda: overworld.load_chunk(*CHUNK))
        assert isinstance(outcome.get("error"), RuntimeError), outcome
    finally:
        overworld.unload_chunk_request(*CHUNK)


def test_remove_plugin_chunk_tickets_off_thread_raises(
    server: Server, plugin: Plugin
) -> None:
    """Verify remove_plugin_chunk_tickets raises RuntimeError off the server thread."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    outcome = run_off_thread(lambda: overworld.remove_plugin_chunk_tickets(plugin))
    assert isinstance(outcome.get("error"), RuntimeError), outcome


def test_failed_plugin_ticket_leaves_no_ticket(server: Server, plugin: Plugin) -> None:
    """Verify a False from add_plugin_chunk_ticket for a fresh chunk never leaves the plugin holding a ticket."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    added = overworld.add_plugin_chunk_ticket(*BEYOND_WORLD, plugin)
    try:
        assert (plugin in overworld.get_plugin_chunk_tickets(*BEYOND_WORLD)) is added
    finally:
        overworld.remove_plugin_chunk_ticket(*BEYOND_WORLD, plugin)


def test_add_plugin_chunk_ticket_is_false_only_for_a_duplicate(
    server: Server, plugin: Plugin
) -> None:
    """Verify False from add_plugin_chunk_ticket means the plugin already held the chunk, as documented."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    try:
        added = overworld.add_plugin_chunk_ticket(*BEYOND_WORLD, plugin)
        held = plugin in overworld.get_plugin_chunk_tickets(*BEYOND_WORLD)
        assert added is True or held is True, (
            "False was returned although the plugin held no ticket"
        )
    finally:
        overworld.remove_plugin_chunk_ticket(*BEYOND_WORLD, plugin)


def test_unload_chunk_with_a_plugin_ticket_still_reports_not_loaded(
    server: Server, plugin: Plugin
) -> None:
    """Verify unload_chunk is True for a chunk a plugin ticket holds but that is not resident yet."""
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    overworld.add_plugin_chunk_ticket(*SECOND_CHUNK, plugin)
    try:
        assert overworld.is_chunk_loaded(*SECOND_CHUNK) is False
        assert overworld.unload_chunk(*SECOND_CHUNK) is True
        assert plugin in overworld.get_plugin_chunk_tickets(*SECOND_CHUNK)
    finally:
        overworld.remove_plugin_chunk_ticket(*SECOND_CHUNK, plugin)


def test_inventory_interact_event_is_the_cancellable_inventory_event_base() -> None:
    """Verify InventoryInteractEvent sits under InventoryEvent, is cancellable and carries who_clicked."""
    assert issubclass(InventoryInteractEvent, InventoryEvent)
    assert hasattr(InventoryInteractEvent, "cancel")
    assert hasattr(InventoryInteractEvent, "who_clicked")
    assert issubclass(EnchantItemEvent, InventoryEvent)
    assert not issubclass(EnchantItemEvent, InventoryInteractEvent)
