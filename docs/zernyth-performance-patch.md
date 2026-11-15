# Performance patch and bounded workload validation

## Scope

This source patch targets Zernyth's managed Java task runtime and development validation
process. Existing packet/connection optimizations from the preceding working-copy changes
remain part of the proposed combined commit. Public Java signatures, native Endstone
scheduling, gameplay hooks and plugin ownership contracts retain their existing behavior.

The production change maintains a conservative earliest queued deadline under the existing
runtime lock. A pulse can skip queue traversal when all tasks are still delayed. Admission,
requeue and repeat scheduling update that deadline. Cancellation can leave an earlier stale
deadline, which permits an extra scan while preserving timely execution. Timeout inspection
is skipped when no asynchronous execution permits are held. Server callbacks continue to
check their own duration on exit.

Bulk cancellation clears the captured pending queue before completing individual controls.
Running tasks retain their cooperative cancellation state, accounting and permits until their
actual exit. Callbacks admitted during cancellation continue through the existing admission
checks. Disable still rejects new work, releases queued references and never waits for workers.

## Reproducible benchmark process

`scripts/zernyth_benchmark.py` runs existing Java or productive JNI fixtures in fresh processes.
It supports alternating before/after runtime JARs, exact artifact hashes and focused Java
case-name filtering. No software or dependencies are downloaded by this runner.

Each process is limited to 120 seconds, a 512 MiB Java heap, 1536 MiB observed RSS and
up to four logical processors. The suite deadline is 900 seconds. Process resources are
sampled every 20 ms. Empty measurements, failed processes and exceeded limits prevent PASS.
Output must use a new directory below the repository's `build/` directory.

The resource observations include process wall time, sampled CPU seconds, average CPU core
equivalents, sampled peak RSS, Windows private memory, sampled peak thread count and CPU
affinity. JVM output additionally includes heap usage, GC counters, callback batch timings
and thread allocation counters where supported. Whole-process CPU includes startup, JIT and
cleanup. Task wall duration and queue wait are separate observations.

Example using already provisioned Gradle, JDK and dependencies follows. The vendor override
is appropriate for the locally tested Oracle JDK. Use the default vendor for Temurin builds.

```powershell
gradle -p java --offline -PzernythJavaVendor=any build :runtime:lifecycleBenchmarkJar

python scripts/zernyth_benchmark.py --java-home "$env:JAVA_HOME" `
  --runtime java/runtime/build/libs/runtime-1.0.0.jar `
  --baseline-runtime build/performance-patch/runtime-before.jar `
  --scenario java --filter managed- --forks 3 `
  --output build/performance-patch/repeat-managed

python scripts/zernyth_benchmark.py --java-home "$env:JAVA_HOME" `
  --runtime java/runtime/build/libs/runtime-1.0.0.jar `
  --scenario jni --forks 3 --output build/performance-patch/repeat-jni
```

The baseline runtime must be built and archived before the optimization. The local saved
baseline has the same 1.0.0 API/native version and includes the earlier packet improvements.
Results in this report compare this patch with that baseline.

Use the existing optional native DLL host when the normal test executable is blocked locally.
`scripts/zernyth_release.py` now honors `--test-host` for archived-plugin regression and
bounded lifecycle stress as well as the full native suite. Default CTest execution remains
available. JNI diagnostics reject the archived-plugin gate.

```powershell
python scripts/zernyth_release.py test --java-home "$env:JAVA_HOME" `
  --test-host build/RelWithDebInfo/tests/zernyth_java_test_host.dll
python scripts/zernyth_release.py regression --java-home "$env:JAVA_HOME" `
  --test-host build/RelWithDebInfo/tests/zernyth_java_test_host.dll
python scripts/zernyth_release.py stability --profile bounded-stress `
  --java-home "$env:JAVA_HOME" `
  --test-host build/RelWithDebInfo/tests/zernyth_java_test_host.dll
```

## Synthetic workload coverage

- 256 and 4096 outstanding tasks whose deadlines remain in the future.
- Immediate server handoff and tick pulse, admission and individual cancellation.
- Admission and bulk cancellation of 256 pending tasks.
- Earlier deadlines inserted into an existing delayed queue, cancellation of the earliest
  task, capacity recovery and eventual dispatch of the original later task.
- 64 bounded CPU tasks through a mock native host, with at most two simultaneous callbacks.
- 64 virtual-thread tasks simulating blocking I/O, with at most four simultaneous callbacks.
- Full-queue rejection, workload checksums, completed task states and permit recovery.
- Existing native event, packet, service, scheduler, player API and lifecycle benchmark cases.
- Existing cancellation races, timeout warnings, re-enable and resource-cleanup regressions.

