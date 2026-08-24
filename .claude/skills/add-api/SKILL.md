---
name: add-api
description: Add a new public API, event, enum, or method to Endstone end-to-end across all layers (public C++ header -> bedrock layer -> core -> runtime hook -> Python binding), then build, regenerate stubs, and update the CHANGELOG. Use when adding or exposing a feature in the API, e.g. "add a new event", "expose X on Player", "add a RespawnReason enum", "implement <feature> in the API".
---

# Add a new API / feature to Endstone

Endstone exposes a Bukkit-like API over Bedrock Dedicated Server (BDS). A new
API touches several layers; do them in order, then build + regenerate stubs +
changelog. Skip layers that don't apply (a pure data type needs no hook; a new
bedrock accessor needs no public API).

> **Mirror BDS over being clever.** When in doubt, reproduce exactly what BDS /
> Bukkit do — same names, same shapes, same call paths. "Mirror bds is more
> important" than a tidier-but-divergent design. Don't invent.

## Layers (source of truth -> exposure)

1. **Public C++ API** — `include/endstone/...`
   The header-only API plugin authors compile against. Define the class / event
   / enum / method here with Doxygen comments. It was ported from Bukkit, so
   **mirror Paper/Bukkit** for naming and semantics where an equivalent exists.
   - **Mirror Bukkit's package/folder structure**, not just names:
     `org.bukkit.block.Biome` -> `include/endstone/block/biome.h`,
     `org.bukkit.block.Container` -> `include/endstone/block/container.h`. Put
     the header in the subfolder matching the Bukkit package; the Python module
     usually follows the same grouping.
   - C++ **enum values are `CamelCase`** (`Death`, `EndPortal`); Python exposes
     them `UPPER_CASE` (see step 5).
   - **Registry types** (a named, enumerable type like `BlockType`, `Biome`):
     `class Biome : public Registry<Biome>::Type {};` + `using BiomeId =
     Identifier<Biome>;`. Look-ups are by string id; the public surface is
     `id`, `translation_key`, static `get(name)`, plus `==`/`!=`/`hash`/`str`.
   - Doc conventions live in the **`cpp-docs`** skill (no `@brief`, etc.).
   - Verify Bukkit-derived wording/behaviour against the **Paper Javadocs**, not
     memory: <https://jd.papermc.io/paper/26.2/>. The version is part of the URL —
     an older one serves stale wording.

2. **Bedrock layer** — `src/bedrock/...`
   Type-/ABI-compatible reconstructions of BDS. Expose whatever the core needs
   from BDS here (an accessor over a member, a function declaration, or a full
   re-implementation of a non-virtual BDS function). See **Reconstruction rules**
   below — this is the layer where correctness is easiest to get subtly wrong.

3. **Core** — `src/endstone/core/...`
   Implements the public API by wrapping bedrock types. Most `Endstone*` classes
   live here.
   - For a **registry type**, add an `EndstoneRegistry<Endstone*, ::BdsType>`
     specialization in `registry.cpp` (its `create()` pre-populates the cache,
     e.g. via `level.getBiomeRegistry().forEachBiome(...)`) and register it in
     `server.cpp` (`registries_[typeid(Biome)] = ...::create();`). The
     `EndstoneX` wrapper holds a `const ::BdsType&` and reports `getId()` /
     `getTranslationKey()` from it.
   - For a **complex/polymorphic tree** exposed to Python (e.g. `BlockState`
     subclasses), derive the base from `endstone::Object` so `poly.h`'s
     `polymorphic_type_hook` can downcast — the hook only fires for
     `Object`-derived types.

4. **Runtime hooks** — `src/endstone/runtime/bedrock_hooks/...`
   Where you intercept BDS to *fire events* or inject behaviour. Gameplay events
   are caught in `script_*_gameplay_handler.cpp` (BDS dispatches its own event,
   the hook re-fires the `endstone::` event via `callEvent`). One file per BDS
   subsystem. (Read-only accessors like `Block.getBiome()` need **no** hook.)

5. **Python binding** — `src/endstone/python/*.cpp`
   ```cpp
   auto cls = py::class_<MyEvent, PlayerEvent>(m, "MyEvent", "...");
   py::native_enum<MyEvent::Reason>(cls, "Reason", "enum.Enum", "...")
       .value("DEATH", MyEvent::Reason::Death)        // UPPER_CASE
       .value("END_PORTAL", MyEvent::Reason::EndPortal)
       .export_values()
       .finalize();
   cls.def_property_readonly("reason", &MyEvent::getReason, "...");   // snake_case
   ```
   Docstring conventions live in the **`python-doc`** skill.

