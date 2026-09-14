import json
import sys
from pathlib import Path
from typing import Any

from .common import TERMINAL_STATUSES, atomic_write_json


def read_terminal_result(path: Path) -> dict[str, Any] | None:
    payload = read_result(path)
    if payload is None or payload.get("status") not in TERMINAL_STATUSES:
        return None
    return payload


def read_current_result(path: Path, *, run_id: str, world_name: str) -> dict[str, Any] | None:
    """Read only a report written by the current isolated world run."""
    payload = read_result(path)
    if payload is None:
        return None
    if payload.get("run_id") != run_id or payload.get("world_name") != world_name:
        return None
    return payload


def cleanup_driver_ack_files(path: Path) -> None:
    """Remove the final ACK and any interrupted atomic-write temporary files."""
    paths = [path, *path.parent.glob(f"{path.name}.*.tmp")]
    errors: list[str] = []
    for candidate in paths:
        try:
            candidate.unlink(missing_ok=True)
        except OSError as exc:
            errors.append(f"{candidate}: {exc}")
    if errors:
        raise RuntimeError("driver acknowledgement cleanup failed: " + "; ".join(errors))


def write_infrastructure_failure(
    path: Path,
    *,
    run_id: str,
    world_name: str | None,
    phase: str,
    message: str,
    returncodes: dict[str, int | None] | None = None,
) -> dict[str, Any]:
    """Publish a terminal report for failures outside the event assertions."""
    payload: dict[str, Any] = {
        "schema_version": 2,
        "status": "infrastructure_failed",
        "run_id": run_id,
        "world_name": world_name,
        "infrastructure": {
            "phase": phase,
            "message": message,
            "returncodes": returncodes or {},
        },
    }
    try:
        atomic_write_json(path, payload)
    except OSError as exc:
        print(f"[runner] Could not write infrastructure failure report {path}: {exc}", file=sys.stderr)
    return payload


def read_result(path: Path) -> dict[str, Any] | None:
    try:
        payload = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError):
        return None
    if not isinstance(payload, dict):
        return None
    return payload
