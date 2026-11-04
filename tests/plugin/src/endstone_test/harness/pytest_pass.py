import gc
from dataclasses import dataclass, field
from pathlib import Path

import pytest

from endstone_test.reporting import logging_to
from endstone_test.tests.conftest import clear_runtime_context, set_runtime_context


@dataclass
class PytestOutcome:
    exit_code: int
    passed: int = 0
    failed: list[str] = field(default_factory=list)
    skipped: list[str] = field(default_factory=list)

    def summary(self) -> dict:
        return {
            "exit_code": self.exit_code,
            "passed_tests": self.passed,
            "failed_tests": self.failed,
            "skipped_tests": self.skipped,
        }

    def merged(self, other: "PytestOutcome") -> "PytestOutcome":
        return PytestOutcome(
            self.exit_code or other.exit_code,
            self.passed + other.passed,
            self.failed + other.failed,
            self.skipped + other.skipped,
        )


class _Collector:
    def __init__(self, outcome: PytestOutcome):
        self.outcome = outcome

    def pytest_runtest_logreport(self, report) -> None:
        if report.outcome == "skipped":
            reason = (
                report.longrepr[2]
                if isinstance(report.longrepr, tuple)
                else str(report.longrepr)
            )
            self.outcome.skipped.append(f"{report.nodeid}: {reason}")
        elif report.outcome == "failed":
            self.outcome.failed.append(f"{report.nodeid} ({report.when})")
        elif report.when == "call":
            self.outcome.passed += 1


def run_pytest(
    plugin,
    marker: str,
    targets: list[str] | None = None,
    args: list[str] | None = None,
    **context,
) -> PytestOutcome:
    """Run the in-server pytest suites selected by marker, blocking the server thread until done.

    `targets` narrows the run to dotted test modules or node ids under endstone_test.tests, and `args` is passed on
    to pytest as given.
    """
    outcome = PytestOutcome(exit_code=-1)
    set_runtime_context(server=plugin.server, plugin=plugin, **context)
    try:
        with logging_to(plugin.logger):
            outcome.exit_code = int(
                pytest.main(
                    [
                        "-s",
                        "--color=yes",
                        "-p",
                        "no:cacheprovider",
                        "-m",
                        marker,
                        *(args or []),
                        "--pyargs",
                        *(targets or ["endstone_test.tests"]),
                    ],
                    plugins=[_Collector(outcome)],
                )
            )
    finally:
        clear_runtime_context()
        gc.collect()
    return outcome


def run_repo_tests(plugin, folder: str, excluded: dict[str, str]) -> PytestOutcome:
    tests = Path(folder)
    config = tests.parent / "pyproject.toml"
    outcome = PytestOutcome(exit_code=-1)
    try:
        with logging_to(plugin.logger):
            outcome.exit_code = int(
                pytest.main(
                    [
                        "-s",
                        "--color=yes",
                        "-p",
                        "no:cacheprovider",
                        "--import-mode=importlib",
                        "--rootdir",
                        str(tests.parent),
                        *(["-c", str(config)] if config.is_file() else []),
                        "--ignore",
                        str(tests / "plugin"),
                        *[arg for nodeid in excluded for arg in ("--deselect", nodeid)],
                        str(tests),
                    ],
                    plugins=[_Collector(outcome)],
                )
            )
    finally:
        gc.collect()
    return outcome
