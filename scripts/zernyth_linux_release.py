"""Package a matched Ubuntu 24.04 / CPython 3.12 build without BDS or a JDK."""

import argparse
import gzip
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import tarfile
import tempfile

import zernyth_release as release


ROOT = Path(__file__).resolve().parents[1]


def digest(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def check_native_paths(stage, prefixes):
    """Reject private build roots embedded in ELF payloads without rewriting binaries."""
    needles = [str(path).encode() for path in prefixes if path]
    checked = 0
    for path in stage.rglob("*"):
        if not path.is_file():
            continue
        with path.open("rb") as stream:
            if stream.read(4) != b"\x7fELF":
                continue
            stream.seek(0)
            data = stream.read()
        if any(prefix in data for prefix in needles):
            raise ValueError("Private build path in native payload: " + path.relative_to(stage).as_posix())
        checked += 1
    return checked


def check_shared_linkage(stage, readelf):
    """Require the replaceable LGPL dependencies in the runtime's actual ELF linkage."""
    runtime = stage / "lib/python/endstone/libendstone_runtime.so"
    output = subprocess.check_output([readelf, "-dW", str(runtime)], text=True, timeout=30)
    for library in ("libelf", "libdwarf"):
        if not re.search(r"\(NEEDED\).*\[" + library + r"\.so(?:\.[^\]]+)?\]", output):
            raise ValueError("Runtime is missing the shared dependency: " + library)


def safe_member(name):
    """Accept portable relative archive paths with a single installation root."""
    if "\\" in name or name.startswith("/") or any(
            part in ("", ".", "..") for part in name.split("/")) or ":" in name:
        raise ValueError("Unsafe archive member")


def verify_archive(path):
    """Verify bounded regular-file payloads and their complete checksum inventory."""
    with tarfile.open(path, "r:gz") as archive:
        members = []
        total = 0
        for item in archive:
            total += item.size
            if len(members) >= 30000 or item.size < 0 or total > 1024**3:
                raise ValueError("Archive resource limit")
            members.append(item)
        names = [item.name for item in members]
        if len(names) != len(set(names)):
            raise ValueError("Duplicate archive member")
        for item in members:
            safe_member(item.name)
            if not item.isfile():
                raise ValueError("Archive contains a non-regular file")
        roots = {name.split("/")[0] for name in names}
        if len(roots) != 1:
            raise ValueError("Expected one installation root")
        prefix = roots.pop() + "/"
        manifest_entry = archive.getmember(prefix + "manifest.json")
        if manifest_entry.size > 8 * 1024**2:
            raise ValueError("Oversized manifest")
        manifest = json.load(archive.extractfile(manifest_entry))
        if set(names) != {prefix + name for name in manifest} | {prefix + "manifest.json"}:
            raise ValueError("Incomplete archive inventory")
        required = {"BUILD.json", "install.sh", "start.sh", "bin/run.py", "requirements.lock"}
        if not required.issubset(manifest):
            raise ValueError("Missing launcher payload")
        for relative, expected in manifest.items():
            safe_member(relative)
            with archive.extractfile(prefix + relative) as stream:
                if hashlib.file_digest(stream, "sha256").hexdigest() != expected:
                    raise ValueError("Checksum mismatch: " + relative)
        forbidden = ("bedrock_server", "libmimalloc.so", "AGENTS.md", "CLAUDE.md")
        if any(name.rsplit("/", 1)[-1] in forbidden for name in names):
            raise ValueError("Forbidden distribution payload")
    return {"status": "PASS", "files": len(names), "sha256": digest(path)}


def conan_path(conan, reference, folder=None):
    command = [conan, "cache", "path", reference]
    if folder:
        command += ["--folder", folder]
    # The checkout's .conanrc would override the explicitly supplied CONAN_HOME.
    return Path(subprocess.check_output(command, cwd=ROOT.parent, text=True).strip())


def write_archive(stage, output):
    """Publish a verified deterministic archive atomically, preserving existing releases."""
    with tempfile.NamedTemporaryFile(dir=output.parent, prefix=".zernyth-", delete=False) as stream:
        temporary = Path(stream.name)
    try:
        with temporary.open("wb") as raw, gzip.GzipFile(
                filename="", fileobj=raw, mode="wb", mtime=0) as compressed:
            with tarfile.open(fileobj=compressed, mode="w", format=tarfile.PAX_FORMAT) as archive:
                for path in sorted(stage.rglob("*")):
                    if not path.is_file():
                        continue
                    name = stage.name + "/" + path.relative_to(stage).as_posix()
                    info = archive.gettarinfo(str(path), arcname=name)
                    info.uid = info.gid = 0
                    info.uname = info.gname = ""
                    info.mtime = 0
                    executable = path.stat().st_mode & 0o111 or path.suffix == ".sh"
                    info.mode = 0o755 if executable else 0o644
                    with path.open("rb") as source:
                        archive.addfile(info, source)
        result = verify_archive(temporary)
        # Both files are on the output filesystem. link() fails if a release already exists.
        os.link(temporary, output)
        return result
    finally:
        temporary.unlink(missing_ok=True)


def dependencies(graph, conan):
    """Resolve reviewed package revisions from the existing cache without downloads."""
    nodes = []
    for node in graph["graph"]["nodes"].values():
        if node.get("context") == "build":
            node["package_folder"] = None
            continue
        if not node.get("ref") or not node.get("prev") or not node.get("package_id"):
            continue
        reference = f"{node['ref']}:{node['package_id']}#{node['prev']}"
        node["package_folder"] = str(conan_path(conan, reference))
        nodes.append(node)
    for library in ("libelf/", "libdwarf/"):
        node = next(item for item in nodes if item["ref"].startswith(library))
        if str(node["options"].get("shared")).lower() != "true":
            raise ValueError("Linux distribution requires shared " + library)
    return nodes


def system_libraries(stage, system_root):
    """Copy matched ABI libraries and their full installed-package license notices."""
    llvm = system_root / "usr/lib/llvm-20/lib"
    paths = [llvm / name for name in ("libc++.so.1", "libc++abi.so.1", "libunwind.so.1")]
    paths.append(system_root / "usr/lib/x86_64-linux-gnu/libpython3.12.so.1.0")
    for path in paths:
        shutil.copyfile(path.resolve(strict=True), stage / "lib" / path.name)
    for package in ("libc++1-20", "libc++abi1-20", "libunwind-20", "libpython3.12t64"):
        notice = system_root / "usr/share/doc" / package / "copyright"
        if not notice.is_file():
            raise ValueError("Missing system-library license notice: " + package)
        destination = stage / "LICENSES/system" / package
        destination.mkdir(parents=True)
        shutil.copy2(notice, destination / "copyright")
    shutil.copytree(Path("/usr/share/common-licenses"), stage / "LICENSES/system/common-licenses")


def package(args):
    metadata = release.check_artifacts(args.build_dir)
    if metadata["platform"] != "Linux" or not metadata["pythonVersion"].startswith("3.12."):
        raise ValueError("Expected a matched Linux CPython 3.12 build")
    if metadata.get("sanitizers"):
        raise ValueError("Instrumented artifacts cannot become release packages")
    graph = json.loads(args.graph.read_text())
    nodes = dependencies(graph, args.conan)
    version = release.version()
    name = f"zernyth-engine-{version}-ubuntu24-x86_64-cp312"
    args.output.mkdir(parents=True, exist_ok=True)
    output = args.output / (name + ".tar.gz")
    if output.exists():
        raise ValueError("Refusing to replace an existing release archive")
    with tempfile.TemporaryDirectory(prefix="zernyth-linux-") as temporary:
        stage = Path(temporary) / name
        for folder in ("lib/python", "java", "bin", "LICENSES", "sources", "docs"):
            (stage / folder).mkdir(parents=True, exist_ok=True)
        subprocess.run([args.cmake, "--install", str(args.build_dir), "--prefix",
                        str(stage / "lib/python"), "--component", "endstone_wheel"], check=True)
        release_backend = ROOT / "packaging/zernyth_build.py"
        spec = importlib.util.spec_from_file_location("zernyth_archive_backend", release_backend)
        backend = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(backend)
        backend.copy_sources(ROOT / "endstone", stage / "lib/python/endstone")
        (stage / "lib/python/endstone/_version.py").write_text(
            "__version__ = " + repr(metadata["endstoneBase"]) + "\n")
        for path in list((stage / "lib/python").rglob("*.dbg")) + list(
                (stage / "lib/python").rglob("libmimalloc.so")):
            path.unlink()
        for module in ("api", "runtime"):
            shutil.copy2(release.java_artifacts()[module], stage / "java" / f"{module}-{version}.jar")
        for node in nodes:
            if node["ref"].startswith(("libelf/", "libdwarf/")):
                package_folder = Path(node["package_folder"])
                for path in (package_folder / "lib").glob("*.so*"):
                    shutil.copyfile(path.resolve(strict=True), stage / "lib" / path.name)
                source = conan_path(args.conan, node["ref"], "source")
                key = node["ref"].split("#")[0].replace("/", "-")
                shutil.copytree(source, stage / "sources" / key, symlinks=False)
                recipe = conan_path(args.conan, node["ref"])
                shutil.copytree(recipe, stage / "sources" / (key + "-recipe"), symlinks=False)
                recipe_sources = conan_path(args.conan, node["ref"], "export_source")
                if recipe_sources.is_dir():
                    shutil.copytree(recipe_sources, stage / "sources" / (key + "-recipe-sources"),
                                    symlinks=False)
        with tempfile.TemporaryDirectory(prefix="zernyth-license-audit-") as audit:
            audit = Path(audit)
            (audit / "endstone/fonts").mkdir(parents=True)
            backend.licenses(audit, graph)
            shutil.copytree(audit / "zernyth/licenses", stage / "LICENSES/dependencies")
            shutil.copy2(audit / "endstone/fonts/OFL.txt", stage / "lib/python/endstone/fonts/OFL.txt")
        system_libraries(stage, args.system_root)
        shutil.copytree(args.wheelhouse, stage / "wheelhouse")
        shutil.copy2(args.wheelhouse / "requirements.lock", stage / "requirements.lock")
        for file in ("install.sh", "start.sh"):
            (stage / file).write_text((ROOT / "packaging/linux" / file).read_text(), newline="\n")
        shutil.copy2(ROOT / "packaging/linux/run.py", stage / "bin/run.py")
        shutil.copy2(ROOT / "packaging/linux/README.md", stage / "README.md")
        for file in ("LICENSE", "THIRD_PARTY_NOTICES.md"):
            shutil.copy2(ROOT / file, stage / "LICENSES" / file)
        shutil.copy2(ROOT / "docs/zernyth-stable-1.0.md", stage / "docs/release-notes.md")
        shutil.copy2(ROOT / "config/conan/linux-release.profile", stage / "sources/zernyth-linux-release.profile")
        shutil.copy2(ROOT / "config/conan/hooks/hook_zernyth_build_metadata.py",
                     stage / "sources/hook_zernyth_build_metadata.py")
        shutil.copy2(args.lockfile, stage / "sources/DEPENDENCIES.lock")
        native_count = check_native_paths(stage, (ROOT, os.environ.get("CONAN_HOME"), args.private_build_root))
        check_shared_linkage(stage, args.readelf)
        metadata.update(bds="1.26.60.6", bdsSha256=args.bds_sha256, allocator="system",
                        distribution="Ubuntu 24.04 x86_64 archive", librariesReplaceable=True,
                        nativePathPrivacy="PASS", nativeFilesChecked=native_count)
        (stage / "BUILD.json").write_text(json.dumps(metadata, indent=2) + "\n")
        # All inputs have been explicitly selected. Full build caches and server files stay outside.
        files = sorted(path for path in stage.rglob("*") if path.is_file())
        manifest = {path.relative_to(stage).as_posix(): digest(path) for path in files}
        (stage / "manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
        result = write_archive(stage, output)
    output.with_suffix(output.suffix + ".sha256").write_text(result["sha256"] + "  " + output.name + "\n")
    print(json.dumps(result, indent=2))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build-dir", type=Path, required=True)
    parser.add_argument("--graph", type=Path, required=True)
    parser.add_argument("--lockfile", type=Path, default=ROOT / "config/zernyth-linux-conan.lock")
    parser.add_argument("--wheelhouse", type=Path, required=True)
    parser.add_argument("--system-root", type=Path, default=Path("/"))
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--bds-sha256", required=True)
    parser.add_argument("--private-build-root", type=Path, help="Additional root rejected in ELF payloads")
    parser.add_argument("--cmake", default="cmake")
    parser.add_argument("--conan", default="conan")
    parser.add_argument("--readelf", default="readelf", help="Installed GNU or LLVM readelf")
    args = parser.parse_args()
    if not re.fullmatch("[0-9a-f]{64}", args.bds_sha256):
        parser.error("An audited BDS executable SHA-256 is required")
    package(args)


if __name__ == "__main__":
    main()
