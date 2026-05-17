---
name: bump-bds
description: Update Endstone to support a new Bedrock Dedicated Server (BDS) version - regenerate the symbol offset tables and port src/bedrock to the new ABI. Use when bumping the supported BDS version (e.g. "add support for BDS 1.26.x", "bump the BDS version").
---

# Bump Endstone to a new BDS version

Two phases:

- **Phase 1** regenerates the symbol offset tables - which hook resolves to
  which address. It tells you *which* symbols broke.
- **Phase 2** ports `src/bedrock/` to the new ABI using the header diff. It
  tells you *why* they broke and *how* to fix them.

## NDA boundary (read first)

This workflow may use two private Mojang-derived artifacts:

- `bedrock-headers` - required for ABI porting. This private repo contains C++
  headers reconstructed from BDS binaries.
- `bedrock_server.pdb` - useful when available for Windows symbol resolution,
  but not guaranteed to be published for every BDS release.

**Both artifacts are NDA-protected.** Never copy header bodies, class
definitions, full member layouts, PDB dumps, symbol listings, or other private
artifact contents into the public `endstone` repo, its commits, PRs, issues,
logs, or this skill. Endstone's `src/bedrock/` is a hand-written, minimal
reimplementation - only what Endstone needs, in Endstone's own naming - which
is the DMCA-safe form. Treat headers, PDBs, generated dumps, and diffs as
private working references only.

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
- `pdbtool` - `cargo install pdbtool` (Microsoft pdb-rs). Reads the Windows PDB
  when one is available.
- `bedrock-headers` for the target version. Headers are mandatory for Phase 2
  ABI porting and must remain private.
- Optional: the Windows BDS PDB (`bedrock_server.pdb`) for the target version.
  Use it when available; some BDS releases do not have a public/accessible PDB.
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
   - If the Windows PDB is unavailable, skip the Windows PDB regeneration for
     now and rely on Linux pattern results plus the required header diff to
     identify ABI changes. Update Windows symbols later if/when a PDB becomes
     available.
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
7. `src/common/world/item`
8. `src/common/world/level/block`
9. `src/common/world/level/{dimension,biome}` and remaining `src/common/world/level/*` (chunk, material, storage, level core)
10. `src/common/world/*` - remaining world (`attribute`, `effect`, `events`, `inventory`, `response`, ...)
11. `src-deps` other than SharedTypes (Certificates, VanillaComponents, ...), then anything else
12. **Cross-validate** - once every ABI change is in, re-review the whole
    `src/bedrock/` diff against the bedrock-headers diff. Every edited function
    signature, vtable slot, member type/order, and structural change must trace
    to a concrete change in `git diff android/r<prev> android/r<new>`. Reject
    anything not backed by the diff: no invented types, no guessed members, no
    hallucinated signatures, no "looks-right" edits. A change that cannot be
    matched to the header diff is wrong - revert or fix it. This stage exists
    because the porting stages, especially when parallelised across agents, can
    introduce plausible but unfounded edits - they must all be matched up.

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
  Endstone's own (`lower_case_`); do not copy Mojang's names. Declare a new
  member's type for real - never a same-size stand-in (`std::string` etc.). If
  the real type would drag in a heavy include chain, add a forward declaration
  to `src/bedrock/forward.h` in alphabetic order and use the type incomplete
  (fine for pointers, references, and container value types like
  `std::vector` / `std::unordered_map`).
- **Width-ambiguous integers** - bedrock-headers are reconstructed from a
  Linux/Android build, where `unsigned long` is 64-bit; on Windows (LLP64) it
  is 32-bit. Whenever the diff shows `unsigned long`, port it as
  `std::uint64_t` so the type is 64-bit on both of Endstone's target platforms.
- **One type per corresponding file** - when a change needs a BDS type Endstone
  does not yet declare, add it in its *own* `src/bedrock/` header mirroring the
  BDS file that defines it (snake_case path), then `#include` it - do not paste
  a foreign type's definition inline into whatever header happens to need it.
  (A forward declaration of a helper type may sit in the header that uses it,
  as BDS does; a forward declaration needed across many headers goes in
  `src/bedrock/forward.h`.)
- **Structural refactors** - when BDS introduces a base class (e.g. 1.26.20's
  `BaseConnectionRequest`), mirror it: add the base header, re-parent the
  derived classes, move the shared members into the base. When BDS removes a
  class, `git rm` the Endstone header once `grep` confirms nothing else
  references it. Follow the BDS structure closely; only the file name differs
  (snake_case). Keep it minimal - declare only what Endstone uses.

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

### Real BDS changes that still need no `src/bedrock/` action

A change can be a genuine BDS change yet need nothing in Endstone, because
Endstone deliberately models only a subset. Rule these out before editing:

