---
name: bump-bds
description: Update Endstone to support a new Bedrock Dedicated Server (BDS) version - regenerate the symbol offset tables and port src/bedrock to the new ABI. Use when bumping the supported BDS version (e.g. "add support for BDS 1.26.x", "bump the BDS version").
---

# Bump Endstone to a new BDS version

Every bump is the same two jobs:

1. **Regenerate the symbol offset tables** - which hook resolves to which
   address (`src/bedrock/symbols/{windows,linux}.h`).
2. **Port `src/bedrock/` to the new ABI** - fix the signatures, vtable orders
   and member layouts that changed, so those offsets land on the right code and
   memory is read at the right offsets.

*How you discover what changed* (and the new signatures job 1 needs) depends on
your reference material. Pick the scenario - the rest of the skill is split
along it:

- **Scenario A - full (you have `bedrock-headers`).** The dwarf2cpp header diff
  tells you exactly what changed and why. The canonical path; use it whenever
  headers for the target version exist. -> *Scenario A* below.
- **Scenario B - limited (no headers; only IDA databases).** You have a Linux
  BDS database (RTTI present) and a Windows BDS database, and maybe a *stale*
  PDB - but no header diff. You reverse-engineer each ABI change directly from
  the binaries, driven by **symbol misses** (build) and **runtime crashes**.
  -> *Scenario B* below.

Both scenarios share **The symbol pipeline**, **Editing src/bedrock correctly**,
**Finish**, and most **Gotchas**. A real bump is often mostly A with a few B
spot-checks (confirm a vtable against the binary), or runs as B until headers
land and then finishes as A.

## NDA boundary (read first)

This workflow may use two private Mojang-derived artifacts:

- `bedrock-headers` - C++ headers reconstructed from BDS binaries. Required for
  Scenario A; **absent by definition in Scenario B**.
- `bedrock_server.pdb` - useful for Windows symbol resolution when available and
  *current*, but not published for every release (and a stale one is a trap -
  see Scenario B).

**Both artifacts are NDA-protected.** Never copy header bodies, class
definitions, full member layouts, PDB dumps, symbol listings, or other private
artifact contents into the public `endstone` repo, its commits, PRs, issues,
logs, or this skill. Endstone's `src/bedrock/` is a hand-written, minimal
reimplementation - only what Endstone needs, in Endstone's own naming - which is
the DMCA-safe form. Treat headers, PDBs, generated dumps, decompiler output, and
diffs as private working references only.

---

# The symbol pipeline (shared)

## How it works

```
scripts/configs/{windows,linux}.toml          signature configs, hand-maintained
        |  scripts/dump_symbols.py
        v
src/bedrock/symbols/{windows,linux}.h         std::array of name -> offset (committed)
```

`src/bedrock/symbol.h` `get_symbol()` looks a symbol up by `__FUNCDNAME__` - the
mangled name of Endstone's own declaration in `src/bedrock/`. The symbol-table
key IS the signature of Endstone's reimplementation. Unresolved symbols are
written as `0` and dropped by the dumper (that hook is disabled; the build still
succeeds, unless any TU actually consumes the missing name - then `consteval`
`get_symbol()` throws at compile time).

## Prerequisites

- `uv` - runs `dump_symbols.py` (PEP 723 inline deps, no manual install).
- `pdbtool` - `cargo install pdbtool` (Microsoft pdb-rs). Reads a Windows PDB
  when one is available *and current*.
- The target version published in `EndstoneMC/bedrock-server-data` (the Linux
  path downloads the binary from it - check its `versions.json`).
- **Scenario A only:** `bedrock-headers` for the target version (must remain
  private).
- Optional: the Windows BDS PDB (`bedrock_server.pdb`) for the target version.
  Confirm its version matches the exe before trusting `--pdb` (a stale PDB
  silently mis-resolves moved symbols - see Scenario B).

## Procedure

1. **Branch** off the current release branch (e.g. `v0.11`):
   `git checkout -b feat/<NN.NN>-support` (naming follows `feat/26.10-support`).
2. **Bump the config versions** - set `version = "<X.Y.Z>"` in both
   `scripts/configs/windows.toml` and `scripts/configs/linux.toml`, using the
   3-component release string from bedrock-server-data `versions.json`.
3. **Regenerate** (run in the background, 25 s to a few minutes):
   - Windows: `uv run --script scripts/dump_symbols.py scripts/configs/windows.toml --pdb <path>/bedrock_server.pdb`
   - Linux: `uv run --script scripts/dump_symbols.py scripts/configs/linux.toml`
   - No (current) Windows PDB? Drop `--pdb` and rely on the byte-`pattern`
     fallback per entry; resolve the gaps the Scenario-B way.
4. **Triage the failures** - this tells you *which* symbols broke (the input to
   the porting work). *How* you find the fix is per-scenario.
   - **Windows (PDB by name, then byte pattern):** each entry is looked up by
     mangled `name` in the PDB; entries the PDB has no public record for
     (lambdas, function-local statics) fall back to scanning the entry's
     `pattern`. A miss means *both* failed - the mangled name is gone (MSVC
     encodes the full signature incl. return type, const-ness and access) *and*
     the byte pattern no longer matches. A PDB hit is name-verified; a fallback
     hit ("Found signature (fallback)") is only pattern-verified, like Linux.
   - **Linux (byte-pattern scan):** a miss = the `pattern` in
     `configs/linux.toml` no longer matches. The function usually still exists -
     the pattern went stale. A Linux hit is a pattern match *labelled* with the
     config name; it is not name-verified.
   - Failed on **both** -> real signature/API change.
   - **Windows only** -> the mangled name changed: signature, return type,
     const-ness or access. Itanium omits the return type, so a pure return-type
     change leaves the Linux name intact. A const/access change is fixed in
     Endstone's `src/bedrock/` *declaration* (`__FUNCDNAME__` derives from it),
     not the config alone.
   - **Linux only** -> stale byte pattern; re-extract it (see Gotchas).

---

# Editing src/bedrock correctly (shared)

Whatever told you *what* changed, the edit obeys the same rules. ABI edits are
easy to get subtly wrong - a wrong vtable slot or member offset corrupts memory
silently, caught by neither a compile nor a PR review. **Build and test
iteratively; never batch many unverified ABI edits.**

- **Function signatures** (especially hooked / `ENDSTONE_HOOK`) - parameter
  types, const/ref, return type must match BDS exactly, or `__FUNCDNAME__` stops
  matching the symbol.
- **Virtual functions** - the vtable order must match BDS. An added / removed /
  reordered virtual shifts every slot below it; mirror the new order (use `= 0`
  placeholders for virtuals Endstone does not implement). Only the slot *count*
  matters for ABI - one `virtual void <name>() = 0;` is one slot whatever its
  signature.
- **Members** - **type, order and size must match for layout**; member *names*
  stay Endstone's own (`lower_case_`), never Mojang's. Width-ambiguous integers:
  bedrock-headers/Linux build `unsigned long` is 64-bit, Windows (LLP64) 32-bit -
  port `unsigned long` as `std::uint64_t` (64-bit on both targets).
- **The first member after a base is per-ABI.** Itanium allocates derived
  members from `dsize(base)`, MSVC from `sizeof(base)`, so a first member with
  alignment < 8 lands at 44 on Linux and 48 on Windows under `Packet` (48/44).
  Mirror whatever BDS's own class starts with - an inline scalar shifts the same
  way, an 8-aligned sub-object does not. `clang++ --target=x86_64-pc-linux-gnu
  -Xclang -fdump-record-layouts` on a self-contained repro prints `dsize` and
  every offset; run it for both targets rather than reasoning about it.
- **Template arguments** - a class template's *default* arguments are part of
  its declaration: copy them verbatim, never guess (e.g. `brstd::bitset`'s
  word-type defaults to `unsigned int`). Never drop an *explicit* argument to
  lean on a default; spell every argument the actual instantiation spells
  (apply the int-width rule to those too).
- **One type per corresponding file** - a needed BDS type Endstone lacks goes in
  its *own* `src/bedrock/` header mirroring the BDS file (snake_case path), then
  `#include`d - do not paste a foreign definition inline. A forward declaration
  used across many headers goes in `src/bedrock/forward.h` (alphabetical); for a
  heavy include chain, forward-declare and use the type incomplete (fine for
  pointers, references, and container value types).
- **Structural refactors** - when BDS introduces a base class, mirror it (add
  the base header, re-parent, move shared members down). When BDS removes a
  class, `git rm` once `grep` confirms nothing references it. Follow BDS
  structure; only the file name differs (snake_case). Keep it minimal.
- **Knowing the type vs placeholdering it.** Scenario A: declare the real type -
  *never* a same-size stand-in. Scenario B: when you cannot name a type/signature
  precisely, use a documented placeholder (see *Scenario B - Placeholders*) -
  but the **size / order / slot-count must still be exact**.

After the edit: update the mangled `name` in `scripts/configs/{windows,linux}.toml`,
re-run the dumper, and update any affected hook in
`src/endstone/runtime/bedrock_hooks/`.

---

# Scenario A - full port with bedrock-headers

The header diff is the source of truth: it lists every signature, vtable and
member change. Work it stage by stage, then apply each via *Editing src/bedrock
correctly*.

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
rework. Scope: the `handheld/` tree **and** the top-level `src/base/` tree;
**skip `handheld/src-client/`** (game client) and the other top-level `src/`
subtrees (`account`, `external`, `gui` - client / Xbox / third-party). Within
each stage, deep-dive only the intersection. (The first attempt used 3 coarse
stages; "handheld/src non-world" alone was 467 files / 44 intersecting - too
big. Use this finer split:)

