"""Offline RC build/validation/packaging. Never starts BDS or changes network policy."""
from __future__ import annotations

import argparse
import hashlib
import io
import json
import os
import platform
import re
import shutil
import subprocess
import sys
import tempfile
import time
import zipfile
from pathlib import Path, PurePosixPath
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
RESULTS = ROOT / "build/release-013"
FIXTURES = {"ZERNYTH_GAMEPLAY_TEST_JAR": "integration", "ZERNYTH_GAMEPLAY_FAILURE_TEST_JAR": "failure-integration",
            "ZERNYTH_ADVANCED_TEST_JAR": "advanced-integration", "ZERNYTH_ADVANCED_PEER_TEST_JAR": "advanced-peer-integration",
            "ZERNYTH_NETWORK_TEST_JAR": "network-integration", "ZERNYTH_NETWORK_PEER_TEST_JAR": "network-peer-integration",
            "ZERNYTH_ASYNC_JAR": "async-integration", "ZERNYTH_BENCHMARK_JAR": "benchmark-plugin"}


def version():
    value = (ROOT / "zernyth-version.txt").read_text(encoding="utf-8").strip()
    if not re.fullmatch(r"\d+\.\d+\.\d+(?:-(?:rc|alpha)\.\d+)?", value):
        raise ValueError("A stable, RC or alpha release version is required")
    return value


def digest(path):
    with Path(path).open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def workspace(path):
    path = Path(path).resolve()
    if not path.is_relative_to(ROOT / "build"):
        raise ValueError("Output must remain in this repository's build directory")
    return path


def run(command, *, cwd=ROOT, env=None, timeout=1800, capture=False):
    command = [str(x) for x in command]
    if os.name == "nt" and Path(command[0]).suffix.lower() in (".bat", ".cmd"):
        if any(any(c in part for c in '&|<>^%!"\r\n') for part in command):
            raise ValueError("Unsafe batch argument")
        # Pass one CreateProcess command line: list2cmdline on the whole /c payload
        # would introduce backslash-quotes, which cmd.exe does not interpret like CRT argv.
        command = 'cmd /d /s /c ""' + command[0] + '" ' + subprocess.list2cmdline(command[1:]) + '"'
    return subprocess.run(command, cwd=cwd, env=env, timeout=timeout, check=True,
                          capture_output=capture, text=True, encoding="utf-8", errors="replace")


def tool(name, explicit=None):
    result = explicit or shutil.which(name)
    if not result or not Path(result).is_file():
        raise ValueError(f"Missing installed tool: {name}; no automatic installation")
    return str(Path(result).resolve())


def java_env(home):
    env = os.environ.copy()
    for key in ("JAVA_TOOL_OPTIONS", "JDK_JAVA_OPTIONS", "_JAVA_OPTIONS", "JAVA_OPTS", "GRADLE_OPTS"):
        env.pop(key, None)  # Owned validation JVMs must not inherit agents/debug listeners.
    home = Path(home or env.get("JAVA_HOME", "")).resolve()
    executable = home / "bin" / ("java.exe" if os.name == "nt" else "java")
    result = run([executable, "-XshowSettings:properties", "-version"], env=env, capture=True)
    settings = result.stdout + result.stderr
    def setting(name):
        found = re.search(r"^\s*" + re.escape(name) + r"\s*=\s*(.+)$", settings, re.M)
        if not found:
            raise ValueError(f"Missing JDK setting {name}")
        return found.group(1).strip()
    release = setting("java.version")
    if release.split(".")[0] != "25":
        raise ValueError("Build/test requires an existing JDK 25")
    env.update(JAVA_HOME=str(home), UV_OFFLINE="1", PIP_NO_INDEX="1")
    env["PATH"] = str(home / "bin") + os.pathsep + env.get("PATH", "")
    return env, {"version": release, "vendor": setting("java.vendor")}


def java_artifacts():
    v = version()
    return {name: ROOT / f"java/{name}/build/libs/{name}-{v}.jar" for name in ("api", "runtime", "example")}


def jar_version(path):
    with zipfile.ZipFile(path) as jar:
        if jar.getinfo("META-INF/MANIFEST.MF").file_size > 65536:
            raise ValueError("Oversized JAR manifest")
        raw = jar.read("META-INF/MANIFEST.MF").decode("utf-8").replace("\r\n ", "")
        attrs = dict(line.split(": ", 1) for line in raw.splitlines() if ": " in line)
        value = attrs.get("Zernyth-Engine-Version")
        if value is None or value != attrs.get("Implementation-Version"):
            raise ValueError("Inconsistent JAR provenance")
        return value


