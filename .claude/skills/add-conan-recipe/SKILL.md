---
name: add-conan-recipe
description: Add a Conan recipe for a third-party dependency under recipes/ and publish it to the endstone/conan Cloudsmith remote. Use when the user wants to package a C/C++ library that isn't on conancenter (or needs project-specific patches) so endstone can consume it via conan rather than FetchContent. Examples - "add a recipe for libwhatever", "package zydis on cloudsmith", "replace the X FetchContent with a conan dep".
---

# Add a Conan recipe

This skill scaffolds a Conan 2.x recipe under `recipes/<name>/all/`, validates
it on Windows and Linux, and publishes it (recipe-only by default) to the
`endstone` Cloudsmith remote. The layout mirrors conan-center-index so the
recipe could be upstreamed later with minimal change.

Execute steps in order. **STOP** points wait for user input.

## Inputs to collect up front

Before scaffolding, you need:

1. **Package name** (lowercase, kebab-case) and **version** (e.g. `funchook` /
   `1.1.3`).
2. **Source tarball URL**. Prefer the GitHub release archive
   (`https://github.com/<owner>/<repo>/archive/refs/tags/v<version>.tar.gz`).
   If upstream uses submodules, you'll need a separate URL per submodule plus
   its pinned commit SHA (see Step 2.b).
3. **Build system** (CMake is the only one this skill handles cleanly; if
   it's autotools/meson/Make, stop and tell the user this skill assumes CMake
   — they may want to extend it).
4. **License SPDX identifier** — read the upstream LICENSE file.
5. **Whether the user wants to upload after local validation**. Default:
   recipe-only upload to `endstone` remote.

If any of these are unclear, ask once with `AskUserQuestion` before
scaffolding.

---

## Step 1 - directory layout

Create the folder tree:

```
recipes/<name>/
├── config.yml
└── all/
    ├── conandata.yml
    ├── conanfile.py
    ├── patches/                  # optional, only if upstream needs patching
    │   └── <version>-<short>.patch
    └── test_package/
        ├── CMakeLists.txt
        ├── conanfile.py
        └── test_package.<c|cpp>
```

`config.yml` maps versions to their recipe folder:

```yaml
versions:
  "<version>":
    folder: all
```

---

## Step 2 - sources and sha256

### 2.a Single tarball

Download the tarball to a scratch location and hash it:

```shell
mkdir -p build/sha-tmp
curl -sL -o build/sha-tmp/src.tar.gz "<URL>"
sha256sum build/sha-tmp/src.tar.gz
```

Write `recipes/<name>/all/conandata.yml`:

```yaml
sources:
  "<version>":
    url: "<URL>"
    sha256: "<HASH>"
```

### 2.b Tarball + submodule(s)

GitHub release archives don't include git submodules. If `git ls-tree HEAD
<submodule-path>` on the upstream repo shows a commit SHA, download the
submodule's archive at that exact commit:

```
https://github.com/<owner>/<repo>/archive/<sha>.tar.gz
```

Hash it the same way. The `conandata.yml` then names each source so the
recipe's `source()` can unpack them to the right relative paths:

```yaml
sources:
  "<version>":
    main:
      url: "..."
      sha256: "..."
    # one entry per submodule, named after the submodule's path
    <submodule>:
      url: "..."
      sha256: "..."
