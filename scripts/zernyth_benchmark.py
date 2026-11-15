"""Bounded local Java/JNI benchmarks with process-level resource observations.

Requires existing build artifacts and psutil. Never downloads dependencies or starts BDS.
CPU observations cover the whole process, including startup, JIT and cleanup.
"""

import argparse
import json
import os
from pathlib import Path
import statistics
import subprocess
import sys
import time

import psutil

import zernyth_release as release

ROOT = Path(__file__).resolve().parents[1]


def bounded(value, lower, upper):
    number = int(value)
    if not lower <= number <= upper:
        raise argparse.ArgumentTypeError(f"Expected {lower}..{upper}")
    return number


def capture(command, env, directory, log, timeout, rss_limit, cpus):
    """Observe a single fresh process and stop it if a configured limit is exceeded."""
    started = time.monotonic()
    peak_rss = peak_private = peak_threads = 0
    cpu_seconds = 0.0
    with log.open("w", encoding="utf-8") as stream:
        process = subprocess.Popen(command, cwd=directory, env=env, stdout=stream,
                                   stderr=subprocess.STDOUT)
        try:
            observed = psutil.Process(process.pid)
            affinity = observed.cpu_affinity()[:cpus] if hasattr(observed, "cpu_affinity") else []
            if affinity:
                observed.cpu_affinity(affinity)
            while process.poll() is None:
                try:
                    memory = observed.memory_info()
                    peak_rss = max(peak_rss, memory.rss)
                    peak_private = max(peak_private, getattr(memory, "private", 0))
                    peak_threads = max(peak_threads, observed.num_threads())
                    cpu = observed.cpu_times()
                    cpu_seconds = max(cpu_seconds, cpu.user + cpu.system)
                except psutil.NoSuchProcess:
                    break
                if peak_rss > rss_limit:
                    raise RuntimeError("Benchmark exceeded its process RSS limit")
                if time.monotonic() - started > timeout:
                    raise TimeoutError("Benchmark exceeded its process deadline")
                time.sleep(0.02)
            if process.wait() != 0:
                raise RuntimeError(f"Benchmark failed; see {log.name}")
        finally:
            if process.poll() is None:
                process.kill()
            process.wait()
    wall = time.monotonic() - started
    return {"processWallSeconds": wall, "sampledCpuSeconds": cpu_seconds,
            "averageCpuCoreEquivalents": cpu_seconds / wall, "peakSampledRssBytes": peak_rss,
            "peakSampledPrivateBytes": peak_private or None, "peakSampledThreads": peak_threads,
            "cpuAffinity": affinity, "samplingIntervalMillis": 20}


