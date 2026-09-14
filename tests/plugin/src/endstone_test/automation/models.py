from __future__ import annotations

from collections.abc import Callable
from dataclasses import dataclass


@dataclass(frozen=True)
class EventScenario:
    """One server-thread action and the events it must cause."""

    name: str
    expected_events: tuple[str, ...]
    action: Callable[[], None]
    timeout_ticks: int = 80
    # Number of server ticks to keep the scenario active after all expected
    # events and driver/GameTest acknowledgements are ready. This lets delayed
    # world-simulation callbacks settle before fixture cleanup and verification.
    settle_ticks: int = 0
    driver_action: str | None = None
    requires_player: bool = False
    driver_data: dict[str, object] | None = None
    gametest_case: str | None = None
    cleanup: Callable[[], None] | None = None
    cancel_events: tuple[str, ...] = ()
    isolated: bool = False
    verify: Callable[[dict[str, list[dict]]], None] | None = None
    ready: Callable[[dict[str, list[dict]]], bool] | None = None


@dataclass(frozen=True)
class ScenarioOutcome:
    name: str
    expected_events: tuple[str, ...]
    observed_events: dict[str, int]
    missing_events: tuple[str, ...]
    ticks: int
    driver_action: str | None = None
    error: str | None = None

    @property
    def passed(self) -> bool:
        return self.error is None and not self.missing_events

    def to_dict(self) -> dict:
        return {
            "name": self.name,
            "status": "passed" if self.passed else "failed",
            "expected_events": list(self.expected_events),
            "observed_events": self.observed_events,
            "missing_events": list(self.missing_events),
            "ticks": self.ticks,
            "driver_action": self.driver_action,
            "error": self.error,
        }