def check_artifacts(build):
    metadata = json.loads((build / "zernyth-build.json").read_text(encoding="utf-8"))
    if metadata["zernyth"] != version() or metadata["javaEnabled"] != "ON":
        raise ValueError("Native build is not the matching Java-enabled RC")
    for name, path in java_artifacts().items():
        if jar_version(path) != version():
            raise ValueError(f"Mismatched {name} JAR")
    with zipfile.ZipFile(java_artifacts()["api"]) as jar:
        header = jar.read("dev/zernyth/api/Server.class")[:8]
        if header[:4] != b"\xca\xfe\xba\xbe" or int.from_bytes(header[6:8], "big") != 69:
            raise ValueError("API bytecode is not Java 25")
    return metadata


def build_all(args):
    env, jdk = java_env(args.java_home)
    if env.get("CI", "").lower() == "true" and (not env.get("CONAN_HOME") or Path(env["CONAN_HOME"]).resolve().is_relative_to(ROOT)):
        raise ValueError("CI requires a preprovisioned CONAN_HOME outside the clean checkout")
    gradle, cmake = tool("gradle", args.gradle), tool("cmake", args.cmake)
    RESULTS.mkdir(parents=True, exist_ok=True)
    conan = [tool("conan", args.conan)]
    # Conan gives a checkout's .conanrc precedence over CONAN_HOME. CI runs from
    # the checkout parent with an explicit recipe/output path to use its supplied cache.
    conan_cwd = ROOT.parent if env.get("CI", "").lower() == "true" else ROOT
    lock = RESULTS / "conan.lock"
    command = conan + ["install", str(ROOT), "--output-folder=" + str(ROOT),
                       "--no-remote", "--build=never", "-s", "&:build_type=RelWithDebInfo",
                       "--lockfile-out=" + str(lock)]
    profiles = (["-pr:h", "default", "-pr:h", str(ROOT / "config/conan/windows-sanitized.profile")]
                if os.name == "nt" else [])
    command += profiles
    input_lock = getattr(args, "lockfile", None) or (lock if lock.exists() else None)
    if input_lock:
        command += ["--lockfile=" + str(Path(input_lock).resolve())]
    run(command, cwd=conan_cwd, env=env)
    graph = run(conan + ["graph", "info", str(ROOT), "--no-remote", "--format=json", "-s", "&:build_type=RelWithDebInfo",
                         "--lockfile=" + str(lock)] + profiles, cwd=conan_cwd, env=env, capture=True)
    graph_data = json.loads(graph.stdout)
    for node in graph_data["graph"]["nodes"].values():
        if node.get("binary") == "Cache" and node.get("prev") and node.get("package_id"):
            reference = f'{node["ref"]}:{node["package_id"]}#{node["prev"]}'
            node["package_folder"] = run(conan + ["cache", "path", reference], cwd=conan_cwd, env=env, capture=True).stdout.strip()
    (RESULTS / "dependency-graph.json").write_text(json.dumps(graph_data, indent=2), encoding="utf-8")
    sentry = next(node for node in graph_data["graph"]["nodes"].values()
                  if str(node.get("ref", "")).startswith("sentry-native/") and node.get("package_folder"))
    # Refresh the cached install path on both platforms after a dependency rebuild.
    native_runtime_arguments = ["-DSENTRY_NATIVE_BIN_DIR=" + (Path(sentry["package_folder"]) / "bin").as_posix()]
    if getattr(args, "test_host", None):
        native_runtime_arguments.append("-DZERNYTH_BUILD_JAVA_TEST_HOST=ON")
    run([gradle, "--offline", "--no-daemon", f"-PzernythJavaVendor={args.vendor}", "build", ":runtime:allTestJars",
         ":runtime:thirdPartyLicenses", ":api:publishApiPublicationToSdkRepository"], cwd=ROOT / "java", env=env)
    run([cmake, "--preset", "conan-relwithdebinfo", "-DENDSTONE_ENABLE_JAVA=ON", "-DENDSTONE_GENERATE_STUBS=OFF",
         "-DFETCHCONTENT_FULLY_DISCONNECTED=ON", "-DCMAKE_GTEST_DISCOVER_TESTS_DISCOVERY_MODE=PRE_TEST",
         f"-DZERNYTH_SANITIZERS={args.sanitizers}",
         f"-DZERNYTH_GRADLE={gradle}", f"-DZERNYTH_JAVA_VENDOR={args.vendor}", f"-DPython_EXECUTABLE={sys.executable}"] + native_runtime_arguments, env=env)
    run([cmake, "--build", str(args.build_dir), "--target", "zernyth_java"], env=env)
    run([cmake, "--build", str(args.build_dir), "--parallel", str(args.jobs)], env=env)
    metadata = check_artifacts(args.build_dir)
    (RESULTS / "build-result.json").write_text(json.dumps({"status": "PASS", "native": metadata, "jdk": jdk}, indent=2), encoding="utf-8")


