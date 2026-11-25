from types import SimpleNamespace
from unittest.mock import Mock

import pytest

from endstone.event import PacketReceiveEvent, PacketSendEvent, PlayerJoinEvent, event_handler
from endstone.plugin import Plugin


def make_plugin():
    manager = SimpleNamespace(register_event=Mock())
    logger = SimpleNamespace(error=Mock())
    plugin = SimpleNamespace(
        name="packet_test", is_enabled=True, server=SimpleNamespace(plugin_manager=manager),
        logger=logger, _listeners=[]
    )
    return plugin, manager, logger


def test_packet_ids_are_stored_as_a_snapshot():
    ids = [9, 10]

    @event_handler(packet_ids=ids)
    def handle(event: PacketReceiveEvent):
        pass

    ids.append(11)
    assert handle._packet_ids == (9, 10)


def test_packet_ids_omitted_keeps_unfiltered_registration():
    plugin, manager, _ = make_plugin()

    class Listener:
        @event_handler
        def handle(self, event: PacketReceiveEvent):
            pass

    Plugin.register_events(plugin, Listener())
    assert manager.register_event.call_count == 1
    assert manager.register_event.call_args.kwargs == {}
    assert manager.register_event.call_args.args[0] == PacketReceiveEvent.__name__


def test_packet_ids_are_forwarded_for_both_packet_directions():
    plugin, manager, _ = make_plugin()

    class Listener:
        @event_handler(packet_ids=[9, 15])
        def incoming(self, event: PacketReceiveEvent):
            pass

        @event_handler(packet_ids=[10])
        def outgoing(self, event: PacketSendEvent):
            pass

    Plugin.register_events(plugin, Listener())
    assert manager.register_event.call_count == 2
    registrations = {
        call.args[0]: call.kwargs["packet_ids"] for call in manager.register_event.call_args_list
    }
    assert registrations == {PacketReceiveEvent.__name__: [9, 15], PacketSendEvent.__name__: [10]}


def test_packet_ids_rejected_for_other_events():
    plugin, manager, logger = make_plugin()

    class Listener:
        @event_handler(packet_ids=[9])
        def handle(self, event: PlayerJoinEvent):
            pass

    Plugin.register_events(plugin, Listener())
    manager.register_event.assert_not_called()
    logger.error.assert_called_once()


@pytest.mark.parametrize("ids", [[-1], [1.5], [True], ["9"], "9"])
def test_packet_ids_must_be_non_negative_integers(ids):
    with pytest.raises(ValueError):
        event_handler(packet_ids=ids)


def test_empty_packet_id_filter_is_distinct_from_unfiltered():
    @event_handler(packet_ids=[])
    def handle(event: PacketSendEvent):
        pass

    assert handle._packet_ids == ()