1. `src/base` (top-level, *not* under `handheld/`) - the shared `Core` library:
   foundational utilities and low-level types (`BinaryStream`, ...). Easy to
   miss because every staged path below lives under `handheld/` while this tree
   is separate; a missed change here (e.g. a new `BinaryStream` virtual)
   silently shifts a vtable that Phase 1 can never flag.
2. `src-deps/SharedTypes` - shared types and enums
3. `src/common/network` - packets, network types, packet-id / disconnect enums
4. `src/common/server` (incl. `server/commands`) - server and command system
5. `src/common/entity` - ECS components
6. `src/common/{certificates,resources,scripting,platform,locale,gameplayhandlers,...}` - remaining non-world
7. `src/common/world/actor`
8. `src/common/world/item`
9. `src/common/world/level/block`
10. `src/common/world/level/{dimension,biome}` and remaining `src/common/world/level/*` (chunk, material, storage, level core)
11. `src/common/world/*` - remaining world (`attribute`, `effect`, `events`, `inventory`, `response`, ...)
12. `src-deps` other than SharedTypes (Certificates, VanillaComponents, ...), then anything else
13. **Cross-validate** - once every ABI change is in, re-review the whole
    `src/bedrock/` diff against the bedrock-headers diff. Every edited function
    signature, vtable slot, member type/order, and structural change must trace
    to a concrete change in `git diff android/r<prev> android/r<new>`. Reject
    anything not backed by the diff: no invented types, no guessed members, no
    hallucinated signatures, no "looks-right" edits. A change that cannot be
    matched to the header diff is wrong - revert or fix it. This stage exists
    because the porting stages, especially when parallelised across agents, can
    introduce plausible but unfounded edits - they must all be matched up.

## Reading the diff: noise to skip

dwarf2cpp churn that is *not* a real BDS change:

- **Versioned-namespace churn** - `SharedTypes/v1_26_10/...` becomes
  `v1_26_20/...`; most of that subtree's diff is just the version bump.
- **Template-instantiation churn** - `SharedPtr.h` / `SharedCounter` and similar
  enumerate concrete instantiations (`CopperBlock<ThinFenceBlock>`, ...). The
  set churns every release; Endstone uses its own templates - ignore.
- **File regrouping** - dwarf2cpp regroups types into different generated files.
  A file shown as fully deleted (e.g. `CommonTypes.h`) often just means its
  types moved. Confirm a type is genuinely gone, not relocated.
- **Lambda source-location churn** - `match<(lambda at .../Foo.cpp:47:3)>` -
  line/column numbers shift every build. Pure noise.
- **Declaration reordering** - declarations reordered within a file; the diff
  shows -/+ pairs of identical content moved.

---

# Scenario B - limited port from the binaries (Linux RTTI + Windows DB)

No header diff. You have:

- a **Linux** BDS database - stripped of function names but **RTTI is intact**
  (`_ZTV<len><Class>` vtables, `_ZTI` typeinfo), so polymorphic classes,
  vtables, and Itanium-mangled names are recoverable;
- a **Windows** BDS database - what Endstone actually hooks (and may carry
  *partial* symbols: some methods demangled even though ctors/vtables are not);
- a **previous, named reference DB** for both platforms (the last version, with
  PDB symbols) to diff against;
- possibly a **stale PDB** - treat with suspicion.

Run everything through the ida-pro `py_eval` (see [[reference_idalib_mcp_quirks]]);
note that in `py_eval` two top-level `def`s cannot call each other (exec scope) -
nest helpers in one function. `find_bytes` + `py_eval` xrefs stay responsive
when `search_text` / `xrefs_to` / `make_signature` time out on the busy DB.

## The loop

Without a diff, work is driven by two signals, fixed one at a time (build/test
between each - see *Editing src/bedrock correctly*):

1. **Symbol misses** from the dumper (Phase 1 triage) -> *Finding a new symbol /
   offset* below.
2. **Runtime crashes / misbehaviour** once it runs -> a vtable shift
   (*Detecting vtable changes*) or a member-offset shift (*Detecting data-member
   layout changes*). An AV in an accessor/`_get`/`_setControlBlock`/`unique_ptr`
   deref means a field is read at the wrong offset; clean misbehaviour with no
   fault (e.g. a hook whose argument is garbage) often means a hook landed on the
   wrong function. A `std::_Throw_bad_variant_access` thrown from a
   `Script<...>GameplayHandler::handleEvent*` (`event.visit(...)`) is an
   event-variant drift (*Detecting event-variant changes*).

## Finding a new symbol / offset without a header diff

- **Navigate by string anchor, not symbol.** To locate an unnamed function:
  take a string literal it references (an error/i18n key like
  `commands.setmaxplayers.success.lowerbound`), `find_bytes` the *ASCII hex* of
  the string, `xref` to the referencing function, and read it. Diff it against
  the previous DB's *named* equivalent (e.g. `SetMaxPlayersCommand::execute`) to
  read off the new offsets/signature. Always `lookup_funcs` the name first - the
  Windows DB's partial symbols may already have it.
- **Re-cut a stale / wrong byte pattern.** Prefer a **prologue** pattern (the
  `push` sequence + `sub rsp`) over a call-site one; the match offset is then the
  function start. For a virtual, re-cut **from the vtable**, not a raw scan: find
  the class vtable (Linux RTTI `_ZTV<len><Class>`; Windows via the documented
  string -> ctor -> `__vftable` store route), take the exact slot (mind the
  dtor-slot difference: Itanium 2 dtor slots, MSVC 1), read the prologue there,
  and wildcard only displacements/immediates.
- **Verify a pattern-resolved offset two ways, not one.** (1) It must be a
  **function start** - `ida_funcs.get_func(ea).start_ea == ea`; an offset that
  lands mid-function is conclusively wrong. (2) **Decompile it** and confirm it
  is the *intended* function ([[feedback_decompile_to_confirm]]) - same-named
  overloads (`sendPacket(string&, Reliability, Compressibility)` vs
  `sendPacket(string&, Packet&, ...)`) have different bodies; match the body to
  what your hook expects. A stale prologue pattern does not just *miss* - it can
  silently match a *different* function with the old shape (this bit
  `BatchedNetworkPeer::sendPacket` at 1.26.32: its codegen added `push r12..r15`,
  so the old `55 56 57 53 ...` pattern collided with a packet-trace overload).
- **Sweeping/verifying the whole table: compare the committed offset's *body*
  against the previous version's *named* function - never against a name.** Two
  traps that each produce a false verdict (both bit a real 1.26.32 sweep):
  - **Same RVA != same function across versions.** Do *not* identify the new
    function by reading what name sits at that RVA in the *old* DB - code
    relocates every release, so the old DB's `0x8e8a00` (`changeToValueType`)
    says nothing about the new DB's `0x8e8a00` (which was the correct
    `RepositorySources::initializePackSource`). Decompile the *new* offset's body
    and match its behaviour to the *old named* function: distinctive callees,
    member-offset writes, or constants (the FNV `0x100000001B3`; literal
    factory-call args like `6`/`4`). A near-match in line count / arg count is
    expected to drift with inlining - judge by behaviour, never by size.
  - **Don't trust the target DB's auto-names or hexrays' inferred prototype.**
    The fresh DB mislabelled a 123 KB function as `ItemInstance::fromTag` while
    the *correct* small one was an unnamed `sub_`; and the real 2-arg
    `initializePackSource(this, PackSourceFactory&)` decompiled as a 4-arg
    `(__int64*, const char*, __int64, __int64)`. The body is ground truth; the
    label and the prototype are guesses.
  - Cheap pre-filter for a 60+ entry table: for each entry, confirm the offset
    is a function start and that its referenced **string set** is a superset of
    the old named function's strings (strings are version-stable). That clears
    the string-bearing majority; decompile-and-compare only the string-less
    residue. (Callee-*name* overlap does **not** work - the target DB's callees
    are almost all unnamed `sub_`.)
- **Beware a stale PDB overriding your fix.** `--pdb` resolves by *name* first,
  so a PDB older than the exe returns the *old* RVA for any moved symbol,
  ignoring your re-cut pattern. When the PDB version can't be trusted, do **not**
  blanket-regenerate (it can clobber currently-correct offsets with stale ones).
  Instead fix the one verified entry in `src/bedrock/symbols/<platform>.h`
  **directly** (hand-patch the offset) and update the `pattern` for the next
  clean regen. Cross-check the other platform - the same function on Linux
  (`_ZN...`) often resolved fine (different codegen), confirming a Windows-only
  change.

## Detecting vtable changes (Linux RTTI)

Confirm directly against the binary whether a virtual was **added / removed /
reordered**, and at exactly which slot - name-free. Every polymorphic class has
`_ZTV<len><Class>` + `_ZTI<len><Class>` even in a fresh, PDB-less DB; the slot
targets are `sub_` in *both* DBs (even the named reference only has RTTI symbols,
not the virtuals), so you diff layout without any virtual-function names.

1. **Walk the vtable from its address point.** Resolve `_ZTV<len><Class>`, skip
   each `(offset_to_top, _ZTI<len><Class>)` header pair, then collect qwords
   while the target sits in an executable segment
   (`ida_segment.getseg(q).perm & 1`) or is `__cxa_pure_virtual`; stop at the
   first non-pointer / next typeinfo. **Do not gate on `ida_funcs.get_func`** -
   the fresh DB has not defined most vtable targets as functions yet, so it stops
   the walk early; exec-segment membership works regardless of analysis.
