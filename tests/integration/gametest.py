import json
import shutil
import subprocess
import sys
import uuid
from dataclasses import dataclass
from pathlib import Path
from typing import Any

from .common import BOT_DIR, GAMETEST_DIR, atomic_write_bytes, atomic_write_json, repository_root
from .world import target_world_path


@dataclass
class GameTestProvisioning:
    scope: str
    pack_id: str
    pack_version: list[int]
    installed_pack: Path
    created_pack: Path | None = None
    created_pack_root: Path | None = None
    world_manifest: Path | None = None
    added_world_entry: dict[str, Any] | None = None
    world_manifest_original_entries: list[Any] | None = None
    world_manifest_original_bytes: bytes | None = None
    world_manifest_created: bool = False
    created_world_directory: Path | None = None
    keep: bool = False

    def cleanup(self) -> None:
        if self.keep:
            print(f"[gametest] Keeping provisioned pack: {self.installed_pack}", flush=True)
            return

        errors: list[str] = []
        if self.world_manifest is not None and self.added_world_entry is not None:
            try:
                remove_world_pack_entry(
                    self.world_manifest,
                    self.added_world_entry,
                    self.world_manifest_original_entries,
                    self.world_manifest_original_bytes,
                    self.world_manifest_created,
                )
            except (OSError, TypeError, ValueError, json.JSONDecodeError) as exc:
                errors.append(f"world manifest {self.world_manifest}: {exc}")

        if self.created_pack is not None:
            try:
                if self.created_pack.exists():
                    shutil.rmtree(self.created_pack)
            except OSError as exc:
                errors.append(f"pack {self.created_pack}: {exc}")

        if self.created_pack_root is not None:
            try:
                if self.created_pack_root.exists() and not any(self.created_pack_root.iterdir()):
                    self.created_pack_root.rmdir()
            except OSError as exc:
                errors.append(f"pack directory {self.created_pack_root}: {exc}")

        if self.created_world_directory is not None:
            try:
                if self.created_world_directory.exists() and not any(self.created_world_directory.iterdir()):
                    self.created_world_directory.rmdir()
            except OSError as exc:
                errors.append(f"world directory {self.created_world_directory}: {exc}")

        if errors:
            print("[gametest] Cleanup incomplete: " + "; ".join(errors), file=sys.stderr, flush=True)
        else:
            print(f"[gametest] Cleaned up provisioned pack {self.pack_id}.", flush=True)


def normalize_pack_id(value: Any, label: str) -> str:
    if not isinstance(value, str) or not value.strip():
        raise ValueError(f"{label} must be a UUID string")
    try:
        return str(uuid.UUID(value.strip()))
    except ValueError as exc:
        raise ValueError(f"{label} is not a valid UUID: {value!r}") from exc


def normalize_pack_version(value: Any, label: str) -> list[int]:
    if (
        not isinstance(value, list)
        or len(value) != 3
        or any(isinstance(part, bool) or not isinstance(part, int) or part < 0 for part in value)
    ):
        raise ValueError(f"{label} must be a three-part non-negative integer version")
    return list(value)


def read_behavior_pack_manifest(pack_path: Path) -> tuple[str, list[int]]:
    manifest_path = pack_path / "manifest.json"
    try:
        manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    except FileNotFoundError as exc:
        raise ValueError(f"behavior pack has no manifest.json: {pack_path}") from exc
    except json.JSONDecodeError as exc:
        raise ValueError(f"invalid behavior-pack manifest {manifest_path}: {exc}") from exc
    except OSError as exc:
        raise ValueError(f"cannot read behavior-pack manifest {manifest_path}: {exc}") from exc

    if not isinstance(manifest, dict) or not isinstance(manifest.get("header"), dict):
        raise TypeError(f"behavior-pack manifest has no header object: {manifest_path}")
    header = manifest["header"]
    pack_id = normalize_pack_id(header.get("uuid"), f"{manifest_path} header.uuid")
    version = normalize_pack_version(header.get("version"), f"{manifest_path} header.version")
    return pack_id, version


