# Packet metadata performance validation

## Implemented changes

The Java runtime decodes packet metadata through a listener-owned decoder. It reuses
immutable packet types within the existing 0–1023 identifier range and caches at most
64 connection identities per listener. A cache miss still decodes the supplied metadata.
Each callback receives an independent immutable snapshot. Cached values grant no native
access and retain no player objects or native pointers. Removing the callback releases
its decoder through the existing JNI reference lifecycle.

The native adapter indexes connection views by player UUID and validates the current
player, actor ID, runtime ID, connection activity and thread before delivery. Retired
entries are collected when admitting a connection. UUID text is cached per connection
and copied into owned stack storage before entering Java. Disconnect and runtime-ID
changes invalidate the index. Existing version checks, resource limits, native events
and read-only capabilities remain enforced.

Production thread attachment retains its existing attach/detach contract. Measurements
show substantial attachment cost on initially detached workers. A safe long-lived
attachment optimization requires a proven native thread-exit contract and remains open.

## Validation on 2026-11-14

| Gate | Result | Evidence and limits |
| --- | --- | --- |
| Offline Java build | PASS | Gradle 9.8.2, Oracle JDK 25.0.5.0.3, vendor override |
| Windows native build | PASS | Existing RelWithDebInfo configuration, Clang/MSVC environment |
| Native/JNI suite | PASS | 249 cases through the optional Windows test host |
| JNI checker | PASS | Nine suite JVM processes, zero detected checker diagnostics |
| Python regression suite | PASS | 50 tests |
| Archived plugin JARs | PASS | 0.2.0–0.8.0, seven separate checked JNI processes |
| Java formatting | PASS | 146 manifest-selected Java sources |
| Changed C++ formatting | PASS | Four Zernyth-owned files, clang-format dry run |
| BDS performance and gameplay | NOT TESTED | No server process started in this phase |
| Linux and Temurin | NOT TESTED | Results apply to the local Windows/Oracle environment |
| Fresh test executable under application control | BLOCKED | Windows rejects the newly linked executable |

The Java build includes managed-runtime, network-value, lifecycle, service, configuration,
security and stable-test mock checks. Its bounded parser/JAR fuzz checks use 300 seeded
synthetic cases. Expected rejected inputs can produce `FAIL` diagnostic lines inside
negative-test fixtures. The fixture assertions and build outcome determine their result.
Existing `getenv` deprecation warnings remain in native test fixtures.

### Optional native test host

The opt-in `ZERNYTH_BUILD_JAVA_TEST_HOST` CMake option builds the existing native test
sources as `zernyth_java_test_host.dll`. A fresh Python process loads it through the
standard Windows DLL loader for each exact test case. The host has no installation
rule and belongs only to development tests. Windows application control still applies.

```powershell
cmake -S . -B build/RelWithDebInfo -DZERNYTH_BUILD_JAVA_TEST_HOST=ON
cmake --build build/RelWithDebInfo --target zernyth_java_test_host --parallel 4
python scripts/zernyth_release.py test --java-home "$env:JAVA_HOME" `
  --test-host build/RelWithDebInfo/tests/zernyth_java_test_host.dll
