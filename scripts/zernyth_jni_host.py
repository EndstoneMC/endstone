"""Run the existing native suite from a fresh Windows Python process.

The optional first-party test DLL uses the standard Windows loader and remains
subject to application control. It is never bundled in the production wheel.
Each invocation must use a fresh process because JNI owns one JVM per process.
"""

import argparse
import ctypes
import os
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--library", type=Path, required=True)
    parser.add_argument("test_arguments", nargs=argparse.REMAINDER)
    args = parser.parse_args()
    arguments = args.test_arguments
    if arguments[:1] == ["--"]:
        arguments = arguments[1:]
    if "--gtest_list_tests" not in arguments:
        filters = [value.removeprefix("--gtest_filter=")
                   for value in arguments if value.startswith("--gtest_filter=")]
        if len(filters) != 1 or not re.fullmatch(r"[A-Za-z0-9_./]+", filters[0]):
            parser.error("Select one exact test filter, or use release.py test --test-host for the full suite")
    library = args.library.resolve()
    if os.name != "nt" or not library.is_relative_to(ROOT / "build"):
        parser.error("The test library must be a local Windows build under this repository's build/")
    if library.name != "zernyth_java_test_host.dll" or not library.is_file():
        parser.error("The optional first-party JNI test library must be built first")
    encoded = [b"zernyth_java_test_host", *(value.encode("utf-8") for value in arguments)]
    argv = (ctypes.c_char_p * (len(encoded) + 1))(*encoded, None)
    with os.add_dll_directory(str(library.parent)):
        host = ctypes.CDLL(str(library))
        run = host.zernyth_run_java_tests
        run.argtypes = [ctypes.c_int, ctypes.POINTER(ctypes.c_char_p)]
        run.restype = ctypes.c_int
        return run(len(encoded), argv)


if __name__ == "__main__":
    raise SystemExit(main())