def gametest_pack_candidates(server_folder: Path, explicit: Path | None) -> list[Path]:
    root = repository_root()
    candidates: list[Path] = []
    if explicit is not None:
        if explicit.is_absolute():
            candidates.append(explicit)
        else:
            candidates.extend(
                [
                    server_folder / explicit,
                    Path.cwd() / explicit,
                    root / explicit,
                ]
            )
    else:
        candidates.extend(
            [
                GAMETEST_DIR / "behavior_pack",
                server_folder / "tests" / "integration" / "assets" / "gametest" / "behavior_pack",
            ]
        )

    result: list[Path] = []
    seen: set[Path] = set()
    for candidate in candidates:
        resolved = candidate.resolve()
        if resolved not in seen:
            seen.add(resolved)
            result.append(resolved)
    return result


def find_gametest_pack(server_folder: Path, explicit: Path | None) -> tuple[Path | None, list[Path]]:
    candidates = gametest_pack_candidates(server_folder, explicit)
    for candidate in candidates:
        if candidate.is_dir():
            return candidate, candidates
    return None, candidates


def world_pack_entries(path: Path) -> tuple[list[Any], bytes | None]:
    if not path.exists():
        return [], None
    try:
        original = path.read_bytes()
        payload = json.loads(original.decode("utf-8"))
    except UnicodeDecodeError as exc:
        raise ValueError(f"world behavior-pack file is not UTF-8 JSON: {path}") from exc
    except json.JSONDecodeError as exc:
        raise ValueError(f"invalid world behavior-pack JSON {path}: {exc}") from exc
    except OSError as exc:
        raise ValueError(f"cannot read world behavior-pack file {path}: {exc}") from exc
    if not isinstance(payload, list):
        raise TypeError(f"world behavior-pack file must contain a JSON array: {path}")
    return payload, original


def add_world_pack_entry(path: Path, pack_id: str, version: list[int]) -> tuple[dict[str, Any] | None, bool]:
    entries, _ = world_pack_entries(path)
    for entry in entries:
        if not isinstance(entry, dict) or "pack_id" not in entry:
            continue
        try:
            entry_id = normalize_pack_id(entry["pack_id"], f"{path} pack_id")
        except ValueError:
            continue
        if entry_id != pack_id:
            continue
        entry_version = normalize_pack_version(entry.get("version"), f"{path} version")
        if entry_version != version:
            raise RuntimeError(
                f"world already contains pack {pack_id} with version {entry_version}, requested {version}: {path}"
            )
        return None, False

    added_entry: dict[str, Any] = {"pack_id": pack_id, "version": list(version)}
    entries.append(added_entry)
    atomic_write_json(path, entries)
    return added_entry, True


def remove_world_pack_entry(
    path: Path,
    added_entry: dict[str, Any],
    original_entries: list[Any] | None,
    original_bytes: bytes | None,
    created_file: bool,
) -> None:
    if not path.exists():
        return
    entries, _ = world_pack_entries(path)
    for index in range(len(entries) - 1, -1, -1):
        if entries[index] == added_entry:
            entries.pop(index)
            break
    else:
        print(f"[gametest] Preserving changed world manifest entry: {path}", flush=True)
        return

    if original_entries is not None and entries == original_entries and original_bytes is not None:
        atomic_write_bytes(path, original_bytes)
        return
    if not entries and created_file:
        path.unlink()
        return
    atomic_write_json(path, entries)


def find_installed_pack(pack_root: Path, pack_id: str) -> tuple[Path, list[int]] | None:
    if not pack_root.is_dir():
        return None
    for candidate in sorted(pack_root.iterdir(), key=lambda item: item.name):
        if not candidate.is_dir():
            continue
        try:
            candidate_id, candidate_version = read_behavior_pack_manifest(candidate)
        except ValueError:
            continue
        if candidate_id == pack_id:
            return candidate, candidate_version
    return None


def refresh_managed_pack(source: Path, destination: Path) -> None:
    """Replace a runner-owned pack directory instead of merging stale files."""
    if destination.exists():
        for child in destination.iterdir():
            if child.is_dir():
                shutil.rmtree(child)
            else:
                child.unlink()
    else:
        destination.mkdir(parents=True)
    shutil.copytree(source, destination, dirs_exist_ok=True)