def altered_manifest(source, destination):
    with zipfile.ZipFile(source) as jar, zipfile.ZipFile(destination, "w") as out:
        for item in jar.infolist():
            data = jar.read(item)
            if item.filename == "META-INF/MANIFEST.MF":
                data = data.replace(version().encode(), b"0.0.0-invalid")
            out.writestr(item, data)


def test_env(args):
    env, jdk = java_env(args.java_home)
    env.pop("ZERNYTH_LIFECYCLE_CYCLES", None)
    env.pop("ZERNYTH_LIFECYCLE_PAUSE_MS", None)
    artifacts, v = java_artifacts(), version()
    for path in artifacts.values():
        if not path.is_file():
            raise ValueError(f"Missing artifact {path.name}")
    home = Path(env["JAVA_HOME"])
    env["ZERNYTH_JVM_LIBRARY"] = str(home / ("bin/server/jvm.dll" if os.name == "nt" else "lib/server/libjvm.so"))
    env["ZERNYTH_JAVA_CLASSPATH"] = os.pathsep.join(str(p) for p in (artifacts["api"], artifacts["runtime"], ROOT / f"java/runtime/build/libs/runtime-{v}-benchmark-support.jar"))
    env["ZERNYTH_TEST_PLUGIN_JAR"] = str(artifacts["example"])
    for key, classifier in FIXTURES.items():
        env[key] = str(ROOT / f"java/runtime/build/libs/runtime-{v}-{classifier}.jar")
    env["ZERNYTH_ECOSYSTEM_JARS"] = str(ROOT / "java/runtime/build/libs")
    env["ZERNYTH_PERF_OUTPUT"] = str(RESULTS / "benchmark.json")
    env["JAVA_TOOL_OPTIONS"] = "-Xms64m -Xmx256m -XX:ActiveProcessorCount=4 -Xcheck:jni"
    RESULTS.mkdir(parents=True, exist_ok=True)
    bad_api, bad_runtime = RESULTS / "api-mismatch.jar", RESULTS / "runtime-mismatch.jar"
    altered_manifest(artifacts["api"], bad_api)
    altered_manifest(artifacts["runtime"], bad_runtime)
    env["ZERNYTH_API_MISMATCH_CLASSPATH"] = os.pathsep.join(map(str, (bad_api, artifacts["runtime"])))
    env["ZERNYTH_NATIVE_MISMATCH_CLASSPATH"] = os.pathsep.join(map(str, (bad_api, bad_runtime)))
    return env, jdk


def host_test_names(text):
    """Parse GoogleTest discovery without starting any JVM or trusting shell fragments."""
    suite = ""
    names = []
    for raw in text.splitlines():
        line = raw.split("#", 1)[0].strip()
        if not line:
            continue
        if not raw[0].isspace() and line.endswith("."):
            suite = line
        elif raw[0].isspace() and suite:
            names.append(suite + line)
    if not names or len(names) != len(set(names)):
        raise ValueError("Empty or duplicate native test discovery")
    return names


def tests_with_host(args, env, jdk):
    """Use the same native test sources with one fresh Python/DLL process per case."""
    library = workspace(args.test_host)
    command = [sys.executable, str(ROOT / "scripts/zernyth_jni_host.py"), "--library", library, "--"]
    directory = args.build_dir / "tests"
    discovery = run(command + ["--gtest_list_tests", "--gtest_color=no"], cwd=directory, env=env, capture=True)
    names = host_test_names(discovery.stdout)
    host_results = RESULTS / "host-tests"
    host_results.mkdir(exist_ok=True)
    combined, failed, blocked = [], [], []
    xml = ET.Element("testsuites", tests=str(len(names)))
    suite = ET.SubElement(xml, "testsuite", name="native-host", tests=str(len(names)))
    deadline = time.monotonic() + 900
    for index, name in enumerate(names):
        report = host_results / f"test-{index:03d}.xml"
        if report.exists():
            report.unlink()
        remaining = min(60, deadline - time.monotonic())
        if remaining <= 0:
            raise TimeoutError("Native test-host suite exceeded its 900-second limit")
        result = subprocess.run([str(value) for value in command] + [
            "--gtest_filter=" + name, "--gtest_color=no", "--gtest_output=xml:" + str(report)],
            cwd=directory, env=env, capture_output=True, text=True, encoding="utf-8", errors="replace",
            timeout=remaining, check=False)
        output = result.stdout + result.stderr
        combined.append(output)
        (host_results / f"test-{index:03d}.log").write_text(output, encoding="utf-8")
        cases = ET.parse(report).getroot().findall(".//testcase") if report.is_file() else []
        passed = (result.returncode == 0 and len(cases) == 1
                  and not any(cases[0].find(tag) is not None for tag in ("skipped", "failure", "error")))
        status = "PASS"
        if not passed:
            if "LoadLibrary failed with code 4551" in output or "[WinError 4551]" in output:
                blocked.append(name)
                status = "BLOCKED"
            else:
                failed.append(name)
                status = "FAIL"
        if len(cases) == 1:
            suite.append(cases[0])
        else:
            case = ET.SubElement(suite, "testcase", name=name)
            ET.SubElement(case, "error", message="Native host did not report exactly one test").text = output
        print(f"{status} {index + 1}/{len(names)} {name}", flush=True)
    text = "\n".join(combined)
    diagnostics = len(re.findall(
        r"WARNING in native method|WARNING: JNI|FATAL ERROR in native method|JNI DETECTED ERROR", text))
    status = "FAIL" if failed or diagnostics else "BLOCKED" if blocked else "PASS"
    (RESULTS / "native-host-final.log").write_text(text, encoding="utf-8")
    suite.set("failures", str(len(failed) + len(blocked)))
    ET.ElementTree(xml).write(RESULTS / "host-tests.xml", encoding="utf-8", xml_declaration=True)
    result = {"status": status, "tests": len(names), "passed": len(names) - len(failed) - len(blocked),
              "failed": failed, "blocked": blocked, "jdk": jdk,
              "host": "Python and standard Windows DLL loader",
              "jniProcesses": text.count("Picked up JAVA_TOOL_OPTIONS:"), "jniCheckerDiagnostics": diagnostics}
    (RESULTS / "host-test-result.json").write_text(json.dumps(result, indent=2), encoding="utf-8")
    if status != "PASS":
        raise RuntimeError(f"Native host gate {status}: {len(failed)} failed, {len(blocked)} blocked, "
                           f"{diagnostics} JNI diagnostics; see host-test-result.json")


