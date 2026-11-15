# Python distribution and manual publishing

## Identity and installation model

| Component | 1.0.0 value |
|---|---|
| Distribution / PyPI project | `zernyth-engine` |
| Python version (PEP 440) | `1.0.0` |
| Native and Java artifact version | `1.0.0` |
| Operator-created release tag | `v1.0.0` |
| Console command | `zernyth` |
| Compatibility imports / native plugin API version | `endstone` / `0.11.15` |

This is a **replacement distribution in a dedicated virtual environment**.
PyPI cannot express file-level conflicts with the upstream `endstone` package.
Installing both would overwrite the same import files; do not do so. The Zernyth
launcher refuses an environment with official `endstone` distribution metadata.
After a collision, recreate the venv rather than uninstalling one package in place.
Existing Python plugins keep their `endstone` imports. No `endstone` console
entrypoint is installed by this distribution.

Published Python plugins that declare a pip dependency on the **distribution**
`endstone` need their dependency metadata retargeted for this fork; Python import
compatibility does not make `zernyth-engine` satisfy that distribution requirement.
Do not let such dependencies install the official package into this venv.

Only **Windows x64 / CPython 3.15** has a wheel validation in this phase. The wheel
is ABI-specific, not `abi3` or universal. Linux packaging is rejected. Java 25 and
BDS 1.26.60.6 must be supplied separately; no automatic JDK/BDS downloads occur.

## Offline local build

The PEP 517 backend is `packaging/zernyth_build.py`. Provision the supported native
toolchain, existing Conan/Gradle caches, Python runtime dependencies, `packaging`
26.3, Conan, CMake, Ninja, and Gradle 9.1+ as described in the native build guide.
Activate the supported MSVC/Clang environment. Temurin 25 is the default;
`--vendor any` is an explicit local fallback, not Temurin validation.

```powershell
python scripts/zernyth_release.py build --gradle "$env:ZERNYTH_GRADLE" --lockfile config/zernyth-publish-conan.lock
python scripts/zernyth_release.py test
python -m unittest discover -s tests -p 'test_zernyth_*.py'
python -m pip wheel --no-index --no-deps --no-build-isolation -Cnative-build-dir=build/RelWithDebInfo --wheel-dir dist .
python scripts/zernyth_wheel.py validate --wheel-dir dist
python scripts/zernyth_wheel.py validate --wheel-dir dist --require-sanitized
python scripts/zernyth_wheel.py install-test --wheel-dir dist --wheelhouse "$env:ZERNYTH_WHEELHOUSE"
```

For a locally installed non-Temurin JDK, add `--vendor any` to the native build
command. `ZERNYTH_GRADLE`, `ZERNYTH_CMAKE`, and `ZERNYTH_CONAN` may point to installed
tools. The backend's default build invokes the existing release driver with the
reviewed Windows Conan recipe lock. `native-build-dir` skips rebuilding only after
checking matching Java-enabled native metadata, Python ABI, JAR versions, and Java
25 bytecode. This explicit reuse is intended for the tested build job, not for
arbitrary stale artifacts. Build from a clean reviewed checkout for publishing.

The backend emits deterministic member order, timestamps, and RECORD hashes.
The recipe lock pins Conan recipe revisions, not all package binary hashes or the
complete compiler installation. Equal wheel bytes from the same existing inputs
do not prove independently reproduced native binaries. Version 1.0.0 publishes wheels
only; `build_sdist` fails explicitly rather than creating an incomplete sdist.

## Payload and installation checks

The wheel carries:

- `endstone/`: Python modules, stubs, defaults, language/font resources, native
  `_python` and `_detours` extensions, runtime/loader DLLs and Crashpad components.
- `zernyth/`: CLI, read-only BDS version inspection, path-free native build metadata,
  API/runtime JARs and Java/native dependency license files.
- `.dist-info`: distribution metadata, the `zernyth` entrypoint, complete root
  Apache-2.0 license, attribution, WHEEL tags, and hashed RECORD.

