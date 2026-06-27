---
name: update-signatures
description: Refresh stale byte-pattern signatures in scripts/configs/{windows,linux}.toml so a pattern-only dump_symbols scan resolves every entry, using the ida-pro idalib MCP. Use when the pattern scan misses after a BDS update, or when asked to "update/refresh the signatures", "fix the stale [[signatures]]", or "regenerate the byte patterns". To first LOCATE a function from scratch in a no-PDB DB, see the locate-function skill.
---

# Update the byte-pattern signatures in the symbol configs

Each `[[signatures]]` block in `scripts/configs/{windows,linux}.toml` carries a
`pattern` that `scripts/dump_symbols.py` scans for when no PDB is available (and
as the PDB fallback on Windows). After a BDS rebuild most patterns go **stale** -
the compiler shifted the bytes - so a pattern-only scan resolves the wrong
offset or none. This skill regenerates the stale ones against the new binary
with the **ida-pro idalib MCP**, leaving already-matching ones untouched.

This is the pattern-only counterpart to `bump-bds` Phase 1: that one re-dumps via
the PDB; this one re-cuts the byte patterns themselves.

> **Locating vs cutting.** This skill is the **cut-the-pattern** half: classify
> staleness, regenerate direct/rip/data patterns, pick the most survivable form,
> verify with `dump_symbols`. To first **locate** a function whose pattern went
> stale or missing in a fresh, unnamed, no-PDB DB - find its address from scratch
> by following the numbered locate recipe in the comment block above its entry -
> use the **locate-function** skill, then return here to cut the byte pattern
> from the confirmed address. (The locate recipes, vtable-slot maps, string
> anchors, inlining-drift, and the "improvise, don't bail" craft all live there.)

## Inputs

- **Target binary + its IDA db**: `…/<version>/bedrock_server.exe` with a
  `bedrock_server.exe.i64` beside it.
- **Ground truth = `src/bedrock/symbols/windows.h`** - the `{"name", RVA}` table
  the PDB dump produced. Treat these RVAs as authoritative. A symbol's
  `VA = 0x140000000 + RVA` (Windows image base). Precompute the VA table once
  (e.g. `base=$((0x140000000)); printf '0x%X' $((base+RVA))`).
- **idalib tools** are deferred - load first with ToolSearch
  `select:idalib_open,find_bytes,make_signature_for_function,make_signature,xrefs_to,find_xref_signatures,get_bytes,list_globals,idalib_switch,idalib_list,idalib_current`.

## Operational gotchas (read first - these cost the most time)

There is **one** idalib worker and it processes one request at a time. Work
**sequentially in a single session**; do NOT fan subagents onto it (they
serialize and contend for zero speedup).

### Opening the DB / dirty-unpacked recovery

`idalib_open(<…>.i64, session_id:"bds", run_auto_analysis:false)`. If it fails
with `Failed to open database`, the db is in a **dirty unpacked state**: the
`.i64` sits beside live `.id0/.id1/.id2/.nam/.til` files (a prior IDA/idalib
session exited uncleanly). idalib has no "open anyway?" override, so recover:

1. Kill the workers. Identify them by path so you don't kill unrelated
   `python.exe`: `Get-CimInstance Win32_Process -Filter "Name='python.exe' OR
   Name='idalib-mcp.exe'" | ? { $_.CommandLine -match 'ida-pro-mcp' } | % {
   Stop-Process -Id $_.ProcessId -Force }`.
2. **Move** (not delete - reversible, so prefer this over deleting) the unpacked
   files `bedrock_server.exe.{id0,id1,id2,nam,til}` into a backup subfolder
   (instant same-volume rename; preserves any analysis newer than the `.i64`).
   If a move says "in use", a worker respawned and re-grabbed the handles -
   kill again, then move.
3. Reopen the `.i64`. It re-unpacks cleanly from the packed snapshot.

A fresh-from-`.exe` open is a non-starter: with no `.pdb` beside it, re-analysis
won't recognize functions by mangled name and `make_signature_for_function`
fails. The existing `.i64` (PDB-derived names) is required.

### Session idle-recycle - drive continuously