2. **Length per class brackets the change.** A primary vtable lays out
   `[base virtuals][derived adds]` in order, so each class's *own* `_ZTV` length
   localises a net add/remove to one class. For a hierarchy, the most-derived
   concrete class (e.g. `ServerPlayer` covers `Actor -> Mob -> Player ->
   ServerPlayer`) gives the whole chain in one read. Equal length on every level
   = no net change (still verify order).
3. **Structural fingerprint** confirms no same-count shuffle, name-free: tag each
   slot `P` = `__cxa_pure_virtual`, `T` = this-adjusting thunk (`48 83 ef` /
   `48 81 ef` = `sub rdi`), `R` = repeats previous target (shared-stub runs),
   `.` = normal. Identical tag strings across DBs => no reshuffle.
4. **Localise the exact slot by signature alignment.** Per slot, build a
   recompilation-robust signature - decode the first ~6 instructions
   (`ida_ua.decode_insn`) keeping mnemonic + operand register classes but
   **dropping immediates and displacements**. Bridge the two IDA processes via a
   temp file: dump the old DB's per-slot signatures to JSON, switch DBs,
   recompute, `difflib.SequenceMatcher` the two lists. `insert`/`delete` opcodes
   are the real structural change; `replace` opcodes are functions whose body
   changed at the same slot - ignore them.
5. **Confirm by decompile, never by the heuristic alone**
   ([[feedback_decompile_to_confirm]]). Decompile the boundary in both DBs: the
   shifted neighbour (`NEW[slot+1]`) must match `OLD[slot]`, and the inserted
   `NEW[slot]` is often *referenced by* its shifted neighbour (e.g. a new
   per-position helper the shifted loop calls via `vtbl+offset`) - the tightest
   possible confirmation.
6. **Map the slot to the Endstone declaration, then validate locally.** Count
   the header's virtuals (dtor = 2 slots; each overload = 1; skip commented-out;
   honour `#ifdef __linux__`) plus any base's slots. **Anchor the count** on a
   virtual whose address you know in both versions (a hooked one from the toml) -
   its slot must equal your predicted index. Beware: an Endstone header may omit
   Linux-only virtuals (the `#blameMojang` ones), so the cumulative count can be
   short of the real vtable - do not trust it globally. Decompile the few slots
   *around* the change and match them to neighbouring declarations (a const/
   non-const overload pair returning the same `this+N` subobject is an
   unmistakable anchor). If the local sequence lines up, the insert point is
   pinned regardless of any global gap.
7. **Add the placeholder.** A single non-dtor `virtual void <name>() = 0;`
   occupies exactly one slot. Name it a clearly-marked placeholder (don't invent
   a Mojang name) and comment the observed signature/behaviour. If you cannot
   confirm the change is on *both* platforms, match the existing `#ifdef __linux__`
   pattern rather than risk shifting the Windows vtable.

For one class you do not need IDA at all: `lief` + `capstone` on the shipped
binaries resolve `_ZTS<len><Class>` -> typeinfo -> vtable -> per-slot disassembly
in seconds, on both the previous and the new ELF. (`lief`'s `Binary.relocations`
comes back empty on the stripped BDS ELF - parse `.rela.dyn` yourself as 24-byte
`(offset, info, addend)` records and keep `info & 0xffffffff == 8`.)

## Locating a vtable on Windows (no RTTI)

`/GR-` leaves no type descriptors, but a small interface is still findable
name-free - and this is the only way to confirm a Linux-derived vtable verdict on
the platform Endstone actually hooks.

1. **Scan `.rdata` for a run of consecutive pointers to `lea rax, [rcx+d]; ret`
   stubs.** Trivial base-subobject getters are ICF-folded to **one stub per
   displacement** binary-wide (~100 in a 160 MB `.text`), so a class with N of
   them is a run of N adjacent stubs with distinct increasing displacements, and
   the displacements read off the member layout directly. These stubs are 16-byte
   aligned and `cc`-padded but have **no `.pdata` record** (leaf, no unwind) - a
   `.pdata` function-start filter silently drops every one of them and the scan
   returns nothing.
2. **Identify the class from the dtor slot, not a name.** The slot before the run
   is the scalar deleting dtor; its teardown pins both the class and its `sizeof`
   (a virtual-deleting `unique_ptr` at `+N` = the last member). Comparing that
   body against the last release that shipped a PDB is the identification.
3. **Never read the table's END from "the next qword is not code".** MSVC packs
   vftables back to back in `.rdata`, so the next table's slot 0 is a code
   pointer - the same class reads as 4 slots in one release and 8 in the next
   purely by what the linker put after it. The terminator is: the next qword's own
   *address* is the target of a `lea` + `mov [reg], rax` vfptr store.
4. **A displacement proves the slot order, never the semantics.** Which member a
   getter returns comes from the last PDB-bearing release (`??_7<Class>@@6B@` plus
   the named getters); carry that mapping forward version by version and diff
   displacements. Equal displacements at equal slots across the chain is what
   makes an "unchanged" verdict conclusive rather than merely shape-compatible.
5. **To check ONE virtual's slot index in a 400-slot table**, don't walk the
   table - find the function (byte fingerprint of its body, member displacement
   wildcarded), locate the `.rdata` qword holding it, and walk *backwards* while
   the qwords are `.text` pointers; the distance is the index. Do it in the
   PDB-bearing release first to learn the constant offset between the binary index
   and Endstone's declaration count (a base contributing only a virtual dtor is
   `+1`), then apply the same offset to the new release. Cross-anchor on a
   const/non-const overload pair - ICF folds them to one address, so they show up
   as two adjacent slots sharing a target, which is unmistakable.

## Tracing a Bedrock::PubSub notification path

When an Endstone event fed by a `Connector`/`Publisher` stops firing, clear or
convict the BDS side before touching `src/bedrock/`. Four checks settle it.

1. **Resolve a slot-numbered lead to a NAME first.** Itanium spends 2 slots on
   the dtor, MSVC 1, so the same virtual is Itanium slot N and MSVC slot N-1 -
   an off-by-two between platforms. Name the slots from the last PDB-bearing
   release (the proxy/manager virtuals are usually public even when the vftable
   is not) and confirm by `.text` address order, which follows declaration
   order. Acting on "slot 5 was restructured" without this reads a `void`
   helper as the notification gate.
2. **A `dispatch<...>` instantiation is per-signature and normally has exactly
   ONE caller.** Xref it in both binaries: equal caller sets prove there is a
   single publish site and it did not move. This is far stronger than diffing
   the publisher, and it is two `E8` rel32 scans.
3. **Read the notifier itself, not the publisher.** `Level::onChunkLoaded`-style
   notifiers are thin: a chain of proxy vcalls (a read-only early-out, a
   fire-once latch returning `bool`, some side effects, an argument getter) then
   the dispatch. Diff it instruction-by-instruction across versions - a stable
   one stays byte-identical apart from relocated displacements.
4. **Ordering against a state field needs the notifier's caller, not the
   notifier.** Find it by the *literal* argument pair at the state-transition
   call (`tryChangeState(expected, desired)`); that pair is version-stable and
   usually unique. Beware: BDS discards the CAS result and often publishes
   *outside* the lock, so an Endstone-side `state >= X` gate is unsound by
   construction even when the ordering is unchanged. Prefer the guarantees BDS
   already provides (the fire-once latch) over re-deriving them from a field.

## Detecting data-member layout changes (ctor/dtor RE)

A struct's member layout - a member inserted, removed, resized, or moved - is
recoverable directly from its **constructor and destructor**, no headers needed.
Usually crash-driven.

1. **The crash points at the member.** Map the crashing read (an accessor /
   `_get` / `_setControlBlock` / `unique_ptr` deref) back to the Endstone member,
   then diff that struct's ctor/dtor: new (stripped target) DB vs the previous
   named-reference DB.
2. **Two-DB diff.** The previous DB has PDB symbols (named ctor `??0Class@@`,
   dtor `??1Class@@`); the new one usually does not. Extract each member's offset
   from both and align top-down. The first offset that differs localises the
   change; the delta is the size inserted/removed before it. Confirm a later
   anchor member shifted by the *same* delta (a non-uniform delta means more than
   one change - keep going).
3. **Extract offsets with a `this`-relative store tracker.** Decompiled ctors are
   noisy. Run a small register-tracker over the ctor: seed `this`(rcx)=0,
   propagate `this`-derived values through `mov`/`lea` reg copies and stack
   spills, and log every `mov [reg+disp], ...` whose reg is `this`-relative plus
   every `call` whose rcx is `this`-relative (member sub-object ctors). Sorted
   offsets = layout in construction (= declaration) order. Same engine on the
   dtor gives teardown order.
4. **Identify a member's TYPE by its ctor/dtor fingerprint** (MSVC sizes):
   - `std::string` (32): ctor writes capacity `=15` at `+24`; SSO test is the
     `0x80000000000000` bit on the capacity word.
   - `std::vector` (24): ctor zeroes 3 pointers; dtor
     `if (begin) operator delete(begin, end - begin)` - one sized free off the
     stored last/end pointers.
   - `std::unordered_map`/`unordered_set` (always 64, any K/V): ctor sets load
     factor `1.0f` (`0x3f800000`), mask `=7`, bucket count `=8`, a 32-byte
     sentinel list node; dtor frees the bucket vector then walks the list. The
     per-node `operator delete(node, N)` reveals the value type via node size.
   - `shared_ptr`/`weak_ptr` (16) and `Bedrock::NonOwnerPointer<T>` (24 =
     shared_ptr 16 + `T*` 8): dtor is an **atomic refcount release** -
     `if (rep) { atomic_dec(rep->uses); vcall rep->__on_zero(vtbl); ... }`. This
     tells a 24-byte `NonOwnerPointer` from a 24-byte `vector`:
     refcount-release-with-virtual-call vs `operator delete(begin, end-begin)`.
   - `unique_ptr<T>` (8): dtor `if (p) { ~T...; operator delete(p, sizeof(T)) }`.
     Polymorphic `T` -> **virtual deleting dtor** `(**p)(p, 1)`; concrete `T` ->
     fixed-size `operator delete(p, N)`. Distinguishes which of two adjacent
     unique_ptrs moved, and tells `unique_ptr` from a raw pointer (not destroyed).
   - Other fixed sizes: `std::function` 64 (dtor calls a manager via a stored
     vtable), `BaseGameVersion` 32, `Core::Cache` 72, `AABB` 24, `HashedString`
     48, `mce::Color` 16.