- **Additive enum values** beyond Endstone's declared subset. Endstone declares
  only the enumerators it uses; the enum's underlying width is unchanged, so
  values added past Endstone's range need nothing.
- **A class Endstone truncates with `// ...`** - e.g. `NetEventCallback`.
  Virtuals or members added in the undeclared tail are already covered by the
  `// ...`; touch nothing. (One added *in the middle* of the declared part does
  still need handling.)
- **An empty-stubbed type** - Endstone declares some types it never models as
  `struct X {};` (e.g. `NetworkPeer::NetworkStatus`). Internal changes to such a
  type need no action.
- **New free functions or non-virtual methods** Endstone does not declare - no
  layout or vtable impact.

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
- To recover a **renamed** Windows symbol (re-signed, not removed), dump every
  public once with `pdbtool --quiet dump <pdb> psi` and grep for the scope
  (`?<method>@<Class>@@`); the surviving line is the new mangled name. Verify it
  against the bedrock-headers signature before copying it into the config so you
  do not pick the wrong overload.

### BDS itself

- BDS sometimes does **structural refactors**, not just signature tweaks - e.g.
  1.26.20 split packets into a `<Name>Payload` struct plus
  `cerealizer`/`serialize` specializations, and re-parented `ConnectionRequest`
  under `BaseConnectionRequest`. These need real design work in `src/bedrock/`,
  not a mechanical edit - flag them and do them deliberately. When one packet
  shows the `<Name>Payload` split, assume *every* `network/packet/*` got it -
  port them all; `inventory_slot_packet.h` is a good reference for the pattern.
- A function missing from the PDB publics may have been inlined, not removed.
- BDS sometimes promotes a **type alias to a concrete type** - e.g. 1.26.20
  turned `using DimensionType = AutomaticID<Dimension, int>;` into a standalone
  `struct DimensionType`. The mangled name of every symbol taking it changes
  (`...V?$AutomaticID@VDimension@@H@@...` -> `...UDimensionType@@...`) with no
  logic change, so it surfaces as a Windows miss. The fix is structural: make
  Endstone's type a real `struct` too (mirror the BDS layout - often a single
  integer member), replace every direct use of the old underlying type, and
  delete the now-orphaned alias/template header. Editing only the config `name`
  is not enough - `__FUNCDNAME__` comes from Endstone's declaration, so the
  declared type itself must change.
- Removing a class is a **multi-file cascade**, not just one `git rm`. When BDS
  deletes a class, also remove: its `src/endstone/runtime/bedrock_hooks/*.cpp`
  hook, the source-list entries in the relevant `CMakeLists.txt` files, the
  `[[signatures]]` blocks in both `scripts/configs/*.toml`, the entries in the
  generated `src/bedrock/symbol_generator/symbols/*.toml`, and any dependent
  declaration that named the type. `grep` the tree for the class name afterward
  to confirm nothing dangles.

### Symbol table is not optional for the build

- `get_symbol()` is `consteval` and **throws** when a name is absent; the
  generator **drops every `0`-valued entry** from `bedrock_symbols.generated.h`.
  So an unresolved symbol that any TU consumes via `BEDROCK_CALL` /
  `BEDROCK_VAR` / `BEDROCK_CTOR` is a hard **compile** error
  (`C7595: call to immediate function is not a constant expression`), not a
  silently-disabled hook. Phase 1 must resolve *every consumed* symbol before
  the build can go green - Phase 2 alone never produces a buildable tree.
- When a hooked function's signature changes, its `__FUNCDNAME__` changes, so
  the old `name` in `scripts/configs/{windows,linux}.toml` goes stale - update
  it and re-dump, or that hook's TU stops compiling.
- **Temporary verification trick.** To compile-check the Phase-2 port before
  Phase 1 is finished, *temporarily* comment out the `if (val == 0) { ...
  continue; }` skip in `src/bedrock/symbol_generator/main.cpp` so every entry -
  including unresolved `0`s - lands in the generated table. `get_symbol` then
  finds every key and the build goes green, so real ABI/source errors surface.
  **Revert it before committing** - the generator must keep dropping `0`s and
  `get_symbol` must keep throwing so a genuinely missing symbol stays a loud
  error. The real fix is always Phase 1 (re-dump / update stale `name`s).

### Resuming a partially-done bump branch

- Don't trust that an earlier stage is finished. A prior commit may have ported
  only some files of a stage (e.g. some `network/packet/*` but not all) and
  left `.cpp` files referencing the pre-refactor shape. Build early; the first
  compile pass is what surfaces these gaps.

## Record findings here

Append durable, version-independent lessons to **Gotchas**. Keep per-version
specifics (which overload was chosen, what replaced a removed function) in
commit messages and the CHANGELOG, not here.
