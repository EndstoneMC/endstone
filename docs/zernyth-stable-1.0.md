# Zernyth Engine 1.0.0

First stable release, finalized for owner approval on 2026-11-16.
The release scope is Windows x64, CPython 3.15, Java 25 LTS and BDS 1.26.60.6,
based on Endstone 0.11.15. Stable identifies the selected release channel and supported
platform scope. A separate Ubuntu 24.04 x86_64 archive uses CPython 3.12. The evidence
and limitations below define the validated coverage.

## Release contents

- Matching native components, Java API and Java runtime versioned 1.0.0.
- The JNI diagnostics correction verified during the owner's BDS/client test.
- Existing gameplay, events, commands, scheduler, async runtime, forms, services,
  configuration and read-only network metadata APIs.
- The optional stable-test plugin, built separately from the production wheel.
- Developer API sources, Javadocs and the locally generated Maven-compatible SDK repository.
- Retained Endstone Apache-2.0 attribution and complete bundled dependency notices.

The Python distribution is `zernyth-engine==1.0.0`, with command `zernyth`.
The internal `endstone` import and native plugin API identity remain 0.11.15.
Use one distribution in a dedicated environment because upstream Endstone shares files.

## Local artifacts

```text
build/release-final/release/zernyth_engine-1.0.0-cp315-cp315-win_amd64.whl
build/release-final/release/zernyth-engine-1.0.0-ubuntu24-x86_64-cp312.tar.gz
build/release-final/release/zernyth-sdk-1.0.0.zip
build/release-final/release/SHA256SUMS
build/release-final/release/RELEASE-REPORT.json
java/stable-test/build/libs/stable-test-1.0.0.jar
java/api/build/libs/api-1.0.0.jar
java/runtime/build/libs/runtime-1.0.0.jar
java/build/sdk-repository/
```

The wheel bundles the production Java JARs. Install the testplugin JAR only when
its explicit operator-driven probes are wanted. BDS, JDK, worlds, player data,
build logs and private agent instructions are excluded from the wheel.

## Installation

From a directory containing the release wheel:

```powershell
py -3.15 -m venv .venv
.\.venv\Scripts\python.exe -m pip install .\zernyth_engine-1.0.0-cp315-cp315-win_amd64.whl
.\.venv\Scripts\python.exe -m pip check
.\.venv\Scripts\zernyth.exe --version
```

Before starting, inspect your separately installed compatible BDS and JDK:

```powershell
.\.venv\Scripts\zernyth.exe --server "D:\BDS" --java-home "D:\JDK25"
```

Append `--run` to start. Use `stop` in the server console to end the process.
The operator manages network exposure, configuration, allowlists and backups.
The binary inspector reports its own inspection scope without inventing a gameplay
or network-isolation result.

Public PyPI/TestPyPI upload remains a separate explicit owner action. Until version
1.0.0 has been uploaded, install the local wheel. After an approved PyPI upload,
`python -m pip install zernyth-engine==1.0.0` selects this release.

## Update and rollback

Stop the server before changing native components or the embedded Java runtime.
Back up plugin data and server configuration. Install the complete matching wheel
into a new environment or upgrade the existing dedicated environment. Replace old
copies of the same optional testplugin to avoid duplicate plugin names.

For rollback, use the archived previous wheel and the matching plugin/runtime set
in a separate environment. Restore operator backups only when required by a plugin's
data migration. The packaging process performs no world migration or automatic update.

## Ubuntu installation

Use the separate Ubuntu 24.04 archive with an installed CPython 3.12 and JDK 25.
Provide your own original BDS 1.26.60.6 directory. Extract into a new versioned folder:

```sh
tar -xzf zernyth-engine-1.0.0-ubuntu24-x86_64-cp312.tar.gz
cd zernyth-engine-1.0.0-ubuntu24-x86_64-cp312
./install.sh
./start.sh --server /absolute/path/to/bds --java-home /absolute/path/to/jdk25
```