5. **No symbols *and* no locatable ctor/dtor -> Linux RTTI.** `_ZTV<len><Class>`
   exists in a stripped Linux DB; the dtor is at `vtable+16` (Itanium D1
   complete) / `+24` (D0 deleting). Decompile it and read the teardown the same
   way. Caveat: libc++ layouts are *not* the MSVC offsets, but **member order and
   member kind are the same**, and `vector`(24)/`shared_ptr`(16)/`NonOwnerPointer`(24)
   match Windows sizes - enough to confirm *what kind* of member changed and
   *where* in the order. Diff the new Linux dtor vs the old to see the
   extra/missing teardown (the new member appears as an extra teardown adjacent
   to its neighbour).
6. **Caveats.**
   - A member the dtor *destroys* is owned (vector/string/smart-ptr/unique_ptr);
     a **reference or raw pointer member never appears in the dtor**, so absence
     there is not absence in layout - cross-check the ctor store list.
   - Check what Endstone **actually accesses** before padding the unused middle
     (grep the `*Ref`/wrapper that exposes the type) - if it reads a tail member,
     the whole tail must be byte-accurate, not just padded to size.
   - Confirm by decompile, never by size/heuristic alone
     ([[feedback_decompile_to_confirm]]); the tightest confirmation is a shifted
     neighbour whose new offset equals the old member's offset.

### Start from the crash, not from a sweep

A layout bug almost always surfaces as an **access violation inside a trivial
accessor that just returns a member** - `getX() { return x_; }` - or inside
`NonOwnerPointer::_setControlBlock` / a `shared_ptr` copy, because those touch a
control block and fault on garbage. When that happens:

- **Read the displacement out of BDS's own accessor in both versions. That one
  number is the whole answer** and takes minutes; a full ctor/dtor walk takes
  hours. Do it first and only escalate if the accessor cannot be located.
- **Fix, rebuild, re-run.** Each fix moves the crash one step further in, and the
  next trace names the next class for free. Iterating the crash is dramatically
  faster than trying to statically clear every class up front.
- **Rule out your own recent edits before blaming BDS.** If a hook's declared
  return type or parameters changed, a corrupted `this` produces the identical
  symptom. Discriminate by *where* it survives: if the hook already called the
  real function through `ENDSTONE_HOOK_CALL_ORIGINAL` with that `this` and got
  back, `this` is fine and it is a member offset.
- Beware the accessor that appears to work: a `shared_ptr`'s pointer is its
  first 8 bytes, so a getter returning `.get()` keeps working while every member
  *after* it is 8 bytes out. Silent, and it hides the real breakage.

### The change BDS actually makes most often

**A member changing KIND at an unchanged offset**, growing 8 -> 16 and shifting
everything after it - overwhelmingly `std::unique_ptr` -> `std::shared_ptr`, and
it tends to arrive in clusters across ownership-holding classes in one release.
When you find one, **go looking for its siblings** in related classes before the
next crash finds them for you.

