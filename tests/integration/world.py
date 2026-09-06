import base64
import json
import os
import shutil
import socket
import subprocess
import sys
import time
import uuid
from dataclasses import dataclass
from pathlib import Path
from typing import Any

from .common import (
    BOT_DIR,
    EVENT_TEST_ASSETS,
    TEST_WORLD_MARKER,
    WORLD_TRANSACTION_FILE,
    atomic_write_bytes,
    atomic_write_json,
    repository_root,
)
from .process import ensure_node_runtime, stop_process


@dataclass
class TestWorldProvisioning:
    """Own the clean world copy used by one complete automated run."""

    properties_path: Path
    original_properties: bytes
    template_world: Path
    run_world: Path
    run_name: str
    transaction_path: Path
    keep: bool = False

    def cleanup(self) -> None:
        errors: list[str] = []

        if bedrock_server_is_running():
            raise RuntimeError(
                "cannot clean up the event-test world while a BDS process is still running; "
                "the run world and transaction were preserved"
            )

        restored = False
        world_removed = self.keep
        try:
            atomic_write_bytes(self.properties_path, self.original_properties)
            restored = True
        except OSError as exc:
            errors.append(f"server.properties {self.properties_path}: {exc}")

        if restored and not self.keep:
            try:
                worlds_root = (self.run_world.parent).resolve()
                resolved_run = self.run_world.resolve()
                if resolved_run.parent != worlds_root or not resolved_run.name.startswith("endstone_event_run_"):
                    raise RuntimeError(f"refusing to remove unexpected test world: {self.run_world}")
                if resolved_run.exists():
                    shutil.rmtree(resolved_run)
                world_removed = True
            except (OSError, RuntimeError) as exc:
                errors.append(f"test world {self.run_world}: {exc}")
        elif restored and self.keep:
            print(f"[world] Keeping test world copy: {self.run_world}", flush=True)

        if restored and world_removed:
            try:
                self.transaction_path.unlink(missing_ok=True)
            except OSError as exc:
                errors.append(f"transaction {self.transaction_path}: {exc}")

        if errors:
            print("[world] Cleanup incomplete: " + "; ".join(errors), file=sys.stderr, flush=True)
            raise RuntimeError("test-world cleanup failed; the run world was preserved for recovery")
        elif not self.keep:
            print(f"[world] Restored server.properties and cleaned run world {self.run_name}.", flush=True)


class ServerFolderLock:
    """Hold an OS-level lock for the whole lifetime of one runner."""

    def __init__(self, server_folder: Path) -> None:
        self.path = server_folder / ".endstone_event_runner.lock"
        self._file = None

    def acquire(self) -> None:
        self.path.parent.mkdir(parents=True, exist_ok=True)
        handle = self.path.open("a+b")
        try:
            if self.path.stat().st_size == 0:
                handle.write(b"\0")
                handle.flush()
            handle.seek(0)
            if os.name == "nt":
                import msvcrt

                msvcrt.locking(handle.fileno(), msvcrt.LK_NBLCK, 1)
            else:
                import fcntl

                fcntl.flock(handle.fileno(), fcntl.LOCK_EX | fcntl.LOCK_NB)
        except (OSError, ImportError) as exc:
            handle.close()
            raise RuntimeError(f"another event-test runner already owns {self.path.parent}") from exc
        self._file = handle

    def release(self) -> None:
        if self._file is None:
            return
        try:
            if os.name == "nt":
                import msvcrt

                self._file.seek(0)
                msvcrt.locking(self._file.fileno(), msvcrt.LK_UNLCK, 1)
            else:
                import fcntl

                fcntl.flock(self._file.fileno(), fcntl.LOCK_UN)
        except (OSError, ImportError):
            pass
        finally:
            self._file.close()
            self._file = None


def server_property(server_folder: Path, name: str) -> str | None:
    path = server_folder / "server.properties"
    try:
        lines = path.read_text(encoding="utf-8-sig").splitlines()
    except FileNotFoundError:
        return None
    except OSError as exc:
        raise RuntimeError(f"cannot read {path}: {exc}") from exc
    for line in lines:
        stripped = line.strip()
        if not stripped or stripped.startswith("#") or "=" not in stripped:
            continue
        key, value = stripped.split("=", 1)
        if key.strip() == name:
            return value.strip()
    return None


