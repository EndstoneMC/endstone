# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Endstone is a plugin API for Minecraft Bedrock Dedicated Servers (BDS), supporting both Python and C++ plugins. It hooks into the BDS binary to expose a high-level Bukkit-like API for plugin development.

## Build Commands

### Prerequisites
- Windows: clang-cl + lld-link (LLVM 18+, e.g. the "C++ Clang tools for Windows" component of Visual Studio 2022), plus the MSVC toolchain for the Windows SDK and STL headers, and CMake 3.29+
- Linux: Clang 18+ with libc++
- CMake 3.29+ and Conan 2.0+ on PATH. CMake is yours to install — the profile deliberately does not
  `tool_requires` it. Ninja does come from Conan.

> LLVM 18+ is the hard minimum: the public API headers use `std::format`, which is only complete in libc++ from LLVM 18.

### Install dependencies with Conan
The project ships `.conan2/profiles/default` (a Jinja-templated profile) that auto-detects the host compiler and pins the right settings: clang-cl on Windows, clang/libc++ on Linux, Ninja generator. Do not run `conan profile detect` — it would overwrite this file. Just:

```shell
pip install conan
conan install . --build=missing
```

> Windows: run from a Visual Studio Developer prompt with clang-cl/lld-link on PATH.

Do not pass `-of` — the default `cmake_layout` output folder is what the preset and the docs below
assume. In particular never aim it at the wheel's tree (`-of build/conan_out`); see [Build trees](#build-trees).

### Activate Conan build environment (only for the manual CMake path)
The profile pins `&:build_type=RelWithDebInfo` for endstone itself (dependencies stay `Release`), so the build tree is `build/RelWithDebInfo`:

```shell
# Windows (cmd)
.\build\RelWithDebInfo\generators\conanbuild.bat

# Windows (PowerShell)
.\build\RelWithDebInfo\generators\conanbuild.ps1

# Linux
source ./build/RelWithDebInfo/generators/conanbuild.sh
```

### Build with CMake
```shell
cmake --preset conan-relwithdebinfo
cmake --build --preset conan-relwithdebinfo
```

### Install from source (builds Python wheel)
The PEP 517 backend (`conan-py-build`) runs Conan internally, so no separate `conan install` step is required:

```shell
uv pip install -U .
# or with pip:
pip install -U . -C build-dir=./build
```

The build dir is what keeps the C++ build tree persistent across reinstalls (much faster after the
first build). `[tool.uv] config-settings` in `pyproject.toml` supplies it for uv; pip has no
equivalent, so pass `-C build-dir=./build` by hand there. Without it the backend builds in a
tempdir and throws the whole tree away.

`build/package` is the wheel staging tree and is never pruned, so a module deleted from `endstone/` keeps being packaged, and an old `*.dist-info` makes pip reject the build with a version mismatch. After deleting or renaming anything under `endstone/`, `rm -rf build/package` before reinstalling.

### Build trees

Local development and the wheel build own **separate** trees under `build/`. Keep them separate:

| Flow | Tree | Driven by |
| --- | --- | --- |
| Local development | `build/RelWithDebInfo` | `conan install .` then `cmake --preset conan-relwithdebinfo` |
| Wheel | `build/conan_out/build/RelWithDebInfo` | `uv pip install -U .` |

Only the local tree is meant to be driven by hand — it is also the only one that builds the GTest
suite, since the wheel configures with `BUILD_TESTING=OFF`.

Never reconfigure the wheel's tree with `cmake --preset`. `conan-py-build` pins `Python_EXECUTABLE`
to the PEP 517 build environment — an ephemeral venv under `AppData/Local/uv/cache/builds-v0/` that
is deleted as soon as the wheel finishes — via `tools.cmake.cmaketoolchain:extra_variables`. A later
configure over that tree inherits the dead interpreter and fails in pybind11's `find_package(Python)`.
If the two trees ever get crossed (a stale `CMakeUserPresets.json` pointing into `build/conan_out`),
delete `CMakeUserPresets.json` and re-run `conan install .` to regenerate it.

## Testing

### C++ tests (GTest)
```shell
ctest --test-dir build/RelWithDebInfo
```

### Python tests
`testpaths` and `norecursedirs` are set in `pyproject.toml`, so this collects `tests/` and skips `tests/plugin`:

