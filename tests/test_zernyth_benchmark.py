"""Synthetic tests for benchmark limits and aggregation. No server processes are launched."""

import argparse
import io
import importlib.util
from pathlib import Path
import sys
import tempfile
from types import SimpleNamespace
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))
spec = importlib.util.spec_from_file_location("zernyth_benchmark", ROOT / "scripts/zernyth_benchmark.py")
benchmark = importlib.util.module_from_spec(spec)
spec.loader.exec_module(benchmark)
sys.path.pop(0)


class BenchmarkTests(unittest.TestCase):
    def test_invalid_heap_profile_never_starts_process(self):
        argv = ["zernyth_benchmark.py", "--java-home", str(ROOT), "--runtime", str(ROOT / "unused.jar"),
                "--output", str(ROOT / "build/unused-benchmark"), "--heap-mib", "128",
                "--initial-heap-mib", "256"]
        with mock.patch.object(sys, "argv", argv), mock.patch.object(sys, "stderr", io.StringIO()):
            with mock.patch.object(benchmark.subprocess, "Popen") as launch:
                with self.assertRaises(SystemExit) as failure:
                    benchmark.main()
        self.assertEqual(failure.exception.code, 2)
        launch.assert_not_called()

    def test_empty_benchmark_cannot_pass(self):
        with self.assertRaisesRegex(ValueError, "No benchmark"):
            benchmark.summarize([])
        with self.assertRaisesRegex(ValueError, "No benchmark"):
            benchmark.summarize([{"benchmark": {"metrics": []}}])

    def test_resource_bounds(self):
        self.assertEqual(benchmark.bounded("4", 1, 4), 4)
        for value in ("0", "5"):
            with self.assertRaises(argparse.ArgumentTypeError):
                benchmark.bounded(value, 1, 4)

    def test_fork_aggregation_preserves_uncertainty(self):
        records = [{"benchmark": {"metrics": [{"name": "probe", "medianNs": value,
                    "p95BatchMeanNs": value * 2, "javaAllocatedBytesPerOperation": 16}]}}
                   for value in (10, 30, 20)]
        row = benchmark.summarize(records)[0]
        self.assertEqual(row["medianNs"], 20)
        self.assertEqual(row["forkMedianRangeNs"], [10, 30])
        self.assertEqual(row["medianP95BatchMeanNs"], 40)
        self.assertEqual(row["javaAllocatedBytesPerOperation"], 16)

    def test_rss_limit_terminates_and_reaps_process(self):
        process = mock.Mock(pid=123)
        process.poll.return_value = None
        observed = mock.Mock()
        observed.cpu_affinity.return_value = [0, 1]
        observed.memory_info.return_value = SimpleNamespace(rss=200, private=150)
        observed.num_threads.return_value = 2
        observed.cpu_times.return_value = SimpleNamespace(user=0.1, system=0.1)
        with tempfile.TemporaryDirectory() as temporary:
            with mock.patch.object(benchmark.subprocess, "Popen", return_value=process):
                with mock.patch.object(benchmark.psutil, "Process", return_value=observed):
                    with self.assertRaisesRegex(RuntimeError, "RSS limit"):
                        benchmark.capture(["unused"], {}, ROOT, Path(temporary) / "log", 1, 100, 2)
        process.kill.assert_called_once()
        process.wait.assert_called_once()


if __name__ == "__main__":
    unittest.main()
