---
name: bump-bds-version
description: Update Endstone to support a new Bedrock Dedicated Server (BDS) version - regenerate the symbol offset tables and port src/bedrock to the new ABI. Use when bumping the supported BDS version (e.g. "add support for BDS 1.26.x", "bump the BDS version").
---

# Bump Endstone to a new BDS version

Two phases:

- **Phase 1** regenerates the symbol offset tables - which hook resolves to
  which address. It tells you *which* symbols broke.
- **Phase 2** ports `src/bedrock/` to the new ABI using the header diff. It
  tells you *why* they broke and *how* to fix them.

## NDA boundary (read first)

Phase 2 uses `bedrock-headers`, a private repo of C++ headers reconstructed
from Mojang BDS binaries. **That content is NDA-protected.** Never copy header
bodies (class definitions, full member layouts) into the public `endstone`
repo, its commits, PRs, or this skill. Endstone's `src/bedrock/` is a
hand-written, minimal reimplementation - only what Endstone needs, in
Endstone's own naming - which is the DMCA-safe form. The header diff is a
private working reference only.

---

# Phase 1 - regenerate the symbol tables

## How the symbol pipeline works

```
scripts/configs/{windows,linux}.toml          signature configs, hand-maintained
        |  scripts/dump_symbols.py
        v
src/bedrock/symbol_generator/symbols/{windows,linux}.toml   resolved offsets
        |  symbol_generator (CMake target)
        v
bedrock_symbols.generated.h                    std::array of name -> offset
```

`src/bedrock/symbol.h` `get_symbol()` looks a symbol up by `__FUNCDNAME__` - the
mangled name of Endstone's own declaration in `src/bedrock/`. The symbol-table
key IS the signature of Endstone's reimplementation. Unresolved symbols are
written as `0` and skipped by the generator (that hook is disabled; the build
still succeeds).

## Prerequisites

- `uv` - runs `dump_symbols.py` (PEP 723 inline deps, no manual install).
- `pdbtool` - `cargo install pdbtool` (Microsoft pdb-rs). Reads the Windows PDB.
- The Windows BDS PDB (`bedrock_server.pdb`) for the target version.
- The target version must be published in the `EndstoneMC/bedrock-server-data`
  repo - the Linux path downloads the binary from it. Check its `versions.json`.

## Procedure

1. **Branch** off the current release branch (e.g. `v0.11`):
   `git checkout -b feat/<NN.NN>-support` (naming follows `feat/26.10-support`).
2. **Bump the config versions** - set `version = "<X.Y.Z>"` in both
   `scripts/configs/windows.toml` and `scripts/configs/linux.toml`, using the
   3-component release string from bedrock-server-data `versions.json`.
3. **Regenerate** (run in the background, 25 s to a few minutes):
   - Windows: `uv run --script scripts/dump_symbols.py scripts/configs/windows.toml --pdb <path>/bedrock_server.pdb`
   - Linux: `uv run --script scripts/dump_symbols.py scripts/configs/linux.toml`
4. **Triage the failures.** The two platforms fail differently:
   - **Windows (PDB, by name):** a miss = the mangled name is gone from the PDB
     = the signature changed (MSVC mangling encodes the full signature incl.
     return type). A Windows hit is name-verified.
   - **Linux (byte-pattern scan):** a miss = the `pattern` in `configs/linux.toml`
     no longer matches. The function usually still exists - the pattern went
     stale. A Linux hit is a pattern match *labelled* with the config name; it
     is not name-verified.
   - Failed on **both** -> real signature/API change.
   - **Windows only** -> mangled name changed; if only the return type changed,
     the Linux Itanium name is unaffected (Itanium omits return types).
   - **Linux only** -> stale byte pattern; re-extract it (IDA / Ghidra).

The failure list is the input to Phase 2.

---

# Phase 2 - port src/bedrock to the new ABI

## Source: the header diff

`dwarf2cpp` reconstructs C++ headers from a DWARF-bearing BDS build (the Android
build `libminecraftpe.so` carries DWARF; the Windows/Linux server binaries are
stripped). Output lands in `bedrock-headers`, one branch per BDS release
(`android/r26_u1`, `android/r26_u2`, ...).

1. `dwarf2cpp <libminecraftpe.so> --base-dir <build-root> -o <out>` (or `uvx dwarf2cpp`).
2. In `bedrock-headers`: `git checkout -b android/r<NN>_u<N>`, place the output, commit.
3. `git diff android/r<prev> android/r<new>` is the change set.

## The actionable set

`src/bedrock/` is ~655 hand-maintained headers - a small subset of BDS. Most of
a release diff (5000+ files) touches nothing Endstone declares. So:

> **actionable work = (changed headers) intersect (the 655 src/bedrock headers)**

Match by normalized basename (lowercase, strip `_` and `-`): bedrock-headers
`Mob.h` <-> Endstone `mob.h`; `BlockSource.h` <-> `block_source.h`.

