"""Offline PEP 517 backend for matching Windows native and Java artifacts.

By default the existing release driver builds all artifacts. An explicit
native-build-dir setting packages an already built, matching set without rebuilding.
No network resolution, server start, upload, or source-control mutation occurs here.
"""

import base64
import csv
from email.message import EmailMessage
from email import policy
import hashlib
import io
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import sysconfig
import tarfile
import tempfile
import tomllib
import zipfile

from packaging.tags import sys_tags
from packaging.version import Version

ROOT = Path(__file__).resolve().parents[1]
DIST = "zernyth_engine"


def project():
    data = tomllib.loads((ROOT / "pyproject.toml").read_text(encoding="utf-8"))["project"]
    native = (ROOT / "zernyth-version.txt").read_text(encoding="utf-8").strip()
    if data["name"] != "zernyth-engine" or Version(native) != Version(data["version"]):
        raise ValueError("Python and native/Java release versions must agree")
    return data


def metadata_bytes():
    data = project()
    message = EmailMessage(policy=policy.default.clone(max_line_length=0, utf8=True))
    for key, value in (("Metadata-Version", "2.4"), ("Name", data["name"]),
                       ("Version", data["version"]), ("Summary", data["description"]),
                       ("Requires-Python", data["requires-python"]),
                       ("Description-Content-Type", "text/markdown"), ("License-Expression", "Apache-2.0")):
        message[key] = value
    for author in data.get("authors", []):
        message["Author-email"] = f"{author['name']} <{author['email']}>"
    for requirement in data["dependencies"]:
        message["Requires-Dist"] = requirement
    for classifier in data.get("classifiers", []):
        message["Classifier"] = classifier
    for key, url in data["urls"].items():
        message["Project-URL"] = f"{key}, {url}"
    for name in ("LICENSE", "THIRD_PARTY_NOTICES.md"):
        message["License-File"] = name
    # Core Metadata uses a UTF-8 body, not an ASCII/MIME-encoded email payload.
    return message.as_bytes() + (ROOT / data["readme"]).read_text(encoding="utf-8-sig").encode("utf-8")


def write_metadata(directory):
    info = directory / f"{DIST}-{project()['version']}.dist-info"
    info.mkdir(parents=True, exist_ok=True)
    (info / "METADATA").write_bytes(metadata_bytes())
    (info / "entry_points.txt").write_text("[console_scripts]\nzernyth = zernyth.cli:main\n", encoding="utf-8")
    (info / "licenses").mkdir(exist_ok=True)
    for name in ("LICENSE", "THIRD_PARTY_NOTICES.md"):
        shutil.copy2(ROOT / name, info / "licenses" / name)
    return info


def get_requires_for_build_wheel(config_settings=None):
    return []


def prepare_metadata_for_build_wheel(metadata_directory, config_settings=None):
    return write_metadata(Path(metadata_directory)).name


def copy_sources(source, destination):
    for path in sorted(source.rglob("*")):
        relative = path.relative_to(source)
        if any(part in ("__pycache__", "build", ".gradle", ".venv") for part in relative.parts):
            continue
        if path.is_symlink():
            raise ValueError("Symlinks are not accepted in wheel inputs")
        if path.is_file() and path.suffix not in (".pyc", ".pyo"):
            target = destination / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(path, target)


def licenses(stage, graph):
    destination = stage / "zernyth/licenses"
    destination.mkdir(parents=True)
    inventory = []
    for node in graph["graph"]["nodes"].values():
        if not node.get("ref") or not node.get("package_folder"):
            continue
        reference = node["ref"]
        source = Path(node["package_folder"]) / "licenses"
        if reference.startswith("opengl/system#"):
            inventory.append({"reference": reference, "systemComponent": True, "bundled": False})
            continue
        if not source.is_dir() or not any(p.is_file() for p in source.rglob("*")):
            raise ValueError("Missing native dependency license: " + reference)
        key = re.sub(r"[^A-Za-z0-9_.-]", "_", reference.split("#")[0])
        copy_sources(source, destination / "native" / key)
        inventory.append({"reference": reference, "licenses": "native/" + key})
    if not inventory:
        raise ValueError("Native dependency license inventory is empty")
    for name in ("gson-2.14.0", "snakeyaml-2.4"):
        source = ROOT / "java/runtime/build/third-party-licenses" / name
        if not source.is_dir() or not list(source.rglob("pom.xml")):
            raise ValueError("Missing Java dependency license metadata: " + name)
        target = destination / "java" / name
        copy_sources(source, target)
        # Both dependency POMs declare Apache-2.0. Include its complete unmodified text,
        # as well as every notice actually present in the source dependency JAR.
        shutil.copy2(ROOT / "LICENSE", target / "LICENSE")
    shutil.copy2(ROOT / "THIRD_PARTY_NOTICES.md", destination / "THIRD_PARTY_NOTICES.md")
    shutil.copy2(ROOT / "packaging/licenses/JetBrainsMono-OFL.txt", stage / "endstone/fonts/OFL.txt")
    extras = ROOT / "packaging/licenses/sentry-native"
    manifest = json.loads((extras / "inventory.json").read_text(encoding="utf-8"))
    for entry in manifest["files"]:
        source = (extras / entry["path"]).resolve()
        if not source.is_relative_to(extras.resolve()) or source.is_symlink():
            raise ValueError("Unsafe dependency license manifest path")
        if hashlib.sha256(source.read_bytes()).hexdigest() != entry["sha256"]:
            raise ValueError("Dependency source license checksum mismatch")
        target = destination / "sentry-source-notices" / entry["path"]
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, target)
    shutil.copy2(extras / "inventory.json", destination / "sentry-source-notices/inventory.json")
    (destination / "native-inventory.json").write_text(json.dumps(inventory, indent=2), encoding="utf-8")


