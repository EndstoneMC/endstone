"""Validate and install-test first-party wheels without downloads or server starts."""

import argparse
import base64
import csv
from email.parser import BytesParser
import hashlib
import io
import json
import os
from pathlib import Path, PurePosixPath
import re
import subprocess
import sys
import tempfile
import venv
import zipfile

from packaging.metadata import Metadata
from packaging.version import Version
from markdown_it import MarkdownIt


ROOT = Path(__file__).resolve().parents[1]
ENGINE_VERSION = (ROOT / "zernyth-version.txt").read_text(encoding="utf-8").strip()
DISTRIBUTION_VERSION = str(Version(ENGINE_VERSION))


def developer_path_count(data):
    """Count private Windows profile prefixes without disclosing the path strings."""
    pattern = rb"(?i)[a-z]:[\\/]+users[\\/]+"
    wide = rb"(?i)[a-z]\x00:\x00(?:[\\/]\x00)+u\x00s\x00e\x00r\x00s\x00(?:[\\/]\x00)+"
    return len(re.findall(pattern, data)) + len(re.findall(wide, data))


def validate(path, require_sanitized=False):
    with zipfile.ZipFile(path) as wheel:
        names = wheel.namelist()
        if len(names) != len(set(names)):
            raise ValueError("Duplicate wheel members")
        for name in names:
            item = PurePosixPath(name)
            if item.is_absolute() or ".." in item.parts or "\\" in name or ":" in name:
                raise ValueError("Unsafe wheel member")
            if item.name in ("AGENTS.md", "CLAUDE.md", "bedrock_server.exe", ".env") or item.suffix in (".pdb", ".pyc"):
                raise ValueError("Private/server/build artifact in wheel")
        infos = [n.rsplit("/", 1)[0] for n in names if n.endswith(".dist-info/METADATA")]
        if len(infos) != 1:
            raise ValueError("Exactly one distribution metadata directory required")
        info = infos[0]
        metadata = BytesParser().parsebytes(wheel.read(info + "/METADATA"))
        core_metadata = Metadata.from_email(wheel.read(info + "/METADATA"), validate=True)
        if core_metadata.description_content_type != "text/markdown" or not core_metadata.description:
            raise ValueError("A nonempty Markdown package description is required")
        if not MarkdownIt("commonmark").render(core_metadata.description).strip():
            raise ValueError("Package description did not render")
        if metadata["Name"] != "zernyth-engine" or metadata["Version"] != DISTRIBUTION_VERSION:
            raise ValueError("Incorrect distribution identity")
        if "zernyth = zernyth.cli:main" not in wheel.read(info + "/entry_points.txt").decode():
            raise ValueError("Missing Zernyth CLI")
        if "win_amd64" not in wheel.read(info + "/WHEEL").decode():
            raise ValueError("Only Windows x64 is enabled")
        records = list(csv.reader(io.StringIO(wheel.read(info + "/RECORD").decode())))
        if len(records) != len(names) or {r[0] for r in records} != set(names):
            raise ValueError("Incomplete wheel RECORD")
        for name, digest, size in records:
            if name == info + "/RECORD":
                if digest or size:
                    raise ValueError("RECORD must not hash itself")
                continue
            data = wheel.read(name)
            expected = "sha256=" + base64.urlsafe_b64encode(hashlib.sha256(data).digest()).rstrip(b"=").decode()
            if digest != expected or size != str(len(data)):
                raise ValueError("Wheel RECORD checksum mismatch")
        required = ["endstone/__init__.py", "endstone/config/endstone.default.toml", "endstone/fonts/OFL.txt", "zernyth/cli.py",
                    "zernyth/bds.py", "zernyth/BUILD.json", info + "/licenses/LICENSE",
                    info + "/licenses/THIRD_PARTY_NOTICES.md", "zernyth/licenses/native-inventory.json"]
        build = json.loads(wheel.read("zernyth/BUILD.json"))
        if build["javaEnabled"] != "ON" or build["zernyth"] != ENGINE_VERSION:
            raise ValueError("Native version or JNI configuration mismatch")
        for dependency in ("gson-2.14.0", "snakeyaml-2.4"):
            license_path = "zernyth/licenses/java/" + dependency + "/LICENSE"
            required.append(license_path)
            if b"TERMS AND CONDITIONS" not in wheel.read(license_path):
                raise ValueError("Full dependency license text missing")
        notices = "zernyth/licenses/sentry-source-notices/"
        source_inventory = json.loads(wheel.read(notices + "inventory.json"))
        for entry in source_inventory["files"]:
            if hashlib.sha256(wheel.read(notices + entry["path"])).hexdigest() != entry["sha256"]:
                raise ValueError("Dependency source notice checksum mismatch")
        for name in required:
            if name not in names:
                raise ValueError("Missing wheel payload: " + name)
        for module in ("api", "runtime"):
            name = f"zernyth/java/{module}-{build['zernyth']}.jar"
            with zipfile.ZipFile(io.BytesIO(wheel.read(name))) as jar:
                manifest = jar.read("META-INF/MANIFEST.MF").decode()
                if "Zernyth-Engine-Version: " + build["zernyth"] not in manifest:
                    raise ValueError("Java artifact version mismatch")
                cls = "dev/zernyth/api/Server.class" if module == "api" else "dev/zernyth/runtime/Bootstrap.class"
                header = jar.read(cls)[:8]
                if header[:4] != b"\xca\xfe\xba\xbe" or int.from_bytes(header[6:8], "big") != 69:
                    raise ValueError("Java 25 bytecode required")
        for prefix in ("endstone/_python.", "endstone/_detours."):
            if len([n for n in names if n.startswith(prefix) and n.endswith(".pyd")]) != 1:
                raise ValueError("Missing native extension")
        private_paths = {}
        for name in names:
            if name.endswith((".dll", ".exe", ".pyd")):
                count = developer_path_count(wheel.read(name))
                if count:
                    private_paths[name] = count
        if require_sanitized and private_paths:
            raise ValueError("Release blocked: native binaries contain private Windows profile paths; rebuild with sanitized toolchains/caches")
        for basename in ("endstone_runtime.dll", "endstone_runtime_loader.dll", "crashpad_handler.exe", "crashpad_wer.dll"):
            data = wheel.read("endstone/" + basename)
            offset = int.from_bytes(data[60:64], "little")
            if data[:2] != b"MZ" or data[offset:offset+4] != b"PE\0\0" or data[offset+4:offset+6] != b"\x64\x86":
                raise ValueError("Invalid native x64 PE image")
    with path.open("rb") as stream:
        digest = hashlib.file_digest(stream, "sha256").hexdigest()
    return {"status": "PASS", "wheel": path.name, "members": len(names), "sha256": digest,
            "developerPathPrivacy": "BLOCKED" if private_paths else "PASS", "nativePathFindings": private_paths}