Judge it by **kind, not size**: a `shared_ptr` teardown is an atomic refcount
decrement plus a virtual `__on_zero` call; a `unique_ptr` reset is an inline
delete. Same 8-byte delta, completely different fingerprint. Other shapes seen:
a `unique_ptr<T>` replaced by an inline `std::optional<T>` (the destructor stops
running a deleter and starts testing an engaged flag over the value's own body),
and a member relocated within the struct with `sizeof` unchanged - which no size
check can ever detect.

### Proof techniques that settle it quickly

- **A single instruction changing WIDTH at an unchanged offset proves an
  append.** A ctor's `mov qword [this+N], 0` becoming `movups xmmword` means a
  new 8-byte member now sits at `N+8` and is zero-initialised with its
  neighbour. Identical on both platforms, and hard to misread.
- **Uniform-delta check over the whole object.** If *every* store from the first
  divergence to the end moved by exactly the same delta, there is exactly ONE
  change and nothing before it moved. A mixed band (some +8, some 0, some +16)
  means multiple changes - keep going.
- **Base-class removal is visible in the typeinfo kind.** Itanium
  `__vmi_class_type_info` (multiple bases, with the secondary vtable groups) ->
  `__si_class_type_info` (single base) is a removed base, and the removed base's
  own `_ZTS` name disappears from the whole binary. Every member then shifts by
  that base's size.
- **RTTI `offset_to_top` doubles as a size oracle for a base subobject.** A
  secondary base's `offset_to_top` moving -32 -> -40 says the primary subobject
  grew 8 bytes, without decompiling anything.
- **`make_shared`'s allocation size is a whole-object oracle - subtract the right
  control block.** Its `operator new` immediate is `control block + sizeof(T)`:
  **16** on MSVC (`_Ref_count_obj2` = vptr + two `uint32`; the `add reg, 0x10`
  that derives the object confirms it) and **24** on libc++ (its counters are
  `long`, not `int`). Subtract the wrong one and every size is 8 bytes out. This
  is the only size oracle Windows has, `/GR-` leaving no typeinfo. For a packet
  the route needs no symbols beyond one the table already holds:
  `MinecraftPackets::createPacket` is a jump table indexed *directly* by
  `MinecraftPacketIds`, so `table[id]` -> `make_packet<T>` -> `operator new` is
  two hops. Cross-check on Linux, where the D0 dtor's sized
  `operator delete(this, N)` gives `sizeof` independently.
- **When no `operator new` site exists, MSVC's scalar deleting destructor has
  the size.** A class that is only ever stack-constructed (most packets -
  `StartGamePacket` has no `operator new` immediate anywhere in the image) still
  gets `operator delete(this, sizeof(T))` emitted in **vftable slot 0**, so one
  `mov edx, N` settles it. Reach slot 0 name-free: a string literal the class
  owns -> the stub referencing it -> the `.rdata` qword holding that stub ->
  walk back while the qwords are `.text`. The walk over-runs into the *previous*
  vftable (MSVC packs them back to back), so take slot 0 to be the
  scalar-deleting-dtor body (`mov [rcx], vftable` ... `test edx,edx` ->
  `operator delete`), not the start of the run.
- **Sweep every Linux `sizeof` at once off the Itanium D0.** `_ZTS<len><Class>`
  -> typeinfo -> address point -> the deleting dtor's `mov esi, N; jmp <sized
  operator delete>` is `sizeof`, name-anchored, and runs over a list of classes
  in seconds with `lief` alone. Two filters make it trustworthy: the address
  point's `offset_to_top` slot must be **0 and not relocated** (a derived class's
  `__si_class_type_info` base-pointer field otherwise reads as a fake address
  point and hands back the derived class's size - that is how `Connector` reads
  200 instead of 16), and the D0 must be a <=26-instruction body that calls the
  slot before it. The dtor pair is not always slots 0/1 - `Actor`/`Mob`/
  `ServerPlayer` carry it at slots 8/9 - so scan the first ~20 slots.
- **BDS's Windows build is clang-cl, so `__PRETTY_FUNCTION__` literals bridge the
  platforms.** `T *Bedrock::NonOwnerPointer<X>::_get() const [T = X]` and the
  assert expressions are byte-identical in both binaries; only the source-path
  literal differs (`/mnt/vss/_work/1/s/...` vs `D:\a\_work\1\s\...`). Take the
  string out of the Linux D1 you just found by RTTI, locate it in the Windows
  `.rdata`, scan `.text` for the `lea rip` that references it, and the enclosing
  function is the Windows destructor - its deleting-dtor caller carries the
  Windows `sizeof`. This is what settles a class with no vftable anchor; it fails
  only for destructors that reference no distinctive literal.
- **A size delta is a NET.** 1.26.40's `StartGamePacket` reads -8 on Linux and
  -24 on Windows: a cereal migration appending 8 at the tail *on top of* a member
  that shrank 16/32. Decompose before reading anything into the sign or the
  magnitude - one component can be string-free (equal deltas) while another is
  not.
- **The Windows-minus-Linux delta counts the strings.** A change adding N
  `std::string`s shows `delta_win - delta_linux == 8 * N` (32 vs 24); equal
  deltas mean no string moved. Decompose a size jump with it before trusting the
  decomposition - and never assume the sweep's Linux delta is the Windows one.
- **`std::optional<T>` is self-measuring.** The engaged-flag offset read by a
  writer or dtor *is* `sizeof(T)` past the value, so one `cmp byte [this+K], 1`
  pins both the member's start and the payload's size in one instruction.

### Traps that cost real time here

- **The destructor only sees OWNED members.** Deriving layout or `sizeof` from
  the D1/D0 dtor is blind to raw pointers, references and PODs, so an inserted
  non-owning member is invisible and the method will happily report "growth is
  all at the tail" when a mid-struct insertion actually moved everything. Treat a
  dtor-only verdict as a lower bound and cross-check the ctor store list.
- **A `this`-tracker must follow stack spills.** Any function big enough to
  matter spills `this` immediately; a register-only tracker finds one or two
  offsets in a 30 KB function and looks like it worked.
- **Per-platform deltas are NOT transferable.** `std::string` is 32 on MSVC and
  24 on libc++, `unordered_map` 64 vs 40, and mutexes differ too. A *changed*
  verdict transfers across platforms - the same source change - but the byte
  delta and every resulting offset must be derived per platform.
- **The typeinfo name string is not guaranteed NUL-preceded.** Linker string
  packing can place another string's tail immediately before it, so a scanner
  anchored on a leading NUL silently misses classes - and looks like the class
  simply is not there. Search on the trailing NUL and validate by finding the
  typeinfo object that points at the candidate.
- **PIE `.data.rel.ro` is mostly zero on disk.** The real pointers live in the
  `R_X86_64_RELATIVE` addends; reconstruct the section as raw bytes overwritten
  by addends. `offset_to_top` is *not* relocated, which is exactly what delimits
  sub-vtables - so using addends alone silently loses the sub-table boundaries.
- **Windows sret is RDX, not RCX.** For a member function returning a large
  aggregate the convention is `rcx = this`, `rdx = return buffer`, `r8` onward =
  arguments.

### Guarding against the next one

`BEDROCK_STATIC_ASSERT_SIZE` cannot protect a class Endstone **truncates** with
`// ...` - its `sizeof` is the declared prefix, not the real object - and those
are exactly the classes that fail this way, silently, at runtime. The guard that
would catch it is asserting the offset of the last member Endstone reads
(`offsetof`), but on a non-standard-layout class (multiple bases, virtuals) that
is only conditionally-supported and warns. Decide it once as a convention rather
than per class. For any class that is *not* truncated, add the size assert - it
turns this whole failure mode into a compile error.

Two things that make the guard weaker than it looks:

- **A size assert never checks BDS.** `static_assert(sizeof(X) == N)` compares
  Endstone's declaration against a hard-coded literal, so once `N` is stale it
  passes forever and reads as verified. Re-derive every asserted number from the
  new binaries each bump - a green build says nothing (1.26.40:
  `ResourcePacksInfoPacket` asserted 128 against a 136-byte object,
  `ClientboundMapItemDataPacket` 200 against 208; both packets are read and
  mutated by hooks).
- **No `// ...` marker does not mean the class is complete.** `ServerInstance`
  carries no marker yet declares 936 of 1080 bytes. So "add asserts to every
  unmarked class" is not a mechanical sweep: derive the real size first, and
  where the declaration is short either finish it or add the marker - never both
  a silent prefix and an assert.

## Detecting cereal-packet layout changes (the cereal manager)

Once a packet is migrated to **cereal** serialization, its `write(BinaryStream&)`
and `toString` virtuals stop touching members - `write` is a thin wrapper that
builds a default `ReflectionCtx` and tail-calls the ctx serializer, the real work
runs through a reflection visitor, and `toString` is a stub (pre-migration) or
reflection (post). **Do not decompile them for layout - they are not oracles.**
The oracle is the packet's **cereal manager** (its type-erased op function), plus
the `SerializationMode` accessors.

1. **Find the manager.** Each cerealized packet has one `switch`-on-`int`
   function (a `switch 5 cases` jumptable) wired into the packet at construction
   (`*(obj+16) = <manager>` in the ctor / `make_shared` builder) and referencing
   the packet vtable `off_<vtable>`. Its cases:
   - **case 1/2 = copy/move-construct** -> a faithful **memberwise copy** (exact
     offsets *and* widths).
   - **case 4 = clone** -> `operator new(SIZE)` then the same copy (authoritative
     object size).
   - case 0 = type descriptor / entt type-name; case 3 = compare. Ignore both.
2. **Read the ASSEMBLY of case 1/2, never the decompile.** Hex-Rays mixes pointer
   units (`v4[7]` vs `*(v4+38)`) and hides widths; the asm is exact:
   - `movss [dst+off]` = 4-byte float; `mov eax,[src+off]; mov [dst+off],eax` =
     4-byte; `mov rax,...` = 8-byte; `movups xmm...` = 16-byte block / inlined
     sub-object.
   - `movzx eax, byte [src+off]; mov [dst+off], al` = a **1-byte** member.
     **Adjacent 1-byte stores at consecutive offsets** = a member that *narrowed*
     (e.g. a 4-byte enum -> 1 byte).
   - `lea [src+off]; lea [dst+off]; call <helper>` = a non-trivial member
     (string / vector / `RedactableString`) at `off`.
   - A member present in the old layout but with **no store** in the new copy =
     **removed**.
3. **`operator new(N)` in case 4 = the size.** Compare to
   `BEDROCK_STATIC_ASSERT_SIZE`. Beware: 8-byte alignment rounding can keep
   `sizeof` *unchanged* while members narrowed / dropped / were added - never
   trust size alone, read the copy.
4. **`get`/`setSerializationMode` betray a cereal-only migration.** Pre-migration
   both are stubs (Windows: COMDAT-folded onto `disallowBatching`'s address;
   Linux: literal `return 0` / nullsub). Post-migration the packet **overrides**
   them: the vtable slots become `mov eax,[this+OFF]` / `mov [this+OFF],reg`, and
   `OFF` is a **new 4-byte `SerializationMode` member** (init `CerealOnly`=5).
   Find the slot via the PDB-anchored `_read` slot (Windows) / RTTI (Linux) and
   confirm `OFF` equals the copy's last store. With no PDB, skip the vtable
   entirely: `getId` / `getName` / `get`+`setSerializationMode` are emitted as one
   contiguous aligned stub cluster in the packet's own TU on both platforms, so
   the single `.text` rip-`lea` to the `"<Name>Packet"` literal lands on
   `getName` and the next two stubs give the packet id and the `SerializationMode`
   offset for free. On Windows those stubs are leaves with **no `.pdata` record**,
   so a function-range filter silently drops them.
5. **A packet still in a side-by-side mode keeps its hand-written `write` - and
   that is the best member oracle there is.** Only a `CerealOnly` packet's
   `write` is the thin wrapper point 1 warns about; a
   `SideBySide_*`/`SemanticSideBySide_*` one reads every member at its real
   offset with the field's cereal name string in the adjacent argument register.
   It names offsets *and* covers the PODs and raw pointers a destructor never
   touches, so it settles the mid-struct-insertion question a dtor walk cannot.
   The `<Name>Payload` type name and its member display names also appear
   verbatim in the entt/cereal registration function - one `lea` per name with
   the length in `ecx`, in declaration order.
6. **Cross-check the wire with protocol-docs.** The cereal field set == the
   serialized fields; `EndstoneMC/protocol-docs` (`<branch>/packets/<Name>.json`)
   lists them in order, mapping the copy's offsets to names and flagging
   added/removed wire fields. A field can leave the wire *and* the struct together
   (dropped bools), or appear (a second `FilteredName` once a `RedactableString`
   serialises both halves). The sibling `bedrock-protocol` DSL often already
   models the target protocol; a wire model that matches the binary field for
   field, optional for optional, is a second independent derivation.
7. **Date the migration with a raw `grep`, before opening any database.** A
   packet that migrated carries a `<Name>Payload` type-name string and *loses*
   the field-label literals its hand-written `write` passed to the stream
   (`"DecorationUpdate Bit Field"`). Grepping both strings across the shipped
   binaries of the old and new release pins the migration to the exact version
   in seconds, and tells you up front which oracle applies (point 5 vs point 1).
8. **The payload's own constructors are the oracle when there is no
   hand-written `write`.** Reach them from the `operator new(sizeof)` site
   (which also hands you `sizeof`): the ctor stores every member, PODs
   included, and *names* each one by the argument it comes from - a full
   member map with semantics, not just offsets. Different ctors light up
   different halves (an update-path ctor sets the scalars, a
   savedData-path ctor fills the containers), so read more than one.
9. **A tail append does NOT mean nothing before it moved.** A migration wraps
   the members in a payload sub-object and is free to *reorder them into wire
   order* at the same time; the `SerializationMode` at the tail then accounts
   for the whole size delta and the arithmetic looks like a pure append while
   every member has actually shifted. `ClientboundMapItemDataPacket @ 1.26.40`
   is exactly this: 200 -> 208, all 8 bytes explained by the tail member, yet
   every one of its 13 members moved.
10. **Watch for a scalar hoisted out of a container.** The same migration split
   `mMapIds.front()` into a new standalone `ActorUniqueID` at the head of the
   payload plus a vector that is now **empty on every non-creation path**. An
   Endstone accessor written as `vec.front()` stops being merely wrong and
   becomes UB. The empty container is not an error to guard - the value moved.
11. **`writeWithSerializationMode`'s cereal early-out hands you the payload
   offset as one `lea`.** Every cerealized packet tests its mode and, on the
   cereal branch, tail-calls `serialize(ctx, this + N, stream)` - `N` *is* where
   the payload sub-object starts, on both platforms, with no size arithmetic.
   It sits above the manual branch in the same function, so one decompile gives
   the payload base and (point 5) every member offset and cereal name.
12. **A payload sub-object and the same members declared flat differ on Linux
   only, and by exactly the base's tail padding.** Itanium allocates derived
   members from `dsize(base)` (44 for `Packet`), so a flat leading `uint8_t`
   lands at 44 while a payload sub-object - alignment 8 - is forced to 48;
   every later member and `sizeof` shift 8. MSVC never reuses base tail
   padding, so **Windows measures the same either way and its size assert
   passes on a wrong model**. A packet whose assert holds on Windows but is 8
   short on Linux is a missing payload wrapper: model the payload, never an
   `#ifdef __linux__` filler member. `UpdateSoftEnumPacket` is the worked case
   (120/112 real, 120/104 flat). **`sizeof` only shifts when nothing re-aligns
   it**: a later 8-aligned member (a `std::bitset`, a pointer) absorbs the 4
   bytes and both size asserts pass while every member before it is 4 bytes low
   on Linux - and the flat model's first member then reads `Packet`'s
   **uninitialised** tail padding, which is where "the client sends NaN
   rotations" reports come from (`PlayerAuthInputPacket @ 1.26.40`, 232/232
   either way).
