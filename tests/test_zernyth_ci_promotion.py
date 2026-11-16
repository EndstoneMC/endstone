"""Synthetic provenance and checksum tests for release artifact promotion."""

import hashlib
import importlib.util
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch


ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location("ci_promotion_checks", ROOT / "scripts/zernyth_ci.py")
ci = importlib.util.module_from_spec(spec)
spec.loader.exec_module(ci)
spec = importlib.util.spec_from_file_location("hosted_provisioning", ROOT / "scripts/zernyth_hosted_ci.py")
hosted = importlib.util.module_from_spec(spec)
spec.loader.exec_module(hosted)


class PromotionTests(unittest.TestCase):
    def test_toolchain_exports_paths_before_cache_restore(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            binary = root / "LLVM/bin"
            binary.mkdir(parents=True)
            for name in ("clang-cl.exe", "lld-link.exe"):
                (binary / name).write_bytes(b"synthetic, never executed")
            environment = {"ProgramFiles": str(root), "RUNNER_TEMP": str(root / "temporary"),
                           "GITHUB_ENV": str(root / "env"), "GITHUB_PATH": str(root / "path"),
                           "GITHUB_OUTPUT": str(root / "output")}
            with patch.dict(hosted.os.environ, environment, clear=True), \
                    patch.object(hosted.subprocess, "check_output", return_value="clang version 23.1.4\n"):
                hosted.toolchain()
            self.assertIn("CONAN_HOME=", (root / "env").read_text())
            self.assertIn("ZERNYTH_GRADLE=", (root / "env").read_text())
            self.assertIn("fingerprint=", (root / "output").read_text())

    def test_successful_matching_source_run(self):
        commit = "a" * 40
        run = {"id": 123, "head_sha": commit, "conclusion": "success", "status": "completed",
               "event": "workflow_dispatch", "path": ".github/workflows/publish.yml",
               "display_title": f"Release wheel {commit} / build-only"}
        ci.verify_approved_run(run, commit, "123")
        for field, value in {"head_sha": "b" * 40, "conclusion": "failure", "status": "in_progress",
                             "event": "push", "path": ".github/workflows/another.yml",
                             "display_title": "Unrelated build", "id": 124}.items():
            with self.subTest(field=field), self.assertRaises(ValueError):
                ci.verify_approved_run({**run, field: value}, commit, "123")

    def make_bundle(self, directory):
        version = (ROOT / "zernyth-version.txt").read_text().strip()
        names = [f"dist/zernyth_engine-{version}-cp315-cp315-win_amd64.whl", f"sdk/zernyth-sdk-{version}.zip"]
        artifacts = {}
        for name in names:
            path = directory / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(b"synthetic artifact, never executed")
            artifacts[name] = hashlib.sha256(path.read_bytes()).hexdigest()
        metadata = {"schema": 1, "commit": "a" * 40, "buildRunId": "123",
                    "checks": {key: "PASS" for key in
                               ("nativeJni", "pythonTooling", "wheelValidation", "freshInstallation", "pythonApi")},
                    "nativeTests": 249, "jniProcesses": 9, "jniCheckerDiagnostics": 0,
                    "pythonApiTests": 105, "artifacts": artifacts}
        (directory / "VALIDATION.json").write_text(json.dumps(metadata))
        return metadata, names

    def test_bundle_checksums_and_inventory(self):
        with tempfile.TemporaryDirectory() as temporary:
            directory = Path(temporary)
            metadata, names = self.make_bundle(directory)
            self.assertEqual(ci.check_bundle(directory, "a" * 40, "123"), metadata)
            (directory / names[0]).write_bytes(b"modified")
            with self.assertRaisesRegex(ValueError, "checksum"):
                ci.check_bundle(directory, "a" * 40, "123")

    def test_stale_provenance_or_incomplete_evidence_is_rejected(self):
        with tempfile.TemporaryDirectory() as temporary:
            directory = Path(temporary)
            metadata, _ = self.make_bundle(directory)
            for change in ({"commit": "b" * 40}, {"buildRunId": "124"}, {"checks": {}},
                           {"nativeTests": 248}, {"pythonApiTests": 104}, {"jniCheckerDiagnostics": 1},
                           {"jniProcesses": 8}):
                with self.subTest(change=change):
                    (directory / "VALIDATION.json").write_text(json.dumps({**metadata, **change}))
                    with self.assertRaises(ValueError):
                        ci.check_bundle(directory, "a" * 40, "123")

    def test_extra_payload_is_rejected(self):
        with tempfile.TemporaryDirectory() as temporary:
            directory = Path(temporary)
            self.make_bundle(directory)
            (directory / "unexpected-file").write_bytes(b"synthetic")
            with self.assertRaisesRegex(ValueError, "Unexpected files"):
                ci.check_bundle(directory, "a" * 40, "123")

    def test_provisioning_requires_approved_hosted_context(self):
        with patch.dict(hosted.os.environ, {}, clear=True), patch.object(hosted.subprocess, "run") as run:
            with self.assertRaisesRegex(RuntimeError, "explicit approval"):
                hosted.prepare()
        run.assert_not_called()


if __name__ == "__main__":
    unittest.main()