def target_world_path(server_folder: Path, world_name: str | None) -> Path:
    name = world_name or server_property(server_folder, "level-name") or "Bedrock level"
    relative_name = Path(name)
    worlds_root = (server_folder / "worlds").resolve()
    world_path = (worlds_root / relative_name).resolve()
    if relative_name.is_absolute() or world_path == worlds_root or not world_path.is_relative_to(worlds_root):
        raise RuntimeError(f"unsafe GameTest world name: {name!r}")
    return world_path


def resolve_test_world_template(server_folder: Path, explicit: Path | None) -> Path:
    root = repository_root()
    default_template = EVENT_TEST_ASSETS / "endstone_event_template"
    if explicit is None:
        candidates = [default_template]
    elif explicit.is_absolute():
        candidates = [explicit]
    else:
        candidates = [
            EVENT_TEST_ASSETS / explicit,
            Path.cwd() / explicit,
            root / explicit,
            server_folder / explicit,
            server_folder / "worlds" / explicit,
        ]

    seen: set[Path] = set()
    for candidate in candidates:
        resolved = candidate.resolve()
        if resolved in seen:
            continue
        seen.add(resolved)
        if resolved.is_dir() and (resolved / "level.dat").is_file() and (resolved / "db").is_dir():
            return resolved

    if explicit is None:
        return default_template.resolve()
    searched = ", ".join(str(candidate.resolve()) for candidate in candidates)
    raise RuntimeError(
        "dedicated test world template was not found; create one or omit the explicit path "
        f"to let the runner generate it. Searched: {searched}"
    )


def cleanup_stale_test_worlds(server_folder: Path) -> None:
    worlds_root = (server_folder / "worlds").resolve()
    if not worlds_root.is_dir():
        return
    for candidate in worlds_root.glob("endstone_event_run_*"):
        if not candidate.is_dir():
            continue
        if bedrock_server_is_running():
            raise RuntimeError("cannot clean stale event-test worlds while a BDS process is running")
        marker = candidate / TEST_WORLD_MARKER
        try:
            state = marker.read_text(encoding="utf-8").strip()
        except (FileNotFoundError, OSError, UnicodeDecodeError):
            continue
        if state != "cleanup":
            continue
        try:
            shutil.rmtree(candidate)
            print(f"[world] Removed stale test-world copy: {candidate}", flush=True)
        except OSError as exc:
            raise RuntimeError(f"cannot remove stale test world {candidate}: {exc}") from exc


def server_executable_path(server_folder: Path) -> Path:
    names = ("bedrock_server.exe", "bedrock_server") if os.name == "nt" else ("bedrock_server",)
    for name in names:
        path = server_folder / name
        if path.is_file():
            return path
    raise RuntimeError(
        f"cannot generate the dedicated test world because the BDS executable is missing: {server_folder}"
    )


def free_udp_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        sock.bind(("127.0.0.1", 0))
        return int(sock.getsockname()[1])


def override_server_properties(path: Path, overrides: dict[str, str]) -> bytes:
    try:
        original = path.read_bytes()
    except OSError as exc:
        raise RuntimeError(f"cannot read server properties {path}: {exc}") from exc

    try:
        text = original.decode("utf-8-sig")
    except UnicodeDecodeError as exc:
        raise RuntimeError(f"server properties are not UTF-8 text: {path}") from exc

    newline = "\r\n" if "\r\n" in text else "\n"
    lines = text.splitlines(keepends=True)
    seen: set[str] = set()
    updated: list[str] = []
    for line in lines:
        body = line.rstrip("\r\n")
        line_ending = line[len(body) :]
        if not body.lstrip().startswith("#") and not body.lstrip().startswith("!") and "=" in body:
            key = body.split("=", 1)[0].strip()
            if key in overrides:
                prefix = body[: body.index("=") + 1]
                updated.append(f"{prefix}{overrides[key]}{line_ending or newline}")
                seen.add(key)
                continue
        updated.append(line)

    if updated and not updated[-1].endswith(("\n", "\r")):
        updated[-1] += newline
    for key, value in overrides.items():
        if key not in seen:
            updated.append(f"{key}={value}{newline}")

    atomic_write_bytes(path, "".join(updated).encode("utf-8"))
    return original


