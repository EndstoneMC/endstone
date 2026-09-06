from __future__ import annotations

import json
import os
from pathlib import Path

from endstone import Server
from endstone.plugin import Plugin

from endstone_test.recorder import EventRecorder

from .context import AutomationContext
from .coverage import build_coverage
from .models import EventScenario, ScenarioOutcome
from .report import write_report
from .scenarios.catalog import build_scenarios


class AutomatedEventRunner:
    """Run deterministic event smoke scenarios without blocking server ticks."""

    def __init__(
        self,
        plugin: Plugin,
        recorder: EventRecorder,
        *,
        result_path: str | Path,
        stop_server: bool = False,
    ) -> None:
        self.plugin = plugin
        self.server: Server = plugin.server
        self.recorder = recorder
        self.result_path = Path(result_path)
        self.stop_server = stop_server

        self._scenarios: list[EventScenario] = []
        self._outcomes: list[ScenarioOutcome] = []
        self._task = None
        self._index = 0
        self._active: EventScenario | None = None
        self._baseline: dict[str, int] = {}
        self._ticks = 0
        self._ready_since: int | None = None
        self._status = "idle"
        self._action_started = False
        self._driver_ready = False
        self._bot_name = os.getenv("ENDSTONE_TEST_BOT_NAME", "EndstoneEventBot")
        self._run_id = os.getenv("ENDSTONE_TEST_RUN_ID", "").strip() or None
        self._run_number = 0
        self._gametest_case: str | None = None
        self._gametest_state: str | None = None
        self._gametest_error: str | None = None
        driver_ack_path = os.getenv("ENDSTONE_TEST_DRIVER_ACK_FILE", "").strip()
        self._driver_ack_path = Path(driver_ack_path) if driver_ack_path else None
        selected = os.getenv("ENDSTONE_TEST_AUTO_EVENT_SCENARIOS", "")
        self._selected_scenario_names = {
            name.strip() for name in selected.split(",") if name.strip()
        } or None

    def handle_gametest_message(self, message_id: str, message: str) -> None:
        """Consume the tiny GameTest-to-Endstone control channel.

        GameTest still performs the world action and reports its own pass/fail;
        this channel only lets the Python runner correlate that result with the
        Endstone events observed by the registered listeners.
        """
        if message_id != "endstone:gametest":
            return
        parts = str(message).split(" ", 2)
        if len(parts) < 2 or self._active is None:
            return
        state, case = parts[0], parts[1]
        if self._active.gametest_case != case:
            return
        if state == "start":
            self._gametest_state = "running"
            self._gametest_error = None
        elif state == "pass":
            self._gametest_state = "passed"
        elif state == "fail":
            self._gametest_state = "failed"
            self._gametest_error = parts[2] if len(parts) == 3 else "GameTest failed"
        self._write_report()

    def _coverage(self, *, final: bool) -> dict[str, object]:
        return build_coverage(self, final=final)

    @property
    def running(self) -> bool:
        return self._status == "running"

    @property
    def finished(self) -> bool:
        return self._status in {"passed", "failed"}

    def start(self) -> bool:
        """Start the runner on the server scheduler."""
        if self.running:
            return False

        self._outcomes.clear()
        self._scenarios.clear()
        self._index = 0
        self._active = None
        self._run_number += 1
        self._gametest_case = None
        self._gametest_state = None
        self._gametest_error = None
        self._status = "running"
        self._write_report()

        try:
            self._scenarios = self._build_scenarios()
            self._task = self.server.scheduler.run_task(
                self.plugin, self._tick, delay=1, period=1
            )
        except Exception as exc:  # noqa: BLE001
            self._status = "failed"
            self.plugin.logger.error(f"Automated event runner failed to start: {exc}")
            self._write_report()
            return False

        if self._task is None:
            self._status = "failed"
            self.plugin.logger.error(
                "Automated event runner could not schedule its tick task."
            )
            self._write_report()
            return False

        self.plugin.logger.info(
            f"Automated event runner started with {len(self._scenarios)} scenario(s)."
        )
        return True

    def cancel(self) -> None:
        if self._task is not None and self.running:
            self._task.cancel()
            self._task = None
            self._status = "cancelled"
            self._write_report()

    def _tick(self) -> None:
        if self.finished or self._status != "running":
            return

        if self._active is None:
            if self._index >= len(self._scenarios):
                self._finish()
                return

            self._active = self._scenarios[self._index]
            self._baseline = self.recorder.snapshot_counts()
            self._ticks = 0
            self._ready_since = None
            self._action_started = False
            self._driver_ready = False
            self._gametest_case = self._active.gametest_case
            self._gametest_state = None
            self._gametest_error = None
            self.plugin.set_automated_cancel_events(self._active.cancel_events)
            self._write_report()
            return

        self._ticks += 1
        if not self._action_started:
            if (
                self._active.requires_player
                and self.server.get_player(self._bot_name) is None
            ):
                if self._ticks >= self._active.timeout_ticks:
                    self._complete(
                        error=f"player driver did not join as {self._bot_name!r}"
                    )
                return
            try:
                self._active.action()
                self._action_started = True
                self._driver_ready = True
                self._write_report()
            except Exception as exc:  # noqa: BLE001
                self._complete(error=f"{type(exc).__name__}: {exc}")
                return

        # A quit/kick scenario is expected to remove the player.  Check its
        # event assertion before treating the missing player as a driver
        # failure; for every other player-driven scenario, continuing after a
        # disconnected bot only produces a cascade of misleading timeouts.
        if self._gametest_state == "failed":
            self._complete(error=self._gametest_error or "GameTest failed")
            return
        action_status, action_error = self._driver_action_ack()
        if action_status == "failed":
            self._complete(error=action_error or "driver action failed")
            return
        if self._scenario_ready():
            settle_ticks = self._active.settle_ticks
            if settle_ticks > 0:
                if self._ready_since is None:
                    self._ready_since = self._ticks
                if self._ticks - self._ready_since < settle_ticks:
                    return
            self._complete()
            return
        self._ready_since = None
        if (
            self._action_started
            and self._active.requires_player
            and self.server.get_player(self._bot_name) is None
        ):
            self._abort_run(
                error=(
                    f"player driver disconnected during {self._active.name!r}; "
                    "remaining scenarios were not run"
                )
            )
            return
        if self._ticks >= self._active.timeout_ticks:
            error = None
            if self._active.gametest_case is not None and self._gametest_state != "passed":
                error = self._gametest_error or (
                    f"GameTest {self._active.gametest_case!r} did not report pass"
                )
            self._complete(error=error)

    def _expected_events_seen(self) -> bool:
        assert self._active is not None
        return all(
            self.recorder.count(name) > self._baseline.get(name, 0)
            for name in self._active.expected_events
        )

    def _scenario_ready(self) -> bool:
        action_status, _ = self._driver_action_ack()
        if not self._expected_events_seen():
            return False
        if self._active is not None and self._active.ready is not None:
            snapshots = {
                name: self.recorder.snapshots(name)
                for name in self._active.expected_events
            }
            if not self._active.ready(snapshots):
                return False
        if action_status == "pending":
            return False
        if self._active is None or self._active.gametest_case is None:
            return True
        return self._gametest_state == "passed"

    def _driver_action_ack(self) -> tuple[str, str | None]:
        """Return the completion state for the active external driver action.

        The JSON report is owned by this process, so the Node driver uses a
        separate atomically-replaced file for its acknowledgement.  Manual
        ``/test autoevents`` runs have no external driver and retain the old
        behaviour.
        """
        if self._active is None or not self._active.driver_action:
            return "not_required", None
        if self._active.driver_action in {"connect", "console-command"}:
            return "not_required", None
        if self._driver_ack_path is None:
            return "not_configured", None
        try:
            payload = json.loads(self._driver_ack_path.read_text(encoding="utf-8"))
        except (FileNotFoundError, OSError, UnicodeDecodeError, json.JSONDecodeError):
            return "pending", None
        if not isinstance(payload, dict):
            return "pending", None
        if (
            payload.get("scenario") != self._active.name
            or payload.get("action") != self._active.driver_action
        ):
            return "pending", None
        if self._run_id is not None and payload.get("run_id") != self._run_id:
            return "pending", None
        status = payload.get("status")
        if status == "completed":
            return "completed", None
        if status == "failed":
            error = payload.get("error")
            return "failed", str(error) if error else "driver action failed"
        return "pending", None

    def _complete(self, error: str | None = None) -> None:
        assert self._active is not None
        if self._active.cleanup is not None:
            try:
                self._active.cleanup()
            except Exception as exc:  # noqa: BLE001
                cleanup_error = f"{type(exc).__name__}: {exc}"
                error = f"{error}; cleanup failed: {cleanup_error}" if error else cleanup_error
        current = self.recorder.snapshot_counts()
        observed = {
            name: max(0, current.get(name, 0) - self._baseline.get(name, 0))
            for name in self._active.expected_events
        }
        missing = tuple(name for name, count in observed.items() if count == 0)
        if error is None and self._active.verify is not None:
            snapshots = {}
            for name, count in observed.items():
                recorded = self.recorder.snapshots(name)
                snapshots[name] = recorded[-count:] if count else []
            try:
                self._active.verify(snapshots)
            except AssertionError as exc:
                error = f"event assertion failed: {exc}"
        outcome = ScenarioOutcome(
            name=self._active.name,
            expected_events=self._active.expected_events,
            observed_events=observed,
            missing_events=missing,
            ticks=self._ticks,
            driver_action=self._active.driver_action,
            error=error,
        )
        self._outcomes.append(outcome)
        self.plugin.logger.info(
            f"Automated event scenario {'passed' if outcome.passed else 'failed'}: "
            f"{outcome.name}"
        )
        if outcome.missing_events:
            self.plugin.logger.error(
                f"{outcome.name} missing event(s): {', '.join(outcome.missing_events)}"
            )
        if outcome.error:
            self.plugin.logger.error(f"{outcome.name} error: {outcome.error}")

        self._active = None
        self.plugin.set_automated_cancel_events(())
        self._index += 1
        self._gametest_case = None
        self._gametest_state = None
        self._gametest_error = None
        self._write_report()

    def _abort_run(self, *, error: str) -> None:
        """Record a fatal driver loss and stop the run without cascading timeouts."""
        self._complete(error=error)
        self._finish()

    def _finish(self) -> None:
        coverage = self._coverage(final=True)
        self._status = (
            "passed"
            if all(outcome.passed for outcome in self._outcomes)
            and coverage["status"] in {"complete", "selected"}
            else "failed"
        )
        if self._task is not None:
            self._task.cancel()
            self._task = None
        self._write_report()
        self.plugin.logger.info(f"Automated event runner finished: {self._status}")

        if self.stop_server:
            self.server.dispatch_command(self.server.command_sender, "stop")

    def _write_report(self) -> None:
        write_report(self)


    def _build_scenarios(self) -> list[EventScenario]:
        context = AutomationContext.from_runner(self)
        return build_scenarios(context)