13. **A Linux-only assert failure at bump time can be a pre-existing
   mis-model.** Adding size asserts during a bump surfaces every historic
   modelling error at once. Grep-date the `<Name>Payload` string (point 7)
   *before* attributing anything to the new release - `UpdateSoftEnumPacket`
   was identical in 1.26.32/36/40 and only the new assert was new.
14. **A member absent from the registration can still be a WRITE GATE.** A
   migration drops the old bit-field from the wire but keeps it as the member
   that decides which optionals are engaged: each per-member getter tests
   `payload->type & <bit>` and each setter ORs the bit back in on read. So the
   registered name list is the wire, not the live member set. The two
   packet-specific accessor thunks - the 3rd and 4th `lea rax, sub_...` of a
   member's registration block - carry the member offset, the element stride
   and the presence condition in one decompile; read them before concluding a
   member is dead. (`ClientboundMapItemDataPacket @ 1.26.40`: 13 registered
   members, `type` not among them, every one of them gated by it.)
15. **Two vectors serialized as separate cereal members can still be parallel
   arrays.** Independent length prefixes are not independent semantics - check
   the *producer*. BDS's own constructor filling both in one loop over a
   `vector<pair<K,V>>` is the proof that the receiver pairs them by index, and
   any Endstone code that rewrites one must rewrite the other. The reader never
   validates the lengths, so a mismatch is a well-formed packet that breaks the
   client instead of erroring.

Worked example: **BossEventPacket @ 1.26.32** - migrated to cereal-only;
`color`/`overlay` narrowed 4B->1B, both `darken`/`fog` bools removed, a
`SerializationMode` member added at the tail, `sizeof` held at 168/152 purely by
alignment. Proven entirely from the manager copy (`movzx byte` pairs + the
trailing `SerializationMode` store) and `operator new(0xA8/0x98)`; the vtable was
**unchanged** (cereal migration is data-only - verify it separately,
*Detecting vtable changes*). See [[project_bosseventpacket_changed_1_26_32]];
confirm by asm, never by size ([[feedback_decompile_to_confirm]]).

## Detecting event-variant changes (std::variant traps)

Gameplay events reach Endstone through a `std::variant` wrapper -
`EventVariantImpl<Details::ValueOrRef<const E>...>` (aliased `ConstEventVariant`/
`MutableEventVariant`): `LevelGameplayEvent<void>`,
`PlayerGameplayEvent<CoordinatorResult>`, etc. Endstone hooks a
`Script<...>GameplayHandler::handleEventN` and calls `event.visit(visitor)`
(`std::visit`). Drift in the variant's alternatives surfaces as
**`std::_Throw_bad_variant_access`** at that `event.visit` line - the C++ EH
`0xe06d7363` -> `std::terminate` -> `STATUS_FATAL_APP_EXIT 0x40000015`, and the
process exits via crashpad's NoDump code `0xFFFF7001` (or a heap status). It only
fires when a *covered* event actually dispatches: on an idle, pack-less server the
script gameplay handler may not run until shutdown's `LevelStartLeaveGameEvent`,
so **"runs fine, crashes on `stop`" is the classic symptom** - not a startup
crash, even when the size-driving alternative (e.g. a scripting world-init event)
never fires.

**MSVC `std::variant` layout is the whole game.** Storage = `max(sizeof(alt))`;
the discriminant `_Which` (1 byte for <=255 alts) sits **immediately after the
storage** (`offset = sizeof(variant) - pad`). Each `ValueOrRef<const T>` is
`union{const T*; const T value}` + `bool is_pointer_`, so its size is
`max(8,sizeof(T))+1` padded and `is_pointer_` lands at the value size. `std::visit`
reads `_Which`; an out-of-range value (or the `0xFF` valueless sentinel) throws -
NatVis prints `[valueless_by_exception]`. Two drifts cause it; separate them
before fixing:

- **Size drift** - an alternative's struct changed size, moving `max(sizeof(alt))`
  and therefore the `_Which` offset. Endstone (built with the old size) reads
  `_Which` at the stale offset -> garbage index. Latent for *every* event.
- **Set drift** - an alternative inserted/removed/reordered. Size can be
  *unchanged* (if the size driver is untouched), but a new alternative's index is
  out of range for Endstone's shorter variant, and any reorder mis-maps existing
  indices.

**Read the truth from the BDS dispatch + extractor, no symbols needed.** The
caller of the hooked `handleEvent` (the frame above your hook in the crash stack;
crash-report addresses are object-relative, so `imagebase + RVA` lands directly in
the DB) does, right after the vcall:

```
movsx rdx, byte ptr [rcx+OFF]   ; OFF = real _Which offset = real storage size
inc   rdx
call  <extractor>(dest, _Which+1, event)
```

- **`OFF` = the real storage size** - compare to Endstone's `sizeof(variant)-1` /
  `BEDROCK_STATIC_ASSERT_SIZE`; differ => size drift.
- The **extractor switches on `_Which+1`, one `case` per alternative** (`case 0`
  is the valueless stub) - case count = alternative count; compare to the
  `ConstEventVariant<...>` arity => set drift.
- Each case's leading `cmp byte ptr [r8+K], 0/1 ; jz ; mov r8,[r8]` is the
  `ValueOrRef` ptr-vs-value test, so **`K` = that alternative's value size**; the
  largest `K` across cases is the **size driver** - the only alternative that sets
  the variant size.
- **Fingerprint each case to name it:** `_InterlockedIncrement` on a control block
  counts the `WeakRef`/`NonOwnerPointer` members; `memcpy` + a `0xAAAA...AB`
  size-multiply = `std::string`/`std::vector` fields. Match (size, ref count,
  field shape) to Endstone's known alternatives - matches are existing (read off
  their new index), unmatched cases are the inserts (their case index = the
  insertion point). An event whose case carries *no* `WeakRef` refcount cannot be
  an existing player/actor event that holds one - that mismatch is how you prove a
  removal, not just a move.
- **Diff against the previous *named* DB's extractor** - same routine, but its
  per-alternative copy-ctors carry demangled `ValueOrRef<...>` type names, so it
  labels every old slot; diffing the two case lists names the existing slots and
  isolates the new ones.

**Fix.** Mirror the new alternative list (order included) in the
`ConstEventVariant<...>`. The `visitor` matches by *type* (`std::is_same_v`), so
the events Endstone actually handles only need to be **present as the right type** -
their exact index is not memorised, but the total **count** and the **size driver**
must be right. Exploit that:

- A **non-size-driver** alternative may be left empty (`struct E {};`) or a named
  placeholder (`struct UnknownEvent0 {};`) - it still occupies one slot, and an
  empty struct's `ValueOrRef` (16 B) never drives the size. This restores the
  count / order / `_Which` offset without baking in an unconfirmed layout.
- Only the **size driver** (largest `K`) needs a byte-exact body; confirm with
  `BEDROCK_STATIC_ASSERT_SIZE`. After a size-drift fix the driver can become a
  *different* alternative - recompute which one it is.
- **Confirm live**: breakpoint the hooked `handleEvent`, copy `byte
  [event+OFF_real]` into `byte [event+OFF_endstone]`, and continue - if the
  `bad_variant_access` then vanishes across a full start/stop, the offset/size
  mismatch is proven (cdb on the injected `bedrock_server.exe`; lldb needs a
  matching `python3xx.dll`). Confirm by decompile, never size alone
  ([[feedback_decompile_to_confirm]]).

## Placeholders and discipline

When you cannot name a type/signature precisely, placeholder it - but keep the
**size / order / slot-count exact** and document the observed fingerprint:

- A member you can't name: `void *unknown_<off>_;` (8 bytes). For a 24-byte
  `NonOwnerPointer<?>` use `Bedrock::NonOwnerPointer<void *>` - use `void*` not
  `void` as `T` (`NonOwnerPointer<void>` fails to compile via `void& operator*()`;
  `void*&` is fine). A 64-byte unknown map is any real `std::unordered_map<K,V>`.
- A vtable slot you can't name: one `virtual void unknown<slot>() = 0;`.
- **Mark every made-up name with `// TODO(fixme): check the name` on the line
  above it.** Without it a plausible-looking invented name is indistinguishable
  from a verified one, and the person holding the real headers has no way to know
  which to check. Grep the marker to get the full back-fill list.
- **Never carry an inferred name as if it were read from a symbol.** A shape -
  "returns a nested optional", "returns a null unique_ptr" - proves the slot and
  the return kind, never the identity. Name it `unknown<slot>` and put the shape
  in the comment; do not promote a guess into the header.
- Comment every placeholder with offset + fingerprint for later identification,
  and keep Endstone's `lower_case_` naming. When headers later arrive (Scenario
  A), back-fill the real types and cross-validate.

---

# Finish

Re-run the dumper until all symbols resolve (or remaining `0`s are understood).
Build (`cmake --build --preset conan-release`), run tests, add a CHANGELOG entry
("Added support for BDS X.Y.Z"), commit, open the PR.

## Gotchas (shared)

### Whole-binary diff: clearing a hotfix release without IDA

