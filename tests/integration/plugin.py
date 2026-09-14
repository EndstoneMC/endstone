import os
import shutil
import subprocess
import sys
import tempfile
import zipfile
from dataclasses import dataclass
from pathlib import Path

from .common import LIFECYCLE_FIXTURE_DIR, repository_root


@dataclass
class PluginProvisioning:
    """Own the temporary plugin lifecycle fixture installed for one run."""

    python_directory: Path
    build_directory: Path

    def cleanup(self) -> None:
        errors: list[str] = []
        try:
            if self.python_directory.exists():
                shutil.rmtree(self.python_directory)
        except OSError as exc:
            errors.append(f"plugin Python directory {self.python_directory}: {exc}")

        try:
            if self.build_directory.exists():
                shutil.rmtree(self.build_directory)
        except OSError as exc:
            errors.append(f"plugin build directory {self.build_directory}: {exc}")

        if errors:
            raise RuntimeError("; ".join(errors))
        print(f"[plugin] Removed plugin lifecycle fixture: {self.python_directory}", flush=True)


def resolve_runner_python() -> str:
    """Choose a Python environment that can satisfy the embedded runtime."""
    root = repository_root()
    candidates = [Path(sys.executable)]
    if os.name == "nt":
        candidates.append(root / ".venv" / "Scripts" / "python.exe")
    else:
        candidates.append(root / ".venv" / "bin" / "python")

    seen: set[Path] = set()
    expected_version = sys.version_info[:2]
    for candidate in candidates:
        resolved = candidate.resolve()
        if resolved in seen or not resolved.is_file():
            continue
        seen.add(resolved)
        try:
            result = subprocess.run(
                [str(resolved), "-c", "import sys, numpy; print(sys.version_info[:2])"],
                check=False,
                capture_output=True,
                text=True,
                timeout=15,
            )
        except (OSError, subprocess.TimeoutExpired):
            continue
        if result.returncode == 0 and str(expected_version) in result.stdout:
            if resolved != Path(sys.executable).resolve():
                print(f"[python] Current interpreter has no numpy; using {resolved}", flush=True)
            return str(resolved)

    raise RuntimeError(
        "the Python environment used to launch Endstone cannot import numpy; "
        "run this script with the project .venv or install the project dependencies first"
    )


def provision_lifecycle_fixture_plugin(python_executable: str) -> PluginProvisioning:
    """Build and install a minimal plugin used by the lifecycle event scenario."""
    source = LIFECYCLE_FIXTURE_DIR
    if not source.is_dir():
        raise RuntimeError(f"plugin lifecycle fixture source is missing: {source}")

    build_directory = Path(tempfile.mkdtemp(prefix="endstone_lifecycle_fixture_"))
    python_directory = Path(tempfile.mkdtemp(prefix="endstone_lifecycle_plugin_"))
    try:
        result = subprocess.run(
            [
                python_executable,
                "-m",
                "pip",
                "wheel",
                str(source),
                "--no-deps",
                "--wheel-dir",
                str(build_directory),
                "--quiet",
            ],
            check=False,
            capture_output=True,
            text=True,
        )
        if result.returncode != 0:
            detail = (result.stderr or result.stdout).strip()
            raise RuntimeError(f"could not build plugin lifecycle fixture: {detail}")

        wheels = list(build_directory.glob("*.whl"))
        if len(wheels) != 1:
            raise RuntimeError(f"expected one plugin lifecycle fixture wheel, found {len(wheels)} in {build_directory}")

        with zipfile.ZipFile(wheels[0]) as archive:
            archive.extractall(python_directory)
        print(f"[plugin] Installed plugin lifecycle fixture: {python_directory}", flush=True)
        return PluginProvisioning(python_directory, build_directory)
    except Exception:
        shutil.rmtree(python_directory, ignore_errors=True)
        shutil.rmtree(build_directory, ignore_errors=True)
        raise
