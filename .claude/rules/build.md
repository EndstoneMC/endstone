---
paths:
  - "CMakeLists.txt"
  - "**/CMakeLists.txt"
  - "conanfile.py"
  - "recipes/**"
  - "pyproject.toml"
  - ".github/**"
---

# Build

## Conan and CMake

- Keep every third-party dependency a Conan package; custom recipes live under `recipes/` (`funchook`, `raknet`).
- Wire a new dependency with `self.requires(...)` in `conanfile.py`, `find_package(<name> CONFIG REQUIRED)` in the consuming `CMakeLists.txt`, and `<name>::<name>` in `target_link_libraries`.
- Do not guard the build type with `if(NOT CMAKE_BUILD_TYPE)`; pass the type or pin it in a preset.
- Pass the version to CMake as a cache variable (`ENDSTONE_VERSION_FULL`); the numeric version and `ENDSTONE_API_VERSION` come from `project(endstone VERSION ...)` in `include/CMakeLists.txt`, which equals the release version, and the wheel version comes from the tag via setuptools-scm.
- Activate the Conan build environment in the shell before `cmake --build`; without it `ninja` reports `no work to do` and the toolchain is not found.
- Take imgui backend sources from the Conan package folder (`imgui_PACKAGE_FOLDER_<CONFIG>`), never from vendored copies.
- Run Conan from the repo root so `.conanrc` selects the project cache.
- Never configure a sibling build directory such as `build-ninja/`; delete a stale tree and reconfigure it in place.
- Cap builds at 16 jobs (`CMAKE_BUILD_PARALLEL_LEVEL=16`, `tools.build:jobs=16`) at below-normal priority, one build at a time.
- Verify one reconstruction translation unit alone with `ninja -C build/RelWithDebInfo src/bedrock/CMakeFiles/bedrock.dir/<path>.cpp.obj`.

## Wheels

- Do not pin the Python interpreter; `conan-py-build` injects it.
- Do not put `--build=...` in `[tool.conan-py-build].extra-arguments`; it is forwarded to `conan export-pkg`.
- Copy every built wheel into `wheelhouse/` and bump `SETUPTOOLS_SCM_PRETEND_VERSION` between diagnostic builds.
- Run `uv build` for another project from a neutral directory with `--no-config`.
- Start endstone from the repo root with `PYTHONSAFEPATH=1`.
- Do not run `cibuildwheel` from the main checkout; mount the repo `.conan2` through `CIBW_CONTAINER_ENGINE` `create_args` and drop `.conanrc` from a source copy.
- Linux wheels build in the `manylinux_2_31` clang image set in `[tool.cibuildwheel.linux]`.

## Recipes

- Lay out a recipe as `recipes/<name>/config.yml` plus `recipes/<name>/all/{conandata.yml,conanfile.py,patches/,test_package/}`.
- Name recipes lowercase kebab-case and set `license` to the SPDX identifier of upstream's LICENSE; set `url` to the Endstone project and `homepage` to upstream; set `exports_sources = "patches/*"` when patching.
- Pin sources by URL and sha256 in `conandata.yml`; add one named source per upstream submodule from `https://github.com/<owner>/<repo>/archive/<sha>.tar.gz` at the commit `git ls-tree` shows, and unpack it with `get(..., destination=<submodule path>)`.
- Offer `shared` and `fPIC` options, drop `fPIC` on Windows, and `rm_safe("compiler.cppstd")` and `rm_safe("compiler.libcxx")` for pure C libraries.
- List only Conan packages in `requirements()`; handle bundled upstream dependencies in `source()` and `package()` by copying the bundled library, and set `cpp_info.requires` for a Conan dependency, or the link has unresolved symbols.
- When the package reports `Library '<name>-static' not found`, upstream's `OUTPUT_NAME` differs from `cpp_info.libs`; read the package's `lib/` folder.
- Reject unsupported arch or OS in `validate()` with `ConanInvalidConfiguration`; use `cmake_layout(self, src_folder="src")`, `CMakeToolchain` and `CMakeDeps`.
- Copy `LICENSE` to `licenses/` and remove `lib/cmake` in `package()`; declare `cpp_info.libs`, `cmake_file_name`, `cmake_target_name`, Windows `system_libs` and `cpp_info.requires` in `package_info()`.
- Patch upstream `FetchContent` and `ExternalProject_Add` to try `find_package` first with a bundled fallback; confirm `find_package(<dep> CONFIG QUIET)` really succeeds.
- Keep recipe `patches/` flat, distinguish by filename prefix, and select per version in `conandata.yml`.
- Apply per-option patches in `build()`, never in `source()`.
- BDS runs its own compiled RakNet: `recipes/raknet` patches never change live packet handling.
- Keep the `raknet` recipe on one version string and select the vtable patch with the `minecraft_version` option; apply it in `build()`.
- Write a `test_package` with `test_type = "explicit"` that links `<name>::<name>` and exercises a real entrypoint, so a clean exit proves it works; mark an inlined hook target `NOINLINE` and `volatile`.
- Validate a recipe on Windows and Linux, also with non-default options.
- Export a recipe from the committed tree, not a CRLF working copy.
- Publish recipes to the `endstone` Cloudsmith remote recipe-only, never prebuilt binaries.
- Do not ask for the Cloudsmith API key in chat; upload auth is `conan remote login` with a personal API key over HTTP Basic, run by the user; entitlement tokens are read-only.

## CI

- Treat CI as three workflows: `ci.yml` (PR and push), `docker.yml` (nightly image), `build.yml` (wheels, bundle, docker, PyPI, GitHub release; the only one that fires on a tag push).
- Run `wheels` (the matrix in `[tool.cibuildwheel]`) first in `build.yml`; `bundle`, `docker` and `release` (PyPI, GitHub release with the CHANGELOG section) depend on it.
- Gate `build.yml` heavy jobs on its path filter (`src/** include/** endstone/** tests/** recipes/** scripts/**`).
- Key Conan caches per OS and build type, and purge them with `gh cache delete` when a host-built dependency leaks into a wheel build.
- Keep both codecov statuses (project and patch) informational.
- Do not create tags inside a workflow with `GITHUB_TOKEN`; its pushes trigger no workflows.
- Bump actions through dependabot.
- Rerun a failed `build.yml` job on its own (one wheel job, `release` or `docker`); never re-tag, and `docker` is independent of `release`.
- Check CI after every push, through to the end.
