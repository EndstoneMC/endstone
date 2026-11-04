import threading
import uuid

import pytest
from endstone import Logger, Server
from endstone.actor import ActorType
from endstone.command import Command, CommandMap
from endstone.event import ActorSpawnEvent, event_handler
from endstone.inventory import ItemStack
from endstone.level import Location
from endstone.map import MapRenderer
from endstone.permissions import Permission


def test_primary_thread_is_true_on_the_server_thread(server: Server) -> None:
    """Verify the server thread reports itself as the primary thread."""
    assert server.is_primary_thread is True


def test_primary_thread_is_false_on_a_worker_thread(server: Server) -> None:
    """Verify any other thread is not the primary thread."""
    seen: list[bool] = []
    thread = threading.Thread(target=lambda: seen.append(server.is_primary_thread))
    thread.start()
    thread.join()
    assert seen == [False]


@pytest.mark.parametrize("level", list(Logger.Level))
def test_logger_log_accepts_every_level(server: Server, level: Logger.Level) -> None:
    """Verify log takes each level and a message."""
    server.logger.log(level, f"unbound-api log at {level.name}")
    server.logger.log(level=level, message="keyword form")


def test_logger_log_rejects_a_non_level(server: Server) -> None:
    """Verify the level argument is type checked."""
    with pytest.raises(TypeError):
        server.logger.log("info", "message")


def test_add_permission_registers_it(server: Server) -> None:
    """Verify add_permission returns the permission and makes it reachable by name."""
    manager = server.plugin_manager
    name = f"unbound.api.{uuid.uuid4().hex}"
    added = manager.add_permission(Permission(name, "added by test"))
    try:
        assert added.name == name
        assert added.description == "added by test"
        found = manager.get_permission(name)
        assert found is not None
        assert found.name == name
    finally:
        manager.remove_permission(name)
    assert manager.get_permission(name) is None


def test_add_permission_rejects_a_duplicate(server: Server) -> None:
    """Verify adding a permission under an existing name raises."""
    manager = server.plugin_manager
    name = f"unbound.api.{uuid.uuid4().hex}"
    manager.add_permission(Permission(name))
    try:
        with pytest.raises(ValueError):
            manager.add_permission(Permission(name))
    finally:
        manager.remove_permission(name)


def test_register_to_marks_the_command_registered(command_map: CommandMap) -> None:
    """Verify register_to attaches the command to the map and locks its name."""
    command = Command("unboundregister")
    assert command.is_registered is False

    assert command.register_to(command_map) is True
    assert command.is_registered is True

    command.name = "renamed"
    assert command.name == "unboundregister"

    assert command.register_to(command_map) is True


def test_unregister_from_releases_the_command(command_map: CommandMap) -> None:
    """Verify unregister_from detaches the command so it can change again."""
    command = Command("unboundunregister")
    command.register_to(command_map)

    assert command.unregister_from(command_map) is True
    assert command.is_registered is False

    command.name = "renamed"
    assert command.name == "renamed"


def test_unregister_from_an_unregistered_command(command_map: CommandMap) -> None:
    """Verify unregistering a command that was never registered still succeeds."""
    command = Command("unboundnever")
    assert command.unregister_from(command_map) is True
    assert command.is_registered is False


@pytest.fixture
def command_map(server: Server) -> CommandMap:
    return server.command_map


def test_has_conflicting_enchant() -> None:
    """Verify an enchantment conflicts only with ones already on the meta that exclude it."""
    meta = ItemStack("minecraft:diamond_sword").item_meta
    assert meta.has_conflicting_enchant("sharpness") is False

    assert meta.add_enchant("sharpness", 3, True)
    assert meta.has_conflicting_enchant("smite") is True
    assert meta.has_conflicting_enchant("bane_of_arthropods") is True
    assert meta.has_conflicting_enchant("unbreaking") is False
    assert meta.has_conflicting_enchant("no_such_enchantment") is False

    meta.remove_enchants()
    assert meta.has_conflicting_enchant("smite") is False


@pytest.mark.parametrize("contextual", [False, True])
def test_map_renderer_is_contextual(contextual: bool) -> None:
    """Verify the flag given to the constructor is what is read back."""
    assert MapRenderer(is_contextual=contextual).is_contextual is contextual


def test_map_renderer_is_not_contextual_by_default() -> None:
    """Verify a renderer is not contextual unless asked."""
    assert MapRenderer().is_contextual is False


def test_map_renderer_subclass_keeps_the_flag() -> None:
    """Verify a Python subclass that forwards the flag reports it."""

    class Renderer(MapRenderer):
        def __init__(self) -> None:
            MapRenderer.__init__(self, is_contextual=True)

    assert Renderer().is_contextual is True


class SpawnRecorder:
    def __init__(self) -> None:
        self.types: list[ActorType] = []

    @event_handler
    def on_spawn(self, event: ActorSpawnEvent) -> None:
        self.types.append(event.actor_type)


def test_actor_event_reports_the_actor_type(server: Server, plugin, plot) -> None:
    """Verify actor_type on an ActorEvent is the type of the actor involved."""
    recorder = SpawnRecorder()
    plugin.register_events(recorder)
    overworld = server.level.get_dimension("overworld")
    x, y, z = plot.pos(*plot.middle)
    pig = overworld.spawn_actor(Location(overworld, x + 0.5, y, z + 0.5), "minecraft:pig")
    try:
        assert pig is not None
        assert pig.type in recorder.types
        assert all(isinstance(t, ActorType) for t in recorder.types)
        assert str(ActorType.PIG) in {str(t) for t in recorder.types}
    finally:
        if pig is not None:
            pig.remove()
