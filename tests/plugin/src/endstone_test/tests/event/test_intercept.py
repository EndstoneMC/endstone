import pytest
from endstone.plugin import Plugin

from endstone_test.checks import HINTS
from endstone_test.recorder import EventRecorder


def test_every_check_is_tracked(recorder: EventRecorder) -> None:
    """Verify each declared check reached a listener that handles its event."""
    assert set(HINTS) <= set(recorder.tracked)


def test_report_untriggered_checks(recorder: EventRecorder, plugin: Plugin) -> None:
    """Report which cancel and mutate paths are still unexercised, without failing."""
    missing = [name for name in recorder.missing if name in HINTS]
    plugin.logger.info(f"{len(HINTS) - len(missing)}/{len(HINTS)} checks exercised")
    for name in missing:
        plugin.logger.info(f"  not yet exercised: {name} -- {HINTS[name]}")


@pytest.mark.parametrize("key", [k for k in HINTS if k.endswith("/cancel")])
def test_cancel_was_applied(recorder: EventRecorder, key: str) -> None:
    """Verify the cancel path recorded an outcome once its event fired."""
    assert recorder.require(key)


@pytest.mark.parametrize("key", [k for k in HINTS if k.endswith("/mutate")])
def test_mutation_changed_the_value(recorder: EventRecorder, key: str) -> None:
    """Verify the mutation recorded a before and after that actually differ."""
    for snapshot in recorder.require(key):
        pairs = [
            (k[: -len("_before")], v)
            for k, v in snapshot.items()
            if k.endswith("_before")
        ]
        pairs += [("", snapshot["before"])] if "before" in snapshot else []
        for prefix, before in pairs:
            after = snapshot.get(f"{prefix}_after" if prefix else "after")
            assert before != after, f"{key}: {prefix or 'value'} unchanged ({before})"