The installer uses the archive's hash-pinned wheelhouse offline. Inspection checks
the validated BDS executable hash, matching Java artifacts and native Python import.
Append `--run` for an operator-controlled start. The launcher extends Endstone's
existing Linux bootstrap and selects the system allocator. Explicit Mimalloc
preloading failed native startup validation and is excluded from this start path.

The archive includes full component notices and corresponding libelf/libdwarf
sources. These libraries are dynamically linked and remain replaceable with
interface-compatible versions. See the archive README for details.

## Developer SDK

Extract `zernyth-sdk-1.0.0.zip` and use the included API JAR, sources, Javadocs and
local Maven repository. The included plugin template accepts the repository path:

```sh
gradle -PzernythSdkRepository=/absolute/path/to/sdk-repository build
```

Use `-PzernythJavaVendor=any` with an existing Oracle JDK 25. Temurin 25 is the default.
The `examples` directory contains the first-party HelloZernyth example plugin.

After generating the API publication with the existing Gradle build, reproduce the
SDK archive from the repository using `python scripts/zernyth_sdk.py --output
build/release-final/release/zernyth-sdk-1.0.0.zip` on one command line. The packager
checks Java 25 bytecode, matching versions and publication checksums and preserves
existing output files. It performs no build or dependency download.

## Build

Use the existing CMake, Conan and Gradle configuration, Java 25 and trusted offline
caches. `zernyth-version.txt` supplies the native/Java version and `pyproject.toml`
supplies its matching Python version. The release driver accepts stable and earlier
alpha/RC version forms. Python metadata and Java/native provenance must agree.

The local build uses Oracle JDK 25.0.5.0.3 with `-PzernythJavaVendor=any`, Gradle 9.8.2,
Clang 23.1.4, MSVC 14.51, CMake 4.3.1 and CPython 3.15.2. Temurin is the preferred
JDK distribution. The Ubuntu build uses Temurin 25.0.5+8, Clang/libc++ 20.1.2,
CPython 3.12.3 and CMake 4.3.1. Earlier Alpha Windows CI also validated Temurin 25.

## Validation evidence

| Check | Status | Scope |
| --- | --- | --- |
| Java and Windows native build | PASS | Matching 1.0.0 artifacts, existing offline build workflow |
| Java runtime and stable-test suites | PASS | Eight testplugin groups and existing runtime self-tests |
| Python packaging/CI tests | PASS | 75 checks including archive limits/integrity, shared linkage, metadata normalization and installer rollback |
| Python API regression suite | PASS | 105 cases on each platform against packaged native modules; separate from live Python-plugin validation |
| Real BDS/client behavior of included source fixes | PASS | Owner-run corrected Alpha on BDS 1.26.60.6 |
| Native/JNI regression before version increment | PASS | 249 tests, nine JNI checker processes, zero reported diagnostics |
| Native/JNI rerun of 1.0.0 | PASS | Windows DLL host and Linux executable, 249 cases each, nine checker processes each |
| Archived plugin compatibility before version increment | PASS | Existing first-party JARs 0.2 through 0.8 |
| CI execution of 1.0.0 | NOT TESTED | Workflow prepared; no push or remote dispatch |
| Linux BDS, JVM and plugin lifecycle | PASS | Isolated runs with controlled exit 0 and zero JNI checker diagnostics |
| Linux connected-client UI | PASS | Owner confirmed connection and `/zuitest main` after network configuration |
| Windows wheel and SDK installation | PASS | Fresh offline wheel install, extracted SDK template build, byte-identical repeated packaging |
| Ubuntu archive installation and lifecycle | PASS | Fresh offline install and isolated BDS run with four synthetic NetherNet clients, exit 0 |
| Artifact checksum and reproduction checks | PASS | Wheel, SDK and Ubuntu archive repeated byte-identically; final digests accompany the release files |
| Full-core sanitizers and long-duration load | NOT TESTED | Helper sanitizers and bounded workloads have separate evidence |

The local packaging report in `build/release-final/release/RELEASE-REPORT.json` records the
actual final wheel, digest, installation and privacy-check outcomes. Its evidence
is kept locally because raw logs can contain machine paths and player information.