## Staged review order

Review the diff in stages - foundational types first, so later stages do not
rework. Scope: `handheld/` only; **skip `handheld/src-client/`** (game client).
Within each stage, deep-dive only the intersection. (The first attempt used 3
coarse stages; "handheld/src non-world" alone was 467 files / 44 intersecting -
too big. Use this finer split:)

1. `src-deps/SharedTypes` - shared types and enums
2. `src/common/network` - packets, network types, packet-id / disconnect enums
3. `src/common/server` (incl. `server/commands`) - server and command system
4. `src/common/entity` - ECS components
5. `src/common/{certificates,resources,scripting,platform,locale,gameplayhandlers,...}` - remaining non-world
6. `src/common/world/actor`
7. `src/common/world/block`
8. `src/common/world/item`
9. `src/common/world/{dimension,biome}`
10. `src/common/world/*` - remaining world
11. `src-deps` other than SharedTypes (Certificates, VanillaComponents, ...), then anything else

## Applying a change

For each intersecting header with a *real* change (use Gotchas to rule out
noise first), update Endstone's `src/bedrock/` declaration:

- **Function signatures** (especially hooked / `ENDSTONE_HOOK`) - parameter
  types, const/ref, return type must match BDS exactly, or `__FUNCDNAME__`
  stops matching the symbol.
- **Virtual functions** - the vtable order must match BDS. An added / removed /
  reordered virtual shifts every slot below it; mirror the new order (use `= 0`
  placeholders for virtuals Endstone does not implement).
- **Members** - type, order and size must match for layout. Member *names* stay
  Endstone's own (`lower_case_`); do not copy Mojang's names.

Then update the mangled `name` in `scripts/configs/{windows,linux}.toml`,
re-run the dumper, and update any affected hook in
`src/endstone/runtime/bedrock_hooks/`.

ABI edits are easy to get subtly wrong - a wrong vtable slot or member offset
corrupts memory silently, caught by neither a compile nor a PR review. Build
and test iteratively; do not batch many unverified ABI edits.

# Finish

Re-run the dumper until all symbols resolve (or remaining `0`s are understood).
Build (`cmake --build --preset conan-release`), run tests, add a CHANGELOG
entry ("Added support for BDS X.Y.Z"), commit, open the PR.

## Gotchas

### dwarf2cpp diff noise - no real BDS change, skip these

- **Versioned-namespace churn** - `SharedTypes/v1_26_10/...` becomes
  `v1_26_20/...`; most of that subtree's diff is just the version bump.
- **Template-instantiation churn** - `SharedPtr.h` / `SharedCounter` and
  similar enumerate concrete instantiations (`CopperBlock<ThinFenceBlock>`,
  `GlazedTerracottaBlock`, ...). The set churns every release; Endstone uses
  its own templates - ignore entirely.
- **File regrouping** - dwarf2cpp regroups types into different generated
  files. A file shown as fully deleted (e.g. `CommonTypes.h`) often just means
  its types moved. Confirm a type is genuinely gone, not relocated.
- **Lambda source-location churn** - `match<(lambda at .../Foo.cpp:47:3)>` -
  lambda line/column numbers shift every build. Pure noise.
- **Declaration reordering** - declarations get reordered within a file; the
  diff shows -/+ pairs of identical content moved.

### Tooling

- **lief's free build cannot read PDBs** - `lief.pdb.load()` fails with
  "DebugInfo are not available for this build" (PDB/DWARF is LIEF-extended,
  licensed). The Windows path shells out to `pdbtool` instead.
- **dbghelp is too slow** - loading a ~2 GB BDS PDB through dbghelp takes
  minutes; `pdbtool dump ... psi` takes ~25 s.
- **PowerShell `>` writes UTF-16** - a redirected `pdbtool` dump will not grep
  with ASCII patterns; capture via the script's subprocess (UTF-8) or run
  `iconv -f UTF-16 -t UTF-8` first.
- Windows symbol offsets are RVAs: `RVA = section RVA + symbol offset`.
- Some symbols are never public (function-local statics like `getI18n::result`,
  some data) and never appear in `pdbtool ... psi` - keep them on the
  byte-signature path.

### BDS itself

- BDS sometimes does **structural refactors**, not just signature tweaks - e.g.
  1.26.20 split packets into a `<Name>Payload` struct plus
  `cerealizer`/`serialize` specializations, and re-parented `ConnectionRequest`
  under `BaseConnectionRequest`. These need real design work in `src/bedrock/`,
  not a mechanical edit - flag them and do them deliberately.
- A function missing from the PDB publics may have been inlined, not removed.

## Record findings here

Append durable, version-independent lessons to **Gotchas**. Keep per-version
specifics (which overload was chosen, what replaced a removed function) in
commit messages and the CHANGELOG, not here.
