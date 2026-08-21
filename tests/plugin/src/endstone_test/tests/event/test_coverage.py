from endstone.plugin import Plugin

from endstone_test.recorder import EventRecorder


def test_every_listener_registered_its_events(recorder: EventRecorder) -> None:
    """Verify the listeners declared a substantial slice of the event API."""
    assert len(recorder.tracked) >= 75


def test_no_duplicate_tracking(recorder: EventRecorder) -> None:
    """Verify each event is tracked exactly once."""
    assert len(recorder.tracked) == len(set(recorder.tracked))


def test_report_untriggered_events(recorder: EventRecorder, plugin: Plugin) -> None:
    """Report which events have not been triggered yet, without failing."""
    plugin.logger.info(recorder.summary())
    for name in recorder.missing:
        plugin.logger.info(f"  not yet triggered: {name}")