def tests(args):
    check_artifacts(args.build_dir)
    env, jdk = test_env(args)
    if getattr(args, "test_host", None):
        return tests_with_host(args, env, jdk)
    xml = RESULTS / "ctest.xml"
    run([tool("ctest", args.ctest), "--test-dir", args.build_dir, "--output-on-failure", "--parallel", str(args.jobs),
         "--timeout", "60", "--output-junit", xml], env=env, timeout=900)
    root = ET.parse(xml).getroot()
    cases = root.findall(".//testcase")
    if not cases or any(case.find("skipped") is not None or case.find("failure") is not None for case in cases):
        raise ValueError("No tests, skipped tests or failures: gate is not PASS")
    log = args.build_dir / "Testing/Temporary/LastTest.log"
    text = log.read_text(encoding="utf-8", errors="replace")
    if re.search(r"WARNING in native method|WARNING: JNI|FATAL ERROR in native method|JNI DETECTED ERROR", text):
        raise ValueError("JNI checker diagnostic found")
    shutil.copy2(log, RESULTS / "native-final.log")
    (RESULTS / "test-result.json").write_text(json.dumps({"status": "PASS", "tests": len(cases), "jdk": jdk,
        "jniProcesses": text.count("Picked up JAVA_TOOL_OPTIONS:"), "jniCheckerDiagnostics": 0}, indent=2), encoding="utf-8")


def profile(name):
    values = json.loads((ROOT / "config/zernyth-validation-profiles.json").read_text())[name]
    limits = {"cycles": (1, 50), "pauseMillis": (0, 1000), "timeoutSeconds": (1, 600), "heapMiB": (64, 512), "rssMiB": (256, 1536), "cpus": (1, 4)}
    if set(values) != set(limits) or any(type(values[k]) is not int or not lo <= values[k] <= hi for k, (lo, hi) in limits.items()):
        raise ValueError("Invalid bounded profile")
    return values


def regressions(args):
    env, jdk = test_env(args)
    directory = Path(os.environ.get("ZERNYTH_LEGACY_JARS", ROOT / "java/example/build/libs"))
    records = {}
    for v in ("0.2.0", "0.3.0", "0.4.0", "0.5.0", "0.6.0", "0.7.0", "0.8.0"):
        jar = directory / f"example-{v}.jar"
        if not jar.is_file():
            raise ValueError(f"BLOCKED: missing trusted first-party legacy artifact {jar.name}")
        env["ZERNYTH_TEST_PLUGIN_JAR"] = str(jar.resolve())
        if getattr(args, "test_host", None):
            result = run([sys.executable, ROOT / "scripts/zernyth_jni_host.py", "--library",
                          workspace(args.test_host), "--",
                          "--gtest_filter=JavaPluginLoaderTest.ProductionBridgeThroughExistingManager"],
                         cwd=args.build_dir / "tests", env=env, timeout=60, capture=True)
            text = result.stdout + result.stderr
        else:
            run([tool("ctest", args.ctest), "--test-dir", args.build_dir, "--output-on-failure", "--timeout", "60",
                 "-R", "^JavaPluginLoaderTest\\."], env=env, timeout=120)
            log = args.build_dir / "Testing/Temporary/LastTest.log"
            text = log.read_text(encoding="utf-8", errors="replace")
        if "[  PASSED  ] 1 test." not in text:
            raise ValueError("Legacy fixture was not exercised")
        if re.search(r"WARNING in native method|WARNING: JNI|FATAL ERROR in native method|JNI DETECTED ERROR", text):
            raise ValueError("Legacy fixture emitted JNI checker diagnostics")
        (RESULTS / f"regression-{v}.log").write_text(text, encoding="utf-8")
        records[v] = {"status": "PASS", "artifact": jar.name, "sha256": digest(jar)}
    (RESULTS / "regression-result.json").write_text(json.dumps({"status": "PASS", "jdk": jdk, "artifacts": records}, indent=2), encoding="utf-8")


