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
   confirm `OFF` equals the copy's last store.
5. **Cross-check the wire with protocol-docs.** The cereal field set == the
   serialized fields; `EndstoneMC/protocol-docs` (`<branch>/packets/<Name>.json`)
   lists them in order, mapping the copy's offsets to names and flagging
   added/removed wire fields. A field can leave the wire *and* the struct together
   (dropped bools), or appear (a second `FilteredName` once a `RedactableString`
   serialises both halves).

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
- A vtable slot you can't name: one `virtual void <placeholder>() = 0;`.
- Comment every placeholder with offset + fingerprint for later identification,
  and keep Endstone's `lower_case_` naming. When headers later arrive (Scenario
  A), back-fill the real types and cross-validate.

---

# Finish

Re-run the dumper until all symbols resolve (or remaining `0`s are understood).
Build (`cmake --build --preset conan-release`), run tests, add a CHANGELOG entry
("Added support for BDS X.Y.Z"), commit, open the PR.

## Gotchas (shared)

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
  non-monotonic wiggles are normal linker noise.
- A stale pattern can match the **wrong function** entirely (not just miss) -
  see *Scenario B - Finding a new symbol / offset* for the two-way verification
  (function start + decompile-confirm) and the stale-PDB interaction.

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