When the new release is a hotfix off the current one, `lief` alone can *prove*
the bump is ABI-free in minutes - no IDA database, no header diff, no PDB. Both
binaries are already in `~/.bedrock_server/<platform>/`; unzip the old and new
side by side and run these three checks. They also catch the Scenario-B trap
where a stale pattern silently matches a *different* function, so run them on
any pattern-only table however the version was resolved.

1. **RVA-delta bands.** Sort every resolved offset by RVA and print
   `new - old` against the previous committed table. A rebuild with a handful of
   insertions gives a small set of deltas that only ever step *up* with RVA
   (1.26.36: Linux +192/+208/+240 code, +880/+896 `.bss`; Windows +64 then
   +576). A negative delta, a wild outlier, or a flipped relative order is a
   pattern that matched the wrong place.
   - **Section bases move too.** A Linux symbol's delta is *section shift +
     intra-section shift*, so compare `.text`/`.bss` virtual addresses between
     the two ELFs before calling a delta anomalous (1.26.36 `.text` itself moved
     +192, so a +48 insertion inside `.text` reads as +240 on every symbol).
   - On PE, sections are page-aligned, so `.text` growing within its current
     page count leaves `.data` *unmoved* - a global's delta of exactly 0 while
     code shifted is correct, not a stale hit.
2. **Body comparison names the function without a name.** For each entry read
   ~256 bytes at the *new* offset in the new binary and at the *old* offset in
   the old one, and require >=90% byte equality. Same function = near-identical
   prologue and body (only relocated displacements differ); a pattern that
   collided with a different function scores far below. This is the cheap
   stand-in for *decompile-and-compare* ([[feedback_decompile_to_confirm]]) when
   the function is untouched by the release. Data symbols in `.bss` have no file
   content - expect them to read back as unreadable and check their section
   membership and delta instead.
3. **Vtable-invariant sweep proves no slot moved anywhere in the binary** -
   far stronger than checking the classes you happen to suspect:
   - **Linux:** walk `.data.rel.ro` as a qword array and compare index-by-index,
     accepting only the known section shifts as differences. If the section
     stays index-aligned end to end, **no pointer was inserted or removed**, so
     no vtable changed length and none was added or removed. Residual
     divergences are plain data - a packed version constant
     (`0x0001_00<patch>_001a_0001` is major/minor/patch, and it flipping
     `21`->`24` is just 26.33 -> 26.36), counters, build hashes.
   - **Windows:** `.rdata` interleaves vtables with string/float blobs, so a
     changed blob breaks index alignment without meaning anything. Classify each
     qword instead (points into `.text` / into `.rdata` / zero / plain data) and
     compare the **total count of code pointers**. Equal counts across versions
     = no vtable slot added or removed binary-wide.
4. **Protocol version, statically.** `SharedConstants::NetworkProtocolVersion`
   is compared directly inside
   `ServerNetworkHandler::_validateLoginPacket`, whose offset the table already
   holds. Disassemble it (capstone) and collect `cmp` immediates; the protocol
   version is in that set. Run it against the *previous* version first to
   confirm the method reproduces the known-good value, then the new one.
   Counting raw dwords of a candidate value across `.text` does **not** work -
   the value collides with thousands of unrelated constants.
   - **Do not filter candidates by "plausible successor".** Mojang re-bases the
     numbering, it does not only increment: 1.26.36 -> 1.26.40 went 1001 ->
     2168. Bound the `cmp` scan loosely and let the structure pick the answer,
     not the magnitude. Anything downstream assuming small monotonic steps
     (version-gate tables, DSL `since=` boundaries) needs review after a bump.
   - Two symbol-free corroborations, worth running when the jump looks odd:
     scan executable segments for the compare idiom itself
     (`cmp [reg+0x30], imm32` + `jge` + the disconnect-reason store, immediate
     wildcarded), and read the RakNet MOTD builder (xref `"MCPE;"`, then the
     `operator<<` that stringifies the protocol field). Both land on the same
     constant from completely different code.

Do not skip the source-side bump because the offsets moved uniformly:
`shared_constants.h` (`PatchVersion`, and `NetworkProtocolVersion` if it moved)
and the README Minecraft badge still need updating.

### Real BDS changes that still need no `src/bedrock/` action

Endstone deliberately models only a subset. Rule these out before editing:

- **Additive enum values** beyond Endstone's declared subset - the underlying
  width is unchanged, so values added past Endstone's range need nothing.
- **A class Endstone truncates with `// ...`** (e.g. `NetEventCallback`) -
  virtuals/members added in the undeclared tail are already covered; touch
  nothing. (One added *in the middle* of the declared part still needs handling.)
- **An empty-stubbed type** (`struct X {};`, e.g. `NetworkPeer::NetworkStatus`) -
  internal changes need no action.
- **New free functions or non-virtual methods** Endstone does not declare - no
  layout or vtable impact.
- **A newly added `std::lock_guard`.** New `MSVCP140!_Mtx_lock`/`_Mtx_unlock`
  calls bracketing existing code, with the `INT_MAX` -> `INT_MAX - 1` recursion
  sentinel and `std::_Throw_Cpp_error(5)`/`(6)`, is MSVC's inlined
  `std::mutex::lock()`. Threading hardening around unchanged logic - it shifts
  the stack frame and nothing else.

### Tooling

- **lief's free build cannot read PDBs** - `lief.pdb.load()` fails with
  "DebugInfo are not available for this build". The Windows path shells out to
  `pdbtool`.
- **dbghelp is too slow** - a ~2 GB BDS PDB takes minutes; `pdbtool dump ... psi`
  takes ~25 s.
- **PowerShell `>` writes UTF-16** - a redirected `pdbtool` dump won't grep with
  ASCII patterns; capture via the script's subprocess (UTF-8) or `iconv -f
  UTF-16 -t UTF-8` first.
- Windows symbol offsets are RVAs: `RVA = section RVA + symbol offset`.
- Some symbols are never public - function-local statics, lambdas, some data -
  and never appear in `pdbtool ... psi`. The `--pdb` path handles this: any
  entry it cannot resolve by name falls back to scanning that entry's `pattern`
  in the server binary. Such an entry just needs a valid byte `pattern`; its
  `name` can be an arbitrary unique key.
- When a function has no usable symbol at all (an overload dropped from the PDB
  publics, or a lambda) - resolve a *callee* instead: reimplement the function in
  `src/bedrock/` from the decompilation and `BEDROCK_CALL` a function it calls
  that does have a stable symbol. Verify the reimplementation; never guess.
- A function inlined on one platform but out-of-line on the other has a real
  symbol on only one side. Split resolution with `#ifdef`. If the inlined-away
  function is *hooked* (not just called), `#ifdef`-calling the original does not
  help - relocate the hook on that platform to the still-out-of-line caller it
  was inlined into and reconstruct the trigger condition there.
- To recover a **renamed** Windows symbol (re-signed, not removed), dump every
  public once with `pdbtool --quiet dump <pdb> psi` and grep for the scope
  (`?<method>@<Class>@@`); the surviving line is the new mangled name. Verify
  against the signature before copying it into the config.

### Re-extracting a stale Linux pattern

A "Linux only" miss is a stale byte `pattern`; the function still exists.

- **Prefer a prologue pattern over a call-site one.** A call-site pattern
  (`E8 ?? ?? ?? ??` + `rip_relative` / `rip_offset`) resolves via a `call`
  displacement, so it breaks whenever the *caller* changes - even when the
  target is untouched (one release breaks a whole cluster). Re-anchor on the
  function's own prologue wherever it is not inlined.
- **Converting a call-site entry to a prologue entry: delete `rip_relative` and
  `rip_offset`.** A prologue pattern's match offset already *is* the function;
  leaving `rip_relative = true` makes the dumper RIP-decode prologue bytes and
  resolve to a silently-wrong, non-zero address - worse than a `0` miss.
- A **prologue miss** vs a **call-site miss** differ: prologue = the function's
  own codegen changed (callee-saved register set, argument shuffle); call-site =
  a caller changed. Neither implies a signature change. Confirm against the
  Windows mangled name before assuming the config `name` must change.
- **Cross-version dump to triage.** Run the dumper against the *previous*
  version (copy the config, set the old `version`; the output path is fixed, so
  `git checkout` the regenerated table afterward). A pattern that matches the
  previous but not the new version went stale *at* this bump; one that misses
  both was already wrong.
- **RVA-delta plausibility.** Cross-check resolved offsets against the previous
  table: deltas should be positive (the binary grows) and rise broadly
  monotonically with RVA. A negative delta, a wild outlier, or a flipped
  relative order flags a pattern that matched the wrong location. Sub-MB local
  non-monotonic wiggles are normal linker noise. **This only holds while the
  linker keeps its layout.** When a release reshuffles link order, correct
  entries move megabytes *backwards* and swap relative order wholesale - see
  *Verifying a pattern-only table*. Treat deltas as a weak hint, never an arbiter.
- A stale pattern can match the **wrong function** entirely (not just miss) -
  see *Scenario B - Finding a new symbol / offset* for the two-way verification
  (function start + decompile-confirm) and the stale-PDB interaction.

### Verifying a pattern-only table (no PDB)

When a release ships **no PDB**, every Windows entry is resolved by byte pattern,
so *none* is name-verified. A stale pattern does not only miss - it can match a
different function with a similar prologue and yield a plausible, wrong offset.
Verify the whole table against the previous version's table plus both binaries.

