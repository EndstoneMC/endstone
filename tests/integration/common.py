"""Shared constants and atomic file helpers for the event-test runner."""

import json
import os
import tempfile
from pathlib import Path
from typing import Any

TERMINAL_STATUSES = {"passed", "failed", "cancelled", "infrastructure_failed"}
TEST_WORLD_MARKER = ".endstone_event_test_world"
WORLD_TRANSACTION_FILE = ".endstone_event_world_transaction.json"
DRIVER_ACK_SUFFIX = ".driver.json"
RUN_TIMEOUT_SECONDS = 300.0

REPOSITORY_ROOT = Path(__file__).resolve().parents[2]
EVENT_TESTS_ROOT = Path(__file__).resolve().parent
EVENT_TEST_ASSETS = EVENT_TESTS_ROOT / "assets"
BOT_DIR = EVENT_TEST_ASSETS / "bedrock_bot"
GAMETEST_DIR = EVENT_TEST_ASSETS / "gametest"
LIFECYCLE_FIXTURE_DIR = EVENT_TEST_ASSETS / "plugin_lifecycle_fixture"


def repository_root() -> Path:
    return REPOSITORY_ROOT


def env_enabled(name: str) -> bool:
    return os.environ.get(name, "").strip().lower() in {"1", "true", "yes", "on"}


def atomic_write_json(path: Path, payload: Any) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary_path: str | None = None
    try:
        with tempfile.NamedTemporaryFile(
            mode="w",
            encoding="utf-8",
            dir=path.parent,
            prefix=f".{path.name}.",
            suffix=".tmp",
            delete=False,
        ) as temporary:
            temporary_path = temporary.name
            json.dump(payload, temporary, ensure_ascii=False, indent=2)
            temporary.write("\n")
        os.replace(temporary_path, path)
        temporary_path = None
    finally:
        if temporary_path is not None:
            Path(temporary_path).unlink(missing_ok=True)


def atomic_write_bytes(path: Path, payload: bytes) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary_path: str | None = None
    try:
        with tempfile.NamedTemporaryFile(
            mode="wb",
            dir=path.parent,
            prefix=f".{path.name}.",
            suffix=".tmp",
            delete=False,
        ) as temporary:
            temporary_path = temporary.name
            temporary.write(payload)
        os.replace(temporary_path, path)
        temporary_path = None
    finally:
        if temporary_path is not None:
            Path(temporary_path).unlink(missing_ok=True)