```

In `source()`, use `get(..., destination=os.path.join(self.source_folder,
"<submodule>"))` to land it where upstream's CMakeLists expects.

---

## Step 3 - scaffold conanfile.py

Use the funchook recipe (`recipes/funchook/all/conanfile.py`) as the
template. Key elements that almost always apply:

- `name`, `description`, `license`, `url=<our project URL>`,
  `homepage=<upstream URL>`, `topics`, `package_type = "library"`.
- `exports_sources = "patches/*"` if you added patches.
- Settings: `os, arch, compiler, build_type`. Options: at minimum `shared`
  and `fPIC` (delete `fPIC` on Windows in `config_options`). Add upstream
  feature options as needed.
- `configure()`: `rm_safe("compiler.cppstd")` / `rm_safe("compiler.libcxx")`
  if it's a pure C library — otherwise `package_id` carries spurious C++
  fields.
- `requirements()`: only conan deps. Anything bundled in the upstream source
  (submodules, downloaded-at-configure) should be handled in `source()` /
  `package()`, not `requires`.
- `validate()`: reject unsupported arch/OS with `ConanInvalidConfiguration`.
- `source()`: `get(self, **self.conan_data["sources"][self.version], ...)`.
  If there are patches, call `apply_conandata_patches(self)` after `get()`.
- `layout()`: `cmake_layout(self, src_folder="src")`.
- `generate()`: `CMakeToolchain` + `CMakeDeps`. Set the upstream's cache
  variables to disable tests, force install on, pick install dirs.
- `build()`: standard `cmake.configure() + cmake.build()`.
- `package()`: `cmake.install()`, copy LICENSE to `licenses/`,
  `rmdir(self, "<package>/lib/cmake")` etc. for cleanup. If upstream
  doesn't install a bundled static dep, copy it from `build_folder` by
  pattern.
- `package_info()`: declare `cpp_info.libs`, `cmake_file_name`,
  `cmake_target_name`. On Windows add `system_libs` (psapi, dbghelp, etc.)
  per upstream's link line. If consumed-as-static would leave unresolved
  symbols from a conan dep, set `cpp_info.requires = ["<dep>::<dep>"]`.

If upstream's CMakeLists downloads a dep at configure time (e.g.
`FetchContent`, `ExternalProject_Add`), prefer **patching** it to call
`find_package` first and fall back to the bundled path. See
`recipes/funchook/all/patches/1.1.3-external-capstone.patch` for the
short-circuit pattern.

---

## Step 4 - scaffold test_package

Three files:

- `test_package/conanfile.py` — minimal, mirrors funchook's. Sets
  `test_type = "explicit"`, requires `self.tested_reference_str`, runs the
  built executable via `self.run(exe, env="conanrun")` inside `can_run(self)`.
- `test_package/CMakeLists.txt` — `find_package(<name> CONFIG REQUIRED)`,
  link against `<name>::<name>`.
- `test_package/test_package.<c|cpp>` — should exercise at least one symbol
  end-to-end so a successful exit means the package actually works, not just
  that linking succeeded. Watch for compiler inlining tripping up function
  hooking / pointer comparisons — mark relevant functions `NOINLINE` and use
  `volatile` if needed (see funchook's test_package).

**STOP** if you can't think of a meaningful end-to-end test. Ask the user
for a snippet that exercises the library's main entrypoint.

---

## Step 5 - local validation (Windows)

Run from project root with cmake on PATH:

```powershell
$env:PATH = "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin;C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja;$env:PATH"
conan create recipes/<name>/all --version=<version> -pr default --build=missing
```

If the recipe has options, test the non-default value too:

```powershell
conan create recipes/<name>/all --version=<version> -pr default --build=missing -o '<name>/*:<opt>=<value>'
```

Iterate until `Testing the package: Executing test` prints the expected
output. Common failure modes:

- `Library '<name>-static' not found in package` → upstream's `OUTPUT_NAME`
  differs from the recipe's `cpp_info.libs`. Inspect the installed package
  folder (`.conan2/p/b/<name>*/p/lib`) and update.
- Unresolved symbols → upstream installs the main lib but not a bundled
  dep; either copy the bundled lib in `package()` or patch upstream to use
  `find_package`.
- Test executable crashes with no useful output → ABI mismatch between
  separate builds of the same lib (classic when patching a `find_package`
  short-circuit but the conan dep isn't actually in `requires`). Check
  `find_package(<dep> CONFIG QUIET)` actually succeeds — `QUIET` hides the
  miss.

---

## Step 6 - Linux validation in a container

ubuntu:20.04 gives glibc 2.31, which is roughly the floor Endstone targets:

```shell
docker run --rm -v "<absolute project path>:/work" ubuntu:20.04 bash -c '
export DEBIAN_FRONTEND=noninteractive
apt-get update -qq >/dev/null
apt-get install -qq -y --no-install-recommends build-essential ninja-build python3 python3-pip ca-certificates >/dev/null
pip3 install --quiet --no-input "cmake>=3.29" "conan>=2.0"
export PATH=/usr/local/bin:$PATH HOME=/work/.linux-home
conan profile detect --force >/dev/null 2>&1 || true
conan create /work/recipes/<name>/all --version=<version> --build=missing 2>&1 | grep -E "<test-success-marker>|ERROR|Built target"
'
```

After the run, delete the throwaway cache: `rm -rf .linux-home`.

Replace `<test-success-marker>` with a string the test_package prints on
success (e.g. `hooked add` for funchook).

---

## Step 7 - upload to Cloudsmith

**STOP** before uploading. Confirm with the user the upload scope:

- **Recipe only** (default): `--only-recipe`. Consumers build binaries
  locally on first install. Negligible Cloudsmith storage cost.
- **Recipe + binaries**: drop `--only-recipe`. Each `package_id` × option
  combination uploaded contributes real storage. Only do this when CI is
  going to need the binaries.

The Cloudsmith remote is named `endstone` in `.conan2/remotes.json`. Auth
for **upload** requires HTTP Basic Auth with a personal Cloudsmith API key
(entitlement tokens are read-only). The user must run
`conan remote login -p <api-key> endstone <cloudsmith-username>` themselves
in their own shell — don't ask them to paste the key into chat.

Once authenticated:

```shell
conan upload "<name>/<version>" --only-recipe -r=endstone -c
```

Verify:

```shell
conan list "<name>/*" -r=endstone
```

Cloudsmith has a brief post-sync indexing step (~30s); if `conan list`
returns empty immediately after upload, wait and retry. The package
appearing on Cloudsmith's web UI doesn't always mean `conan list` sees it
yet.

For an end-to-end sanity check, blow away the local cache and re-pull:

```shell
conan remove "<name>/*" -c
conan install --requires=<name>/<version> -r=endstone --build=missing
```

---

## Step 8 - wire endstone to consume it (optional)

If the goal was to replace a FetchContent or to add a new dep, then:

1. Add `self.requires("<name>/<version>")` to the project root
   `conanfile.py`.
2. Add `find_package(<name> CONFIG REQUIRED)` in the consuming
   `src/.../CMakeLists.txt` (typically `src/endstone/runtime/CMakeLists.txt`
   for hook-related deps).
3. Update `target_link_libraries(...)` to use `<name>::<name>` (most likely
   already named this way if you replaced a FetchContent alias).
4. Delete the old `third_party/<name>/` folder and its
   `add_subdirectory(<name>)` line in `third_party/CMakeLists.txt`.
5. Confirm `conan install . --build=missing -s compiler.cppstd=20 -c
   tools.cmake.cmaketoolchain:generator=Ninja ...` succeeds.

The `endstone` Cloudsmith remote is already in tracked
`.conan2/remotes.json`; devs cloning the repo will resolve the dep without
any setup beyond `pip install conan` and `conan profile detect`.

---

## Step 9 - commit

Stage only:

- `recipes/<name>/` (the recipe folder)
- `.conan2/remotes.json` if you added a new remote
- `conanfile.py`, the CMakeLists changes, and any deleted `third_party/`
  files if Step 8 was done
- README badge / CONTRIBUTING edits if applicable

**Do not stage** test_package build artifacts (`recipes/.gitignore` already
excludes `**/test_package/build/` and `CMakeUserPresets.json`). Leave the
project's pre-existing untracked files alone.

Commit style matches the repo's conventional-commit history:

```
build: add <name> conan recipe
```

Body: one short paragraph explaining what the recipe wraps + which
platforms were validated, then a bullet list of notable recipe quirks
(bundled deps, patches, non-default `cpp_info`).

**Never add a `Co-Authored-By` line for Claude.**