def stability(args):
    import psutil  # Existing Endstone dependency, never installed here.
    limits = profile(args.profile)
    env, jdk = test_env(args)
    env["ZERNYTH_LIFECYCLE_CYCLES"] = str(limits["cycles"])
    env["ZERNYTH_LIFECYCLE_PAUSE_MS"] = str(limits["pauseMillis"])
    env["JAVA_TOOL_OPTIONS"] = f'-Xms64m -Xmx{limits["heapMiB"]}m -XX:ActiveProcessorCount={limits["cpus"]} -Xcheck:jni'
    executable = args.build_dir / "tests" / ("endstone_test.exe" if os.name == "nt" else "endstone_test")
    command = ([sys.executable, ROOT / "scripts/zernyth_jni_host.py", "--library", workspace(args.test_host), "--"]
               if getattr(args, "test_host", None) else [executable])
    command = [str(value) for value in command]
    started, peak = time.monotonic(), 0
    with (RESULTS / f"stability-{args.profile}.log").open("w", encoding="utf-8") as log:
        proc = subprocess.Popen(command + ["--gtest_filter=JavaPluginLoaderTest.ProductionBridgeThroughExistingManager"],
                                cwd=args.build_dir / "tests", env=env, stdout=log, stderr=subprocess.STDOUT,
                                creationflags=subprocess.CREATE_NO_WINDOW if os.name == "nt" else 0)
        try:
            process = psutil.Process(proc.pid)
            process.cpu_affinity(process.cpu_affinity()[:limits["cpus"]])
            while proc.poll() is None:
                try:
                    peak = max(peak, process.memory_info().rss)
                except psutil.NoSuchProcess:
                    break
                if peak > limits["rssMiB"] * 1024**2 or time.monotonic() - started > limits["timeoutSeconds"]:
                    raise RuntimeError("Stability profile resource limit exceeded")
                time.sleep(.1)
            if proc.wait() != 0:
                raise RuntimeError("Stability fixture failed")
        finally:
            if proc.poll() is None:
                proc.kill()
            proc.wait()
    text = (RESULTS / f"stability-{args.profile}.log").read_text(encoding="utf-8", errors="replace")
    if "[  PASSED  ] 1 test." not in text or "WARNING in native method" in text or "FATAL ERROR in native method" in text:
        raise ValueError("Expected one exercised lifecycle case without JNI diagnostics")
    (RESULTS / f"stability-{args.profile}.json").write_text(json.dumps({"status": "PASS", "profile": limits,
        "jdk": jdk, "peakRssBytes": peak, "wallSeconds": time.monotonic()-started}, indent=2), encoding="utf-8")


def required_files(v, system, module):
    base = {"VERSION", "BUILD.json", "README.md", "LICENSES/Endstone-LICENSE", "bin/run_existing.py", "bin/zernyth_bds.py",
            f"java/api-{v}.jar", f"java/runtime-{v}.jar", "examples/HelloZernyth.jar",
            "lib/python/endstone/__init__.py", "lib/python/endstone/_version.py", f"lib/python/endstone/{module}"}
    if system == "Windows":
        return base | {"lib/python/endstone/endstone_runtime.dll", "lib/python/endstone/endstone_runtime_loader.dll",
                       "lib/python/endstone/" + module.replace("_python.", "_detours.", 1),
                       "lib/python/endstone/crashpad_handler.exe", "lib/python/endstone/crashpad_wer.dll"}
    if system == "Linux":
        return base | {"lib/python/endstone/libendstone_runtime.so", "lib/python/endstone/libmimalloc.so", "lib/python/endstone/crashpad_handler"}
    raise ValueError("Unsupported package platform")


def safe_name(name):
    path = PurePosixPath(name)
    if path.is_absolute() or ".." in path.parts or "\\" in name or ":" in name or path.as_posix() != name:
        raise ValueError("Unsafe archive path")
    if path.name.lower() in ("bedrock_server.exe", "bedrock_server") or path.suffix.lower() == ".pdb":
        raise ValueError("Forbidden distribution content")
    if "__pycache__" in path.parts or path.suffix.lower() == ".pyc":
        raise ValueError("Developer bytecode cache must not enter a release")