def build_wheel(wheel_directory, config_settings=None, metadata_directory=None):
    if sys.platform != "win32" or sysconfig.get_platform() != "win-amd64":
        raise RuntimeError("Only Windows x64 wheels are enabled; use the separate Ubuntu archive on Linux")
    settings = config_settings or {}
    if "native-build-dir" not in settings:
        command = [sys.executable, str(ROOT / "scripts/zernyth_release.py"), "build",
                   "--lockfile", str(ROOT / "config/zernyth-publish-conan.lock")]
        for flag, key in (("--gradle", "ZERNYTH_GRADLE"), ("--cmake", "ZERNYTH_CMAKE"),
                          ("--conan", "ZERNYTH_CONAN")):
            if os.environ.get(key):
                command.extend((flag, os.environ[key]))
        subprocess.run(command,
                       cwd=ROOT, check=True, timeout=3600)
    build = Path(settings.get("native-build-dir", ROOT / "build/RelWithDebInfo")).resolve()
    sys.path.insert(0, str(ROOT / "scripts"))
    import zernyth_release as release
    native = release.check_artifacts(build)
    link_map = build / "src/endstone/runtime/endstone_runtime.map"
    linked = link_map.read_text(encoding="utf-8", errors="replace")
    if "SymFromAddr" not in linked or re.search(r"(?i)libdwarf|\b_?dwarf_[A-Za-z0-9_]+\b", linked):
        raise ValueError("Windows distribution requires the audited DbgHelp backend without linked libdwarf objects")
    if native["platform"] != "Windows" or not native["pythonVersion"].startswith(
            f"{sys.version_info.major}.{sys.version_info.minor}."):
        raise ValueError("Native build platform/Python ABI differs from the wheel interpreter")
    graph = json.loads((ROOT / "build/release-013/dependency-graph.json").read_text(encoding="utf-8"))
    tag = next(sys_tags())
    version = project()["version"]
    filename = f"{DIST}-{version}-{tag}.whl"
    with tempfile.TemporaryDirectory(prefix="zernyth-wheel-") as directory:
        stage = Path(directory)
        copy_sources(ROOT / "endstone", stage / "endstone")
        copy_sources(ROOT / "zernyth", stage / "zernyth")
        # Keep native/Python plugin API identity separate from the PyPI distribution version.
        (stage / "endstone/_version.py").write_text(
            f"__version__ = {native['endstoneBase']!r}\n", encoding="utf-8")
        cmake = release.tool("cmake", os.environ.get("ZERNYTH_CMAKE"))
        release.run([cmake, "--install", build, "--prefix", stage, "--component", "endstone_wheel"])
        for path in (stage / "endstone").rglob("*.pdb"):
            path.unlink()
        required = ("endstone_runtime.dll", "endstone_runtime_loader.dll", "crashpad_handler.exe", "crashpad_wer.dll")
        for name in required:
            if not (stage / "endstone" / name).is_file():
                raise ValueError("Missing native runtime component: " + name)
        for name in ("_python", "_detours"):
            path = stage / "endstone" / (name + sysconfig.get_config_var("EXT_SUFFIX"))
            if not path.is_file():
                raise ValueError("Missing matching native extension: " + name)
        (stage / "zernyth/java").mkdir()
        for name in ("api", "runtime"):
            source = release.java_artifacts()[name]
            shutil.copy2(source, stage / "zernyth/java" / source.name)
        shutil.copy2(ROOT / "scripts/zernyth_bds.py", stage / "zernyth/bds.py")
        (stage / "zernyth/BUILD.json").write_text(json.dumps(native, sort_keys=True), encoding="utf-8")
        licenses(stage, graph)
        (stage / "zernyth/licenses/windows-link-audit.json").write_text(json.dumps({
            "symbolBackend": "DbgHelp", "libdwarfLinked": False,
            "evidence": "Compiler backend configuration and linker object map",
            "linkMapSha256": hashlib.sha256(link_map.read_bytes()).hexdigest(),
        }, sort_keys=True), encoding="utf-8")
        info = write_metadata(stage)
        (info / "WHEEL").write_text(
            f"Wheel-Version: 1.0\nGenerator: zernyth-build\nRoot-Is-Purelib: false\nTag: {tag}\n", encoding="utf-8")
        output = Path(wheel_directory).resolve()
        output.mkdir(parents=True, exist_ok=True)
        with zipfile.ZipFile(output / filename, "w", compression=zipfile.ZIP_DEFLATED, compresslevel=9) as archive:
            records = []
            for path in sorted(stage.rglob("*")):
                if not path.is_file():
                    continue
                relative = path.relative_to(stage).as_posix()
                data = path.read_bytes()
                entry = zipfile.ZipInfo(relative, (2020, 1, 1, 0, 0, 0))
                entry.compress_type = zipfile.ZIP_DEFLATED
                entry.external_attr = 0o100644 << 16
                archive.writestr(entry, data)
                digest = base64.urlsafe_b64encode(hashlib.sha256(data).digest()).rstrip(b"=").decode()
                records.append((relative, "sha256=" + digest, len(data)))
            record_name = info.name + "/RECORD"
            records.append((record_name, "", ""))
            record = io.StringIO(newline="")
            csv.writer(record, lineterminator="\n").writerows(records)
            entry = zipfile.ZipInfo(record_name, (2020, 1, 1, 0, 0, 0))
            entry.compress_type = zipfile.ZIP_DEFLATED
            archive.writestr(entry, record.getvalue())
    return filename


def build_sdist(sdist_directory, config_settings=None):
    raise RuntimeError("This release is wheel-only. Build the reviewed source checkout; no sdist is published.")