```shell
pytest
```

These run against the installed wheel, so reinstall (`pip install -U . -C build-dir=./build`) after changing the bindings, or the tests import a stale `endstone`.

### Runtime tests (live server)
`tests/plugin` is the `endstone-test` plugin, which runs pytest inside a running BDS rather than against the wheel. Install it with `pip install -e tests/plugin`; the server pass runs when the plugin is enabled, and the player/event passes run via `/test` and `/test events`. See `tests/plugin/README.md`.

## Code Style

### C++ (clang-format + clang-tidy)
- Based on Microsoft style with Stroustrup braces
- Naming conventions (from .clang-tidy):
  - Classes/Structs/Enums: `CamelCase`
  - Methods: `camelBack`
  - Private/protected members: `lower_case_` (trailing underscore)
  - Local variables/parameters: `lower_case`
  - Macros: `UPPER_CASE`

### Python (ruff)
- Line length: 120 characters
- Enabled rules: I (isort), F (pyflakes)

### Comments (all languages)
- **This project has a no-comments policy for LLMs.** Do not add a comment unless the user explicitly asks for one. Write code that reads without commentary; if a comment feels necessary, rename or restructure instead.
- This applies to new code and to code you touch. Leave existing comments alone unless the change makes them wrong.
- Never leave "LLM notes" — comments that explain why a change was made, reference the development process, or restate what the code plainly does.
- When explicitly asked for a comment: one short line, terse and human. No multi-line explanations, rationale, design-decision narration, or parenthetical asides. Match the density of the surrounding or original code (e.g. a port stays as terse as its upstream).

## Architecture

### Source Structure

- `include/endstone/` - Public C++ API headers (header-only library for plugin development)
- `src/bedrock/` - Bedrock server type definitions and reverse-engineered structures
- `src/endstone/core/` - Core implementation of the Endstone API
- `src/endstone/runtime/` - Runtime hooks into BDS via funchook (`bedrock_hooks/` subdirectory)
- `src/endstone/python/` - Python bindings via pybind11
- `endstone/` - Python package (CLI, plugin loader, metrics)

### Key Components

1. **Runtime Hooks** (`src/endstone/runtime/bedrock_hooks/`): Intercepts BDS functions to inject Endstone behavior. Each hook file corresponds to a BDS subsystem (player, level, network, etc.).

2. **Bedrock Layer** (`src/bedrock/`): Type-compatible structures matching BDS internal types. Required for ABI compatibility when hooking.

3. **Core Layer** (`src/endstone/core/`): Implements the public API by wrapping bedrock types. Contains subsystems: command, event, inventory, level, plugin, scheduler, scoreboard.

4. **Plugin System**: Supports both C++ plugins (loaded as shared libraries with `endstone_` prefix) and Python plugins (loaded via `endstone.plugin.plugin_loader`).

### Plugin Development

C++ plugins link against `endstone::endstone` (header-only) and use the `endstone_add_plugin()` CMake function. Python plugins inherit from `endstone.plugin.Plugin`.

### Dependencies

Core dependencies managed via Conan: fmt, boost, pybind11, spdlog, nlohmann_json, entt, magic_enum, sentry-native. See `conanfile.py` for full list.

## Conan Recipes

Custom third-party recipes live under `recipes/` and are published to the `endstone` Cloudsmith remote.

- **Upload recipes only, never prebuilt binaries.** Let consumers and CI build the binaries from source: `conan upload "<name>/<version>" -r endstone --only-recipe -c`.

## Git Conventions

- Never add a `Co-Authored-By` line for Claude when creating commits.
- CHANGELOG.md must strictly follow [Keep a Changelog 1.1.0](https://keepachangelog.com/en/1.1.0/):
  - Write for humans (server admins and plugin developers), not machines.
  - Every version gets an entry; latest version comes first.
  - Group changes by type: Added, Changed, Deprecated, Removed, Fixed, Security.
  - Versions and sections must be linkable (reference-style links at bottom).
  - Display the release date of each version.
  - Include user-visible changes and API changes; omit internal implementation details (no refactoring notes, no internal class/struct changes).
  - Prefix breaking changes with `**BREAKING**:` in the Changed or Removed section.
