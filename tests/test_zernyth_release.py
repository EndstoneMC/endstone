"""Packaging tests use synthetic data only; no native code, BDS, or downloads."""
import hashlib
import importlib.util
import json
from pathlib import Path
import tempfile
import unittest
from unittest import mock
from types import SimpleNamespace
import zipfile

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location("zernyth_release", ROOT / "scripts/zernyth_release.py")
release = importlib.util.module_from_spec(spec)
spec.loader.exec_module(release)


class ReleaseTests(unittest.TestCase):
    def test_archived_regressions_use_selected_host(self):
        with tempfile.TemporaryDirectory() as temporary:
            directory = Path(temporary)
            for version in ("0.2.0", "0.3.0", "0.4.0", "0.5.0", "0.6.0", "0.7.0", "0.8.0"):
                (directory / f"example-{version}.jar").write_bytes(b"synthetic, never executed")
            args = SimpleNamespace(java_home=None, test_host=ROOT / "build/test-host.dll",
                                   build_dir=ROOT / "build/RelWithDebInfo")
            result = SimpleNamespace(stdout="[  PASSED  ] 1 test.", stderr="")
            with mock.patch.dict(release.os.environ, {"ZERNYTH_LEGACY_JARS": str(directory)}):
                with mock.patch.object(release, "test_env", return_value=({}, {})):
                    with mock.patch.object(release, "RESULTS", directory):
                        with mock.patch.object(release, "run", return_value=result) as run:
                            release.regressions(args)
            self.assertEqual(run.call_count, 7)
            for call in run.call_args_list:
                command = [str(value) for value in call.args[0]]
                self.assertIn(str(ROOT / "scripts/zernyth_jni_host.py"), command)
            self.assertEqual(json.loads((directory / "regression-result.json").read_text())["status"], "PASS")

    def test_ci_cache_not_inside_clean_checkout(self):
        env = {"CI": "true", "CONAN_HOME": str(ROOT / ".conan2")}
        with mock.patch.object(release, "java_env", return_value=(env, {})):
            with self.assertRaisesRegex(ValueError, "preprovisioned CONAN_HOME"):
                release.build_all(SimpleNamespace(java_home=None))

    def test_ci_conan_uses_supplied_cache_outside_checkout(self):
        with tempfile.TemporaryDirectory() as temporary:
            base = Path(temporary)
            graph = {"graph": {"nodes": {"1": {"ref": "sentry-native/0.14.2",
                                              "package_folder": str(base / "cache/package")}}}}
            environment = {"CI": "true", "CONAN_HOME": str(base / "cache")}
            args = SimpleNamespace(java_home=None, gradle=None, cmake=None, conan=None,
                                   vendor="temurin", sanitizers="", jobs=4, build_dir=ROOT / "build/RelWithDebInfo",
                                   test_host=ROOT / "build/RelWithDebInfo/tests/zernyth_java_test_host.dll")

            def result(command, **kwargs):
                return SimpleNamespace(stdout=json.dumps(graph) if command[1:3] == ["graph", "info"] else "")

            with mock.patch.object(release, "java_env", return_value=(environment, {})), \
                    mock.patch.object(release, "tool", side_effect=lambda name, explicit=None: name), \
                    mock.patch.object(release, "RESULTS", base / "results"), \
                    mock.patch.object(release, "run", side_effect=result) as run, \
                    mock.patch.object(release, "check_artifacts", return_value={}):
                release.build_all(args)
            conan_calls = [call for call in run.call_args_list if call.args[0][0] == "conan"]
            self.assertTrue(conan_calls)
            for call in conan_calls:
                self.assertEqual(call.kwargs["cwd"], ROOT.parent)
            self.assertIn(str(ROOT), conan_calls[0].args[0])
            self.assertIn("--output-folder=" + str(ROOT), conan_calls[0].args[0])
            configure = next(call for call in run.call_args_list if "--preset" in call.args[0])
            self.assertIn("-DZERNYTH_BUILD_JAVA_TEST_HOST=ON", configure.args[0])
            self.assertIn("-DSENTRY_NATIVE_BIN_DIR=" + (base / "cache/package/bin").as_posix(),
                          configure.args[0])
    def test_version_and_bounded_profiles(self):
        self.assertRegex(release.version(), r"^\d+\.\d+\.\d+(?:-(?:rc|alpha)\.\d+)?$")
        for name in ("smoke", "lifecycle", "bounded-stress", "observation"):
            self.assertLessEqual(release.profile(name)["cycles"], 50)

    def test_host_discovery_handles_parameter_comments(self):
        result = release.host_test_names("Example.\n  First\nTyped/0. # TypeParam = int\n  Second\n")
        self.assertEqual(result, ["Example.First", "Typed/0.Second"])

    def test_host_discovery_rejects_empty_output(self):
        with self.assertRaisesRegex(ValueError, "Empty"):
            release.host_test_names("No test groups\n")

    def test_host_discovery_rejects_duplicate_tests(self):
        with self.assertRaisesRegex(ValueError, "duplicate"):
            release.host_test_names("Example.\n  First\n  First\n")

    def test_output_scope(self):
        with self.assertRaises(ValueError):
            release.workspace(ROOT / "endstone")

    def test_forbidden_paths(self):
        for name in ("../evil", "/absolute", "root/../evil", "root\\evil", "C:/evil", "root/bedrock_server.exe", "root/private.pdb", "root/__pycache__/private.pyc"):
            with self.subTest(name=name), self.assertRaises(ValueError):
                release.safe_name(name)

    def test_jar_version(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "synthetic.jar"
            with zipfile.ZipFile(path, "w") as jar:
                jar.writestr("META-INF/MANIFEST.MF", "Manifest-Version: 1.0\r\nImplementation-Version: 0.9.0-rc.1\r\nZernyth-Engine-Version: 0.9.0-rc.1\r\n\r\n")
            self.assertEqual(release.jar_version(path), "0.9.0-rc.1")

    def fixture(self, directory, jar_version=None):
        stage = directory / "synthetic"
        stage.mkdir()
        v, module = release.version(), "_python.cp315-win_amd64.pyd"
        for name in release.required_files(v, "Windows", module):
            path = stage / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(b"synthetic, never execute")
        (stage / "VERSION").write_text(v)
        (stage / "BUILD.json").write_text(json.dumps({"zernyth": v}))
        for name in (f"java/api-{v}.jar", f"java/runtime-{v}.jar", "examples/HelloZernyth.jar"):
            with zipfile.ZipFile(stage / name, "w") as jar:
                value = jar_version or v
                jar.writestr("META-INF/MANIFEST.MF", f"Implementation-Version: {value}\r\nZernyth-Engine-Version: {value}\r\n\r\n")
                jar.writestr("dev/zernyth/api/Server.class", b"\xca\xfe\xba\xbe\x00\x00\x00\x45")
        files = {p.relative_to(stage).as_posix(): release.digest(p) for p in stage.rglob("*") if p.is_file()}
        (stage / "manifest.json").write_text(json.dumps({"version": v, "platform": "Windows", "pythonModule": module, "files": files}))
        (stage / "SHA256SUMS").write_text("".join(f"{files[n]}  {n}\n" for n in sorted(files)))
        return stage

    def test_zip_reproduction_and_verification(self):
        with tempfile.TemporaryDirectory() as directory:
            base = Path(directory)
            stage = self.fixture(base)
            first, second = base / "first.zip", base / "second.zip"
            release.reproducible_zip(stage, first)
            release.reproducible_zip(stage, second)
            self.assertEqual(release.digest(first), release.digest(second))
            self.assertEqual(release.verify_archive(first)["status"], "PASS")
            with self.assertRaises(FileExistsError):
                release.reproducible_zip(stage, first)

    def test_tampering(self):
        with tempfile.TemporaryDirectory() as directory:
            base = Path(directory)
            stage = self.fixture(base)
            (stage / "README.md").write_text("tampered")
            archive = base / "bad.zip"
            release.reproducible_zip(stage, archive)
            with self.assertRaisesRegex(ValueError, "Checksum"):
                release.verify_archive(archive)

    def test_mixed_version_even_with_updated_checksums(self):
        with tempfile.TemporaryDirectory() as directory:
            base = Path(directory)
            stage = self.fixture(base, "0.8.0")
            archive = base / "mixed.zip"
            release.reproducible_zip(stage, archive)
            with self.assertRaisesRegex(ValueError, "JAR version"):
                release.verify_archive(archive)

    def test_unexpected_file(self):
        with tempfile.TemporaryDirectory() as directory:
            base = Path(directory)
            stage = self.fixture(base)
            (stage / "unexpected.txt").write_text("unlisted")
            archive = base / "bad.zip"
            release.reproducible_zip(stage, archive)
            with self.assertRaisesRegex(ValueError, "Unexpected"):
                release.verify_archive(archive)

    def test_zip_traversal_rejected_without_extraction(self):
        with tempfile.TemporaryDirectory() as directory:
            archive = Path(directory) / "bad.zip"
            with zipfile.ZipFile(archive, "w") as output:
                output.writestr("root/../evil.txt", "bad")
            with self.assertRaises(ValueError):
                release.verify_archive(archive)


if __name__ == "__main__":
    unittest.main()
