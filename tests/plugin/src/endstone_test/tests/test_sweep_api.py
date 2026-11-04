import importlib

import pytest
from endstone import Server
from endstone.event import Event, event_handler
from endstone.permissions import PermissionLevel
from endstone.plugin import Plugin

CHUNK = (0, 0)


class NamedEvent(Event):
    NAME = "sweep_api_named_event"


named_events: list[Event] = []
_registered = False


class NamedListener:
    @event_handler
    def on_named(self, event: NamedEvent):
        named_events.append(event)


def test_metrics_submodule_paths_are_gone():
    import endstone.metrics as metrics

    for name in ("SimplePie", "AdvancedPie", "CustomChart", "Metrics"):
        assert hasattr(metrics, name)
    for path in ("endstone.metrics.charts", "endstone.metrics.base", "endstone.metrics.config"):
        with pytest.raises(ImportError):
            importlib.import_module(path)


def test_custom_event_name_attribute_is_not_the_registered_name(server: Server, plugin: Plugin):
    global _registered
    if not _registered:
        plugin.register_events(NamedListener())
        _registered = True
    named_events.clear()
    event = NamedEvent()
    assert event.event_name == f"{NamedEvent.__module__}.{NamedEvent.__qualname__}"
    assert event.event_name != NamedEvent.NAME
    server.plugin_manager.call_event(event)
    assert named_events == [event]


def test_not_null_parameter_rejects_none(server: Server):
    with pytest.raises(TypeError):
        server.command_sender.add_attachment(None, "endstone_test.granted", True)


def test_every_dimension_resolves_through_the_level(server: Server):
    level = server.level
    dimensions = level.dimensions
    assert len(dimensions) >= 3
    for dimension in dimensions:
        assert level.get_dimension(dimension.id) is not None
        assert level.get_dimension(dimension.id).id == dimension.id


def test_block_reports_its_dimension_in_every_dimension(server: Server):
    for dimension in server.level.dimensions:
        assert dimension.load_chunk(*CHUNK)
        block = dimension.get_block_at(0, 64, 0)
        assert block.dimension.id == dimension.id
        assert block.location.dimension.id == dimension.id


def test_block_biome_outside_the_vertical_range(server: Server):
    for dimension in server.level.dimensions:
        assert dimension.load_chunk(*CHUNK)
        assert dimension.get_block_at(0, 100000, 0).biome is not None
        assert dimension.get_block_at(0, -100000, 0).biome is not None


def test_remove_permission_keeps_default_lists(server: Server):
    manager = server.plugin_manager
    sender = server.command_sender
    name = "endstone_test.command.testoptmsg"
    levels = (PermissionLevel.OPERATOR, PermissionLevel.CONSOLE)

    def listed(level):
        return name in [perm.name for perm in manager.get_default_permissions(level)]

    assert all(listed(level) for level in levels)
    manager.remove_permission(name)
    assert manager.get_permission(name) is None
    sender.recalculate_permissions()
    assert all(listed(level) for level in levels)
    assert sender.is_permission_set(name)
    assert sender.has_permission(name)