The validator checks identity, checksums, Windows PE architecture, required files,
Java 25 class headers, and matching artifact versions. The installation test creates
a new venv, installs **all dependencies offline** from the provisioned wheelhouse,
runs `pip check`, imports the native extensions, and executes the installed
`zernyth --version` and `--help` commands outside the source directory. It never
starts BDS. A missing dependency fails the check rather than contacting an index.

No proprietary BDS, JDK, test world, private instruction file, or local build cache
belongs in the wheel. Gson/SnakeYAML POM notices and full Apache-2.0 texts are
included. Native cached-package license directories are required; `opengl/system`
is an unbundled OS dependency. File checks are not a legal certification; review
third-party notices before public redistribution.
The retained JetBrains Mono font is accompanied by its original project copyright
and complete SIL OFL 1.1 text; its previous missing sidecar license is corrected.

### Build-path sanitization

`cmake/ZernythBuildPaths.cmake` maps both Windows path spellings at compile time,
uses basename-only PDB references at link time, and emits linker maps outside the
wheel. `config/conan/windows-sanitized.profile` applies corresponding settings to
Sentry/Crashpad (MSVC deterministic compilation plus `/pathmap`) and Cpptrace/RakNet
(Clang prefix mapping). No native transport code or PE bytes are modified.

With the dependency sources already cached, provision sanitized packages explicitly:

```powershell
conan install . --no-remote --build=sentry-native/* --build=cpptrace/* --build=raknet/* -pr:h default -pr:h config/conan/windows-sanitized.profile --lockfile=config/zernyth-publish-conan.lock
```

The release driver otherwise uses `--build=never` and the same profile, and passes
the resolved Sentry binary directory explicitly to CMake so reconfiguration cannot
retain an obsolete package path. Rebuild the core after provisioning dependencies.
The wheel validator's `--require-sanitized` check is mandatory before upload.

The Windows linker map additionally verifies DbgHelp symbol resolution and rejects
linked libdwarf objects. The Conan graph includes libdwarf as a recipe dependency,
but its LGPL implementation is not linked into this Windows distribution. License
files remain included conservatively. Linux distribution needs its own linkage and
license validation. A path-free evidence record and 19 original Sentry/Crashpad
source license notices are included in the wheel.

## Dedicated GitHub runner prerequisites

`.github/workflows/publish.yml` is manual-only. It builds one Windows wheel; it
does not create tags, releases, commits, or pushes. Provision an isolated trusted
runner labelled `self-hosted`, `Windows`, `X64`, `zernyth-publish`, with:

- CPython 3.15 x64 and the validated Clang/MSVC/CMake/Ninja toolchain on PATH.
- Python build/test dependencies, PyYAML, psutil, packaging, and markdown-it-py
  (already required by Rich). Strict Core Metadata and CommonMark rendering checks
  are performed by the wheel validator; Twine is not an additional requirement.
- Repository variables `ZERNYTH_GRADLE` (installed Gradle executable),
  `ZERNYTH_CONAN_HOME` (provisioned cache **outside** the checkout), and
  `ZERNYTH_WHEELHOUSE` (complete trusted Windows dependency wheels).
- Optional `ZERNYTH_TOOL_PATHS` supplies semicolon-separated directories for
  installed Python, Conan, LLVM, CMake and Ninja tools. These are added to the
  individual job's PATH. The CI build invokes Conan from the checkout parent
  with explicit recipe/output paths so `.conanrc` cannot replace the supplied cache.
- `ZERNYTH_PYTHON` specifies the full path to the preinstalled CPython 3.15
  executable. Windows steps use cmd and Python for UTF-8 file commands, allowing
  the runner to honor the machine's existing PowerShell script policy.
- Conan profiles and the recipe/package revisions in the checked-in Windows lock.
- Existing Gradle dependency caches. No dependency compilation/download fallback
  is allowed by the native build driver.

`actions/setup-java` provisions Temurin 25 when the manually approved workflow is
run. This requires Java distribution network access on a cache miss; no such
download was performed during local development. GitHub also needs access to
action sources and artifact storage. Only the upload jobs contact the selected
package index. The upload jobs use GitHub-hosted Ubuntu, fetch only this run's
wheel artifact, do not check out or build project code, and receive OIDC permission.