- **Check pattern uniqueness first.** `find_signature` uses `re.search`, so it
  takes the **first** match and never checks for a second. Re-run each pattern
  with `re.finditer` and count both matches *and distinct resolved addresses*.
  Many matches collapsing to one address is harmless; n matches resolving to n
  addresses means the answer was decided by `.text` ordering alone - that is
  where wrong offsets come from. Also run the new patterns against the *previous*
  binary and assert each lands on the old known-good offset; that validates the
  pattern, though not the first-match choice in the new binary.
- **`.pdata` is the decisive function-start check on Windows - run it on every
  entry first.** Parse the exception directory (data dir 3) as 12-byte
  RUNTIME_FUNCTION records and exact-match the RVA against a `BeginAddress`.
  If it is not a `BeginAddress`, the offset is not a function start. Follow
  chained continuation records (`UNW_FLAG_CHAININFO`, `flags = byte0 >> 3`,
  next.Begin == cur.End) to get the true extent for the later checks.
- **Referenced-string sets are the strongest cheap identity signal** - strings
  are version-stable while code is not. Resolve every RIP-relative reference in
  the extent into `.rdata`/`.data` and read ASCII/UTF-16. A genuine match gives a
  near-identical or superset string set; large disjointness is a false match.
  Filter the ubiquitous allocator strings (`void *operator new(size_t)`,
  `We failed to allocate %zu bytes.`, `pointer || size == 0`, the
  `MemoryOperators.cpp` path) - they appear in almost every function.
- **For string-less functions, walk the call closure 2-4 deep and compare that
  string set.** This is what catches the hard cases; a body with zero strings
  clears nothing on its own.
- **A normalized instruction signature beats raw byte equality** - mnemonic plus
  operand *register class*, with all immediates and displacements dropped.
  It survives relocation and re-register-allocation, unlike raw bytes.
- **But structural similarity measures the pattern's function *class*, not
  identity.** A pattern that selects a family (e.g. thread-safe-static singleton
  accessors) makes every candidate score ~0.96 against the old function. Always
  pair a high ratio with a semantic discriminator - the marker string named in
  the pattern's own locate recipe (the numbered comment block above its
  `[[signatures]]` entry) is exactly the right one.
- **Judge by behaviour, not size.** Extent length and instruction count drift
  legitimately with inlining; do not flag on size alone.
- **Some tiny tail-call thunks legitimately have no `.pdata` entry** (e.g. an
  argument shuffle plus `jmp`). Before calling it a defect, check whether the
  previous known-good version also lacked one; if so, verify by following the
  `jmp` and comparing the target instead.
- **Data symbols are not functions.** Globals have no `.pdata`, no prologue, no
  strings, and need not be 16-byte aligned. Verify by section membership plus
  reference population: scan `.text` for every 4-byte window where
  `R + 4 + disp == target`, group hits by containing `.pdata` function, and check
  that the dominant referencer carries the locate recipe's marker string and that
  the referencer count is stable across versions. Random hits are negligible over
  a ~170 MB `.text`, so the scan is essentially noise-free.

The Linux side needs the same audit - the shipped ELF is stripped (no `.symtab`,
only a tiny `.dynsym`), so every Linux entry is pattern-resolved too.

- **The FDE table is the Linux `.pdata` - and it is exact.** `.eh_frame_hdr`
  holds a sorted binary-search table of one `<i4 initial_location, i4 fde_ptr>`
  pair per function, datarel to the hdr vaddr; the FDE then gives the exact
  length (`pc_begin` pcrel at fde+8, `pc_range` at fde+12). Exact-match the
  offset against an `initial_location`, and validate the decode by asserting
  `pc_begin` equals the table value. Exact extents also remove the tail-call
  problem, so no flow-following heuristic is needed.
- **The dangerous miss is a match *inside* the right function.** When a prologue
  gains one instruction (an extra `push`), a stale pattern matches a few bytes
  late and the bytes there still decode as a plausible prologue - only the
  FDE/`.pdata` exact-match catches it. Alignment is a useful secondary hint.
- **RTTI vtable slots are the only name-anchored check on Linux.** Rebuild
  `.data.rel.ro` from its `R_X86_64_RELATIVE` addends, key vtables by their
  Itanium typeinfo name, and confirm the offset sits at the same slot of the same
  class in both binaries. Covers roughly a quarter of a table for free.
- **A vtable slot that *moved* is not automatically wrong.** Prove it: the
  class's primary vtable must have grown by exactly the shift, and the
  neighbouring slots must align old-to-new (difflib over per-slot function
  fingerprints) with old slot N mapping to new slot N+k.
- **Expand strings caller-side, not just callee-side.** Index every `E8` rel32 in
  `.text` with numpy for an exact caller map (millions of calls in seconds), then
  compare the *callers'* string sets. This is what decides entries that have no
  strings of their own and no callee strings either.
- **Weight strings by distinctiveness rather than maintaining a filter list.**
  Sample a few thousand random functions, count string frequency, and treat
  anything appearing in >0.2% of them as generic. Matching only on boilerplate is
  nearly no evidence.
- **Report `old - new` and `new - old` separately.** A release adding strings or
  callees is routine; a genuine match *losing* its old anchors is not. Collapsing
  to one Jaccard hides the direction that matters.
- **Cross-platform agreement separates "wrong offset" from "changed body".** A
  stale-pattern collision is a per-platform accident. A symbol that scores low
  structurally on Windows *and* Linux independently means the code changed; low
  on one platform only points at that platform's offset.
- **`.bss` data symbols have no file content** (`SHT_NOBITS`) - nothing to
  byte-verify, no prologue, no FDE, no vtable. Use the reference scan, with the
  ELF wrinkle that the displacement is not always last: run the
  `u32(p) + p == (T - text_lo - 4 - imm_len) mod 2^32` comparison for `imm_len`
  in 0/1/2/4, then re-decode each hit to confirm. Compare the reference *count*
  and per-function distribution rather than strings - the static-init blobs that
  reference such globals legitimately change content between releases.
- **A function reachable only through thunks looks unreachable.** Zero direct
  callers plus zero vtable slots can just mean an adjustor thunk (small FDE, call
  site at +3) or a 5-byte `jmp` island just before the entry. Index `E9` rel32
  too; the thunk topology itself should mirror 1:1 across versions.

### BDS itself

- BDS sometimes does **structural refactors**, not just signature tweaks - e.g.
  1.26.20 split packets into a `<Name>Payload` struct plus `cerealizer`/
  `serialize` specializations, and re-parented `ConnectionRequest` under
  `BaseConnectionRequest`. Real design work, not a mechanical edit. When one
  packet shows the `<Name>Payload` split, assume *every* `network/packet/*` got
  it; `inventory_slot_packet.h` is a good reference.
- A function missing from the PDB publics may have been inlined, not removed.
- Whole-program optimization can **drop the unused `this`** from a non-virtual
  member that never touches it - every argument shifts down one integer register
  (Linux: first real arg moves `rsi` -> `rdi`; spot `mov <reg>, rdi` where the
  old prologue had `mov <reg>, rsi`). Mirror by declaring the function `static`
  in `src/bedrock/` (`#ifdef`-split if only one platform optimized it). This
  changes `__FUNCDNAME__` (Itanium loses the cv-qualifier `_ZNK...` -> `_ZN...`;
  MSVC loses the `B` const code) - the `name` must change, not just the pattern.
- BDS sometimes promotes a **type alias to a concrete type** - e.g. 1.26.20
  turned `using DimensionType = AutomaticID<Dimension, int>;` into a `struct
  DimensionType`. The mangled name of every symbol taking it changes with no
  logic change (a Windows miss). The fix is structural: make Endstone's type a
  real `struct` too (mirror the layout), replace every use, delete the orphaned
  alias header. Editing only the config `name` is not enough - `__FUNCDNAME__`
  comes from the declaration.
- Removing a class is a **multi-file cascade**: also remove its
  `src/endstone/runtime/bedrock_hooks/*.cpp` hook, the `CMakeLists.txt` source
  entries, the `[[signatures]]` blocks in both configs, the generated
  `src/bedrock/symbols/*.h` entries, and any dependent declaration. `grep` the
  class name afterward to confirm nothing dangles.

### Symbol table is not optional for the build

- `get_symbol()` is `consteval` and **throws** when a name is absent; the dumper
  **drops every `0`-valued entry**. So an unresolved symbol any TU consumes via
  `BEDROCK_CALL` / `BEDROCK_VAR` / `BEDROCK_CTOR` is a hard **compile** error
  (`C7595: call to immediate function is not a constant expression`), not a
  silently-disabled hook. You must resolve every *consumed* symbol before the
  build goes green - porting alone never produces a buildable tree.
- When a hooked function's signature changes, its `__FUNCDNAME__` changes, so the
  old `name` in the configs goes stale - update it and re-dump.
- **Temporary verification trick.** To compile-check the port before symbols are
  finished, *temporarily* comment out the `if offset == 0: continue` skip in
  `write_symbols_header` in `scripts/dump_symbols.py` so every entry (incl. `0`s)
  lands in the header; `get_symbol` then finds every key and the build goes
  green, surfacing real ABI/source errors. **Revert before committing** - the
  dumper must keep dropping `0`s so a genuinely missing symbol stays a loud
  error.

### Resuming a partially-done bump branch

- Don't trust that an earlier stage is finished. A prior commit may have ported
  only some files of a stage (e.g. some `network/packet/*` but not all) and left
  `.cpp` files referencing the pre-refactor shape. Build early; the first compile
  pass surfaces these gaps.

## Record findings here

Append durable, version-independent lessons to the matching section (shared
**Gotchas**, or the scenario that owns the technique). Keep per-version specifics
(which overload was chosen, what replaced a removed function) in commit messages
and the CHANGELOG, not here.
