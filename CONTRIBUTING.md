# Contributing to Endstone

Thank you for your interest in contributing to Endstone! This document provides guidelines and information to help you
get started.

## Table of Contents

- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [Building](#building)
- [Testing](#testing)
- [Code Style](#code-style)
- [Submitting Changes](#submitting-changes)
- [Project Structure](#project-structure)
- [Architecture Overview](#architecture-overview)

## Getting Started

Endstone is a plugin API for Minecraft Bedrock Dedicated Servers (BDS) that supports both Python and C++ plugins. It
hooks into the BDS binary to expose a high-level Bukkit-like API for plugin development.

### Prerequisites

**Windows:**

- Clang 18+ (clang-cl) with the MSVC toolchain (Visual Studio 2022)
- CMake
- Ninja
- Conan 2.0+

**Linux:**

- Clang 18+ with libc++
- CMake
- Ninja
- Conan 2.0+

## Development Setup

### 1. Install Conan

```bash
pip install conan
```

The repo ships a Jinja-templated default profile at `.conan2/profiles/default` that auto-detects the host compiler and pins the right settings (clang-cl on Windows, clang/libc++ on Linux, Ninja generator). **Do not run `conan profile detect`** — it would overwrite this file.

### 2. Install Project Dependencies

```shell
conan install . --build=missing
```

> Windows: run from a Visual Studio Developer prompt with clang-cl/lld-link on PATH.

### 3. Activate Conan Build Environment

Only needed for the manual `cmake --preset` path below; `pip install`-based builds don't need it.

**Windows (cmd):**

```shell
build\RelWithDebInfo\generators\conanbuild.bat
```

**Windows (PowerShell):**

```shell
build\RelWithDebInfo\generators\conanbuild.ps1
```

**Linux:**

```bash
source build/RelWithDebInfo/generators/conanbuild.sh
```

## Building

### Build with CMake

```bash
cmake --preset conan-relwithdebinfo
cmake --build --preset conan-relwithdebinfo
```

### Install from Source (builds Python wheel)

The PEP 517 backend (`conan-py-build`) runs Conan internally, so no separate `conan install` is required:

```bash
pip install -U .
# or with uv:
uv pip install -U .
```

To keep the C++ build tree persistent across reinstalls (much faster after the first build), pass a build dir via `config_settings`:

```bash
pip install -U . -C build-dir=./build
```

## Testing

### C++ Tests (GTest)

```bash
ctest --preset conan-relwithdebinfo
```

### Python Tests

```bash
pytest
```

### Runtime Tests

Runtime tests verify the API works correctly within a live Bedrock Dedicated Server. These tests are essential for validating functionality that depends on the server environment.

**Install the test plugin:**

```bash
pip install -e tests/plugin
```

**Run runtime tests:**

1. Start a Bedrock Dedicated Server with Endstone
2. The `endstone_test` plugin will load automatically and runs the server-only tests on startup
3. Join the server and run `/test` to execute the tests that need a connected player
4. Run `/test events` to see which events have fired and to assert on what was recorded
5. Run the `/test <subcommand>` probes for the API a real client is needed for (forms, toasts, particles, sounds,
   maps) — see [tests/plugin/README.md](tests/plugin/README.md)

Before submitting a PR, please ensure:

- All C++ tests pass
- All Python tests pass
- Runtime tests pass (if your changes affect the API)
- The project builds successfully on your target platform

## Generating Type Stubs

`endstone/**/__init__.pyi` and the `lazy.attach` re-export lists in `endstone/**/__init__.py` are generated from the
built `_python` extension. The build regenerates them: the `endstone_stubs` target reruns whenever the extension
relinks, so they can never go stale, and any failure fails the build.

Nothing to run by hand. After changing a binding, build as usual and commit whatever appears in `git status`:

```bash
cmake --build --preset conan-relwithdebinfo
git status                 # the regenerated .pyi / __init__.py
```

Generation needs [uv](https://docs.astral.sh/uv/) on `PATH`; the dependencies come from the script's PEP 723 header.
Without uv it falls back to the interpreter CMake found, which then needs `griffe`, `lazy-loader` and `ruff`
installed. Configure with `-DENDSTONE_GENERATE_STUBS=OFF` to skip generation entirely.

The pieces of the stubs that have no pybind11 counterpart — the generic `Registry`, `Server.get_registry`, the `_T`
typevar — live in `src/endstone/python/stubgen.pat`. If a binding is renamed and a pattern there stops matching, the
build fails and names the pattern.

Note that generation writes into the source tree, so two build directories configured against one checkout would
race; use one, or turn generation off in the others.

## Code Style

### C++

Endstone uses **clang-format** and **clang-tidy** for code quality enforcement.

**Style Guidelines:**

- Based on Microsoft style with Stroustrup braces
- Naming conventions:
    - Classes/Structs/Enums: `CamelCase`
    - Methods: `camelBack`
    - Private/protected members: `lower_case_` (trailing underscore)
    - Local variables/parameters: `lower_case`
    - Macros: `UPPER_CASE`

Before submitting C++ code, ensure it passes clang-format and clang-tidy checks.

### Python

Endstone uses **ruff** for Python code quality.

**Configuration:**

- Line length: 120 characters
- Enabled rules: I (isort), F (pyflakes)

Before submitting Python code, ensure it passes ruff checks.

## Submitting Changes

### Branching Model

Endstone uses a release-focused branching model:

- **`main`** — the latest stable release. Protected; no direct pushes. Updated only through the release process.
- **`develop`** — the active development branch. All new work is integrated here.
- **`v0.x`** — per-API-version branches (e.g. `v0.10`, `v0.11`) for maintenance and patch releases of each minor
  version.

Open pull requests against **`develop`**. Version-specific fixes and backports target the relevant **`v0.x`** branch.

### Pull Request Process

1. **Fork and Branch**: Create a feature branch from `develop`
2. **Make Changes**: Follow the code style guidelines
3. **Test**: Run all tests and ensure builds pass
4. **Commit**: Write clear, concise commit messages
5. **PR**: Create a pull request with:
    - Clear description of changes
    - Reference to any related issues
    - Screenshots or examples for UI changes
    - Test results

### Commit Message Guidelines

Follow conventional commits format:

- `feat:` for new features
- `fix:` for bug fixes
- `docs:` for documentation changes
- `style:` for code style changes (formatting, etc.)
- `refactor:` for code refactoring
- `test:` for adding or updating tests
- `chore:` for maintenance tasks

Example:

```
feat(player): add event for player join

Implement PlayerJoinEvent that fires when a player connects to the server.
```

## Project Structure

```
endstone/
├── include/endstone/          # Public C++ API headers (header-only)
├── src/
│   ├── bedrock/              # Bedrock server type definitions and reverse-engineered structures
│   └── endstone/
│       ├── core/             # Core implementation of the Endstone API
│       │   ├── command/      # Command system
│       │   ├── event/        # Event handling
│       │   ├── inventory/    # Inventory management
│       │   ├── level/        # Level/world management
│       │   ├── plugin/       # Plugin system
│       │   ├── scheduler/    # Task scheduling
│       │   └── scoreboard/   # Scoreboard system
│       ├── runtime/          # Runtime hooks into BDS via funchook
│       │   └── bedrock_hooks/  # Hook files for BDS subsystems
│       └── python/           # Python bindings via pybind11
├── endstone/                  # Python package (CLI, plugin loader, metrics)
├── plugins/                   # First-party Python plugins
│   └── devtools/             # Registry inspector (Dear ImGui)
├── tests/                     # Test files
│   ├── bedrock/              # C++ tests for bedrock layer
│   ├── plugin/               # Runtime test plugin (pip install -e tests/plugin)
│   ├── *.cpp                 # C++ unit tests
│   └── *.py                  # Python unit tests
└── conanfile.py              # Dependency management
```

## Architecture Overview

Endstone follows a layered architecture:

### 1. Runtime Hooks (`src/endstone/runtime/bedrock_hooks/`)

- Intercepts BDS functions to inject Endstone behavior
- Each hook file corresponds to a BDS class (think of it as a patch file)

### 2. Bedrock Layer (`src/bedrock/`)

- Type-compatible structures matching BDS internal types
- Required for ABI compatibility when hooking

### 3. Core Layer (`src/endstone/core/`)

- Implements the public API by wrapping bedrock types
- Contains all subsystems: command, event, inventory, level, plugin, scheduler, scoreboard
- Provides the high-level API that plugins use

### 4. Plugin System

- **C++ Plugins**: Loaded as shared libraries with `endstone_` prefix
- **Python Plugins**: Loaded via `endstone.plugin.plugin_loader`
- Both inherit from respective base classes and implement required methods

### Key Dependencies

Managed via Conan in `conanfile.py`:

- `fmt` - Formatting library
- `boost` - Boost libraries
- `pybind11` - Python bindings
- `spdlog` - Logging framework
- `nlohmann_json` - JSON library
- `entt` - Entity component system
- `magic_enum` - Enum reflection
- `sentry-native` - Crash reporting