def reproducible_zip(stage, output):
    paths = sorted(p for p in stage.rglob("*") if p.is_file())
    with Path(output).open("xb") as stream, zipfile.ZipFile(stream, "w", zipfile.ZIP_DEFLATED, compresslevel=9) as archive:
        for path in paths:
            if path.is_symlink():
                raise ValueError("Symlink in package")
            name = stage.name + "/" + path.relative_to(stage).as_posix()
            safe_name(name)
            item = zipfile.ZipInfo(name, (1980, 1, 1, 0, 0, 0))
            item.compress_type = zipfile.ZIP_DEFLATED
            item.external_attr = ((0o100755 if path.name == "crashpad_handler" else 0o100644) << 16)
            archive.writestr(item, path.read_bytes(), compresslevel=9)


def verify_archive(path):
    with zipfile.ZipFile(path) as archive:
        entries = archive.infolist()
        if len(entries) > 20000 or sum(x.file_size for x in entries) > 1024**3:
            raise ValueError("Archive resource limit")
        names = [x.filename for x in entries]
        if len(names) != len(set(names)):
            raise ValueError("Duplicate archive entries")
        for item in entries:
            safe_name(item.filename)
            if (item.external_attr >> 16) & 0o170000 == 0o120000:
                raise ValueError("Archive symlink")
        roots = {n.split("/")[0] for n in names}
        if len(roots) != 1:
            raise ValueError("Expected one distribution root")
        prefix = roots.pop() + "/"
        def read(name, limit=1024*1024):
            item = archive.getinfo(prefix + name)
            if item.file_size > limit:
                raise ValueError("Metadata too large")
            return archive.read(item)
        manifest = json.loads(read("manifest.json"))
        v = read("VERSION").decode().strip()
        if v != manifest["version"] or not re.fullmatch(r"\d+\.\d+\.\d+(?:-(?:rc|alpha)\.\d+)?", v):
            raise ValueError("Package version mismatch")
        files = manifest["files"]
        actual = {n[len(prefix):] for n in names}
        if actual != set(files) | {"manifest.json", "SHA256SUMS"}:
            raise ValueError("Unexpected or missing package files")
        if not required_files(v, manifest["platform"], manifest["pythonModule"]).issubset(files):
            raise ValueError("Missing runtime payload")
        for name, expected in files.items():
            with archive.open(prefix + name) as stream:
                if hashlib.file_digest(stream, "sha256").hexdigest() != expected:
                    raise ValueError("Checksum mismatch: " + name)
        checksum_text = "".join(f"{files[n]}  {n}\n" for n in sorted(files))
        if read("SHA256SUMS", 4*1024*1024).decode().splitlines() != checksum_text.splitlines():
            raise ValueError("Checksum inventory mismatch")
        build = json.loads(read("BUILD.json"))
        if build["zernyth"] != v:
            raise ValueError("Native metadata mismatch")
        for name in (f"java/api-{v}.jar", f"java/runtime-{v}.jar", "examples/HelloZernyth.jar"):
            data = read(name, 64*1024*1024)
            if jar_version(io.BytesIO(data)) != v:
                raise ValueError("JAR version mismatch")
        with zipfile.ZipFile(io.BytesIO(read(f"java/api-{v}.jar", 64*1024*1024))) as jar:
            info = jar.getinfo("dev/zernyth/api/Server.class")
            if info.file_size > 4*1024*1024:
                raise ValueError("API class too large")
            header = jar.read(info)[:8]
            if header[:4] != b"\xca\xfe\xba\xbe" or int.from_bytes(header[6:8], "big") != 69:
                raise ValueError("API is not Java 25")
        return {"status": "PASS", "version": v, "files": len(files), "archiveSha256": digest(path)}


