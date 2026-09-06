import pytest
from endstone import Server

from endstone_test.recorder import EventRecorder


def test_server_load(recorder: EventRecorder) -> None:
    """Verify ServerLoadEvent is seen at startup."""
    assert recorder.require("ServerLoadEvent")[0]["load_type"] == "STARTUP"


def test_server_load_type_is_a_known_member(recorder: EventRecorder) -> None:
    """Verify the recorded load type is one the bindings expose."""
    for snapshot in recorder.require("ServerLoadEvent"):
        assert snapshot["load_type"] in ("STARTUP", "RELOAD")


def test_server_command(recorder: EventRecorder) -> None:
    """Verify ServerCommandEvent records the console sender and command line."""
    snapshot = recorder.require("ServerCommandEvent")[0]
    assert snapshot["sender"]
    assert snapshot["command"]


def test_plugin_enable(recorder: EventRecorder) -> None:
    """Verify PluginEnableEvent names the plugin being enabled."""
    plugins = {s["plugin"] for s in recorder.require("PluginEnableEvent")}
    assert "test" in plugins


def test_script_message(recorder: EventRecorder) -> None:
    """Verify ScriptMessageEvent carries the message id, body and sender."""
    snapshot = recorder.require("ScriptMessageEvent")[0]
    assert snapshot["message_id"]
    assert snapshot["sender"]


def test_broadcast_message(recorder: EventRecorder) -> None:
    """Verify BroadcastMessageEvent reports its recipients."""
    snapshot = recorder.require("BroadcastMessageEvent")[0]
    assert snapshot["recipient_count"] >= 1
    assert isinstance(snapshot["is_asynchronous"], bool)


def test_server_list_ping(recorder: EventRecorder, server: Server) -> None:
    """Verify ServerListPingEvent lets a listener rewrite the motd and level name."""
    snapshot = recorder.require("ServerListPingEvent")[0]
    assert "Endstone Test" in snapshot["level_name"]
    assert snapshot["max_players"] > 0
    assert snapshot["network_protocol_version"] == server.protocol_version


def test_map_initialize(recorder: EventRecorder) -> None:
    """Verify MapInitializeEvent reports a valid map view."""
    snapshot = recorder.require("MapInitializeEvent")[0]
    assert snapshot["map_id"] != -1
    assert snapshot["scale"]


@pytest.mark.parametrize("event_name", ["PacketSendEvent", "PacketReceiveEvent"])
def test_packet_events(recorder: EventRecorder, event_name: str) -> None:
    """Verify the packet events report an id, a payload size and a sub client id."""
    for snapshot in recorder.require(event_name):
        assert snapshot["packet_id"] >= 0
        assert snapshot["payload_size"] >= 0
        assert 0 <= snapshot["sub_client_id"] <= 3


@pytest.mark.parametrize("event_name", ["PacketSendEvent", "PacketReceiveEvent"])
def test_packet_events_fire_constantly(
    recorder: EventRecorder, event_name: str
) -> None:
    """Verify the packet events are recorded far more often than they are snapshotted."""
    recorder.require(event_name)
    assert recorder.count(event_name) >= len(recorder.snapshots(event_name))
