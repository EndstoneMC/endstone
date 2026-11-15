# VPS performance and runtime validation

Measurements taken on 15 November 2026. Release decisions must use the individual
validation results below. The test area is separate from the VPS application services.

## Environment and limits

- Ubuntu 24.04.5, Linux 6.8.0-147, x86-64 KVM guest.
- Eight AMD EPYC-Rome virtual CPUs and 16 GiB RAM.
- Eclipse Temurin 25.0.5+8, Gradle 9.8.2 and CMake 4.3.1.
- Native builds use a privately extracted Clang/libc++ 20 toolchain and Conan cache.
- Original BDS 1.26.60.6, protocol 2225, new worlds with seed `20261115`.
- BDS and synthetic clients communicate inside a disposable Linux network namespace.
  The namespace has two private virtual interfaces and no external route.
- BDS is limited to two virtual CPUs, a 2 GiB sampled RSS ceiling, and bounded
  startup, observation and shutdown deadlines. Clients run on two different CPUs.
- Existing application services remain active. Measurements include VPS scheduling
  uncertainty and shared-host effects.

## Java scheduler comparison

The comparison uses three alternating pairs of fresh JVMs, two CPU affinity slots,
15 warmup batches, 30 measurement batches and operation scale four. Both variants
use `-Xms64m -Xmx256m -XX:ActiveProcessorCount=2 -Xbatch`.

Values below are medians across the three process results. Latencies describe the
existing Java mock harness. The delayed-queue cases contain future tasks and measure
a tick pulse before their due time.

| Measurement | Before | After |
| --- | ---: | ---: |
| Complete managed suite, process CPU time | 5.43 s | 2.37 s |
| Complete managed suite, elapsed time | 5.410 s | 2.349 s |
| Sampled peak process RSS | 104.36 MiB | 104.58 MiB |
| Task admission and cancellation | 237.53 ns | 285.97 ns |
| Server enqueue and pulse | 518.40 ns | 536.60 ns |
| Pulse with 256 future tasks | 4.949 us | 19.85 ns |
| Pulse with 4,096 future tasks | 80.642 us | 15.84 ns |
| Admission and bulk cancellation of 256 tasks | 67.410 us | 68.152 us |

The future-task shortcut reduces the measured managed-suite CPU cost. Admission,
immediate dispatch and bulk cancellation have higher observed medians in this VPS
run. Three forks provide limited confidence about small differences. The suite's
sampled peak RSS is approximately unchanged.

The raw files retain batch samples and P95 batch means. A batch-mean percentile
does not describe the latency distribution of individual operations. P99 is omitted.
These measurements establish the cost of the specific Java harness cases.

## Original BDS baseline

Each run creates a fresh test world. The phases are ten seconds idle, 35 seconds
for connection and movement, and 20 seconds of continued movement. The headless
clients request a radius of four and use NetherNet. Timings include initial world
generation and connection work.

| Measurement | Four clients | Sixteen clients |
| --- | ---: | ---: |
| Successful joins | 4/4 | 16/16 |
| BDS exit code | 0 | 0 |
| Client exit code | 0 | 0 |
| Sampled peak BDS RSS | 465.69 MiB | 486.57 MiB |
| Steady-phase CPU, one core = 100% | 32.37% | 67.03% |
| Observed game-time progression over the run | 19.986 ticks/s | 19.629 ticks/s |
| Client-received packets at the last statistics sample | 24,913 | 284,064 |
| Reported packet decode errors | 0 | 0 |
| Reported unexpected disconnects | 0 | 0 |

Tick progression comes from bounded `time query gametime` observations. This gives
an aggregate tick rate. Per-tick latency percentiles require additional measurements.
The client RTT probe returned zero usable samples. RTT and interactive response-time
claims remain **NOT TESTED**. The clients also reported movement corrections.

These are vanilla BDS measurements. Zernyth overhead must be measured separately
with matching native artifacts and workloads.

## Test-client corrections

The pinned EndstoneMC bedrock-bench source is
`9724d832f7f28b307cd923f98cb2f1753bcc4d45`. The unmodified NetherNet client initially
failed negotiation with error 37. A test-only copy was corrected to use client
identity encoding and the supplied encoding selector in go-nethernet. Transport
and Minecraft login then needed to share the same identity key. The corrected
client joined the offline test server successfully.

The BDS binary and Zernyth transport hooks retain their original behavior. The
test-only dependency changes are preserved in `harness-client-fixes.patch` in the
local evidence directory. The corrected bot SHA-256 is
`c6de0b4ddff38c8d7b5c91c653ab53e472be5ab1875eb9046767518d0f3c55b3f`.

A separate one-client RakNet diagnostic helped isolate the client problem. It is
excluded from the NetherNet results above.

## Validation status

