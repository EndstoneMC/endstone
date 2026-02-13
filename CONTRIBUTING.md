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

- MSVC 2017+ (Visual Studio 2022 recommended)
- CMake
- Ninja
- Conan 2.0+

**Linux:**

- Clang 20+ with libc++
- CMake
- Ninja
- Conan 2.0+

## Development Setup

### 1. Install Dependencies

First, install Conan and set up your profile:

```bash
pip install conan
conan profile detect
```

### 2. Install Project Dependencies with Conan

**Windows:**

```shell
conan install . --build=missing -s compiler.cppstd=20 -c tools.cmake.cmaketoolchain:generator=Ninja
```

**Linux:**

```bash
conan install . --build=missing -s compiler.cppstd=20 -s compiler.libcxx=libc++ -c tools.cmake.cmaketoolchain:generator=Ninja
```

### 3. Activate Conan Build Environment

**Windows (cmd):**

```shell
.\build\Release\generators\conanbuild.bat
```

**Windows (PowerShell):**

```shell
.\build\Release\generators\conanbuild.ps1
```

**Linux:**

```bash
source ./build/Release/generators/conanbuild.sh
```

## Building

### Build with CMake

```bash
cmake --preset conan-release
cmake --build --preset conan-release
```

### Install from Source (builds Python wheel)

```bash
pip install -U .
```

## Testing

### C++ Tests (GTest)

```bash
ctest --test-dir build/Release
```

### Python Tests

```bash
pytest
```

### Runtime Tests

Runtime tests verify the API works correctly within a live Bedrock Dedicated Server. These tests are essential for validating functionality that depends on the server environment.

**Install the test plugin:**

```bash
pip install -e tests/endstone_test
```

**Run runtime tests:**

1. Start a Bedrock Dedicated Server with Endstone
2. The `endstone_test` plugin will load automatically
3. Run `/test` in-game or from the console to execute the test suite

Before submitting a PR, please ensure:

- All C++ tests pass
- All Python tests pass
- Runtime tests pass (if your changes affect the API)
- The project builds successfully on your target platform

## Generating Type Stubs

After adding new Python bindings, you need to regenerate the type stub files to ensure IDE support and type checking
work correctly.

### When to Regenerate Stubs

You should regenerate stubs after:

- Adding new Python bindings in `src/endstone/python/`
- Modifying existing C++ classes exposed to Python
- Installing the wheel with `pip install .`

### Stub Generation Process

1. **Install the wheel** to make the bindings available:

   ```bash
   pip install -U .
   ```

2. **Install endstone-stubgen** (if not already installed):

   ```bash
   pip install endstone-stubgen
   ```

3. **Run the stubgen script**:

   ```bash
   python scripts/stubgen.py
   ```

   This will:
    - Load the `endstone._python` module from the installed wheel
    - Generate `.pyi` stub files in a temporary `stubs/` directory
    - Process and format the stubs (adjusting imports, removing Registry duplicates, etc.)
    - Copy the processed stubs to the `endstone/` directory
    - Run `ruff format` and `ruff check` to ensure code style compliance

4. **Verify the generated stubs**:

   Check the generated `.pyi` files in the `endstone/` directory to ensure they correctly represent your new bindings.

5. **Commit the changes**:

   The generated stub files should be committed along with your binding changes.

### Example Workflow

```bash
# 1. Make your binding changes
# (e.g., edit src/endstone/python/... )

# 2. Build and install
pip install -U .

# 3. Generate stubs
python scripts/stubgen.py

# 4. Verify
git status  # Check the generated .pyi files

# 5. Commit
git add endstone/*.pyi
git commit -m "docs: update type stubs for new bindings"
```

### Troubleshooting

If you encounter errors running the stubgen script:

- **"endstone-stubgen not installed"**: Install it with `pip install endstone-stubgen`
- **Module not found errors**: Make sure you've installed the wheel with `pip install .`
- **Incomplete stubs**: Verify that your bindings are properly exposed in the pybind11 module

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

### Pull Request Process

1. **Fork and Branch**: Create a feature branch from `main`
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
├── tests/                     # Test files
│   ├── bedrock/              # C++ tests for bedrock layer
│   ├── endstone_test/        # Runtime test plugin (pip install -e tests/endstone_test)
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
