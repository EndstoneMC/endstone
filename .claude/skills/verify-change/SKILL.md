---
name: verify-change
description: Builds Endstone and proves a change with the GTest suite, the installed wheel's pytest run, the stub check, clang-format and ruff. Use before every commit that touches code or build files, after a refactor that must not change behaviour, or when asked to verify, check or test a change.
---

# Verify Change

Read first: `.claude/rules/build.md`, `.claude/rules/qa/testing.md`, `.claude/rules/dev/python.md`.

1. From the repo root, run the CLAUDE.md install and configure commands when `build/RelWithDebInfo` is missing or `conanfile.py` changed.
2. Build: `cmake --build --preset conan-relwithdebinfo`.
3. Run `ctest --test-dir build/RelWithDebInfo --output-on-failure` and find each new test's name in the output.
4. When bindings or `endstone/` changed, run `uv pip install -U .` then `pytest`.
5. Run `git diff --exit-code -- endstone/` to check the stubs are current.
6. Run `clang-format --dry-run --Werror <touched C++ files>`.
7. Run `uvx ruff check <files>` and `uvx ruff format --check <files>` on touched Python.
8. When `src/bedrock` layout changed, state the Linux size asserts are unverified until the Linux CI job passes.
9. Report each command with its result; on a failure, fix it and restart from step 2.
