# CLAUDE.md

Endstone is a plugin API for Minecraft Bedrock Dedicated Server (BDS). It hooks the BDS binary and exposes a Paper-style API to C++ and Python plugins.

<!-- Add a line here only when it is non-obvious, recurring and actionable. Conventions belong in .claude/rules/, procedures in .claude/skills/. -->

## Commands

Windows needs clang-cl and lld-link (LLVM 18+, the "C++ Clang tools for Windows" Visual Studio component) plus the MSVC toolchain, run from a Visual Studio developer prompt. Linux needs Clang 18+ with libc++. Both need CMake 3.29+ and Conan 2.0+ on PATH.

```shell
conan install . --build=missing                            # after a clone or a conanfile.py change
source build/RelWithDebInfo/generators/conanbuild.sh       # Windows: conanbuild.ps1 or conanbuild.bat
cmake --preset conan-relwithdebinfo
cmake --build --preset conan-relwithdebinfo
ctest --test-dir build/RelWithDebInfo --output-on-failure -R <test>
uv pip install -U .                                        # build and install the wheel
pytest tests/<file>.py::<test>
clang-format --dry-run --Werror <files>
uvx ruff check <files> && uvx ruff format --check <files>
```

The live-server harness in `tests/plugin` runs scenarios inside a real BDS; drive it with the `validate-change` skill, and see `tests/plugin/README.md` for its flags.

## Gotchas

- Never run `conan profile detect`; it overwrites the shipped `.conan2/profiles/default`.
- Never pass `-of` to `conan install`; the preset expects the default `build/RelWithDebInfo`.
- The wheel builds in its own tree, `build/conan_out/build/RelWithDebInfo`, configured against a temporary Python that is deleted afterwards; never run `cmake --preset` over it.
- When `CMakeUserPresets.json` points into `build/conan_out`, delete it and rerun `conan install .`.
- Only `build/RelWithDebInfo` builds the GTest suite; the wheel configures with `BUILD_TESTING=OFF`.
- `pytest` imports the installed wheel; reinstall after changing the bindings or `endstone/`.
- With pip, install as `pip install -U . -C build-dir=./build`; uv reads the build dir from `pyproject.toml`, and without one the C++ tree is rebuilt from scratch.
- `build/package` is never pruned; after deleting or renaming anything under `endstone/`, run `rm -rf build/package` before reinstalling.
- Never edit generated files by hand: the `.pyi` stubs and `lazy_loader` lists under `endstone/`, and `src/bedrock/symbols/*.h`.
- Never copy bedrock-headers content into the repo, a commit, a PR or an issue; it is under NDA.

## Code map

- `include/endstone/`: the header-only public C++ API; plugins link `endstone::endstone`.
- `src/endstone/core/`: the API implemented over BDS types.
- `src/bedrock/`: BDS types reconstructed to its ABI; `symbols/` holds the generated offset tables.
- `src/endstone/runtime/bedrock_hooks/`: detours into BDS, one file per hooked `src/bedrock` header.
- `src/endstone/python/`: the pybind11 bindings; `endstone/` is the Python package with the CLI and plugin loader.
- `tests/`: the GTest and pytest suites; `tests/plugin/` is the live-server harness.
- C++ plugins are `endstone_*` shared libraries built with `endstone_add_plugin()`; Python plugins subclass `endstone.plugin.Plugin`.

## Rules, skills and agents

- Conventions live in `.claude/rules/`: shared ones at the top, team-owned ones in `product/`, `dev/`, `re/` and `qa/`. Most load when a matching file is read; `agents.md`, `comments.md` and `github.md` always load.
- Procedures are skills in `.claude/skills/`; run `verify-change` before every commit that touches code.
- Big tasks go to a team lead in `.claude/agents/<team>/`; `rules/agents.md` maps each team to its work.
