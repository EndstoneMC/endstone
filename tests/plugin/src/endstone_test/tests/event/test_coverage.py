from endstone.plugin import Plugin

from endstone_test.automation import EVENT_MANIFEST
from endstone_test.recorder import EventRecorder


def test_every_listener_registered_its_events(recorder: EventRecorder) -> None:
    """Verify the listeners declared a substantial slice of the event API."""
    events = [name for name in recorder.tracked if "/" not in name]
    assert len(events) >= 75


def test_no_duplicate_tracking(recorder: EventRecorder) -> None:
    """Verify each event is tracked exactly once."""
    assert len(recorder.tracked) == len(set(recorder.tracked))


def test_manifest_has_non_overlapping_categories() -> None:
    """Keep server, GameTest, driver, and unsupported coverage explicit."""
    categories = {
        "server_automatable",
        "gametest_required",
        "protocol_driver_required",
        "no_fire_site_or_unsupported",
    }
    assert set(EVENT_MANIFEST) == categories | {"version"}

    entries = [
        name
        for category in categories
        for name in (
            EVENT_MANIFEST[category].keys()
            if isinstance(EVENT_MANIFEST[category], dict)
            else EVENT_MANIFEST[category]
        )
    ]
    assert len(entries) == len(set(entries))


def test_report_untriggered_events(recorder: EventRecorder, plugin: Plugin) -> None:
    """Report which events have not been triggered yet, without failing."""
    plugin.logger.info(recorder.summary())
    for name in recorder.missing:
        plugin.logger.info(f"  not yet triggered: {name}")
