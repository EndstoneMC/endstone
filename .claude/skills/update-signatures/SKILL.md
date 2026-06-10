---
name: update-signatures
description: Refresh stale byte-pattern signatures in scripts/configs/{windows,linux}.toml so a pattern-only dump_symbols scan resolves every entry, using the ida-pro idalib MCP. Use when the pattern scan misses after a BDS update, or when asked to "update/refresh the signatures", "fix the stale [[signatures]]", or "regenerate the byte patterns".
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

### Multi-match "capture-all" - rarely works, verify it

Uniqueness isn't strictly required: a pattern that matches *several* call sites
that **all** call the same target resolves fine (every match's `E8` computes the
same offset). But the generic arg-setup shape (`lea rdx,[rbp]; lea r8,[rbp];
mov rcx,[rbp]; call`) precedes *any* 3-arg call, so it has **massive false
positives** (a real run found 60+ matches scattered across the binary). The only
thing unique to a given function's calls is the (varying) call target, which
can't be encoded position-independently. So capture-all only works with a longer
idiom genuinely specific to that function's call sites - and you **must verify**
it: `find_bytes` the candidate, and confirm *every* match's call target equals
the windows.h VA (no match in unrelated code). If you can't prove zero false
positives, don't use it.

### Escape hatch - convert a hard rip entry to DIRECT

When no call site is uniquely signaturable within 32 bytes, signature the
**function itself** with `make_signature_for_function(name)` (prologues are
distinctive) and rewrite the entry to direct form: replace `pattern` and
**delete the `rip_relative` and `rip_offset` lines**. The resolved address is the
same function, so the table is still correct; leaving `rip_relative=true` on a
prologue pattern would make the dumper RIP-decode prologue bytes into a
silently-wrong address - worse than a miss. Flag every such form change in the
summary so it can be vetoed. Recurring triggers seen in practice:

- **All callers in one giant function** (e.g. `addEnumValues`, called only from
  the `CommandRegistry` ctor) - every call site shares the same post-call code.
- **`void`-return functions** (`registerAlias`, `registerCommand`) - generic
  post-call setup, never unique forward.
- **Only tail-call thunks** call it (`send@NetworkSystem`, `Send_…360NoVDP`) -
  the unique signatures all start `E9`.
- **A single non-vtable call site** that isn't unique even at 32 (`knockback`,
  `stopSleepInBed`).

Note the trade-off: a direct prologue is **version-specific** and the long
`xmm`-save prologues (`44 0F 29 ..` runs, e.g. `explode`, `knockback`) are the
most likely to drift on the next BDS rebuild. Prefer a stable call site when one
exists; fall back to direct only when none does.

## Step 4 - data globals (`BlockState::StateListNode::mHead`, `Enchant::mEnchants`)

Their pretty names don't resolve in idalib, but you already have their VAs from
windows.h, so go straight to `xrefs_to(global_VA, limit:6)`. The referencing
instructions are rip-relative `mov`/`lea` (`48 8B 0D`, `48 8B 1D`, `48 8D 3D`,
`48 89 0D`, …) → `make_signature` at one of them, `rip_offset=3` (confirm from
the first bytes). These have few xrefs, so `find_xref_signatures(<address>)` also
works if you prefer. (If the VA isn't known, find it via
`list_globals(filter:"*mEnchants*")` etc., or `find_xref_signatures("<pretty
name>")`.)

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

## Record findings here

Append durable, tooling-level lessons (idalib quirks, new escape hatches) to the
gotchas above. Keep per-version specifics in commit messages / CHANGELOG.
