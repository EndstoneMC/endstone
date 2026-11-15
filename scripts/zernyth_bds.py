"""Read-only BDS inspection and configuration preparation. Does not launch a server."""
import argparse
import ctypes
import hashlib
import json
import os
from pathlib import Path

EXPECTED = "1.26.60.6"


def file_version(path):
    if os.name != "nt":
        return None  # No guessed Linux offsets/strings/signatures.
    api = ctypes.WinDLL("version", use_last_error=True)
    api.GetFileVersionInfoSizeW.argtypes = [ctypes.c_wchar_p, ctypes.POINTER(ctypes.c_uint32)]
    api.GetFileVersionInfoW.argtypes = [ctypes.c_wchar_p, ctypes.c_uint32, ctypes.c_uint32, ctypes.c_void_p]
    api.VerQueryValueW.argtypes = [ctypes.c_void_p, ctypes.c_wchar_p, ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_uint32)]
    size = api.GetFileVersionInfoSizeW(str(path), None)
    if not 0 < size <= 1024*1024:
        raise ValueError("Missing or invalid BDS file-version resource")
    data = ctypes.create_string_buffer(size)
    if not api.GetFileVersionInfoW(str(path), 0, size, data):
        raise OSError(ctypes.get_last_error(), "Cannot inspect BDS version")
    pointer, length = ctypes.c_void_p(), ctypes.c_uint32()
    if not api.VerQueryValueW(data, "\\", ctypes.byref(pointer), ctypes.byref(length)) or length.value < 52:
        raise ValueError("Invalid fixed version information")
    fields = ctypes.cast(pointer, ctypes.POINTER(ctypes.c_uint32))
    if fields[0] != 0xFEEF04BD:
        raise ValueError("Invalid version signature")
    return ".".join(map(str, (fields[2] >> 16, fields[2] & 65535, fields[3] >> 16, fields[3] & 65535)))


def inspect(path):
    executable = Path(path).resolve()
    if executable.is_dir():
        executable /= "bedrock_server.exe" if os.name == "nt" else "bedrock_server"
    if not executable.is_file():
        raise FileNotFoundError("An existing BDS executable is required; nothing is downloaded")
    with executable.open("rb") as stream:
        sha = hashlib.file_digest(stream, "sha256").hexdigest()
    found = file_version(executable)
    return {"binaryInspection": "PASS" if found == EXPECTED else "NOT TESTED" if found is None else "FAIL",
            "fileVersion": found, "expectedVersion": EXPECTED, "sha256": sha,
            "isolation": "NOT TESTED", "bdsRun": "NOT TESTED", "gameplay": "NOT TESTED",
            "reason": "Read-only binary inspection. Network exposure is operator-managed; this inspection does not start BDS or validate gameplay."}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path)
    parser.add_argument("--prepare", type=Path)
    args = parser.parse_args()
    result = inspect(args.source)
    if args.prepare:
        root = Path(__file__).resolve().parents[1]
        destination = args.prepare.resolve()
        if not destination.is_relative_to(root / "build") or destination.exists():
            raise ValueError("Require a new test directory under this repository's build/")
        destination.mkdir(parents=True)
        (destination / "inspection.json").write_text(json.dumps(result, indent=2), encoding="utf-8")
        (destination / "server.properties.template").write_text(
            "server-name=Zernyth isolated test\nlevel-name=zernyth-smoke-013\nlevel-seed=1309\n"
            "server-ip=127.0.0.1\nserver-port=29484\nserver-portv6=29485\nenable-lan-visibility=false\n"
            "online-mode=true\nallow-list=true\nmax-players=1\nemit-server-telemetry=false\n", encoding="utf-8")
        (destination / "endstone.toml.template").write_text("[network]\nipv6 = false\nstun-servers = []\n", encoding="utf-8")
        (destination / "allowlist.json.template").write_text("[]\n", encoding="utf-8")
        (destination / "DO-NOT-START.txt").write_text(result["reason"] + "\nNo BDS files/worlds copied. Templates do not establish isolation.\n", encoding="utf-8")
    print(json.dumps(result, indent=2))


if __name__ == "__main__":
    main()
