"""Bounded synthetic tests for the private-build metadata source hook."""

import hashlib
import importlib.util
import os
from pathlib import Path
import tempfile
from types import SimpleNamespace
import unittest
from unittest.mock import patch


path = Path(__file__).resolve().parents[1] / "config/conan/hooks/hook_zernyth_build_metadata.py"
spec = importlib.util.spec_from_file_location("zernyth_metadata_hook", path)
hook = importlib.util.module_from_spec(spec)
spec.loader.exec_module(hook)


class BuildMetadataTests(unittest.TestCase):
    def test_unrelated_dependencies_are_untouched(self):
        hook.pre_build(SimpleNamespace(name="another-library"))

    def test_unknown_version_is_rejected(self):
        with self.assertRaisesRegex(RuntimeError, "changing versions"):
            hook.pre_build(SimpleNamespace(name="openssl", version="0.0.0"))

    def test_known_patch_is_idempotent_and_preserves_other_source(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / "util/mkbuildinf.pl"
            source.parent.mkdir(parents=True)
            original = "# synthetic license header\n" + hook.ANCHOR + "\n# synthetic remainder\n"
            source.write_text(original, encoding="utf-8")
            args = SimpleNamespace(name="openssl", version="3.6.5", source_folder=directory)
            with patch.object(hook, "SOURCE_SHA256", hashlib.sha256(original.encode()).hexdigest()), \
                    patch.dict(os.environ, {"ZERNYTH_BUILD_PATH_ROOT": directory, "CONAN_HOME": directory}):
                hook.pre_build(args)
                expected = source.read_bytes()
                hook.pre_build(args)
                self.assertEqual(source.read_bytes(), expected)
            self.assertEqual(source.read_text().replace(hook.NORMALIZED, hook.ANCHOR), original)

    def test_unreviewed_source_is_rejected_without_changes(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / "util/mkbuildinf.pl"
            source.parent.mkdir(parents=True)
            source.write_bytes(b"unreviewed synthetic input")
            args = SimpleNamespace(name="openssl", version="3.6.5", source_folder=directory)
            with patch.dict(os.environ, {"ZERNYTH_BUILD_PATH_ROOT": directory, "CONAN_HOME": directory}):
                with self.assertRaisesRegex(RuntimeError, "differs"):
                    hook.pre_build(args)
            self.assertEqual(source.read_bytes(), b"unreviewed synthetic input")


if __name__ == "__main__":
    unittest.main()