The worker is recycled after a short idle window (minutes), which **unbinds the
`bds` session** (`idalib_switch bds` → "not found", calls return "No database
bound"). Long conversational pauses between idalib calls trigger this. So: once
the DB is open, **drive the regeneration in tight back-to-back tool calls** -
don't stop to discuss mid-run. When the session is lost, re-run the
open/recovery above and rebind, then continue. Check liveness with
`idalib_current` / `idalib_list`. This bit us repeatedly - every time the
conversation paused, the next idalib call failed and needed a reopen.

### Memory watch

The user may ask to kill+restart the worker if its memory exceeds a threshold
(e.g. 24 GB). Caveat: the worker holding a multi-GB **memory-mapped** db reports
near-**0** `WorkingSetSize` (the mapping isn't in the working set), so working
set alone undercounts - if you need a true figure, look at `PrivateMemorySize`
/ commit. In practice the workers stayed tiny throughout a full 66-entry run, so
this is a backstop, not a routine event. To restart: kill (as above) → reopen.

### Timeouts

`make_signature*` and `find_xref_signatures` do full-binary uniqueness scans and
can hit the 60 s tool timeout, especially the first call on a cold worker, on
large stack-frame functions, and on heavily-referenced targets. Batch direct
regenerations ~4 at a time; do big functions solo and retry once on timeout. A
timed-out result is **indeterminate**, not a confirmed failure - re-run it.

## Process discipline

- **Many small steps, reported.** Regenerate **one function at a time** (or in
  batches of ~4), editing the toml and reporting after each, so a stall is
  immediately visible and progress survives a session drop. The user explicitly
  wants this cadence over big silent batches.
- **Check fast, fix narrow.** One batched `find_bytes` pass classifies every
  entry as keep/stale cheaply; only the regeneration calls are expensive. Don't
  churn entries that already match.
- **Fold the toml edit into the same step** as the signature you just got, then
  move on (fix-one-save-next).

## Step 1 - staleness check (batched, cheap)

Convert each toml pattern's single `?` → `??` and pass **all** entries to one
`find_bytes(patterns:[…], limit:2)` call. Per entry: **1 match → keep** (not
stale); **0 or ≥2 → stale**, regenerate. `limit:2` is enough to classify (2
means "≥2"). Re-run any entry that returns `error: Tool timed out` **on its own**
(indeterminate, not a confirmed 0).

Sanity-check the tool, not just the patterns: pick a known-good entry that
returns 0 and confirm with `get_bytes(VA,16)` that the bytes *genuinely* changed
vs the old pattern, before trusting a near-universal "stale" verdict.

## Step 2 - regenerate DIRECT entries (no `rip_relative`)

`make_signature_for_function(name, format:"ida")`. It returns `addr` (the entry
VA) and a `signature` already using single `?` wildcards.

- **Verify**: `addr` MUST equal `0x140000000 + windows.h RVA` for that name. If
  not, stop and report (name resolved wrong, or windows.h is itself stale).
- Copy `signature` **verbatim** into `pattern`; keep `relative=false`, `extra=0`,
  no rip fields. **Transcribe carefully** - a single wrong byte makes the pattern
  miss; it's the easiest mistake to make and `find_bytes` later catches it.
- Mangled names resolve in idalib for normal functions; pass the name string.
  (You may pass the VA instead, but then `addr==VA` verification is vacuous.)

## Step 3 - regenerate RIP entries (`rip_relative=true`)

**Do NOT reach for `find_xref_signatures` first.** Its cost scales with the
**number of callers** - it signatures *every* one before returning. It's instant
at ~16 callers (the result reports `total_xrefs`) but **stalls past the timeout**
on hot/virtual/factory functions called from hundreds of sites. `top`/`max_length`
only limit what's *returned* and per-site length, not how many callers it walks.
Use the bounded fallback instead:

1. `xrefs_to(target_VA, limit:6)` - lists a few call sites cheaply, no signature
   work, can't stall. Each xref has `type` and the containing `fn`. **Skip**:
   - `type:"data"` xrefs - those are vtable slots / data tables, not calls.
   - callers with `fn.size` `0x5` (or any tiny forwarder) - a `jmp` thunk, not a
     `call`.
   For a **virtual** function (`UEAA`/`UEBA`) most xrefs are `data` (vtable
   entries); the few `code` xrefs are the override/devirtualized direct calls -
   often just one. For a normal function they're spread across real callers.
2. `make_signature([call-site addrs], max_length:16)` on those specific
   addresses - bounded generation, not a caller sweep. A call site is a
   `call rel32`, so the signature starts `E8 ? ? ? ?` → `rip_offset=1`.
3. **Pick the most maintainable** result (your judgment, *not* necessarily the
   shortest): an `E8`-starting signature with concrete anchor bytes after the
   call (recognizable reg/stack ops, distinctive opcodes), avoiding all-wildcard
   chains (`E8 ???? E8 ???? E8 ????`), `E9`-then-`CC` thunk padding, and -
   when an in-function option exists - signatures that span into `CC` padding
   + the next function's prologue (make_signature does this to reach uniqueness;
   it's valid but the most likely to drift on a rebuild).
4. **Length backoff (the rule)**: if every sampled site is "not unique within
   length constraints" at 16, retry **those sites at `max_length:32`**. **Never
   exceed 32.** If 32 still yields nothing, try a couple *more* call sites
   (`xrefs_to` with a higher `limit`) before giving up on the rip form.

Set fields: `relative=false`, `extra=0`, `rip_relative=true`, `rip_offset=<N>`
from the chosen signature's first instruction:

| first bytes | instruction | rip_offset |
|---|---|---|
| `E8` / `E9` | call/jmp rel32 | 1 |
| `48 8D 05/0D`, `4C 8D 05/0D` | lea rip-rel | 3 |
| `48 8B 05/0D`, `4C 8B 05/0D`, `48 89 05/0D` | mov rip-rel | 3 |
| `FF 15` | call `[rip+x]` | 2 |

Almost every fallback signature starts `E8` → `rip_offset=1`. Old configs used
larger offsets (5/6/7/8/10/12/15/29) only because their pattern began at the
*argument setup before* the call; the new E8-first patterns make those obsolete -
**update `rip_offset` to match the new pattern**, don't leave the old value.

### Resolving "hard" call sites - capture-all, pre-call, string anchors

`find_signature` in dump_symbols uses `re.search`, which returns the **first**
match in `.text` and computes the target from the call displacement there. So a
call-site pattern does **not** need to be unique - the bar is just **"the first
match resolves to the target function."** False positives at *higher* addresses
are harmless (never reached). This unlocks several techniques, in rough order of
preference. The tool for all of them is `make_signature_for_range(start, end,
wildcard_operands:true)` - it encodes a fixed byte range operand-wildcarded with
**no uniqueness requirement** (unlike `make_signature`, which gives up when not
unique). After building a candidate, `find_bytes` it to inspect matches; the
final dump_symbols diff is the real arbiter (a wrong first match = a wrong
offset in the diff).

1. **Forward capture-all** (`rip_offset=1`): `make_signature_for_range` from the
   call site forward ~28 bytes. Works if the *first* match in `.text` is a true
   caller (common when callers are homogeneous - template instantiations - or the
   lowest-address caller sits early in `.text`). The generic 3-arg shape
   (`lea rdx,[rbp]; lea r8,[rbp]; mov rcx,[rbp]; call`) is **too generic** - it
   precedes *any* such call, its first match is usually a false positive
   (`addEnumValues`' arg-setup matched 60+ scattered sites), and even
   `registerOverloadInternal`'s distinctive `call; load; null-check; virtual-call`
   collided with a sibling function called identically. You need following bytes
   specific to this function's call sites.

2. **Pre-call anchoring** (what the old configs' `rip_offset` 15/29 did): include
   the **argument-setup bytes before the `E8`** to reach uniqueness.
   `rip_offset = (call_addr − pattern_start) + 1`. A distinctive pre-call op - a
   `lea r8,[rip]` (string-literal arg), a `mov r9d,<enum imm>`, a specific
   register shuffle - often makes it unique (got `knockback`, `stopSleepInBed`,
   `disconnectClientWithMessage`, `send` down to single matches). Decode the few
   instructions before the call (`get_bytes` + hand-decode, or `disasm`) to find
   an instruction-aligned start.

3. **String-literal anchoring** (most stable): if a nearby `movabs rax,"<name>"`
   builds a well-known **enum/command name** that feeds the call, anchor on those
   literal ASCII bytes. Minecraft's core names (`"GameMode"`, `"RValueParam"`, …)
   don't change between builds, so this is *more* durable than a register-
   allocation-dependent prologue, not less. `rip_offset = (call_addr − movabs) + 1`.
   (Watch which call the string actually feeds - stack slots get reused, so a
   name may flow into a *sibling* method, not the one you want.)

4. **Direct prologue** (escape hatch): `make_signature_for_function(name)`, then
   delete `rip_relative`/`rip_offset`. Always available, but version-specific.

### Choosing between a call-site pattern and the direct prologue

**The one rule of thumb: pick the pattern most likely to SURVIVE the next BDS
build.** Everything below serves that. Two levers move survival:
- **Shorter is better.** Every wildcarded-operand byte is still a fixed opcode
  byte that a recompile can change; the more fixed bytes, the higher the chance
  one shifts and breaks the match. Minimise length - never carry a pattern longer
  than needed for uniqueness, and trim long `xmm`-save prologues especially.
- **E8 (a `call`/`jmp` site) recovers better.** A call-site pattern keys off the
  *caller's* surrounding bytes + a wildcarded rel32; even when the target's own
  prologue is reshuffled by register allocation / frame-size / inlining (which it
  is, almost every build), the call idiom and its anchor often persist, so the
  pattern re-resolves. A direct prologue is hostage to exactly the bytes that
  churn most. So a **short UNIQUE E8 beats a direct prologue** even at comparable
  length - prefer it.

But survival also means **not mis-resolving**: a direct prologue can only *miss*
→ `0` → a loud `consteval` error, whereas a **non-unique (first-match)** call-site
can **silently resolve to the wrong address** if a future build reorders a
same-idiom call below the true caller. A short E8 only helps survival if it is
**unique** (or its first `.text` match is provably the target). So the working
order: **shortest UNIQUE E8 > minimal-unique direct prologue > long anything**.
Keep the direct prologue when (a) the function is a **virtual / thunk-only**
target (vtable-dispatched, no `E8` call site exists - confirm via `xrefs_to`:
all-`data` xrefs, or the lone code caller is a tiny `0x5`/`0x34` thunk), (b) the
call site is followed by a **generic idiom** (shared_ptr/NonOwnerPointer release
chains, the security-cookie epilogue, bare member-load+cmp) that won't go unique
without an over-long pattern, or (c) the shortest unique E8 is **longer** than the
prologue. (Seen this run: `_trySpreadTo` 40B prologue → 14B unique E8;
`getBurnDuration`/`resolve@ResolveHelper` ~0x40 xmm prologues → ~22-24B unique E8;
but `getActorName`/`lookupByName` E8s came out *longer* than the prologue, and
`createPack`/`prepareFromRecipes`/`deserialize`/`createSpawnedActor` had only
generic call sites - all kept direct.) The older "⅓-shorter" thresholds below are
just this principle quantified:

- **Unique** call-site pattern (single `find_bytes` match): no mis-resolve risk -
  keep it when it is at least **~⅓ shorter** than the prologue.
- **First-match** call-site pattern (multiple matches, first one correct):
  carries the mis-resolve risk - keep it only when **significantly shorter
  (≈½ or less)**; otherwise revert to direct for maintainability.
- The decisive win is avoiding the long `xmm`-save prologues (`44 0F 29 ..` runs -
  `explode` is ~90 bytes vs a ~25-byte call site). When the prologue is already
  compact (`addEnumValues` 51B vs a 53B anchored call site; `createSpawnedActor`
  /`registerAlias`/`deserialize` ~40-50B vs call sites only ⅓ shorter), **keep
  direct**.

Tag the provenance comment with the safety level: `# capture-all (first match)`
vs `# … (unique)`. A **unique** pre-call/string-anchored pattern has no
mis-resolve risk - prefer it over a first-match one when both are short. Rewrite
to direct form by deleting the `rip_relative`/`rip_offset` lines; leaving
`rip_relative=true` on a prologue pattern makes the dumper RIP-decode prologue
bytes into a silently-wrong address - worse than a miss. Flag every form change
in the summary.

## Step 4 - data globals (`BlockState::StateListNode::mHead`, `Enchant::mEnchants`)

Their pretty names don't resolve in idalib, but you already have their VAs from
windows.h, so go straight to `xrefs_to(global_VA, limit:6)`. The referencing
instructions are rip-relative `mov`/`lea` (`48 8B 0D`, `48 8B 1D`, `48 8D 3D`,
`48 89 0D`, …) → `make_signature` at one of them, `rip_offset=3` (confirm from
the first bytes). These have few xrefs, so `find_xref_signatures(<address>)` also
works if you prefer. (If the VA isn't known, find it via
`list_globals(filter:"*mEnchants*")` etc., or `find_xref_signatures("<pretty
name>")`.) Any rip ref to a given global computes the same address, so a
first-match ref is fine. To find the global itself from scratch in a no-PDB DB
(via its initializer), see **locate-function**.

## Special cases

- **`?executeWorldInteraction@InventoryTransaction@@…`** - the toml comment notes
  the name is arbitrary (it hooks a lambda); it will NOT resolve by name. Only run
  the Step-1 `find_bytes` check: if still unique, **keep it**; else flag for manual
  handling. Never `make_signature` it.
- **`?randomTick@LeavesBlock@@…`** - resolves to a tail-call **thunk**
  (`48 89 D1 E9 … CC CC …` = `mov rcx,rdx; jmp`). `make_signature_for_function`
  correctly returns the thunk signature - keep it.

## Provenance comments (required)

Add **one concise line** above each regenerated `pattern` recording how it was
found (the user wants this on every entry you touch). Place it right before
`pattern =`:

- rip / call-site: `# xref call site in <Caller>` (name the containing function).
- direct (own prologue): `# function prologue`.
- direct via escape hatch: `# function prologue (<one-line reason>)`, e.g.
  `# function prologue (call sites not unique within 32)`.
- data global: `# rip-relative load in <Caller>`.

The numbered-step comment block above an entry is the from-scratch locate recipe -
keep it; it is authored/followed via the **locate-function** skill, not
regenerated here.

## Finish - verify and summarize

The gold-standard verification is the dumper diff (it exercises the real RIP
math against the PDB-derived table):

```
uv run --script scripts/dump_symbols.py scripts/configs/windows.toml   # no --pdb
git --no-pager diff src/bedrock/symbols/windows.h                       # empty == every offset matches
git checkout src/bedrock/symbols/windows.h                             # revert the regenerated header
```

Expect `Finished signature scanning: N/N items successful` and an **empty** diff
(a CRLF warning is harmless). An empty diff means a pure pattern scan resolves
every entry to the exact PDB offset. Re-run it once more after adding the
provenance comments, to confirm the commented toml still parses and resolves.

Report a summary: **kept** (already unique) vs **regenerated direct** vs
**regenerated rip** vs **rip→direct form changes** (list them) vs **manual**.

## Refreshing across a version bump: verify successes, union-first

When the config is **retargeted to a new BDS version**, the dump splits entries
into **failures** (`Pattern not found`) and **successes** - but do NOT trust the
successes. A stale pattern can still match *some* function in the new binary and
silently resolve to a **wrong offset** - a false positive that is worse than a
clean miss (which at least fails loudly). Verify every resolved entry, then fix
both buckets.

### Verify a "success" - DECOMPILE every resolved entry

Run with the **old named DB and the new (unnamed/no-PDB) DB both open** (two
GUI-MCP instances; `list_instances` / `select_instance`). For **every** resolved
entry (the dumper's "successes" too, not just the failures), `VA = 0x140000000 +
dumped RVA`, then **`decompile` the function there and confirm its behaviour
matches the old namesake** - the actual logic, distinctive constants, args/return
shape, named call graph. A stale pattern can silently resolve to a *wrong*
function (worse than a clean miss, which fails loudly), so every one must be read.

**Do NOT confirm by size, string-overlap, vtable-slot index, address
neighbourhood, or pattern-uniqueness** - each has silently lied on a real refresh
(a plausible-size wrong match; a slot that drifted to a stub; a right function in
an unexpected cluster). Those are triage hints at most; the decompile is the
verdict. The full equivalence-judgement craft (recompilation-invariant tells, how
a wrong match reads categorically differently, the no-baseline lambda case) lives
in **locate-function**; `func_profile` only narrows candidates - it never confirms.

### Fix a stale/wrong pattern - UNION first, else REPLACE

For each entry to re-cut, **first try a version-union pattern**, then fall back:

1. **Union (preferred - survives the next bump too).** Get the function's prologue
   (or the anchored call site) in **both** versions, diff the bytes, and `?`-mask
   only the bytes that differ - immediates, rip/stack displacements, frame sizes -
   keeping the opcode skeleton. If the masked pattern `find_bytes`-matches **both**
   versions and stays unique (extend it until it does), keep it.
2. **Replace (when union is impossible).** If the prologue changed *shape* - an
   inserted/removed instruction (a new `push r12` = `41 54`, `xmm` spills, a
   widened `83 EC imm8` → `81 EC imm32` frame, reordered setup) - then no
   fixed-length `?`-mask can align two **different-length** byte runs; `?` masks a
   byte, it cannot absorb an insertion. Cut a fresh **new-version-only** pattern,
   extended far enough to be unique (`find_bytes` → exactly 1 match, at the entry).
   `startSleepInBed` is the canonical replace: 1.26.20 had 6 pushes + a small
   frame; 1.26.32 added `push r12`, `xmm6-10` spills, and a wider frame - the two
   prologues share only `55 41 57 41 56` (too generic), so it was replaced.

### Refreshing an existing rip entry: keep the form, re-cut per the rules above

The call-site craft is already covered above (**"Resolving hard call sites"** +
**"Choosing between a call-site pattern and the direct prologue"**): E8-first vs
pre-call vs string-literal anchoring, the `rip_offset` table, the length backoff,
that the generic 3-arg shape's first match is usually a false positive, and the
unique-≥⅓-shorter / first-match-≈½-shorter safety bar. Don't re-derive it. Three
refresh-time notes on top:

- **Re-cut an existing rip entry as a rip entry first**, then apply the Choosing
  rule - it was made a call-site deliberately (its prologue is an inlined/churning
  shape), so re-anchor on a call site and only fall back to a prologue when no call
  site can be made to resolve. A **unique** call site is best (no first-match
  hazard): `_sendMessage` and `forEachBlockType` each got one; `registerCommand`
  only a first-match (100+ near-identical setup callers), so it stays tagged as
  such - if a build later inserts a lower-address same-idiom call, revisit it.
- **Verify the resolve TARGET, not just uniqueness** - a unique pattern at the
  WRONG call still yields a wrong offset. Compute `target = (E8_addr + 5) +
  sign_extend(rel32)` and confirm it equals the function's entry VA; for a
  first-match pattern, confirm the **lowest-address** match resolves to the target.
- The `make_signature` uniqueness scan **times out on a cold worker**; the bounded
  hand-build (`get_bytes` at the call site → assemble → `find_bytes`) is instant.
  New anchor worth knowing: the `function_ref`-passing idiom (a `forEach`-style fn)
  - `lea rax,[invoke_fn]; mov [rbp+a],rax; lea rax,[ctx]; mov [rbp+b],rax; lea
  rcx,[this]; lea rdx,[&fnref]; call` - anchor pre-call, extend one post-call insn
  for uniqueness.

### Locating the function first

If an entry is **stale/missing and the function must be re-found from scratch**
(no current address to cut from), that is the **locate-function** skill: it
follows the numbered locate recipe (string anchors, indirect caller/callee hops,
magic-constant anchors, packet vtables, vtable-slot maps, data-global
initializers) and improvises past inlining / vtable drift. Locate + confirm
there, then come back to this skill to cut and verify the byte pattern.

### Neutralize a confirmed-wrong pattern you can't re-find yet

If an entry is confirmed wrong but you can't re-cut it this pass, replace its
`pattern` with an obvious hex-speak **sentinel** that cannot match, so it fails
*visibly* (`Pattern not found`) instead of silently mis-resolving to a wrong
offset. Use a recognizable placeholder, e.g. `0B 50 1E 7E DE CE A5 ED`
("OBSOLETE DECEASED"). Keep the numbered locate-recipe comment block above it -
that block is the recipe for the eventual re-find (see **locate-function**).

## Record findings here

Append durable, tooling-level lessons (idalib quirks, new escape hatches, pattern
survival rules) to the gotchas above. Keep per-version specifics in commit
messages / CHANGELOG. From-scratch locate craft (anchors, vtable maps, inlining
drift) belongs in **locate-function**.