def prepare_flat_world(node: str, world_path: Path) -> Path:
    """Switch a newly created Bedrock world to the local flat-world format."""
    level_dat = world_path / "level.dat"
    if not level_dat.is_file():
        raise RuntimeError(f"cannot prepare flat test world without level.dat: {level_dat}")
    helper = BOT_DIR / "prepare_flat_world.js"
    if not helper.is_file():
        raise RuntimeError(f"flat test-world helper is missing: {helper}")
    subprocess.run([node, str(helper), str(level_dat)], check=True)
    return level_dat


def create_test_world_template(
    server_folder: Path,
    template_world: Path,
    *,
    node: str | None = None,
) -> None:
    """Create the persistent clean superflat template once with local BDS."""
    worlds_root = (server_folder / "worlds").resolve()
    resolved_template = template_world.resolve()
    managed_template = (EVENT_TEST_ASSETS / "endstone_event_template").resolve()
    if resolved_template != managed_template:
        raise RuntimeError(
            "automatic test-world generation is limited to "
            f"{managed_template}; provide an existing template for custom paths"
        )
    if resolved_template.exists():
        raise RuntimeError(f"test-world template is incomplete and will not be overwritten: {resolved_template}")

    if node is None:
        node = ensure_node_runtime(BOT_DIR)

    properties_path = server_folder / "server.properties"
    if bedrock_server_is_running():
        raise RuntimeError("cannot generate the event-test template while a BDS process is already running")
    executable = server_executable_path(server_folder)
    resolved_template.parent.mkdir(parents=True, exist_ok=True)
    staging_world = worlds_root / f"endstone_event_template_build_{uuid.uuid4().hex[:12]}"
    template_staging = resolved_template.parent / f".{resolved_template.name}.build_{uuid.uuid4().hex[:12]}"
    original_properties = properties_path.read_bytes()
    transaction_path = write_world_transaction(
        server_folder,
        {
            "version": 1,
            "kind": "template",
            "properties_path": str(properties_path.resolve()),
            "original_properties": base64.b64encode(original_properties).decode("ascii"),
            "world_path": str(staging_world.resolve()),
            "cleanup_world": True,
            "keep": False,
        },
    )

    def generate_world_phase(phase: str) -> None:
        """Let BDS create/save the world, then stop it before the next phase."""
        process: subprocess.Popen[str] | None = None
        process_returncode: int | None = None
        ready = False
        try:
            port = free_udp_port()
            port_v6 = free_udp_port()
            override_server_properties(
                properties_path,
                {
                    "level-name": staging_world.name,
                    "level-seed": "endstone-event-tests-template",
                    "server-port": str(port),
                    "server-portv6": str(port_v6),
                    "enable-lan-visibility": "false",
                },
            )
            print(f"[world] BDS generation phase: {phase}", flush=True)
            process = subprocess.Popen(
                [str(executable)],
                cwd=server_folder,
                stdin=subprocess.PIPE,
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
                text=True,
                encoding="utf-8",
            )
            deadline = time.monotonic() + 120
            while time.monotonic() < deadline:
                if process.poll() is not None:
                    raise RuntimeError(f"BDS exited with code {process.returncode} during {phase}")
                if (staging_world / "level.dat").is_file():
                    # Let BDS finish the first save before it receives stop.
                    time.sleep(2)
                    ready = (staging_world / "level.dat").is_file() and (staging_world / "db").is_dir()
                    if ready:
                        break
                time.sleep(0.5)
            if not ready:
                raise TimeoutError(f"timed out generating the test-world template during {phase}: {resolved_template}")
        finally:
            if process is not None and process.poll() is None:
                try:
                    if process.stdin is not None:
                        process.stdin.write("stop\n")
                        process.stdin.flush()
                except OSError:
                    pass
                try:
                    process.wait(timeout=30)
                except subprocess.TimeoutExpired:
                    stop_process(process)
            if process is not None:
                process_returncode = process.returncode

        if process_returncode not in {0, None}:
            raise RuntimeError(f"BDS exited with code {process_returncode} while finalizing {phase}")

    try:
        print(f"[world] Generating clean test-world template: {resolved_template}", flush=True)
        # BDS has no usable server.properties switch for this on the target
        # version.  First create a normal level.dat, patch its Generator and
        # FlatWorldLayers tags, then remove the old chunks so BDS regenerates
        # them from the flat generator on the second startup.
        generate_world_phase("initial world creation")
        prepare_flat_world(node, staging_world)
        db_path = (staging_world / "db").resolve()
        if db_path.parent != staging_world.resolve() or db_path.name != "db":
            raise RuntimeError(f"refusing to remove unexpected world database: {db_path}")
        if db_path.is_dir():
            shutil.rmtree(db_path)
        (staging_world / "level.dat_old").unlink(missing_ok=True)
        generate_world_phase("superflat chunk regeneration")
    finally:
        atomic_write_bytes(properties_path, original_properties)

    try:
        if not (staging_world / "level.dat").is_file() or not (staging_world / "db").is_dir():
            raise RuntimeError(f"incomplete test-world template staging directory: {staging_world}")
        if resolved_template.exists():
            raise RuntimeError(f"test-world template appeared during generation: {resolved_template}")
        if template_staging.exists():
            raise RuntimeError(f"test-world template staging path already exists: {template_staging}")

        # BDS can only create a world inside its own worlds directory.  Keep
        # that location strictly temporary, then publish the reusable fixture
        # under the integration assets owned by this repository.
        shutil.copytree(staging_world, template_staging)
        if not (template_staging / "level.dat").is_file() or not (template_staging / "db").is_dir():
            raise RuntimeError(f"incomplete copied test-world template: {template_staging}")
        shutil.rmtree(staging_world)
        template_staging.rename(resolved_template)
        transaction_path.unlink(missing_ok=True)
    except BaseException as exc:
        if template_staging.exists():
            shutil.rmtree(template_staging, ignore_errors=True)
        # The transaction is deliberately persisted for crash recovery, but a
        # normal exception should also leave the server usable before this
        # invocation returns.  Keep the original failure as the primary error
        # if recovery itself encounters a second filesystem failure.
        try:
            recover_world_transaction(server_folder)
        except BaseException as recovery_error:  # noqa: BLE001 - preserve the original failure
            exc.add_note(f"immediate test-world recovery also failed: {recovery_error}")
        raise


