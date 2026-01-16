# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Endstone is a plugin API for Minecraft Bedrock Dedicated Servers (BDS), supporting both Python and C++ plugins. It hooks into the BDS binary to expose a high-level Bukkit-like API for plugin development.

## Build Commands

### Prerequisites
- Windows: MSVC 2017+ (Visual Studio 2022 recommended)
- Linux: Clang 9+ with libc++
- CMake, Ninja, Conan 2.0+

### Install dependencies with Conan
```shell
pip install conan
conan profile detect

# Windows
conan install . --build=missing -s compiler.cppstd=20 -c tools.cmake.cmaketoolchain:generator=Ninja

# Linux
conan install . --build=missing -s compiler.cppstd=20 -s compiler.libcxx=libc++ -c tools.cmake.cmaketoolchain:generator=Ninja
```

### Activate Conan build environment
```shell
# Windows (cmd)
.\build\Release\generators\conanbuild.bat

# Windows (PowerShell)
.\build\Release\generators\conanbuild.ps1

# Linux
source ./build/Release/generators/conanbuild.sh
```

### Build with CMake
```shell
cmake --preset conan-release
cmake --build --preset conan-release
```

### Install from source (builds Python wheel)
```shell
pip install -U .
```

## Testing

### C++ tests (GTest)
```shell
ctest --test-dir build/Release
```

### Python tests
```shell
pytest tests/endstone/python
```

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
