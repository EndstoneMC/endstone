"""Explicit existing-BDS launcher. Inspection never starts or installs a server."""

import argparse
from importlib import metadata
import json
import os
from pathlib import Path
import re
import subprocess

from . import get_version


def reject_conflicting_distribution():
    """Refuse a shared environment whose endstone files may have been overwritten."""
    try:
        metadata.distribution("endstone")
    except metadata.PackageNotFoundError:
        return
    raise RuntimeError("endstone and zernyth-engine share files. Install Zernyth in a separate fresh venv.")


def _java_runtime(home):
    """Check an installed JDK 25 and its JVM library without starting BDS."""
    home = Path(home).resolve()
    env = os.environ.copy()
    for key in ("JAVA_TOOL_OPTIONS", "JDK_JAVA_OPTIONS", "_JAVA_OPTIONS"):
        env.pop(key, None)
    probe = subprocess.run([str(home / "bin/java.exe"), "-version"], capture_output=True,
                           text=True, timeout=15, check=True, env=env)
    if not re.search(r'version "25[.\"]', probe.stdout + probe.stderr):
        raise ValueError("JDK 25 is required")
    jvm = home / "bin/server/jvm.dll"
    if not jvm.is_file():
        raise ValueError("Missing JDK 25 JVM library")
    return jvm


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--version", action="version", version="Zernyth Engine " + get_version())
    parser.add_argument("--server", type=Path, required=True, help="Existing compatible BDS directory")
    parser.add_argument("--java-home", type=Path, default=os.environ.get("JAVA_HOME"))
    parser.add_argument("--run", action="store_true", help="Explicitly start BDS using the operator-managed network configuration")
    args = parser.parse_args(argv)
    reject_conflicting_distribution()
    if os.name != "nt":
        parser.error("This release wheel is supported only on Windows x64")
    if args.java_home is None:
        parser.error("Set JAVA_HOME or --java-home to an installed JDK 25")
    home = args.java_home.resolve()
    try:
        jvm = _java_runtime(home)
    except ValueError as error:
        parser.error(str(error))
    from .bds import inspect
    server = args.server.resolve()
    result = inspect(server)
    print(json.dumps(result, indent=2))
    if result["binaryInspection"] != "PASS":
        parser.error("BDS version compatibility not established")
    package = Path(__file__).resolve().parent
    build = json.loads((package / "BUILD.json").read_text(encoding="utf-8"))
    jars = [package / "java" / f"{name}-{build['zernyth']}.jar" for name in ("api", "runtime")]
    if not all(p.is_file() for p in jars):
        parser.error("Incomplete Java runtime installation")
    if not args.run:
        print("Inspection only; no BDS process started. Use --run to start with your operator-managed network configuration.")
        return
    os.environ.update(JAVA_HOME=str(home), ZERNYTH_JAVA_ENABLED="1",
                      ZERNYTH_JVM_LIBRARY=str(jvm),
                      ZERNYTH_JAVA_CLASSPATH=os.pathsep.join(map(str, jars)))
    from endstone.cli.windows import WindowsBootstrap

    class ExistingServer(WindowsBootstrap):
        def _add_loopback_exemption(self):
            # The upstream launch method calls this hook; Zernyth changes no OS policy.
            pass

    backend = ExistingServer(server_folder=str(server), no_confirm=False, remote="", interactive=False)
    backend._validate()
    raise SystemExit(backend._run())