The real client run verified lifecycle, diagnostics, authorized commands, services,
configuration, delayed tasks/cancellation, player/world/inventory reads, existing
entities, 170 movement callbacks, 2730 read-only packet callbacks, form submission
and zero native listener/task/service counts after cleanup. It does not establish
all inventory/block mutations, teleportation, every form failure/disconnect case,
event cancellation, repeated-generation behavior, long-term uptime or capacity.

Windows application control remains enabled. The current native tests use the
first-party DLL host through the standard loader. Standalone test-executable restrictions
remain separate from those successful checks. Earlier compiler warnings and the
specific fixture-reuse details are retained in the performance validation report.

The first final-archive smoke used a world name that the stable-test plugin correctly
rejected. A new test directory with the required `zernyth-stable-test` world passed
all selected probes. This was a test-driver correction. The production plugin's
world protection remains intact.

## Reproducing the Ubuntu archive

Use the documented Clang 20/libc++ toolchain, Python 3.12 development files, JDK 25,
Conan and CMake. Provision dependencies explicitly before an offline build. The
additional host profile `config/conan/linux-release.profile` selects shared libelf
and libdwarf, supplies the missing declaration in libelf's configure probe and maps
private diagnostic source paths. Rebuild `sentry-native`, `cpptrace`, `raknet` and `libcurl`
after adopting this profile.
Use `config/zernyth-linux-conan.lock` for the reviewed recipe revisions.

For the pinned OpenSSL 3.6.5 build, copy
`config/conan/hooks/hook_zernyth_build_metadata.py` into the dedicated release
cache's `extensions/hooks` directory. Set `CONAN_HOME` and
`ZERNYTH_BUILD_PATH_ROOT` explicitly before rebuilding OpenSSL. The hook checks the
exact upstream metadata-generator hash and normalizes private prefixes in the
diagnostic compiler-flags string before compilation. Actual compiler arguments
retain their original behavior. The hook and modified-source notice accompany the
archive. Changes to the OpenSSL version or generator require review.

Refresh `SENTRY_NATIVE_BIN_DIR` from the selected Conan package on reconfiguration.
An old cached value can otherwise install a Crashpad handler from an earlier
dependency build. The release helper refreshes this path on both platforms.

Configure with `ZERNYTH_SANITIZE_BUILD_PATHS=ON`, `ENDSTONE_SEPARATE_DEBUG_INFO=ON`
and `ZERNYTH_BUILD_PATH_ROOT` set to the common private build root. Core compilation
maps source/cache paths without editing the resulting binaries. The archive builder
also rejects the source checkout, Conan cache and an optional `--private-build-root`
in ELF payloads. Original third-party source fixtures and CPython documentation
examples retain their upstream contents.

`scripts/zernyth_linux_release.py` accepts the matched native build, reviewed Conan
graph, pinned offline wheelhouse, system-library staging root and audited BDS
executable SHA-256. It packages complete notices, replaceable shared libraries and
corresponding sources. Repeating packaging against identical inputs must produce
an identical SHA-256. Existing output archives are preserved.

## API and platform boundaries

- Java 25 is required. Preview features are unused.
- The wheel targets Windows x64 and CPython 3.15. The Ubuntu archive targets CPython 3.12.
- BDS 1.26.60.6 is the documented native version. Additional BDS versions need review.
- Packet access exposes read-only metadata and capability-checked observation.
- Bukkit/Paper support remains a limited porting adapter.
- Native gameplay access retains server-thread and lifetime checks.
- Java plugins are trusted in-process code; classloaders provide no complete sandbox.
- Task cancellation is cooperative. Running Java code cannot be safely force-stopped.

## Manual publishing

The manual `publish.yml` workflow has `build-only`, `testpypi` and `pypi` targets.
It builds and verifies the versioned wheel and checks the digest before OIDC upload.
A production upload requires the owner-created `v1.0.0` tag to identify the reviewed
release commit. Environment approvals and Trusted Publisher configuration remain
owner-controlled. Preparing these files creates no commit, tag, push or upload.

See [publishing configuration](pypi.md) and [testplugin checklist](../java/stable-test/README.md).
