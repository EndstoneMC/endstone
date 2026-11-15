"""Offline Linux archive installer and explicit existing-BDS launcher."""

import argparse
import hashlib
import json
import os
from pathlib import Path
import platform
import re
import shutil
import subprocess
import sys
import venv
import zipfile


ROOT = Path(__file__).resolve().parents[1]


def metadata():
    """Reject a foreign platform before importing native modules."""
    data = json.loads((ROOT / "BUILD.json").read_text(encoding="utf-8"))
    if platform.system() != "Linux" or platform.machine() != "x86_64":
        raise RuntimeError("This archive requires Linux x86_64")
    if sys.version_info[:2] != (3, 12):
        raise RuntimeError("This archive requires CPython 3.12")
    return data


def verify_files(paths):
    """Verify selected release artifacts without restricting replacement shared libraries."""
    manifest = json.loads((ROOT / "manifest.json").read_text(encoding="utf-8"))
    for relative in paths:
        path = (ROOT / relative).resolve()
        if not path.is_relative_to(ROOT) or not path.is_file():
            raise RuntimeError("Missing or unsafe release file: " + relative)
        with path.open("rb") as stream:
            digest = hashlib.file_digest(stream, "sha256").hexdigest()
        if digest != manifest[relative]:
            raise RuntimeError("Release checksum mismatch: " + relative)


def install():
    """Create a fresh local environment from the bundled dependency wheels."""
    metadata()
    files = json.loads((ROOT / "manifest.json").read_text(encoding="utf-8"))
    verify_files(files)
    environment = ROOT / ".venv"
    if environment.exists():
        raise RuntimeError("An environment already exists. Extract updates into a new directory.")
    pip_wheels = list((ROOT / "wheelhouse").glob("pip-*.whl"))
    if len(pip_wheels) != 1:
        raise RuntimeError("Expected one bundled pip bootstrap wheel")
    # Atomic reservation prevents two installers from sharing a partially created environment.
    environment.mkdir()
    try:
        venv.EnvBuilder(with_pip=False).create(environment)
        python = environment / "bin/python"
        env = os.environ.copy()
        env.update(PYTHONPATH=str(pip_wheels[0]), PYTHONNOUSERSITE="1",
                   PIP_NO_INDEX="1", PIP_DISABLE_PIP_VERSION_CHECK="1")
        subprocess.run([str(python), "-m", "pip", "install", "--no-index", "--find-links",
                        str(ROOT / "wheelhouse"), "--require-hashes", "-r",
                        str(ROOT / "requirements.lock")], env=env, check=True, timeout=180)
        env.pop("PYTHONPATH", None)
        subprocess.run([str(python), "-m", "pip", "check"], env=env, check=True, timeout=30)
        (environment / ".zernyth-install-complete").write_text("1.0.0\n")
    except BaseException:
        shutil.rmtree(environment)
        raise
    print("PASS offline installation. Use ./start.sh --server PATH --java-home PATH to inspect.")


def inspect(args, data):
    """Check matching BDS, JDK and native artifacts without starting BDS."""
    files = json.loads((ROOT / "manifest.json").read_text(encoding="utf-8"))
    verify_files(name for name in files if name.startswith(("lib/python/", "java/")))
    binary = args.server.resolve() / "bedrock_server"
    if not binary.is_file():
        raise RuntimeError("Select an existing BDS 1.26.60.6 directory")
    with binary.open("rb") as stream:
        digest = hashlib.file_digest(stream, "sha256").hexdigest()
    if digest != data["bdsSha256"]:
        raise RuntimeError("The BDS binary differs from the validated Linux 1.26.60.6 build")
    home = args.java_home.resolve()
    probe_env = os.environ.copy()
    for name in ("JAVA_TOOL_OPTIONS", "JDK_JAVA_OPTIONS", "_JAVA_OPTIONS"):
        probe_env.pop(name, None)
    probe = subprocess.run([str(home / "bin/java"), "-version"], env=probe_env,
                           capture_output=True, text=True, timeout=15, check=True)
    if not re.search(r'version "25[.\"]', probe.stdout + probe.stderr):
        raise RuntimeError("Use an installed Java 25 JDK")
    if not (home / "lib/server/libjvm.so").is_file():
        raise RuntimeError("The selected JDK is missing lib/server/libjvm.so")
    for module in ("api", "runtime"):
        jar = ROOT / "java" / f"{module}-{data['zernyth']}.jar"
        with zipfile.ZipFile(jar) as archive:
            text = archive.read("META-INF/MANIFEST.MF").decode("utf-8")
        if f"Zernyth-Engine-Version: {data['zernyth']}" not in text:
            raise RuntimeError("Mismatched Java artifact: " + module)
    # Native loading needs the bundled ABI libraries before Python itself starts.
    env = os.environ.copy()
    env["PYTHONPATH"] = str(ROOT / "lib/python")
    env["LD_LIBRARY_PATH"] = str(ROOT / "lib") + ":" + env.get("LD_LIBRARY_PATH", "")
    subprocess.run([sys.executable, "-c", "import endstone._python; print('PASS native Python import')"],
                   cwd=ROOT, env=env, check=True, timeout=30)
    print(f"PASS Zernyth {data['zernyth']}, BDS {data['bds']}, Java 25, CPython 3.12")


def run_server(args, data):
    """Reuse Endstone's Linux launcher with the validated allocator and library search path."""
    library_path = str(ROOT / "lib") + ":" + os.environ.get("LD_LIBRARY_PATH", "")
    python_paths = [str(ROOT / "lib/python"), *sys.path]
    os.environ.update(JAVA_HOME=str(args.java_home.resolve()), ZERNYTH_JAVA_ENABLED="1",
                      ZERNYTH_JVM_LIBRARY=str(args.java_home.resolve() / "lib/server/libjvm.so"),
                      ZERNYTH_JAVA_CLASSPATH=":".join(
                          str(ROOT / f"java/{module}-{data['zernyth']}.jar")
                          for module in ("api", "runtime")))
    sys.path.insert(0, str(ROOT / "lib/python"))
    from endstone.cli.linux import LinuxBootstrap

    class ArchiveBootstrap(LinuxBootstrap):
        @property
        def _endstone_runtime_env(self):
            env = super()._endstone_runtime_env
            env["LD_LIBRARY_PATH"] = library_path
            env["PYTHONPATH"] = ":".join(python_paths)
            env["LD_PRELOAD"] = "libc.so.6:" + str(self._endstone_runtime_path)
            return env

    args.server.joinpath("plugins").mkdir(exist_ok=True)
    backend = ArchiveBootstrap(str(args.server.resolve()), False, "", False)
    backend._validate()
    return backend._run()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--install", action="store_true")
    parser.add_argument("--server", type=Path)
    parser.add_argument("--java-home", type=Path)
    parser.add_argument("--run", action="store_true")
    args = parser.parse_args()
    data = metadata()
    if args.install:
        if args.run or args.server or args.java_home:
            parser.error("--install creates the environment independently of server startup")
        install()
        return 0
    if not args.server or not args.java_home:
        parser.error("--server and --java-home are required")
    inspect(args, data)
    if args.run:
        return run_server(args, data)
    print("Inspection only. Append --run for an operator-controlled server start.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