## Reconstruction rules (`src/bedrock`)

Bedrock types must be **ABI-compatible** with BDS, so the compiler computes the
right per-platform offsets. Confirm everything against the `../bedrock-headers`
repo (NDA — never leak) and the IDA databases (`ida-pro` skill / idalib MCP).
Reconstructed `brstd` containers live in
`../bedrock-headers/src/base/Platform/include/Platform/brstd/`; dwarf2cpp dumps
there are **template-instantiated** (verbose, per-type) — read the structure
from them, infer the generic template. The Linux `*_symbols.debug` build is far
less inlined than Windows — use it to read *mechanism*; use the Windows build
matching the target version for *offsets*.

Match the BDS header **1:1** for everything that affects ABI or semantics:
- **Types exactly**, including reference-vs-pointer (`getElement` returns
  `const Biome &`, not `const Biome *` — a ref/ptr mismatch is a real bug).
- `class` vs `struct` keyword.
- Member **declaration order** and the **count/order of virtual functions**
  (these drive offsets and vtable slots — a missing/extra virtual shifts every
  later slot and silently dispatches the wrong function).
- `const`-qualification, template form (explicit specialization vs generic),
  enum underlying types.

But name to **Endstone** conventions, NOT the BDS spelling:
- Members: snake_case-ify the BDS name (`m2DBiomes` -> `biomes_2d`,
  `mUse3DBiomeMaps` -> `use_3d_biome_maps_`, `mVal` -> `value_`), trailing `_`
  for private; methods stay `camelBack`. The existing `LevelChunk` uses
  `level_`/`dimension_`, not `mLevel`.

Layout mechanics:
- **No explicit offsets / no manual padding** — cross-platform (Linux + Windows
  ABIs differ). Use a real-typed member of the right type so the compiler lays
  it out; pin the result with `BEDROCK_STATIC_ASSERT_SIZE(T, win, linux)` /
  `static_assert(sizeof(T) == ...)`.
- Zero-size empty members (stateless hashers, comparators): use the
  `NO_UNIQUE_ADDRESS` macro (`[[msvc::no_unique_address]]` on the clang-cl/MSVC
  ABI — plain `[[no_unique_address]]` is **ignored** there). Guard the macro
  definition with `#ifndef NO_UNIQUE_ADDRESS`. (This is how `SpinLockImpl`
  stays 24 bytes with its `std::hash` member.)

Hooking:
- **Hooking a virtual by symbol: use a standalone detour holder, not a derived
  class.** Writing `class TridentItem : public Item { ... override; }` compiles
  clean but forces out a vtable, typeinfo and deleting destructor, which drag in
  every `Item::` virtual as undefined. The runtime links `-Wl,--no-undefined`,
  so it fails at **link**, not compile - single-TU verification will not catch
  it. Declare a standalone class with the member alone, as the
  `ScriptPlayerGameplayHandler` hooks do.
- **Keep `virtual` on that declaration.** It is load-bearing on Windows: MSVC
  encodes it in the decoration (`U` = public virtual, `Q` = public non-virtual),
  and `hook::install()` pairs detours to targets by exported mangled name. Drop
  `virtual` and the name decorates differently, misses the table, and throws
  "Unable to find target function for detour" at startup. Check the emitted
  object with `nm`: it should define exactly the mangled name in
  `symbols/<platform>.h`, with `GLOBAL DEFAULT` visibility, and no undefined
  base-class symbols.
- **Prefer a `[[signatures]]` entry + `ENDSTONE_HOOK` over a vtable hook.** A
  vtable ordinal is per-platform and fails *silently* when it drifts - it just
  dispatches the wrong function. A byte pattern fails loudly at
  `dump_symbols.py` time, and on Windows `--pdb` resolves the name outright.
  Reserve `vhook::create<Ordinal>` for targets where no stable pattern can be
  cut, and say why in the PR.

Packets:
- **Never touch raw wire bytes, in either direction.** Don't hand-parse an
  inbound payload, and don't compose an outbound one by writing fields into a
  `BinaryStream` or sending raw bytes. Reconstruct the packet layout, build it
  with `MinecraftPackets::createPacket(...)`, assign the typed payload, and
  send with `sendNetworkPacket`. Field order, widths and varint encoding belong
  to BDS; a reconstruction gets them from the compiler and breaks the build
  when they move, where hand-written bytes just emit a malformed packet.
  The one sanctioned exception is a payload whose types would drag in a whole
  subsystem for no gain: `EndstonePlayer::spawnParticle` hand-writes
  SpawnParticleEffect on purpose, because doing it faithfully means
  reconstructing Molang. Invoking the exception means naming the subsystem
  you are avoiding, not just preferring bytes.
