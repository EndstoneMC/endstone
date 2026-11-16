"""Write UTF-8 GitHub Actions path and digest outputs using preinstalled Python."""

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import urllib.request


def add_tool_paths(value, destination):
    directories = [Path(item) for item in value.split(os.pathsep) if item.strip()]
    for directory in directories:
        if not directory.is_dir():
            raise ValueError(f"Configured tool directory does not exist: {directory}")
    with destination.open("a", encoding="utf-8", newline="\n") as output:
        for directory in directories:
            output.write(str(directory) + "\n")


def write_wheel_digest(wheel, destination):
    with wheel.open("rb") as source:
        digest = hashlib.file_digest(source, "sha256").hexdigest()
    with destination.open("a", encoding="utf-8", newline="\n") as output:
        output.write("sha256=" + digest + "\n")


def verify_approved_run(run, commit, run_id):
    """Accept an already successful build-only run for the same reviewed source."""
    if not re.fullmatch(r"[0-9a-f]{40}", commit) or not re.fullmatch(r"[1-9][0-9]{0,19}", str(run_id)):
        raise ValueError("A full release SHA and numeric validated run ID are required")
    if (str(run.get("id")) != str(run_id) or run.get("head_sha") != commit
            or run.get("conclusion") != "success" or run.get("status") != "completed"
            or run.get("event") != "workflow_dispatch"
            or run.get("path") != ".github/workflows/publish.yml"
            or run.get("display_title") != f"Release wheel {commit} / build-only"):
        raise ValueError("Promotion requires a successful build-only run of this exact release commit")


def approved_run(run_id, commit):
    """Read run provenance from the repository's authenticated Actions API."""
    if os.environ.get("GITHUB_REPOSITORY") != "MakisImperium/endstone-zernyth":
        raise ValueError("Unexpected release repository")
    if not re.fullmatch(r"[1-9][0-9]{0,19}", run_id):
        raise ValueError("Invalid validated run ID")
    request = urllib.request.Request(
        f"https://api.github.com/repos/MakisImperium/endstone-zernyth/actions/runs/{run_id}",
        headers={"Authorization": "Bearer " + os.environ["GITHUB_TOKEN"],
                 "Accept": "application/vnd.github+json", "User-Agent": "Zernyth-Release"})
    with urllib.request.urlopen(request, timeout=30) as response:
        run = json.load(response)
    verify_approved_run(run, commit, run_id)


def check_bundle(directory, commit, run_id):
    """Check the immutable build bundle and its measured validation evidence."""
    metadata_path = directory / "VALIDATION.json"
    if metadata_path.stat().st_size > 1024 * 1024:
        raise ValueError("Oversized validation metadata")
    metadata = json.loads(metadata_path.read_text(encoding="utf-8"))
    if (metadata.get("schema") != 1 or metadata.get("commit") != commit
            or str(metadata.get("buildRunId")) != str(run_id)):
        raise ValueError("Build bundle provenance mismatch")
    checks = metadata.get("checks", {})
    required = {"nativeJni", "pythonTooling", "wheelValidation", "freshInstallation", "pythonApi"}
    if set(checks) != required or any(value != "PASS" for value in checks.values()):
        raise ValueError("Build bundle has incomplete validation gates")
    if metadata.get("nativeTests") != 249 or metadata.get("pythonApiTests") != 105:
        raise ValueError("Build bundle has incomplete native/Python API coverage")
    if metadata.get("jniCheckerDiagnostics") != 0 or metadata.get("jniProcesses") != 9:
        raise ValueError("Build bundle JNI checker evidence differs from the required suite")
    version = (Path(__file__).resolve().parents[1] / "zernyth-version.txt").read_text().strip()
    expected = {f"dist/zernyth_engine-{version}-cp315-cp315-win_amd64.whl", f"sdk/zernyth-sdk-{version}.zip"}
    if set(metadata.get("artifacts", {})) != expected:
        raise ValueError("Unexpected release bundle inventory")
    actual = {p.relative_to(directory).as_posix() for p in directory.rglob("*") if p.is_file()}
    if actual != expected | {"VALIDATION.json"}:
        raise ValueError("Unexpected files in release bundle")
    for name, digest in metadata["artifacts"].items():
        path = directory / name
        if path.is_symlink() or not re.fullmatch(r"[0-9a-f]{64}", digest):
            raise ValueError("Unsafe build artifact")
        with path.open("rb") as stream:
            if hashlib.file_digest(stream, "sha256").hexdigest() != digest:
                raise ValueError("Release bundle checksum mismatch")
    return metadata


