"""Synthetic Linux archive checks, without loading native code or starting BDS."""

import hashlib
import gzip
import importlib.util
import io
import json
from pathlib import Path
import sys
import tarfile
import tempfile
import unittest
from unittest.mock import patch


sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
import zernyth_linux_release as linux
sys.path.pop(0)


class LinuxArchiveTests(unittest.TestCase):
    def test_conan_cache_lookup_avoids_checkout_configuration(self):
        with patch.object(linux.subprocess, "check_output", return_value="/synthetic/cache\n") as lookup:
            self.assertEqual(linux.conan_path("conan", "synthetic/1.0"), Path("/synthetic/cache"))
        self.assertEqual(lookup.call_args.kwargs["cwd"], linux.ROOT.parent)

    def test_oversized_payload_is_rejected_before_reading_it(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "oversized.tar.gz"
            info = tarfile.TarInfo("release/oversized")
            info.size = 2 * 1024**3
            with gzip.open(path, "wb") as stream:
                stream.write(info.tobuf())
            with self.assertRaisesRegex(ValueError, "resource limit"):
                linux.verify_archive(path)

    def test_shared_lgpl_linkage_is_required(self):
        linked = "(NEEDED) Shared library: [libelf.so.0]\n(NEEDED) Shared library: [libdwarf.so.2]"
        with patch.object(linux.subprocess, "check_output", return_value=linked):
            linux.check_shared_linkage(Path("synthetic-stage"), "never-executed-readelf")
        with patch.object(linux.subprocess, "check_output", return_value=linked.splitlines()[0]):
            with self.assertRaisesRegex(ValueError, "libdwarf"):
                linux.check_shared_linkage(Path("synthetic-stage"), "never-executed-readelf")

    def test_native_build_paths_are_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            payload = root / "library.so"
            payload.write_bytes(b"\x7fELF synthetic /private/build/source.cpp")
            with self.assertRaisesRegex(ValueError, "Private build path"):
                linux.check_native_paths(root, ("/private/build",))
            self.assertEqual(linux.check_native_paths(root, ("/different/root",)), 1)

    def verify(self, entries, manifest=None):
        base = [(name, b"synthetic payload") for name in
                ("BUILD.json", "install.sh", "start.sh", "bin/run.py", "requirements.lock")]
        supplied = {name for name, _ in entries}
        entries = [item for item in base if item[0] not in supplied] + entries
        if manifest is not None:
            manifest = {**{name: hashlib.sha256(value).hexdigest()
                           for name, value in base if name not in supplied}, **manifest}
        manifest = manifest if manifest is not None else {
            name: hashlib.sha256(value).hexdigest() for name, value in entries
        }
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "synthetic.tar.gz"
            with tarfile.open(path, "w:gz") as archive:
                for name, value in [*entries, ("manifest.json", json.dumps(manifest).encode())]:
                    info = tarfile.TarInfo("release/" + name)
                    info.size = len(value)
                    archive.addfile(info, io.BytesIO(value))
            return linux.verify_archive(path)

    def test_complete_manifest(self):
        self.assertEqual(self.verify([("BUILD.json", b"{}")])["status"], "PASS")

    def test_tampered_file(self):
        with self.assertRaisesRegex(ValueError, "Checksum mismatch"):
            self.verify([("file", b"modified")], {"file": hashlib.sha256(b"original").hexdigest()})

    def test_duplicate_member(self):
        with self.assertRaisesRegex(ValueError, "Duplicate"):
            self.verify([("file", b"one"), ("file", b"two")])

    def test_unlisted_file(self):
        with self.assertRaisesRegex(ValueError, "Incomplete"):
            self.verify([("file", b"data")], {})

    def test_private_and_proprietary_files(self):
        for name in ("bedrock_server", "libmimalloc.so", "AGENTS.md", "CLAUDE.md"):
            with self.subTest(name=name), self.assertRaisesRegex(ValueError, "Forbidden"):
                self.verify([(name, b"synthetic content")])

    def test_traversal_paths(self):
        for name in ("../escape", "/absolute", "dir\\escape", "a//b", "drive:file", "a/./b"):
            with self.subTest(name=name), self.assertRaisesRegex(ValueError, "Unsafe"):
                linux.safe_member(name)

    def test_symbolic_link_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "link.tar.gz"
            with tarfile.open(path, "w:gz") as archive:
                info = tarfile.TarInfo("release/link")
                info.type = tarfile.SYMTYPE
                info.linkname = "/outside"
                archive.addfile(info)
            with self.assertRaisesRegex(ValueError, "non-regular"):
                linux.verify_archive(path)

    def test_archive_is_reproducible_and_preserves_existing_output(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            stage = root / "release"
            stage.mkdir()
            files = {}
            for name in ("BUILD.json", "install.sh", "start.sh", "bin/run.py", "requirements.lock"):
                path = stage / name
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(b"synthetic payload")
                files[name] = linux.digest(path)
            (stage / "manifest.json").write_text(json.dumps(files))
            first, second = root / "first.tar.gz", root / "second.tar.gz"
            linux.write_archive(stage, first)
            linux.write_archive(stage, second)
            self.assertEqual(first.read_bytes(), second.read_bytes())
            original = first.read_bytes()
            with self.assertRaises(FileExistsError):
                linux.write_archive(stage, first)
            self.assertEqual(first.read_bytes(), original)
            self.assertFalse(list(root.glob(".zernyth-*")))


class LinuxInstallerTests(unittest.TestCase):
    def setUp(self):
        path = Path(__file__).resolve().parents[1] / "packaging/linux/run.py"
        spec = importlib.util.spec_from_file_location("linux_operator_launcher", path)
        self.launcher = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(self.launcher)
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name)
        (self.root / "manifest.json").write_text("{}")
        (self.root / "wheelhouse").mkdir()
        (self.root / "wheelhouse/pip-synthetic.whl").write_bytes(b"Never executed")

    def install(self, failure=None):
        with patch.object(self.launcher, "ROOT", self.root), \
                patch.object(self.launcher, "metadata"), \
                patch.object(self.launcher.venv, "EnvBuilder"), \
                patch.object(self.launcher.subprocess, "run", side_effect=failure):
            self.launcher.install()

    def test_existing_environment_is_preserved(self):
        environment = self.root / ".venv"
        environment.mkdir()
        marker = environment / "user-file"
        marker.write_text("retain")
        with self.assertRaisesRegex(RuntimeError, "already exists"):
            self.install()
        self.assertEqual(marker.read_text(), "retain")

    def test_install_failure_cleans_own_environment(self):
        with self.assertRaisesRegex(RuntimeError, "synthetic failure"):
            self.install(RuntimeError("synthetic failure"))
        self.assertFalse((self.root / ".venv").exists())

    def test_success_records_completion(self):
        self.install()
        self.assertTrue((self.root / ".venv/.zernyth-install-complete").is_file())


if __name__ == "__main__":
    unittest.main()