Worker scenarios use a ten-second deadline and a two-second executor termination limit.
Their per-task P95 is computed from 64 actual task observations. Tick pacing in these tests
is driven by the mock host. Production tick guarantees cannot be inferred from that pacing.

## Measurement conditions and limitations

Measurements were performed on 2026-11-15 using Windows 11, an Intel Core Ultra 7 356H and
Oracle JDK 25.0.5.0.3. The runner restricts process affinity to four available logical
processors and uses `-Xms256m -Xmx512m -XX:ActiveProcessorCount=4 -Xbatch`.
The Java harness uses 15 warmup rounds, 30 measured rounds and operation scale four.
The native batch harness retains three warmup and 15 measured batches.

The final managed comparison uses three independent alternating process pairs. A focused
admission control uses five pairs. P95 benchmark values describe batch means. Individual
packet tails and P99 are outside this measurement. Process resource peaks can fall between
samples. Allocation counters cover Java allocations on the measured thread, excluding native
allocations. Background activity, hybrid-core scheduling, JIT and GC contribute uncertainty.
Raw data includes fork ranges and preliminary runs that showed differing results.

Earlier unrestricted-affinity measurements have different conditions and should be evaluated
separately from this restricted-affinity run. Measurements of unchanged code paths are controls.

## Real BDS gate

**BLOCKED.** An isolated Windows/Linux BDS environment was unavailable during this phase.
The registered VirtualBox guest is macOS. Windows Sandbox, Docker and Hyper-V management
were unavailable. Host-only adapter presence and loopback signaling alone do not establish
isolation of every BDS transport path. Existing server directories and worlds were untouched.

Actual client response latency, BDS tick duration, world-generation throughput, real entity
loads, network throughput and production server memory remain **NOT TESTED** in this phase.
The Java/JNI mock results do not establish cross-server performance rankings. A fourfold
advantage over AllayMC, PNX, PMMP or other engines remains an unvalidated goal.

## Commit and package preparation

The combined source state is pending owner review. Candidate paths, a targeted secret scan,
ownership checks and a proposed commit message are retained in `build/performance-patch/`.
The targeted scan checks private-key markers, common GitHub/AWS credential formats and
personal Windows paths. It cannot establish the absence of every possible secret.

No commit or publication was performed. A final wheel built from the accepted source state
and its fresh-install/BDS validation belong to the subsequent packaging phase.

## Final measured results

### Managed workload

| Case | Before median ns | After median ns | Change | Before fork range ns | After fork range ns |
| --- | ---: | ---: | ---: | --- | --- |
| `managed-admission-cancel-java-mock` | 448.85 | 341.61 | -23.9% | 436.27–484.75 | 283.11–346.68 |
| `managed-server-enqueue-pulse-java-mock` | 725.71 | 468.19 | -35.5% | 667.48–746.36 | 406.65–488.81 |
| `managed-delayed-256-pulse` | 6261.12 | 23.38 | -99.6% | 5844.12–8194.50 | 19.25–23.75 |
| `managed-delayed-4096-pulse` | 180129.38 | 18.62 | -100.0% | 177674.62–186902.88 | 17.75–18.75 |
| `managed-256-admit-cancel-all` | 155055.00 | 68870.00 | -55.6% | 149315.00–159767.50 | 64510.00–75722.50 |

The bulk row measures a group of 256 admissions followed by cancellation. It is a group latency.
The delayed rows measure a pulse with all deadlines in the future. No callback work is performed.
The large reduction is specific to avoiding repeated scans of a delayed queue.

A separate five-pair admission control yielded the following values.

| Case | Before median ns | After median ns | Change | Before fork range ns | After fork range ns |
| --- | ---: | ---: | ---: | --- | --- |
| `managed-admission-cancel-java-mock` | 320.77 | 312.45 | -2.6% | 295.26–425.04 | 280.04–346.75 |

The focused admission ranges overlap. Admission improvements from the mixed suite are context-sensitive.
Java allocation in this focused case remained approximately 384 bytes per operation. The delayed
pulse cases allocate zero measured Java bytes both before and after. No RAM reduction is claimed.

### Productive JNI with native mocks

| Case | Before median ns | After median ns | Change | Before fork range ns | After fork range ns |
| --- | ---: | ---: | ---: | --- | --- |
| `production-java-event-dispatch` | 3071.60 | 2891.65 | -5.9% | 2925.50–3404.35 | 2829.85–3523.65 |
| `production-packet-listener` | 5735.05 | 5847.40 | +2.0% | 5624.65–6290.20 | 5524.90–5977.70 |
| `production-packet-64-players` | 5890.80 | 5921.80 | +0.5% | 5725.20–6150.25 | 5831.20–6255.15 |
| `production-packet-four-listeners` | 20780.85 | 21150.25 | +1.8% | 20492.80–20893.30 | 20676.75–21817.30 |
| `jni-production-service-lookup` | 1865.05 | 1829.85 | -1.9% | 1855.22–1884.67 | 1815.92–1886.80 |
| `native-scheduler-enqueue-heartbeat` | 712.50 | 716.50 | +0.6% | 495.50–723.00 | 694.50–750.00 |

