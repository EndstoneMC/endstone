from endstone_test.recorder import EventRecorder


def test_unknown_command(recorder: EventRecorder) -> None:
    """Verify UnknownCommandEvent reports its sender, command line and message."""
    snapshots = recorder.require("UnknownCommandEvent")
    snapshot = next(
        item
        for item in snapshots
        if item["command_line"] == "endstone_test_unknown_command_custom"
    )
    assert snapshot["sender"]
    assert snapshot["message"] == "Endstone Test replaced the unknown-command message"


def test_unknown_command_message_can_be_suppressed(
    recorder: EventRecorder,
) -> None:
    """Verify assigning None suppresses the unknown-command message."""
    snapshots = recorder.require("UnknownCommandEvent")
    snapshot = next(
        item
        for item in snapshots
        if item["command_line"] == "endstone_test_unknown_command_silent"
    )
    assert snapshot["message"] is None