- Reconstruct only the **layout** — the payload member, `serialization_mode`,
  and the size assert. Don't override `getId`, `getName`, `write` or `_read`:
  we never construct these by value, we `static_cast` a packet BDS already
  built, so the overrides are dead weight that emit a vtable in our TU and pull
  in `<stdexcept>` for bodies that only throw. `emote_packet.h` is the model.
  The exception is a packet Endstone genuinely constructs and sends.

Helpers:
- **Don't add a one-use helper lambda or helper function** to factor out a
  condition or a bit of formatting. Inline the expression, or bind it to a
  plain `const auto` local named for the value, matching what the surrounding
  function already does.

Markers & faithfulness:
- **`// Endstone` marks only things we invented or changed** (synthetic helpers,
  `// Endstone: private -> public`). A faithful re-declaration of a real BDS
  member/function gets **no marker** — confirm it's real first.
- When you re-implement a non-virtual BDS function, match its **algorithm and
  call path**, not just the result. Prefer the real container method
  (`flat_set::find` via a transparent comparator) over a hand-rolled linear
  scan; prefer the real callee (`Dimension::getDefaultBiomeId()` ->
  `lookupById`) over a convenient near-equivalent. If you must diverge (a callee
  sits at an unreconstructed deep vtable slot, etc.), leave a `// NOTE:` saying
  what BDS does and why you differ.
- **Watch for prior misreconstructions.** A wrongly-named/typed virtual at the
  *correct* vtable slot still "links" but corrupts results — e.g.
  `Dimension::getDefaultBiome()` returning `HashedString` was actually BDS's
  `getDefaultBiomeId()` returning `BiomeIdType` at that slot (non-virtual
  neighbours like `getSeaLevel` don't take slots, so cross-check the *virtual*
  order, not line numbers). Fix these when you touch them.
- Use BDS's own vocabulary types: e.g. `brstd::function_ref<void(const T&)>`
  where BDS takes one (its second template arg defaults to the first), not
  `std::function`.

Name clashes (`endstone::X` vs bedrock `::X`):
- In an `endstone::`-namespace TU where both `endstone::Biome` and the bedrock
  `::Biome` are visible, unqualified `Biome` resolves to `endstone::Biome`.
  Qualify the bedrock type as `::Biome` (lambda params, casts, etc.). This bit
  `vanilla_data.cpp`'s `forEachBiome` lambda and is the same trap as
  `::BlockType`.

`<windows.h>` `min`/`max` pollution:
- RakNet headers (`<RakNetTypes.h>`, `<RakPeerInterface.h>`, `<SocketIncludes.h>`)
  pull in `<winsock2.h>`/`<windows.h>`, whose `min`/`max` macros clobber
  `std::min`/`std::max` in any TU that transitively includes them. Guard the
  **include site** (not a global define) with `#ifdef _WIN32` + `#ifndef`-guarded
  `WIN32_LEAN_AND_MEAN` and `NOMINMAX` before the RakNet `#include`.

## Build, stubs, changelog (after the code compiles)

Tooling note: there is **no `python` on PATH** — use the project venv
(`.venv/Scripts/python.exe`) or `uv run --no-project python`. On Windows the C++
build needs a VS Developer environment: `VsDevCmd.bat -arch=amd64` then
`build/RelWithDebInfo/generators/conanbuild.bat` (clang-cl + lld-link).

6. **Build.** The local dev tree is the one you drive by hand; it relinks
   `_python`, which is what regenerates the stubs (step 7):
   ```shell
   cmake --build --preset conan-relwithdebinfo
   ```
   To `import endstone` or run `pytest`, install the wheel into `.venv` as well:
   ```shell
   rm -rf ./build/package/endstone-*.dist-info   # stale dist-info => version mismatch
   uv pip install -U .
   ```
   Do **not** add `-C build-dir=./build`: `[tool.uv] config-settings` in
   `pyproject.toml` already supplies it, and passing it twice reaches
   `conan-py-build` as a list (`TypeError: argument should be a str or an
   os.PathLike object ... not 'list'`). See `bump-bds` for the wider
   build/toolchain context. **Never use an editable install** (a stale in-tree
   `endstone/_python.pyd` will shadow site-packages).
   - To verify one reconstruction TU in isolation (e.g. when unrelated files are
     mid-refactor), build just its object with the real flags:
     `ninja -C build/RelWithDebInfo src/bedrock/CMakeFiles/bedrock.dir/<path>.cpp.obj`.
     This also confirms your `static_assert` sizes/offsets without a full link.