Select the reviewed **full 40-character commit SHA**. It must equal the checkout.
The production target additionally requires the existing `v1.0.0` tag to
resolve to that SHA. Default target is `build-only`. Review the workflow's action
versions before enabling it; referenced action tags are not immutable SHA pins.

## Trusted Publisher settings

Create a pending publisher if the project does not yet exist, or add a publisher
to the existing project. TestPyPI and PyPI are separate registries and require
separate configurations:

| Field | TestPyPI | PyPI |
|---|---|---|
| Project | `zernyth-engine` | `zernyth-engine` |
| Owner | `MakisImperium` | `MakisImperium` |
| Repository | `endstone-zernyth` | `endstone-zernyth` |
| Workflow filename | `publish.yml` | `publish.yml` |
| Environment | `testpypi` | `pypi` |
| Upload endpoint | `https://test.pypi.org/legacy/` | PyPA action default |

Review the protection rules on both GitHub environments before choosing an upload
target. GitHub applies the rules configured by the owner. The explicit manual
`target` selection controls whether this workflow uploads. The default remains
`build-only`. A second reviewer or prevent-self-review setting is an owner policy
choice. The last remote inspection found zero registered self-hosted runners and
no reviewer rules on either environment. Register the dedicated provisioned runner
before expecting a build to execute. No persistent PyPI API token is required.

See the official [Trusted Publishing instructions](https://docs.pypi.org/trusted-publishers/using-a-publisher/)
and [environment configuration guidance](https://docs.pypi.org/trusted-publishers/adding-a-publisher/).

## First TestPyPI upload — operator steps

1. Review these source changes, commit and push them yourself. No local tool here
   performs those actions. Record the exact reviewed commit SHA.
2. Provision the dedicated runner, caches, dependency wheelhouse, and repository
   variables. Review environment protection and verify the TestPyPI Trusted Publisher.
3. Run **Zernyth release wheel (manual)** with that SHA and `build-only`. Inspect
   native/JNI/Java/Python results, metadata/rendering checks, fresh installation, wheel contents,
   licenses, and the uploaded wheel artifact. Stop on any failed gate.
4. Run the workflow with the **same SHA** and target `testpypi`; approve only the
   `testpypi` environment job after reviewing the completed build checks.
5. In a fresh Windows CPython 3.15 venv, install ordinary dependencies from your
   trusted normal dependency source, then install the selected release explicitly from
   TestPyPI with `--no-deps`. Avoid an ambiguous mixed-index dependency search:

   ```powershell
   python -m pip install --no-deps --index-url https://test.pypi.org/simple/ zernyth-engine==1.0.0
   zernyth --version
   python -m pip check
   ```

   Use the wheel's `Requires-Dist` for the dependency list; the local workflow's
   offline wheelhouse test already installs that complete list automatically.
6. Inspect an existing compatible BDS installation without `--run`. A real BDS
   launch and connected-client validation remain a separate operator action.

PyPI production upload is a separate explicit target and protected-environment
approval after TestPyPI validation and user-created tag verification. Version
1.0.0 is immutable once uploaded; future changes require a new version. Neither
registry receives an upload automatically. The older 0.9.0a1 alpha was previously uploaded to TestPyPI.

## Upstream workflow deviations

The inherited `build.yml` wheel matrix and Docker publishing job are restricted
to `EndstoneMC/endstone`, as its PyPI/release job already was. The inherited docs
deployment is similarly restricted. This avoids obsolete Endstone-named wheel
jobs, Docker registry logins/uploads, and upstream-style Pages deployment in this
fork. Native source and transport behavior are unchanged. Zernyth packaging and
publishing use the new explicit Windows workflow instead.

The 1.0.0 workflow explicitly builds and selects Zernyth's first-party Windows test
DLL host. It runs the existing native/JNI cases in fresh Python-hosted processes
through the standard Windows loader. Local application-control policy remains
enabled. The current local evidence covers 249 tests and nine checked JVM processes;
the updated remote workflow requires a separately approved build-only execution.