These native/control-path timing ranges mostly overlap. The packet medians include small increases.
This patch changes managed scheduling and supplies no new native packet optimization.

### Whole-process observations

| Suite / variant | CPU seconds | Process wall seconds | Peak sampled RSS MiB | Peak sampled threads |
| --- | ---: | ---: | ---: | ---: |
| Managed / before | 6.609 | 7.077 | 202.94 | 25 |
| Managed / after | 1.188 | 1.334 | 207.89 | 25 |
| JNI / before | 12.750 | 13.674 | 245.39 | 25 |
| JNI / after | 13.828 | 15.729 | 245.84 | 25 |

Each cell is the median across three process observations. It includes setup, warmup, measurement
and cleanup. The managed suite uses the same operation counts in both variants. Its CPU savings
are dominated by the delayed-queue scenario. The complete JNI fixture shows higher median CPU
and wall time after this patch. This mixed observation prevents a claim of global CPU improvement.
Peak RSS is slightly higher in both final suites. A fraction-of-RAM advantage remains unproven.

### Bounded worker tasks

| Kind | Tasks | Max concurrent | Virtual tasks | P95 queue ms | P95 callback wall ms |
| --- | ---: | ---: | ---: | ---: | ---: |
| CPU | 64 | 2 | 0 | 70.667 | 0.636 |
| IO | 64 | 4 | 64 | 98.344 | 5.004 |

I/O callbacks simulate blocking through a three-millisecond sleep. CPU callbacks use a bounded
integer sum with a checked checksum. Neither accesses native gameplay objects. These are
mock-host task measurements. Queue wait includes configured pacing and accumulated backlog.

## Initial Windows validation status

| Gate | Status | Evidence |
| --- | --- | --- |
| Java 25 build | PASS | Offline Gradle build, custom Java checks, 69 managed-runtime assertions |
| CMake Java target | PASS | Explicit `zernyth_java` target |
| Windows native build | PASS | Existing configured native artifacts up to date |
| Native/JNI suite | PASS | 249 cases, nine JVM processes, zero checker diagnostics |
| Python tests | PASS | 56 tests including resource bounds, empty measurements and regression host routing |
| Archived plugins | PASS | Seven first-party example JARs, 0.2.0–0.8.0 |
| Bounded lifecycle stress | PASS | 25 cycles, checked JNI, configured time/RSS/CPU limits |
| Formatting | PASS | Manifest-scoped Java and four changed Zernyth C++ files |
| BDS workload isolation | BLOCKED | Suitable isolated runtime unavailable |
| BDS/client latency and resource use | NOT TESTED | No BDS run in this phase |
| Cross-server speed or RAM advantage | NOT TESTED | No matching competitor workload measurements |
| Linux / Temurin | NOT TESTED | Local Windows / Oracle results only |

The native suite uses the documented standard-loader DLL host. Two C++ plugin-load tests
use the previously validated first-party fixture with identical source and SDK headers, as
recorded in the preceding performance report. Fresh executable/fixture application-control
restrictions remain. A successful JNI checker run is one diagnostic observation.

Raw results, immutable baseline hashes, build logs and candidate-commit review files are stored
locally under `build/performance-patch/`. They are excluded from the commit candidates.

Subsequent Linux/Temurin tests and bounded NetherNet BDS measurements are recorded in
[the VPS validation report](zernyth-vps-validation.md). Its gates track the follow-up
environment separately from this initial Windows validation.

## Small-heap experiment

The optimized managed-only suite also completed three fresh processes with `-Xms32m -Xmx128m`.
The median sampled peak RSS was **74.00 MiB**, sampled CPU **0.906 seconds** and
whole-process wall time **0.935 seconds**. The same operation counts were used.

This changes the benchmark heap profile, including both initial and maximum heap. It is a
separate tuning experiment and was run after the paired comparison. Clock/JIT/GC variation
limits direct timing conclusions. It does not establish production BDS memory requirements
or a suitable heap size for arbitrary third-party plugins. Production JVM settings were
not changed.

```powershell
python scripts/zernyth_benchmark.py --java-home "$env:JAVA_HOME" `
  --runtime java/runtime/build/libs/runtime-1.0.0.jar --filter managed- `
  --initial-heap-mib 32 --heap-mib 128 --forks 3 `
  --output build/performance-patch/repeat-small-heap
```