def install_test(wheel, wheelhouse, api_tests=False):
    if not wheelhouse or not wheelhouse.is_dir():
        raise ValueError("Offline installation requires a preprovisioned dependency wheelhouse")
    with tempfile.TemporaryDirectory(prefix="zernyth-install-") as temporary:
        directory = Path(temporary)
        venv.EnvBuilder(with_pip=True).create(directory / "venv")
        python = directory / "venv/Scripts/python.exe"
        env = os.environ.copy()
        env.pop("PYTHONPATH", None)
        env.update(PIP_NO_INDEX="1", PIP_DISABLE_PIP_VERSION_CHECK="1", PYTHONNOUSERSITE="1")
        def run(args):
            result = subprocess.run([str(python), *args], cwd=directory, env=env,
                                    timeout=180, capture_output=True, text=True)
            if result.returncode:
                raise RuntimeError("Isolated installation check failed:\n" + result.stdout + result.stderr)
            return result
        run(["-m", "pip", "install", "--no-index", "--find-links", str(wheelhouse.resolve()), str(wheel.resolve())])
        run(["-m", "pip", "check"])
        run(["-I", "-c", "import endstone._python, endstone._detours; "
             f"from importlib.metadata import version; assert version('zernyth-engine') == {DISTRIBUTION_VERSION!r}; "
             "import endstone; assert endstone.__version__ == '0.11.15'; "
             "import zernyth.cli; zernyth.cli.reject_conflicting_distribution()"])
        run(["-I", "-c", "import os; from zernyth.cli import _java_runtime; "
             "assert _java_runtime(os.environ['JAVA_HOME']).is_file()"])
        executable = directory / "venv/Scripts/zernyth.exe"
        for option in ("--version", "--help"):
            result = subprocess.run([str(executable), option], cwd=directory, env=env,
                                    timeout=30, check=True, capture_output=True, text=True)
            if option == "--version" and DISTRIBUTION_VERSION not in result.stdout:
                raise ValueError("CLI version mismatch")
        if api_tests:
            run(["-m", "pip", "install", "--no-index", "--find-links", str(wheelhouse.resolve()), "pytest"])
            configuration = directory / "pytest.ini"
            configuration.write_text("[pytest]\n", encoding="utf-8")
            files = sorted(str(p) for p in (ROOT / "tests").glob("test_*.py")
                           if not p.name.startswith("test_zernyth_"))
            result = run(["-m", "pytest", "--assert=plain", "--import-mode=importlib", "-c",
                          str(configuration), "-q", *files])
            if "105 passed" not in result.stdout:
                raise ValueError("The packaged Python API suite did not report all 105 cases")
    return {"installation": "PASS", "nativeImport": "PASS", "cli": "PASS", "pipCheck": "PASS",
            "java25Detection": "PASS", "pythonApiTests": 105 if api_tests else "NOT TESTED", "bds": "NOT TESTED"}


def release_ref():
    requested = os.environ.get("RELEASE_COMMIT", "")
    actual = subprocess.check_output(["git", "rev-parse", "HEAD"], text=True).strip()
    if not re.fullmatch(r"[0-9a-fA-F]{40}", requested) or requested.lower() != actual:
        raise ValueError("Exact reviewed release SHA required")
    if os.environ.get("RELEASE_TARGET") == "pypi":
        tagged = subprocess.check_output(["git", "rev-parse", f"refs/tags/v{ENGINE_VERSION}^{{commit}}"], text=True).strip()
        if tagged != actual:
            raise ValueError(f"Production requires the operator-created v{ENGINE_VERSION} tag at this commit")
    target = os.environ.get("RELEASE_TARGET", "build-only")
    if target not in ("build-only", "testpypi", "pypi"):
        raise ValueError("Unknown release target")
    print("Release commit verified")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("validate", "install-test", "release-ref"))
    parser.add_argument("--wheel-dir", type=Path, default=Path("dist"))
    parser.add_argument("--wheelhouse", type=Path)
    parser.add_argument("--require-sanitized", action="store_true", help="Fail release validation on embedded private profile paths")
    parser.add_argument("--api-tests", action="store_true", help="Run the packaged Python API suite using the offline pytest wheels")
    args = parser.parse_args()
    if args.command == "release-ref":
        release_ref()
        return
    wheels = list(args.wheel_dir.glob("*.whl"))
    if len(wheels) != 1:
        raise ValueError("Exactly one release wheel required in the output directory")
    print(json.dumps(validate(wheels[0], args.require_sanitized), indent=2))
    if args.command == "install-test":
        print(json.dumps(install_test(wheels[0], args.wheelhouse, args.api_tests), indent=2))


if __name__ == "__main__":
    main()
