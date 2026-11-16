"""Bounded synthetic packaging/security checks. Never loads foreign native code."""

from email.parser import BytesParser
import hashlib
import importlib.util
import io
import json
import os
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch
from types import SimpleNamespace
import warnings
import zipfile

import yaml
from packaging.metadata import Metadata

ROOT = Path(__file__).resolve().parents[1]


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, ROOT / path)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


BACKEND = load("zernyth_build_test", "packaging/zernyth_build.py")
WHEEL = load("zernyth_wheel_test", "scripts/zernyth_wheel.py")
BDS = load("zernyth_bds_test", "scripts/zernyth_bds.py")


class WheelTests(unittest.TestCase):
    def reject_archive(self, names, expected):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "synthetic.whl"
            with warnings.catch_warnings():
                warnings.simplefilter("ignore", UserWarning)
                with zipfile.ZipFile(path, "w") as archive:
                    for name in names:
                        archive.writestr(name, "synthetic input, never execute")
            with self.assertRaisesRegex(ValueError, expected):
                WHEEL.validate(path)

    def test_distribution_identity_and_utf8_description(self):
        metadata = BytesParser().parsebytes(BACKEND.metadata_bytes())
        Metadata.from_email(BACKEND.metadata_bytes(), validate=True)
        self.assertEqual(metadata["Name"], "zernyth-engine")
        self.assertEqual(metadata["Version"], "1.0.0")
        self.assertIn("Zernyth", BACKEND.metadata_bytes().decode("utf-8"))
        self.assertNotIn("asyncio", metadata.get_all("Requires-Dist"))

    def test_metadata_and_entrypoint_are_preparable_without_build(self):
        with tempfile.TemporaryDirectory() as directory:
            info = BACKEND.prepare_metadata_for_build_wheel(directory)
            path = Path(directory) / info
            self.assertIn("zernyth = zernyth.cli:main", (path / "entry_points.txt").read_text())
            self.assertIn("TERMS AND CONDITIONS", (path / "licenses/LICENSE").read_text())

    def test_binary_inspection_does_not_invent_runtime_results(self):
        with tempfile.TemporaryDirectory() as directory:
            executable = Path(directory) / "synthetic.exe"
            executable.write_bytes(b"Synthetic parser input; never execute")
            with patch.object(BDS, "file_version", return_value=BDS.EXPECTED):
                result = BDS.inspect(executable)
        self.assertEqual(result["binaryInspection"], "PASS")
        self.assertEqual(result["bdsRun"], "NOT TESTED")
        self.assertEqual(result["isolation"], "NOT TESTED")

    def test_no_silent_linux_wheel(self):
        with patch.object(BACKEND.sys, "platform", "linux"):
            with self.assertRaisesRegex(RuntimeError, "Windows"):
                BACKEND.build_wheel("unused")

    def test_traversal_rejected(self):
        self.reject_archive(["../outside"], "Unsafe")

    def test_windows_path_rejected(self):
        self.reject_archive(["C:/outside"], "Unsafe")

    def test_duplicate_members_rejected(self):
        self.reject_archive(["same", "same"], "Duplicate")

    def test_private_instructions_rejected(self):
        self.reject_archive(["zernyth/AGENTS.md"], "Private")

    def test_bds_executable_rejected(self):
        self.reject_archive(["endstone/bedrock_server.exe"], "Private")

    def test_compiled_cache_rejected(self):
        self.reject_archive(["zernyth/__pycache__/cli.pyc"], "Private")

    def test_embedded_private_paths_detected_without_printing_values(self):
        self.assertGreater(WHEEL.developer_path_count(b"C:\\Users\\synthetic\\build\\a.pdb"), 0)
        self.assertGreater(WHEEL.developer_path_count("C:\\Users\\synthetic\\a.pdb".encode("utf-16-le")), 0)
        self.assertEqual(WHEEL.developer_path_count(b"D:\\build\\a.pdb"), 0)

    def test_missing_dependency_cache_does_not_download(self):
        with patch.object(WHEEL.subprocess, "run") as run:
            with self.assertRaisesRegex(ValueError, "wheelhouse"):
                WHEEL.install_test(Path("unused.whl"), None)
            run.assert_not_called()

    def test_release_sha_must_be_exact(self):
        with patch.dict(os.environ, {"RELEASE_COMMIT": "main"}), \
                patch.object(WHEEL.subprocess, "check_output", return_value="a" * 40):
            with self.assertRaisesRegex(ValueError, "SHA"):
                WHEEL.release_ref()

    def test_production_requires_matching_tag(self):
        with patch.dict(os.environ, {"RELEASE_COMMIT": "a" * 40, "RELEASE_TARGET": "pypi"}), \
                patch.object(WHEEL.subprocess, "check_output", side_effect=["a" * 40, "b" * 40]):
            with self.assertRaisesRegex(ValueError, "tag"):
                WHEEL.release_ref()

    def test_cli_rejects_official_endstone(self):
        from zernyth import cli
        with patch.object(cli.metadata, "distribution", return_value=object()):
            with self.assertRaisesRegex(RuntimeError, "separate fresh venv"):
                cli.reject_conflicting_distribution()

    def test_java_runtime_rejects_old_jdk(self):
        from zernyth import cli
        result = SimpleNamespace(stdout="", stderr='openjdk version "21.0.9"')
        with tempfile.TemporaryDirectory() as directory, patch.object(cli.subprocess, "run", return_value=result):
            with self.assertRaisesRegex(ValueError, "JDK 25"):
                cli._java_runtime(directory)

    def test_java_runtime_requires_real_jvm_library(self):
        from zernyth import cli
        result = SimpleNamespace(stdout="", stderr='openjdk version "25.0.5"')
        with tempfile.TemporaryDirectory() as directory, patch.object(cli.subprocess, "run", return_value=result):
            with self.assertRaisesRegex(ValueError, "JVM library"):
                cli._java_runtime(directory)

    def test_build_only_verifies_the_requested_commit(self):
        with patch.dict(os.environ, {"RELEASE_COMMIT": "a" * 40, "RELEASE_TARGET": "build-only"}), \
                patch.object(WHEEL.subprocess, "check_output", return_value="a" * 40) as lookup:
            WHEEL.release_ref()
            self.assertEqual(lookup.call_count, 1)

    def test_unknown_release_target_is_rejected(self):
        with patch.dict(os.environ, {"RELEASE_COMMIT": "a" * 40, "RELEASE_TARGET": "unknown"}), \
                patch.object(WHEEL.subprocess, "check_output", return_value="a" * 40):
            with self.assertRaisesRegex(ValueError, "Unknown release target"):
                WHEEL.release_ref()

    def test_original_dependency_notices_match_inventory(self):
        base = ROOT / "packaging/licenses/sentry-native"
        inventory = json.loads((base / "inventory.json").read_text())
        self.assertGreaterEqual(len(inventory["files"]), 10)
        for entry in inventory["files"]:
            self.assertEqual(hashlib.sha256((base / entry["path"]).read_bytes()).hexdigest(), entry["sha256"])
        self.assertTrue((base / "external/crashpad/third_party/mini_chromium/mini_chromium/LICENSE").is_file())

    def test_publish_workflow_manual_and_privilege_separation(self):
        data = yaml.load((ROOT / ".github/workflows/publish.yml").read_text(), Loader=yaml.BaseLoader)
        self.assertEqual(list(data["on"]), ["workflow_dispatch"])
        self.assertEqual(data["permissions"], {"contents": "read"})
        checkout = next(index for index, step in enumerate(data["jobs"]["wheel"]["steps"])
                        if "actions/checkout" in step.get("uses", ""))
        preparation = data["jobs"]["wheel"]["steps"][0]["run"]
        self.assertGreater(checkout, 0)
        self.assertIn("RELEASE_COMMIT", preparation)
        self.assertIn("[0-9a-f]{40}", preparation)
        self.assertEqual(data["jobs"]["wheel"]["runs-on"], "windows-2025")
        self.assertNotIn("safe.directory=*", preparation)
        build_step = next(step["run"] for step in data["jobs"]["wheel"]["steps"]
                          if step.get("name", "").startswith("Build matching"))
        self.assertIn("scripts/zernyth_release.py build", build_step)
        self.assertIn("scripts/zernyth_release.py test --test-host", build_step)
        self.assertIn("--test-host build/RelWithDebInfo/tests/zernyth_java_test_host.dll", build_step)
        for name in ("testpypi", "pypi"):
            job = data["jobs"][name]
            self.assertEqual(job["environment"]["name"], name)
            self.assertEqual(job["permissions"]["id-token"], "write")
            self.assertEqual(job["runs-on"], "ubuntu-latest")
            self.assertEqual(job["needs"], ["wheel", "promote"])
            self.assertFalse(any("checkout" in step.get("uses", "") for step in job["steps"]))
            publisher = job["steps"][-1]
            self.assertNotIn("password", publisher.get("with", {}))
        self.assertEqual(data["jobs"]["testpypi"]["steps"][-1]["with"]["repository-url"],
                         "https://test.pypi.org/legacy/")
        promotion = data["jobs"]["promote"]
        self.assertNotIn("id-token", promotion["permissions"])
        self.assertEqual(promotion["permissions"]["actions"], "read")

    def test_upstream_uploads_are_guarded(self):
        data = yaml.load((ROOT / ".github/workflows/build.yml").read_text(), Loader=yaml.BaseLoader)
        for name in ("wheels", "docker", "release"):
            self.assertIn("github.repository == 'EndstoneMC/endstone'", data["jobs"][name]["if"])
        docs = yaml.load((ROOT / ".github/workflows/docs.yml").read_text(), Loader=yaml.BaseLoader)
        self.assertEqual(docs["jobs"]["deploy"]["if"], "github.repository == 'EndstoneMC/endstone'")


if __name__ == "__main__":
    unittest.main()