```

Use the existing configured compiler environment and offline dependencies. The default
test route remains CTest. The host route enforces one test per process, a 60-second
per-case deadline and a 900-second suite deadline. Skips, failures and JNI diagnostics
prevent a passing gate.

Two C++ plugin-load tests initially encountered another application-control block on
the freshly built test-plugin DLL. Their final run used an earlier first-party CI
fixture with byte-identical test source and all 190 public SDK headers. Its SHA-256 is
`e63dea9b4fd8bf4771cb44ddc3e6f16839f7d34837099c959d21f9ffe0281101`.
The fresh blocked fixture was retained separately. This verifies compatibility with
that cached fixture. Loading the freshly linked fixture remains unverified.

## Benchmark method

The existing `BenchmarkHarness` and productive JNI performance fixture were extended.
Inputs are synthetic and native server objects are mocks. Measurements use Windows 11,
an Intel Core Ultra 7 356H, Oracle JDK 25.0.5.0.3 and the same RelWithDebInfo build.
JVM arguments were `-Xms256m -Xmx512m -XX:ActiveProcessorCount=4 -Xbatch`.
Checker-enabled correctness tests run separately from timing measurements.

Five independent before/after JVM pairs alternate execution order. The Java harness
uses 15 warmup rounds, 30 measured rounds and a scale of four, producing 40,000
operations per metadata sample. The native harness retains three warmup batches and
15 measured batches, usually 2,000 operations per sample. Its embedded Java harness
uses the longer Java settings. Each process has a 90-second deadline.

Tables report the median of five process medians in wall-clock nanoseconds per
operation. P95 values in raw results describe batch means. They do not estimate the
tail latency of individual packets. P99 and statistical confidence claims are omitted.
This local harness has no JMH isolation or dedicated machine reservation. Background
activity and JIT/GC variation contribute to uncertainty. Initial shorter-warmup runs
showed unstable results, including regressions, and are retained with the final data.

### Java metadata conversion

| Workload | Before ns | After ns | Change | Allocated bytes before → after |
| --- | ---: | ---: | ---: | ---: |
| Repeated player | 31.53 | 25.90 | −17.8% | 384 → 296 |
| Anonymous unknown type | 18.02 | 15.29 | −15.2% | 168 → 104 |
| Cycling 64 players | 49.67 | 36.61 | −26.3% | 432 → 344 |
| Cycling 128 players | 50.89 | 44.77 | −12.0% | 432 → 376 |

These cases include fresh input strings representing metadata delivered by JNI.
The 128-player case exceeds the identity-cache capacity and exercises fallback decoding.
Allocation values measure the current Java platform thread through ThreadMXBean.

### Native event → JNI → Java callback

| Workload | Before ns | After ns | Change | Before range | After range |
| --- | ---: | ---: | ---: | --- | --- |
| One packet listener | 2876.95 | 2437.50 | −15.3% | 2627–4468 | 2265–2488 |
| Anonymous packet | 1492.05 | 1412.80 | −5.3% | 1359–2204 | 1337–1461 |
| Cycling 64 players | 3031.05 | 2784.35 | −8.1% | 2701–4433 | 2343–3617 |
| Four packet listeners | 10657.55 | 10038.35 | −5.8% | 9594–11469 | 8442–12265 |

Ranges show the minimum and maximum process medians. Overlapping ranges limit the
strength of timing conclusions. Java allocation for one productive packet callback
fell from approximately 344 to 256 bytes in this fixture. Native heap allocations
are outside this counter. The decoder adds bounded retained state per registration.

Unchanged-path controls include approximately 11.15 → 11.17 ns for synthetic JNI ping
and 18.7 → 18.7 ns for an already-attached JNI frame. ClassLoader creation measured
546,720 → 568,775 ns (+4.0%), with overlapping ranges of 519,121–710,029 and
526,521–572,548 ns. No ClassLoader code changed. Registration measured +1.9% with
overlapping ranges. These observations do not establish a systematic regression.

Detached worker attach/detach measured about 16.1 microseconds per frame after the
changes. The owned-worker batch-attached control measured about 18.5 ns. These are
different attachment workloads. No corresponding production optimization is claimed.
Scheduler and gameplay paths retain their existing implementation.

## Reproduction and evidence

Build Java with the existing offline Gradle configuration, then use `:runtime:benchmarkJava`
or invoke `dev.zernyth.runtime.BenchmarkHarness` directly with the test classes, API and
runtime JARs on its classpath. Its bounded system properties are
`zernyth.benchmark.warmup`, `zernyth.benchmark.rounds`, `zernyth.benchmark.scale` and
`zernyth.benchmark.output`. Pass the matching benchmark-plugin JAR as its first argument.

For native measurements use the existing test environment established by
`scripts/zernyth_release.py`, set `ZERNYTH_PERF_OUTPUT`, then run the exact filter
`JavaPerformanceTest.ReproducibleProductionBridgeHarness` through CTest or the optional
test host. Keep native, API and runtime versions matched for each comparison.

Local evidence is retained under `build/performance-1.0/`, including the baseline
runtime JAR and test-host DLL, five paired Java and native result sets,
`comparison-final.json`, fixture provenance, compatibility logs and build logs.
Full suite results are also in `build/release-013/host-test-result.json` and
`host-tests.xml`. Local raw evidence is excluded from source distribution.

Changed tracked files belong to Zernyth except the existing CMake integration file.
That file has only a 126-byte optional-include append. Its original byte prefix matches
the initial working-copy SHA-256 exactly. Public Java API sources and signatures were
unchanged in this phase.

The previously built stable wheel retains SHA-256
`1d6c680e6cbef691dc1bf583112fce68b197804908d3be1a4ca970ba628e66ce`.
The source build contains these optimizations. The installed stable validation server
and that wheel retain their prior contents. Packaging and real BDS performance
validation of the optimized sources remain subsequent steps.

## Files changed in this phase

- `CMakeLists.txt`
- `config/zernyth-format-files.json`
- `java/runtime/src/main/java/dev/zernyth/runtime/Network.java`
- `java/runtime/src/test/java/dev/zernyth/fixture/NetworkPlugin.java`
- `java/runtime/src/test/java/dev/zernyth/fixture/PerformancePlugin.java`
- `java/runtime/src/test/java/dev/zernyth/runtime/BenchmarkHarness.java`
- `java/runtime/src/test/java/dev/zernyth/runtime/NetworkValueTest.java`
- `scripts/zernyth_release.py`
- `src/endstone/core/plugin/java_network.cpp`
- `tests/test_java_network.cpp`
- `tests/test_java_performance.cpp`
- `tests/test_zernyth_release.py`
- `cmake/ZernythJavaTestHost.cmake`
- `tests/java_test_host.cpp`
- `scripts/zernyth_jni_host.py`
- `docs/zernyth-performance-1.0.md`