def summarize(records):
    """Aggregate process medians, retaining independent-fork ranges and batch P95."""
    if not records or not records[0]["benchmark"]["metrics"]:
        raise ValueError("No benchmark cases were measured")
    names = [metric["name"] for metric in records[0]["benchmark"]["metrics"]]
    rows = []
    for name in names:
        values = [next(m for m in record["benchmark"]["metrics"] if m["name"] == name)
                  for record in records]
        medians = [value["medianNs"] for value in values]
        allocated = [value["javaAllocatedBytesPerOperation"] for value in values
                     if value.get("javaAllocatedBytesPerOperation") is not None]
        rows.append({"name": name, "medianNs": statistics.median(medians),
                     "forkMedianRangeNs": [min(medians), max(medians)],
                     "medianP95BatchMeanNs": statistics.median(v["p95BatchMeanNs"] for v in values),
                     "javaAllocatedBytesPerOperation": statistics.median(allocated) if allocated else None})
    return rows


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--java-home", type=Path, required=True)
    parser.add_argument("--runtime", type=Path, required=True, help="Matching local runtime JAR")
    parser.add_argument("--baseline-runtime", type=Path,
                        help="Optional matching baseline JAR; alternates before/after process order")
    parser.add_argument("--output", type=Path, required=True, help="New evidence directory under build/")
    parser.add_argument("--scenario", choices=("java", "jni"), default="java")
    parser.add_argument("--filter", help="Optional literal Java case-name substring")
    parser.add_argument("--forks", type=lambda v: bounded(v, 1, 7), default=5)
    parser.add_argument("--cpus", type=lambda v: bounded(v, 1, 4), default=4)
    parser.add_argument("--heap-mib", type=int, choices=(128, 256, 512), default=512)
    parser.add_argument("--initial-heap-mib", type=int, choices=(32, 64, 128, 256), default=256)
    args = parser.parse_args()
    if args.filter and (args.scenario != "java" or len(args.filter) > 80):
        parser.error("Case filtering requires Java mode and at most 80 characters")
    if args.initial_heap_mib > args.heap_mib:
        parser.error("Initial heap must not exceed maximum heap")
    runtime = args.runtime.resolve()
    output = args.output.resolve()
    if not runtime.is_relative_to(ROOT) or not runtime.is_file():
        parser.error("Runtime must be an existing repository artifact")
    baseline = args.baseline_runtime.resolve() if args.baseline_runtime else None
    if baseline and (not baseline.is_relative_to(ROOT) or not baseline.is_file()):
        parser.error("Baseline must be an existing repository artifact")
    if not output.is_relative_to(ROOT / "build") or output.exists():
        parser.error("Output must be a new directory under build/")
    java = args.java_home / "bin" / ("java.exe" if os.name == "nt" else "java")
    if not java.is_file():
        parser.error("An existing local JDK is required")
    version = release.version()
    api = ROOT / f"java/api/build/libs/api-{version}.jar"
    support = ROOT / f"java/runtime/build/libs/runtime-{version}-benchmark-support.jar"
    plugin = ROOT / f"java/runtime/build/libs/runtime-{version}-benchmark-plugin.jar"
    lifecycle = ROOT / f"java/runtime/build/libs/runtime-{version}-lifecycle-benchmark.jar"
    required = [api, plugin, lifecycle] + ([support] if args.scenario == "jni" else [])
    if any(not path.is_file() for path in required):
        parser.error("Build the existing benchmark fixtures first")
    env = os.environ.copy()
    for key in ("JAVA_TOOL_OPTIONS", "JDK_JAVA_OPTIONS", "_JAVA_OPTIONS"):
        env.pop(key, None)
    flags = [f"-Xms{args.initial_heap_mib}m", f"-Xmx{args.heap_mib}m",
             f"-XX:ActiveProcessorCount={args.cpus}", "-Xbatch",
             "-Dzernyth.benchmark.warmup=15", "-Dzernyth.benchmark.rounds=30",
             "-Dzernyth.benchmark.scale=4"]
    if args.filter:
        flags.append("-Dzernyth.benchmark.filter=" + args.filter)
    output.mkdir(parents=True)
    records = []
    variants = [("before", baseline), ("after", runtime)] if baseline else [("after", runtime)]
    plans = [(label, artifact, index) for index in range(args.forks)
             for label, artifact in (variants if index % 2 == 0 else list(reversed(variants)))]
    deadline = time.monotonic() + 900
    for label, artifact, index in plans:
        raw = output / f"{label}-{index}.json"
        if args.scenario == "java":
            classpath = os.pathsep.join(map(str, [ROOT / "java/runtime/build/classes/java/test",
                                                artifact, api]))
            command = [str(java), *flags, "-Dzernyth.benchmark.output=" + str(raw), "-cp", classpath,
                       "dev.zernyth.runtime.BenchmarkHarness", str(plugin), str(lifecycle)]
            directory = ROOT
        else:
            env, _ = release.test_env(args)
            for key in ("JDK_JAVA_OPTIONS", "_JAVA_OPTIONS"):
                env.pop(key, None)
            env["JAVA_TOOL_OPTIONS"] = " ".join(flags)
            env["ZERNYTH_JAVA_CLASSPATH"] = os.pathsep.join(map(str, [api, artifact, support]))
            env["ZERNYTH_PERF_OUTPUT"] = str(raw)
            command = [sys.executable, str(ROOT / "scripts/zernyth_jni_host.py"), "--library",
                       str(ROOT / "build/RelWithDebInfo/tests/zernyth_java_test_host.dll"), "--",
                       "--gtest_filter=JavaPerformanceTest.ReproducibleProductionBridgeHarness"]
            directory = ROOT / "build/RelWithDebInfo/tests"
        remaining = min(120, deadline - time.monotonic())
        if remaining <= 0:
            raise TimeoutError("Benchmark suite exceeded its 900-second deadline")
        resource = capture(command, env, directory, output / f"{label}-{index}.log", remaining,
                           1536 * 1024**2, args.cpus)
        benchmark = json.loads(raw.read_text(encoding="utf-8"))
        records.append({"variant": label, "runtimeSha256": release.digest(artifact),
                        "benchmark": benchmark, "resources": resource})
        print(f"PASS {args.scenario} {label} fork {index + 1}/{args.forks}", flush=True)
    result = {"status": "PASS", "scenario": args.scenario, "runtimeSha256": release.digest(runtime),
              "forks": records, "summary": summarize([r for r in records if r["variant"] == "after"]),
              "bds": "NOT TESTED",
              "limits": {"secondsPerFork": 120, "suiteSeconds": 900, "rssMiB": 1536,
                         "heapMiB": args.heap_mib, "initialHeapMiB": args.initial_heap_mib,
                         "cpus": args.cpus},
              "methodology": "Local batch means, no individual-operation tail latency. Resource counters "
                             "cover startup/JIT/cleanup and are sampled. No cross-server comparison."}
    if baseline:
        result["beforeSummary"] = summarize([r for r in records if r["variant"] == "before"])
    (output / "summary.json").write_text(json.dumps(result, indent=2), encoding="utf-8")


if __name__ == "__main__":
    main()