def provision_gametest_pack(
    server_folder: Path,
    source: Path | None,
    scope: str,
    world_name: str | None,
    keep: bool,
) -> GameTestProvisioning:
    source, candidates = find_gametest_pack(server_folder, source)
    if source is None:
        searched = ", ".join(str(candidate) for candidate in candidates)
        raise RuntimeError(
            "GameTest is enabled but no behavior pack was found. "
            f"Expected tests/integration/assets/gametest/behavior_pack; searched: {searched}"
        )

    pack_id, pack_version = read_behavior_pack_manifest(source)
    pack_root = server_folder / ("system_behavior_packs" if scope == "system" else "behavior_packs")
    provisioning = GameTestProvisioning(
        scope=scope,
        pack_id=pack_id,
        pack_version=pack_version,
        installed_pack=source,
        keep=keep,
    )
    try:
        root_existed = pack_root.exists()
        pack_root.mkdir(parents=True, exist_ok=True)
        if not root_existed:
            provisioning.created_pack_root = pack_root

        installed = find_installed_pack(pack_root, pack_id)
        if installed is not None:
            installed_path, installed_version = installed
            if installed_version != pack_version:
                raise RuntimeError(
                    f"installed behavior pack {pack_id} has version {installed_version}, requested {pack_version}: "
                    f"{installed_path}"
                )
            if (
                installed_path.resolve() != source.resolve()
                and installed_path.name
                not in {
                    "endstone_event_gametest",
                }
                and not installed_path.name.startswith("endstone_gametest_")
            ):
                raise RuntimeError(f"behavior-pack UUID collision with an existing pack: {installed_path}")
            if installed_path.resolve() != source.resolve():
                # This is a runner-owned destination. Keep it synchronized with
                # the checked-in pack so a rerun cannot execute an old script or
                # miss a newly added structure file.
                refresh_managed_pack(source, installed_path)
            provisioning.installed_pack = installed_path
            print(f"[gametest] Synchronized installed behavior pack: {installed_path}", flush=True)
        else:
            destination = pack_root / f"endstone_gametest_{pack_id}"
            suffix = 0
            while destination.exists():
                suffix += 1
                destination = pack_root / f"endstone_gametest_{pack_id}_{suffix}"
            provisioning.created_pack = destination
            shutil.copytree(source, destination)
            provisioning.installed_pack = destination
            print(f"[gametest] Copied behavior pack to: {destination}", flush=True)

        if scope == "world":
            world_path = target_world_path(server_folder, world_name)
            world_existed = world_path.exists()
            world_path.mkdir(parents=True, exist_ok=True)
            if not world_existed:
                provisioning.created_world_directory = world_path
            manifest_path = world_path / "world_behavior_packs.json"
            manifest_existed = manifest_path.exists()
            original_entries, original_bytes = world_pack_entries(manifest_path)
            added_entry, added = add_world_pack_entry(manifest_path, pack_id, pack_version)
            if added:
                provisioning.world_manifest = manifest_path
                provisioning.added_world_entry = added_entry
                provisioning.world_manifest_original_entries = original_entries
                provisioning.world_manifest_original_bytes = original_bytes
                provisioning.world_manifest_created = not manifest_existed
                if not manifest_existed:
                    provisioning.created_world_directory = provisioning.created_world_directory or world_path
                print(f"[gametest] Activated {pack_id} in world: {manifest_path}", flush=True)
            else:
                print(f"[gametest] World already activates {pack_id}: {manifest_path}", flush=True)
        else:
            print(f"[gametest] Activated {pack_id} globally via: {pack_root}", flush=True)
        print(f"[gametest] Pack UUID={pack_id}, version={pack_version}, scope={scope}", flush=True)
        return provisioning
    except (OSError, RuntimeError, TypeError, ValueError, shutil.Error, json.JSONDecodeError):
        provisioning.cleanup()
        raise


def enable_gametest_experiment(node: str, server_folder: Path, world_name: str | None) -> Path:
    """Enable the Beta APIs/GameTest toggle in the target Bedrock world.

    BDS does not expose a server.properties switch for this world-scoped
    experiment.  The repository already pins PrismarineJS's NBT reader, so the
    helper changes only the little-endian level.dat experiment compound.
    """
    world_path = target_world_path(server_folder, world_name)
    level_dat = world_path / "level.dat"
    if not level_dat.is_file():
        raise RuntimeError(f"GameTest requires an existing target world with level.dat; not found: {level_dat}")
    helper = BOT_DIR / "enable_beta_apis.js"
    if not helper.is_file():
        raise RuntimeError(f"GameTest experiment helper is missing: {helper}")
    subprocess.run([node, str(helper), str(level_dat)], check=True)
    return level_dat