def provision_test_world(
    server_folder: Path,
    explicit_template: Path | None,
    keep: bool,
) -> TestWorldProvisioning:
    if bedrock_server_is_running():
        raise RuntimeError("event-test runner requires an idle server folder; stop the existing BDS process first")
    recover_world_transaction(server_folder)
    cleanup_stale_test_worlds(server_folder)
    template_world = resolve_test_world_template(server_folder, explicit_template)
    if not template_world.exists():
        if explicit_template is not None:
            raise RuntimeError(f"test-world template does not exist: {template_world}")
        create_test_world_template(server_folder, template_world)
    if not (template_world / "level.dat").is_file() or not (template_world / "db").is_dir():
        raise RuntimeError(f"test-world template is incomplete (level.dat/db required): {template_world}")

    worlds_root = (server_folder / "worlds").resolve()
    run_name = f"endstone_event_run_{uuid.uuid4().hex[:12]}"
    run_world = worlds_root / run_name
    properties_path = server_folder / "server.properties"
    try:
        original_properties = properties_path.read_bytes()
    except OSError as exc:
        raise RuntimeError(f"cannot read server properties {properties_path}: {exc}") from exc
    if run_world.exists():
        raise RuntimeError(f"generated test-world name already exists: {run_world}")

    transaction_path = write_world_transaction(
        server_folder,
        {
            "version": 1,
            "kind": "run",
            "properties_path": str(properties_path.resolve()),
            "original_properties": base64.b64encode(original_properties).decode("ascii"),
            "world_path": str(run_world.resolve()),
            "cleanup_world": True,
            "keep": keep,
        },
    )

    print(f"[world] Copying clean test world {template_world} -> {run_world}", flush=True)
    try:
        shutil.copytree(template_world, run_world)
        override_server_properties(
            properties_path,
            {
                "level-name": run_name,
                "difficulty": "normal",
            },
        )
        atomic_write_bytes(
            run_world / TEST_WORLD_MARKER,
            ("keep\n" if keep else "cleanup\n").encode("utf-8"),
        )
    except BaseException:
        recover_world_transaction(server_folder)
        raise

    return TestWorldProvisioning(
        properties_path=properties_path,
        original_properties=original_properties,
        template_world=template_world,
        run_world=run_world,
        run_name=run_name,
        transaction_path=transaction_path,
        keep=keep,
    )