7. **Python stubs regenerate themselves — there is nothing to run by hand.**
   `ENDSTONE_GENERATE_STUBS` (ON by default) hangs an `endstone_stubs` target off
   `_python`, so every build that relinks the extension rewrites
   `endstone/**/__init__.pyi` *and* the `lazy.attach` re-export lists in
   `endstone/**/__init__.py`. They can never go stale, and a generation failure
   fails the build.
   - **Never invoke `scripts/stubgen.py` yourself.** It is the CMake driver and
     requires `--extension` (plus `--engine`, `--pattern-file`, `--source-dir`,
     `--work-dir`); CMake supplies them from the built extension.
   - Generation writes into the source tree, so two build dirs configured against
     one checkout would race. Drive one, or pass
     `-DENDSTONE_GENERATE_STUBS=OFF` in the others.
   - Confirm `git diff` shows only your additions. On Windows ruff rewrites every
     `__init__.pyi` with CRLF, so `git status` can list ~23 files while
     `git diff --stat` (line-ending-normalized) shows only the real change.
   - Stub pieces with no pybind11 counterpart (the generic `Registry`,
     `Server.get_registry`, the `_T` typevar) live in
     `src/endstone/python/stubgen.pat`. If a rename stops a pattern matching, the
     build fails and names the pattern.

8. **CHANGELOG** — add a human-readable entry under `## [Unreleased]` in the
   right group (`Added` / `Changed` / `Deprecated` / `Removed` / `Fixed` /
   `Security`). Keep a Changelog 1.1.0; prefix breaking changes with
   `**BREAKING**:`. Write for server admins / plugin devs; omit internal-only
   details (reconstruction/layout changes don't belong here).

## Runtime validation

Compile-time `static_assert`s prove *layout*, not *behaviour*. The authoritative
runtime check is the `endstone_test` plugin's in-server `/test`, which runs the
`tests/plugin` pytest suite live against a running server — the only way
to confirm a reconstructed read path returns the right value. **You can't issue
it; ask the user to run `/test`** after the wheel is installed.

## Conventions checklist
- Header path mirrors the **Bukkit package** (`org.bukkit.block.X` -> `block/x.h`).
- Reconstruct shapes 1:1 (types, ref/ptr, order, vtable slots); name members
  Endstone-style (`snake_case_`), not BDS `mXxx`.
- No explicit offsets/padding; pin sizes with `static_assert` / `NO_UNIQUE_ADDRESS`.
- `// Endstone` marker in `src/bedrock` only for inventions, never real BDS APIs.
- Enum: `CamelCase` in C++, `UPPER_CASE` in Python (`.export_values().finalize()`).
- Python properties/methods are `snake_case`; mirror the C++ getter name.
- Commit style `type(scope): summary` (e.g. `feat(event): ...`); **no
  `Co-Authored-By` line**.

## Worked examples

**Enum on an event — `PlayerRespawnEvent.RespawnReason`** (issue #352), 5 files:
- `include/endstone/event/player/player_respawn_event.h` — `enum class RespawnReason { Death, EndPortal }`, `(Player&, RespawnReason)` ctor, `getRespawnReason()`.
- `src/bedrock/world/actor/player/player.h` — `isRespawningFromTheEnd()` accessor over the real BDS member (no `// Endstone` marker).
- `src/endstone/runtime/bedrock_hooks/script_player_gameplay_handler.cpp` — derive the reason in the `PlayerRespawnEvent` hook.
- `src/endstone/python/event.cpp` — `RespawnReason` native_enum + `respawn_reason` property.
- `CHANGELOG.md` — one bullet under `Added`.

**Registry type + read accessor — `Biome` / `Block.biome`** (a deeper bedrock
reconstruction): public `Biome : Registry<Biome>::Type` (`block/biome.h`) +
`Block::getBiome()`; bedrock `BlockSource::getBiome -> LevelChunk::getBiome ->
SubChunkStorage<Biome>::getElement` reconstructed from the headers (full
`LevelChunk` biome layout, `getDefaultBiomeId` fix, transparent `flat_set::find`
for `lookupById`); core `EndstoneBiome` + `EndstoneRegistry<Biome, ::Biome>` in
`registry.cpp` + `server.cpp` dispatch; `py::class_<Biome>` + `Block.biome`
property. No runtime hook (read-only). Then steps 6-8.