def package(args):
    metadata = check_artifacts(args.build_dir)
    if metadata.get("sanitizers"):
        raise ValueError("Instrumented native builds cannot become release candidates")
    v = version()
    output = workspace(args.output or ROOT / f"build/zernyth-engine-{v}-{platform.system()}.zip")
    output.parent.mkdir(parents=True, exist_ok=True)
    if output.exists():
        raise ValueError("Refusing to replace an existing release archive")
    temp_root = workspace(ROOT / "build/release-staging")
    temp_root.mkdir(parents=True, exist_ok=True)
    # TemporaryDirectory cleanup is scoped to this checked workspace parent.
    with tempfile.TemporaryDirectory(dir=temp_root) as temporary:
        stage = Path(temporary) / f"zernyth-engine-{v}-{platform.system()}"
        stage.mkdir()
        run([tool("cmake", args.cmake), "--install", args.build_dir, "--prefix", stage / "lib/python", "--component", "endstone_wheel"])
        native = stage / "lib/python/endstone"
        for pdb in native.glob("*.pdb"):
            pdb.unlink()  # Only files in this newly owned stage, never source artifacts.
        tracked = run(["git", "ls-files", "-z", "--", "endstone"], capture=True).stdout.split("\0")
        for relative in tracked:
            if not relative:
                continue
            source = ROOT / relative
            if source.suffix not in (".py", ".pyi", ".toml") and source.name != "py.typed":
                continue
            if source.is_symlink():
                raise ValueError("Source package symlink")
            destination = stage / "lib/python" / relative
            destination.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(source, destination)
        (native / "_version.py").write_text(f'__version__ = {metadata["endstoneBase"]!r}\n', encoding="utf-8")
        modules = [p for p in native.glob("_python.*") if p.suffix in (".pyd", ".so")]
        if len(modules) != 1:
            raise ValueError("Exactly one Python extension required")
        module = modules[0].name
        python_version = metadata["pythonVersion"]
        if not python_version.startswith(f"{sys.version_info.major}.{sys.version_info.minor}."):
            raise ValueError("Packaging Python must match native Python ABI")
        for name in ("java", "examples", "plugins", "config", "docs", "LICENSES", "bin"):
            (stage / name).mkdir(exist_ok=True)
        for name, source in java_artifacts().items():
            shutil.copy2(source, stage / ("examples/HelloZernyth.jar" if name == "example" else "java/" + source.name))
        sdk = ROOT / f"java/build/sdk-repository/dev/zernyth/api/{v}"
        if not sdk.is_dir():
            raise ValueError("Missing locally published SDK")
        shutil.copytree(sdk, stage / f"sdk-repository/dev/zernyth/api/{v}")
        template = ROOT / "zernyth-plugin-template"
        for source in template.rglob("*"):
            relative = source.relative_to(template)
            if source.is_file() and (relative.parts[0] == "src" or len(relative.parts) == 1):
                if source.suffix not in (".java", ".yml", ".kts", ".xml", ".md"):
                    continue
                target = stage / "examples/plugin-template" / relative
                target.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(source, target)
        shutil.copy2(ROOT / "LICENSE", stage / "LICENSES/Endstone-LICENSE")
        shutil.copy2(ROOT / "scripts/zernyth_run_existing.py", stage / "bin/run_existing.py")
        shutil.copy2(ROOT / "scripts/zernyth_bds.py", stage / "bin/zernyth_bds.py")
        shutil.copy2(ROOT / "config/zernyth-validation-profiles.json", stage / "config/validation-profiles.json")
        for guide in ("zernyth-release-0.9.md", "zernyth-security-0.8.md", "zernyth-developer-0.7.md", "zernyth-runtime-0.6.md"):
            shutil.copy2(ROOT / "docs" / guide, stage / "docs" / guide)
        licenses = ROOT / "java/runtime/build/third-party-licenses"
        if not licenses.is_dir():
            raise ValueError("Run :runtime:thirdPartyLicenses first")
        shutil.copytree(licenses, stage / "LICENSES/java")
        graph = json.loads((RESULTS / "dependency-graph.json").read_text())
        inventory = []
        for node in graph["graph"]["nodes"].values():
            if not node.get("ref") or not node.get("package_folder"):
                continue
            ref = node["ref"]
            key = re.sub(r"[^A-Za-z0-9_.-]", "_", ref.split("#")[0]) + "-" + hashlib.sha256(ref.encode()).hexdigest()[:12]
            source = Path(node["package_folder"]) / "licenses"
            if source.is_dir():
                shutil.copytree(source, stage / "LICENSES/native" / key)
            inventory.append({"reference": ref, "licenseFilesPresent": source.is_dir()})
        (stage / "LICENSES/dependencies.json").write_text(json.dumps(inventory, indent=2), encoding="utf-8")
        if not inventory:
            raise ValueError("Missing native dependency/license inventory")
        shutil.copy2(RESULTS / "conan.lock", stage / "DEPENDENCIES.lock")
        metadata["sourceRevision"] = run(["git", "rev-parse", "HEAD"], capture=True).stdout.strip()
        metadata["workspaceDirty"] = bool(run(["git", "status", "--porcelain"], capture=True).stdout.strip())
        metadata["conanSettings"] = graph["graph"]["nodes"]["0"]["settings"]
        gates = {"bdsRuntime": "BLOCKED", "gameplayWithClient": "NOT TESTED", "linux": "NOT TESTED",
                 "temurin": "NOT TESTED", "fullCoreSanitizers": "NOT TESTED", "windowsLeakDetection": "BLOCKED", "licenseReview": "NOT TESTED",
                 "ciExecution": "NOT TESTED", "nativeBitReproducibility": "NOT TESTED", "releaseReady": "BLOCKED"}
        for name, filename in (("localBuild", "build-result.json"), ("localNativeJniTests", "test-result.json"), ("legacyPlugins", "regression-result.json")):
            evidence = RESULTS / filename
            gates[name] = json.loads(evidence.read_text())["status"] if evidence.exists() else "NOT TESTED"
        sanitizer_evidence = RESULTS / "sanitizer-result.json"
        if sanitizer_evidence.exists():
            evidence = json.loads(sanitizer_evidence.read_text())
            for kind in ("handle_sanitizer", "handle_ubsan"):
                gates[kind] = evidence.get(kind, {}).get("status", "NOT TESTED")
        (stage / "QUALITY-GATES.json").write_text(json.dumps(gates, indent=2), encoding="utf-8")
        (stage / "VERSION").write_text(v + "\n", encoding="utf-8")
        (stage / "BUILD.json").write_text(json.dumps(metadata, indent=2), encoding="utf-8")
        (stage / "README.md").write_text(f"# Zernyth Engine {v}\n\nRelease candidate, not a stable release.\n"
            "BDS and JDK are NOT included. See docs/zernyth-release-0.9.md.\n"
            "No updater, downloads or server starts run during installation/verification.\n", encoding="utf-8")
        (stage / "plugins/README.md").write_text("Empty by design. Enable examples explicitly in a separately approved test server.\n", encoding="utf-8")
        # Verify imports against staged modules, never the developer checkout or installed Endstone.
        code = "import sys;sys.path.insert(0,sys.argv[1]);import endstone._python as m;from pathlib import Path;assert Path(m.__file__).resolve().is_relative_to(Path(sys.argv[1]).resolve());print('Staged Python extension import PASS')"
        run([sys.executable, "-I", "-B", "-c", code, stage / "lib/python"], timeout=60)
        files = {p.relative_to(stage).as_posix(): digest(p) for p in sorted(stage.rglob("*")) if p.is_file()}
        if not required_files(v, platform.system(), module).issubset(files):
            raise ValueError("Required payload missing")
        (stage / "manifest.json").write_text(json.dumps({"version": v, "platform": platform.system(), "pythonModule": module,
            "releaseStatus": "CANDIDATE", "files": files}, sort_keys=True, indent=2), encoding="utf-8")
        (stage / "SHA256SUMS").write_text("".join(f"{files[n]}  {n}\n" for n in sorted(files)), encoding="utf-8")
        reproducible_zip(stage, output)
        # Same inputs must produce the same ZIP bytes in this Python/compression environment.
        repeat = Path(temporary) / "repeat.zip"
        reproducible_zip(stage, repeat)
        if digest(output) != digest(repeat):
            raise ValueError("ZIP reproduction mismatch")
    result = verify_archive(output)
    output.with_suffix(output.suffix + ".sha256").write_text(result["archiveSha256"] + "  " + output.name + "\n", encoding="utf-8")
    (RESULTS / "package-result.json").write_text(json.dumps(result, indent=2), encoding="utf-8")
    print(json.dumps(result))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("build", "test", "stability", "regression", "package", "verify", "reproduce-java"))
    parser.add_argument("--java-home")
    parser.add_argument("--gradle")
    parser.add_argument("--cmake")
    parser.add_argument("--ctest")
    parser.add_argument("--test-host", type=Path, help="Optional first-party Windows test DLL, fresh host per case")
    parser.add_argument("--conan")
    parser.add_argument("--lockfile", type=Path, help="Explicit reviewed input Conan lockfile")
    parser.add_argument("--vendor", choices=("temurin", "any"), default="temurin")
    parser.add_argument("--sanitizers", choices=("", "address,undefined"), default="")
    parser.add_argument("--build-dir", type=Path, default=ROOT / "build/RelWithDebInfo")
    parser.add_argument("--jobs", type=int, choices=range(1, 9), default=4)
    parser.add_argument("--profile", choices=("smoke", "lifecycle", "bounded-stress", "observation"), default="smoke")
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    args.build_dir = workspace(args.build_dir)
    if args.command == "verify":
        print(json.dumps(verify_archive(args.output)))
    elif args.command == "reproduce-java":
        env, jdk = java_env(args.java_home)
        before = {k: digest(v) for k, v in java_artifacts().items()}
        run([tool("gradle", args.gradle), "--offline", "--no-daemon", f"-PzernythJavaVendor={args.vendor}",
             "--rerun-tasks", ":api:jar", ":runtime:jar", ":example:jar"], cwd=ROOT / "java", env=env)
        after = {k: digest(v) for k, v in java_artifacts().items()}
        if before != after:
            raise ValueError("Java artifact reproduction mismatch")
        (RESULTS / "java-reproduction.json").write_text(json.dumps({"status": "PASS", "jdk": jdk, "sha256": after}, indent=2))
    else:
        {"build": build_all, "test": tests, "stability": stability, "regression": regressions, "package": package}[args.command](args)


if __name__ == "__main__":
    main()