def world_transaction_path(server_folder: Path) -> Path:
    return server_folder / WORLD_TRANSACTION_FILE


def bedrock_server_is_running() -> bool:
    """Return whether a BDS process is still alive anywhere on this host.

    A stale transaction can be left behind when the runner is killed.  Do not
    remove its world while BDS may still have that world open; the caller can
    retry after stopping the orphaned server.  The check is deliberately
    conservative because process inspection does not expose a portable,
    reliable working-directory API.
    """
    if os.name == "nt":
        result = subprocess.run(
            ["tasklist", "/FI", "IMAGENAME eq bedrock_server.exe", "/FO", "CSV", "/NH"],
            check=False,
            capture_output=True,
            text=True,
            encoding="utf-8",
            errors="replace",
        )
        return any(line.strip().startswith('"bedrock_server.exe"') for line in result.stdout.splitlines())

    result = subprocess.run(
        ["pgrep", "-x", "bedrock_server"],
        check=False,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )
    return result.returncode == 0


def write_world_transaction(server_folder: Path, payload: dict[str, Any]) -> Path:
    path = world_transaction_path(server_folder)
    atomic_write_json(path, payload)
    return path


def recover_world_transaction(server_folder: Path) -> None:
    path = world_transaction_path(server_folder)
    if not path.exists():
        return
    try:
        payload = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeDecodeError, json.JSONDecodeError) as exc:
        raise RuntimeError(f"cannot read incomplete test-world transaction {path}: {exc}") from exc
    if not isinstance(payload, dict) or payload.get("version") != 1:
        raise RuntimeError(f"unsupported test-world transaction: {path}")

    properties_path = (server_folder / "server.properties").resolve()
    if payload.get("properties_path") != str(properties_path):
        raise RuntimeError(f"test-world transaction targets an unexpected properties file: {path}")
    try:
        original_properties = base64.b64decode(payload["original_properties"], validate=True)
    except (KeyError, TypeError, ValueError) as exc:
        raise RuntimeError(f"invalid original server.properties backup in {path}") from exc

    if bedrock_server_is_running():
        raise RuntimeError(
            "cannot recover an incomplete test-world transaction while a BDS process is running; "
            "stop the orphaned server and retry"
        )

    atomic_write_bytes(properties_path, original_properties)

    cleanup_world = payload.get("cleanup_world")
    if cleanup_world:
        raw_world = payload.get("world_path")
        if not isinstance(raw_world, str):
            raise RuntimeError(f"test-world transaction has no world path: {path}")
        worlds_root = (server_folder / "worlds").resolve()
        world_path = Path(raw_world).resolve()
        if (
            world_path.parent != worlds_root
            or not world_path.name.startswith("endstone_event_")
            or world_path.name == "endstone_event_template"
        ):
            raise RuntimeError(f"refusing to recover unexpected test world: {world_path}")
        marker_state = None
        try:
            marker_state = (world_path / TEST_WORLD_MARKER).read_text(encoding="utf-8").strip()
        except (FileNotFoundError, OSError, UnicodeDecodeError):
            pass
        # A kept run is intentionally recoverable after the runner itself is
        # killed. Only preserve a fully copied world with its explicit keep
        # marker; an interrupted copy without the marker is still disposable.
        preserve = payload.get("keep") is True and marker_state == "keep"
        if world_path.exists() and not preserve:
            shutil.rmtree(world_path)

    path.unlink()
    print(f"[world] Recovered incomplete test-world transaction: {path}", flush=True)