def bundle(wheel, sdk, destination):
    """Record successful native, tooling and fresh-install checks with exact artifact hashes."""
    root = Path(__file__).resolve().parents[1]
    commit = os.environ["RELEASE_COMMIT"]
    if subprocess.check_output(["git", "rev-parse", "HEAD"], text=True).strip() != commit:
        raise ValueError("Build checkout differs from the approved release SHA")
    native = json.loads((root / "build/release-013/host-test-result.json").read_text())
    if (native.get("status") != "PASS" or native.get("passed") != 249
            or native.get("jniCheckerDiagnostics") != 0 or native.get("jniProcesses") != 9):
        raise ValueError("Native/JNI validation did not pass")
    tooling = (root / "build/python-tooling.log").read_text(encoding="utf-8")
    if not re.search(r"Ran [1-9][0-9]* tests", tooling) or not re.search(r"(?m)^OK\s*$", tooling):
        raise ValueError("Python tooling tests did not pass")
    text = (root / "build/wheel-install.jsonl").read_text(encoding="utf-8").lstrip()
    decoder = json.JSONDecoder()
    validation, end = decoder.raw_decode(text)
    installation, _ = decoder.raw_decode(text[end:].lstrip())
    if (validation.get("status") != "PASS" or validation.get("developerPathPrivacy") != "PASS"
            or installation.get("installation") != "PASS" or installation.get("pythonApiTests") != 105):
        raise ValueError("Fresh wheel installation/API checks did not pass")
    destination.mkdir(parents=True, exist_ok=False)
    artifacts = {}
    for source, folder in ((wheel, "dist"), (sdk, "sdk")):
        target = destination / folder / source.name
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source, target)
        artifacts[target.relative_to(destination).as_posix()] = hashlib.sha256(target.read_bytes()).hexdigest()
    metadata = {"schema": 1, "commit": commit, "buildRunId": os.environ["GITHUB_RUN_ID"],
                "checks": {name: "PASS" for name in
                           ("nativeJni", "pythonTooling", "wheelValidation", "freshInstallation", "pythonApi")},
                "nativeTests": native["passed"], "jniProcesses": native["jniProcesses"],
                "jniCheckerDiagnostics": 0, "pythonApiTests": 105, "artifacts": artifacts}
    (destination / "VALIDATION.json").write_text(json.dumps(metadata, indent=2) + "\n", encoding="utf-8")
    check_bundle(destination, commit, metadata["buildRunId"])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("tools", "digest", "bundle", "approved-run", "verify-bundle"))
    parser.add_argument("--wheel", type=Path)
    parser.add_argument("--sdk", type=Path)
    parser.add_argument("--artifact-dir", type=Path)
    parser.add_argument("--validated-run")
    args = parser.parse_args()
    if args.command == "tools":
        add_tool_paths(os.environ.get("ZERNYTH_TOOL_PATHS", ""), Path(os.environ["GITHUB_PATH"]))
    elif args.command == "digest":
        if args.wheel is None:
            parser.error("digest requires --wheel")
        write_wheel_digest(args.wheel, Path(os.environ["GITHUB_OUTPUT"]))
    elif args.command == "bundle":
        if not args.wheel or not args.sdk or not args.artifact_dir:
            parser.error("bundle requires --wheel, --sdk and --artifact-dir")
        bundle(args.wheel, args.sdk, args.artifact_dir)
    elif args.command == "approved-run":
        approved_run(args.validated_run, os.environ["RELEASE_COMMIT"])
    else:
        metadata = check_bundle(args.artifact_dir, os.environ["RELEASE_COMMIT"], args.validated_run)
        wheel = next((args.artifact_dir / "dist").glob("*.whl"))
        write_wheel_digest(wheel, Path(os.environ["GITHUB_OUTPUT"]))
        print("PASS verified immutable release bundle", metadata["commit"])


if __name__ == "__main__":
    main()