| Gate | Status | Evidence |
| --- | --- | --- |
| Java source build with Temurin 25 on Linux | PASS | Gradle build, 40 tasks, including custom runtime checks |
| Managed Java runtime checks | PASS | 69 assertions and bounded parser checks |
| Seeded parser/JAR fuzzing | PASS | 300 iterations |
| Java scheduler comparison | PASS | Six bounded fresh JVM processes |
| Handle-helper ASan/UBSan/LSan with Clang 20 | PASS | Actual execution, exit 0, shared libc++20, no suppression |
| Complete native Linux build | PASS | Clang/libc++20, Java-enabled native runtime and Python module |
| Production JNI tests on Linux | PASS | 249 native/JNI cases, nine JVM processes, zero checker diagnostics |
| Archived plugin regression tests on Linux | PASS | Existing regression driver, seven first-party archived JARs |
| Bounded lifecycle cycles on Linux | PASS | Existing bounded-stress profile, exit 0 |
| Native Python import from the prepared runtime | PASS | Standalone staging directory, CPython 3.12 ABI |
| Zernyth inside Linux BDS, system allocator | PASS | JVM, plugin activation, four NetherNet clients and controlled exit 0 |
| Explicit Mimalloc plus native runtime preload | FAIL | Reproduced native startup crash with Java disabled |
| Operator's UI plugin lifecycle | PASS | Separate isolated BDS copy, activation and deactivation, exit 0 |
| Vanilla BDS NetherNet baseline | PASS | Four-client and sixteen-client runs, controlled exits |
| Human Bedrock client and UI | PASS | Owner confirmed connection and `/zuitest main` on the prepared Linux test installation |
| Public Linux wheel installation | NOT TESTED | Publishing backend intentionally targets Windows; Ubuntu uses a separate CPython 3.12 archive |
| Cross-server performance comparison | NOT TESTED | Comparable competitor runs are outstanding |

Clang 18 failed native formatting compilation. The repository's Linux toolchain
configuration specifies Clang 20. The initial shared-libc++18 sanitizer run also
reported an allocation/deallocation mismatch that reproduced in an independent
standard-exception probe. A matched static-libc++18 helper run and the shared
libc++20 helper run both completed successfully. Full-core sanitizer coverage remains
**NOT TESTED**.

## Installation status

At the time of this check, TestPyPI provides `zernyth-engine==0.9.0a1` as
`zernyth_engine-0.9.0a1-cp315-cp315-win_amd64.whl`. The public PyPI project endpoint
returned HTTP 404. The working source version is `1.0.0`.

The available TestPyPI wheel requires Windows x64 and CPython 3.15. The prepared
1.0.0 Ubuntu archive uses CPython 3.12, matching native/Java artifacts, a hash-pinned
offline wheelhouse and dynamically linked libelf/libdwarf with corresponding sources.
Its installation and final smoke results are recorded beside the release artifacts.

A separate operator-controlled test installation has been prepared at
`/srv/zernyth-plugin-test`. It contains the matching native/API/runtime 1.0.0 artifacts,
a fresh Python 3.12 environment, privately copied native dependencies, the verified
BDS 1.26.60.6 files and an empty plugin directory. It uses a fresh world and preserves
existing server directories. `start.sh --check` completed successfully, including
artifact hashes, Java 25 detection and the native Python import. `pip check` passed.
The operator starts their server explicitly with `start.sh` after adding their own plugin.

This manual Linux test installation has its own CPython 3.12 ABI. The public Windows
wheel retains its CPython 3.15 requirements. The owner subsequently confirmed
connection and the UI command using a real Bedrock client. Other gameplay cases
retain their individually documented test scope.

### Native startup correction

The operator reported a segmentation fault during world initialization. A separate
network-isolated reproduction with Java disabled confirmed a native startup failure
when Mimalloc and the Endstone runtime were both explicitly preloaded. The native
runtime using the system allocator reached Endstone initialization. The launcher now
preloads the Endstone runtime and uses the system allocator. Native binaries, the
operator's JAR and the operator's world retain their existing contents.

The corrected path completed two bounded isolated BDS tests. One loaded HelloZernyth,
the native C++ fixture and the stable Java probe, accepted four NetherNet clients,
executed the configured probes and shut down with exit 0. The other also loaded,
enabled and disabled `zernyth_ui_showcase` from the operator's JAR and exited 0.
Both logs contained zero JNI checker diagnostics and zero stable-probe failure markers.
Allocator internals and performance differences between allocator configurations
require further investigation. The incompatible explicit Mimalloc configuration is
excluded from the prepared launcher.

The first staging-import attempt ran from the source directory and selected its
Python package before the staged extension. The corrected import uses the separate
runtime directory. The earlier complete Python test invocation stopped during
collection for the same source-path issue. A successful full Linux Python test rerun
has not been recorded.

Raw records, resource samples, compiler logs and test-client corrections are kept
locally under `build/vps-validation/`, outside commit candidates. No release has
been published as part of this validation.
