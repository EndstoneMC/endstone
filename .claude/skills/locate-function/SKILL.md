---
name: locate-function
description: Locate (relocate) a BDS function in a fresh, unnamed, no-PDB IDA database by following the numbered locate recipe in the comment block above its entry in scripts/configs/windows.toml, then confirm it against the old named DB. Use when a byte pattern goes stale/missing after a BDS update and the function must be re-found from scratch, when asked to "relocate/find a function with no PDB", or to "follow/author the locate recipes". Invoked from update-signatures (cut the pattern afterwards) and bump-bds.
---

# Locate a BDS function in a fresh, unnamed database

You are a reverse engineer in a freshly-built `bedrock_server` binary with **no
PDB** - every function is `sub_...`. Given a function's mangled name and its
**old-version RVA**, find its address in the new binary and **confirm** it. This
skill ends once the function is identified and confirmed; cutting the byte
`pattern` from it is the `update-signatures` skill.

**Your instructions are the numbered-step recipe** in the comment block directly
above that entry's `pattern =` in `scripts/configs/windows.toml` - pure numbered
steps (`1.`, `2.`, `3.` ...) written against the previous version. The block names
the function-specific anchors and the path; *this* skill supplies the general
tactics and pitfalls to execute each step. Read it, follow the steps, and when a
step drifts, improvise (below). Keep the **old named DB open beside the new unnamed
one** as the oracle (two instances via `list_instances` / `select_instance`, or
idalib). `VA = 0x140000000 + RVA` (Windows image base; Linux symbols are already
named in the ELF, so this is rarely needed there). **Rename every helper you
walk through** to its mangled name (`rename`) - the DB starts all-`sub_` and each
name makes the next `xrefs_to` / `func_profile` readable. It compounds.

idalib/GUI tools are deferred - load first with ToolSearch
`select:find_bytes,xrefs_to,func_profile,decompile,disasm,get_bytes,get_string,rename,list_globals,list_instances,select_instance`.

## The prime directive: improvise, don't bail

A locate recipe is written against the **old** version. The call
graph and class layout drift, so the recipe's *incidental* details - a caller
count, a Meyers singleton, a "this-only call", a named intermediate hop - go
stale **even when the function is perfectly findable**. The compiler is allowed
to reshape a function (different register allocation, frame size, inlining, 2-3x
size change) without it becoming a different function. So:

**When a step doesn't match what you see, decompile the candidate and judge it by
what it DOES - do not abandon the hunt.** Four rules, each of which cost real time
when ignored:

- **Inlining is the dominant drift - treat a "missing" callee/caller as inlined
  first, vanished last.** When a named intermediate hop can't be found, assume it
  **inlined UP** and xref the still-present anchor one level - the target is one
  hop *closer*, not gone. Tell-tale that a hop got absorbed: the target
  **balloons** in size and inherits the inlined hop's callees and strings.
  (`deserialize@ResourcePackStack` ate `forEachPackId`: `0x15e`->`0xa74`, gained
  `Json::Reader::parse` + `"pack_id"`. `_spread` inlined into `_trySpreadTo`:
  `0x262`->`0x525`. `_readFromLevelStorage`+`deserialize` into the
  `ServerScoreboard` ctor: `0x3d4`->`0x3032`.)
- **Judge equivalence by behaviour, NOT size.** A function can grow 2-3x or shrink
  in one build from inlining / compile flags and still be the same function.
  Size-delta alone is a false signal - on one bump it flagged 13 of 25 entries as
  "wrong"; behaviour confirmed only 5.
- **Caller COUNT is not identity.** Counts shift freely between builds; never
  dismiss a candidate because the block's "has two callers" became 100. Decompile
  and decide. (`getI18n`'s block said its `I18nImpl` ctor "has two callers"; in
  the new DB it had ~100 - the function was still exactly right.)
- **Confirm by recompilation-invariant tells.** What the compiler *cannot* change:
  distinctive **constants** (FNV bases `0x..cbf29ce484222325`, magic divisors
  `0x8e38e38e38e38e39`, enum/event immediates), the **actual math/logic**, **RTTI
  name strings**, and **caller identity** (called by `main` => `DedicatedServer::
  start`; 16 `_registerOverload<...>` callers => `registerOverloadInternal`).
  Anchor on these, not on the call-graph shape the block happened to describe.

A wrong match looks **categorically different** in pseudocode, not just
differently-sized: a 3-line stub, a red-black-tree lookup, a registry-teardown
loop, a `JUMPOUT` into mid-code. That is what "confirm vs the old named DB" rules
out.

## Tools - the cheap workhorse

`func_profile(include_lists:true)` returns a function's strings + callees +
constants + `caller_count` in one compact payload - the workhorse for both
triaging and confirming. Reach for full `decompile` only to settle an ambiguous
case (66 pseudocode dumps exhaust context). `func_profile(include_lists:false)`
gives just `string_ref_count` - the cheapest first filter (`0` => the function
has no string of its own, go indirect). A `func_profile` "Not a function" can be
a fresh-DB analysis gap, not a bad address - check `get_bytes` before condemning
it (a tail-call thunk reads as `48 89 D1 E9 ... CC`). Plus `find_bytes` (string /
byte search), `xrefs_to(VA, limit:6)` (bounded), `disasm` / `get_bytes` (decode a
prologue or call site by hand), `rename`.

## No IDA? Windows relocates fine with `lief` + `capstone` alone

When the databases are still building (or there is no PDB for *either* version),
every tactic below still works from the raw PE - no IDA required:

- **The PE `.pdata` exception directory is the AUTHORITY for function start and
  extent.** Each `RUNTIME_FUNCTION` gives BeginAddress/EndAddress; index it once
  per binary and cache it. An RVA that is not a BeginAddress is conclusively not
  a function start - this is the single most decisive check available on Windows
  and it replaces every prologue-sniffing guess. Caveat: some tiny tail-call
  thunks and leaf accessors legitimately have **no** `.pdata` entry, so absence
  alone is not proof of a bad offset.
- **Reverse the search - do not index ~680k functions.** Find the string in the
  NEW binary, scan executable sections for RIP-relative refs to it, then map each
  referencing address back to its containing function via `.pdata`. That yields a
  candidate set of a handful, cheaply.
- One linear identity finds all rip refs in a single pass: for a disp32 that ends
  an instruction, `disp + offset_of_disp == target_rva - section_va - 4` (check
  `- 8` too for a trailing imm32). Direct `call rel32` falls out of the same
  identity. Absolute 8-byte scans of `.rdata`/`.data` find vtable slots; walk back
  while the preceding qword still points into an executable section to reach the
  vtable start.
- **Exhaustive constant-intersection is the strongest name-free proof.** Take 2-3
  distinctive immediates from the old body, scan the whole new `.text` for each as
  raw little-endian bytes, map hits to functions, intersect. When the intersection
  has the same cardinality in both binaries and one member is independently
  anchored, the other is proven rather than guessed - this rescues a target whose
  anchor string was deleted in the new build.
- **Callee *sizes* fingerprint a call graph without names.** Compare the multiset
  of `.pdata` extents of the distinct call targets; shared helpers keep their
  sizes, and when one drifts it drifts identically for every caller.
- **Follow `UNW_FLAG_CHAININFO` to the primary `RUNTIME_FUNCTION`.** A `.pdata`
  entry whose unwind-info flags nibble has `0x4` set is a cold/outlined *chunk*,
  and its trailing `RUNTIME_FUNCTION` names the real owner. Without that hop a
  chunk resolves as its own "function" and the containing-function lookup lies.
- **Build a direct-call index the same way as the rip index, then filter by
  `.pdata`.** Scan for `E8`, compute `pos + 5 + rel32`, and keep only targets that
  are BeginAddresses. That filter removes essentially every false `E8` byte and
  gives a usable caller/callee graph in a nameless binary.
- **Caller-set intersection relocates a string-poor target.** Map each OLD caller
  to its NEW counterpart via a string unique in *both* binaries, then intersect the
  counterparts' callee sets. Two independently-anchored callers agreeing on one
  callee is proof. Matching caller *counts* then corroborate (a mismatch still
  refutes nothing - see "Caller COUNT is not identity").
- **A caller that maps byte-for-byte pins the call exactly.** When the counterpart
  has the same size and identical structure, the call to the target sits at the
  *same relative offset* inside it - read the callee off that offset instead of
  guessing which `call` it is.
- **Prologue drift is not symmetric across platforms.** The same function can gain
  a saved register on Linux and lose one on Windows in a single bump. Re-derive per
  platform; never port the other platform's fix.

### Linux: `.eh_frame` is the `.pdata` equivalent

- **`.eh_frame_hdr`'s binary-search table is the AUTHORITY for function start.** It
  is a sorted `(initial_location, fde_ptr)` array (`datarel|sdata4`); one
  `struct.iter_unpack` yields every function start (~500k) in seconds, and decoding
  the FDE at `fde_ptr` gives `pc_range` = the EXTENT. Never prologue-sniff.
- **Reverse the search identically**: find the string in the new `.rodata`, find
  RIP-relative refs to it in `.text`, map each back to its containing function via
  the FDE index. One good string usually yields exactly one candidate.
- One linear identity finds the rip refs without disassembling: a disp32 at position
  `p` targets `p + 4 + sext(v32)`, so `(v32 + p) mod 2^32 == (target - 4) mod 2^32`.
  Compute it over the section with numpy in chunks. Filter on a preceding `E8` byte
  and the same pass gives you the direct callers.
- **The binary is PIE - vtable slots live in the `R_X86_64_RELATIVE` addends**, not
  in the file bytes. Rebuild `.data.rel.ro` from `.rela.dyn` before reading any slot.
- **Itanium RTTI hands you any class vtable by name**: find the mangled name
  (`6Player`) in `.rodata`, find the `.data.rel.ro` qword pointing at it (its
  predecessor is the `type_info`), then each vtable is the run of code pointers
  following a slot equal to that `type_info` address. Do **not** require the name to
  be preceded by a `00` - one was packed straight after a float constant and that
  filter silently lost the whole class.
- **Self-naming assert strings state the return type** (`"<ret> Class::method(args)"`
  from the pretty-function macro). Grep them to confirm - or to catch - a signature
  change that the mangled name cannot show.

## Locator tactics (anchor -> target), in order of preference

### 1. Direct string

The function references a **unique** string it owns - an error/log message, a
self-naming validation label (`"3 Player teleportTo"`, `"1 startSleepInBed"`), a
command/translation key, an NBT/DB/registry key. Search the string's bytes
**including the `00` terminator**; its lone **data xref** (a `lea`/`mov`) is
inside the target - walk up to the prologue.

The match need only **resolve to the target**, not be globally unique - a first
match landing in the right function is enough. Judge the string first:

- **Usable:** validation labels, specific content log/error messages, command /
  translation keys, NBT keys.
- **NOT usable (boilerplate, referenced by hundreds of functions):**
  `NonOwnerPointer` asserts (`"Accessing a null NonOwnerPointer"`, the
  `D:\a\_work\1\s\...` source paths), `"%s"` / `"\n"`, `BinaryStream::writeString`
  field labels (`"Data"`). A high `string_ref_count` is often *all* boilerplate.
- **One exception worth knowing:** the *templated* `__PRETTY_FUNCTION__` variant
  `T *Bedrock::NonOwnerPointer<X>::_get() const [T = X]` names its template
  argument, so a rare `X` makes it near-unique and it identifies the function
  by what the function *touches*. BDS is clang everywhere, so these appear
  verbatim on Linux too - use one to cross-validate a Windows candidate against a
  Linux address that still resolves. A `__PRETTY_FUNCTION__` that CHANGED
  (`bool F(...)` -> `SomeResult F(...)`) both confirms identity and flags that the
  mangled name in the config is now stale.

### 2. Indirect via a string-locatable neighbour (no string of its own)

- **Caller / callee hop.** A string-locatable function calls the target (or the
  target calls it). Locate the one with the string, follow the call. **Pin the
  exact call with local landmarks** - "somewhere it calls X" is unfollowable in a
  big caller; give a concrete nearby cue (a specific constant `mov`'d right after
  the call, the call sitting inside an `if cmp .., <imm>`, a distinctive arg
  immediate loaded just before). Common for **virtual base methods**: a derived
  override carries a label and tail-calls its base (`Player::teleportTo` ->
  5-byte `Mob::teleportTo` thunk -> `Actor::teleportTo`).
- **Sibling-shared caller.** The target shares a small caller with an
  already-locatable function and the two are called adjacently there. Locate the
  sibling, step up to the shared caller via its xref, the target is the
  neighbouring `call`. (`compileCommand` is the first call in `runCommand`, just
  above the string-locatable `run@Command`.)
- **Down a forwarder chain.** When the target is a deep helper reached only
  through forwarders, anchor on a string at a **call site** of the chain's entry
  and walk DOWN - the same string locates both the function that *contains* it and
  the *call target* it feeds. (`"Starting Server"`'s xref is the `lea` right
  before a `call Diagnostics::log`; follow `Diagnostics::log` -> 5-byte thunk ->
  `BedrockLog::log_va` -> `_log_va`. Watch for overloads - take the one the thunk
  calls.)

### 3. Magic-constant / behaviour anchor (string-free clusters)

When a whole cluster has no usable strings, search a **recompilation-invariant
immediate** the function bakes in and winnow to the lone function holding the
*whole cluster* of them. (`ExperienceOrb::spawnOrbs` by its XP-split breakpoints
2477/1237/617/...; `DecayingBlocksUtil::_shouldDecay<4>` by the flood-fill bound
1331 (`0x533`) appearing ~10x; a lava `emitFizzParticle` by its LevelEvent-1004
broadcast `66 41 B8 EC 03 FF 15` and float immediates `0.8f=0x3F4CCCCD` /
`2.6f=0x40266666`.) Then walk the single-caller edges of that cluster to the
target.

### 4. Packet vtable (RTTI-name string)

A `*Packet` whose `getName()` returns its own class name string is locatable
through its vtable - and so are its ctor and the ctor's callers. Search
`"<Name>Packet\0"` -> its code xref is the tiny `getName` virtual -> `getName`'s
address is a **slot** in the packet vtable (its data xref) -> walk to the vtable
**START** -> xref the vtable to the function that stores it into `this`. **The
ctor is often inlined into the builder**, so xref the vtable start directly: its
code xrefs are the ctor, the dtor, and any builder that stores the vtable inline -
take the non-ctor/dtor one. One RTTI string thus unlocks `getName`, the vtable,
the ctors, and their (often single) callers - reuse it for any packet's
builder/factory. (`serializeAvailableCommands` is the payload ctor's lone caller;
`completeUsingItem`/`take`/`prepareFromRecipes` build their packet inline;
`_tickServerPlayerMovementCorrectionSystem` is 3 hops down a
`CorrectPlayerMovePredictionPacket` payload ctor.)

### 5. Vtable slot map (class virtuals)

Once **one** virtual of a class is located (by any tactic above), its address is a
slot in the class vtable - so you have the vtable, and every other virtual is a
slot. Read the target's slot **from the old named DB** relative to your anchor
virtual.

**But the slot OFFSET is NOT version-stable** - the base-class virtual order
itself reorders/inserts/removes between builds, so a fixed "+0xNN above the
anchor" lands on the wrong slot (often a shared stub). Re-align by **neighbour
identity**, not by the stored offset:

- **Measure the drift instead of assuming it: pin TWO string-locatable virtuals**,
  as far apart in the vtable as you can. If both land on the same slot indices in
  old and new, there is zero net slot drift across that span and every index in
  between transfers directly - which turns "virtual order barely drifts" from a
  hope into a fact. A virtual sitting one slot from an independently-proven one is
  itself near-proven (declaration order).
- **Prove the span with a diagonal scan when no second string anchor exists.** Score
  `old[i]` vs `new[i]` and `old[i]` vs `new[i+1]` across the slot range and read off
  where the changeover happens; that locates the inserted/removed virtual exactly,
  instead of assuming it fell outside your target. Skip the 1-3 byte stub slots -
  they score 0 either way and carry no information.
- **A derived class's slot may hold a tiny forwarder that tail-calls the base body**,
  so `base_vtable[n] != derived_vtable[n]` is NOT evidence of misalignment. That
  forwarder is itself a strong cross-check: it should be the target's ONLY direct
  caller in both builds.
- Read the **WHOLE** primary vtable (`get_bytes` the region, decode the 8-byte
  slots). Base-class slots repeat **one shared stub address**; the class's **own
  overrides** point into the class's code cluster (same neighbourhood as your
  anchor). `func_profile` those override addresses and identify each by behaviour,
  matching the old DB's sequence - the stored offset only says *roughly* where to
  start.
- **Size proves nothing.** A virtual may be recompiled into a small stub that
  tail-call-forwards the real body, or be **devirtualized** out of the vtable
  entirely. Compare the class's **vtable SIZE** (slot count) between the two DBs;
  a shrink means a virtual was dropped. (`PistonBlockActor::tick` drifted from
  `load+0x40` to `load+0x108` as base virtuals were inserted, yet stayed one big
  virtual - found by reading the whole vtable and picking the lone huge override.)

### 6. Data global via its initializer

A zero-init data global is built at runtime by a unity static-initializer
(`_GLOBAL__sub_I_unity_*`). Anchor on a distinctive string or constant the
initializer *also* builds, find the **rip-relative store/load** of the global
near it. (`BlockState::StateListNode::mHead` via `"conditional_bit"` in the
VanillaStates init's intrusive-list prepend; `Enchant::mEnchants` via
`"enchantment.protect.all"` at the top of `initEnchants`; `Pack::
EDUCATION_METADATA_FILE` via the inline `"education.json"` build.) Any rip ref to
a given global computes the same address, so a first-match ref is fine.

## Confirm the target — DECOMPILE it (ALWAYS)

**Confirmation means decompiling the candidate and matching its BEHAVIOUR to the
old named DB. Nothing else counts.** Read the pseudocode and check the
recompilation-invariant tells: the actual computation/math, distinctive constants
(FNV bases, magic divisors, a `599` cook-time, `1004` LevelEvent, `0.8f`/`2.6f`),
the args/return shape, and the named call graph. Confirm the resolve **TARGET**,
not just that a pattern is unique - a unique pattern at the *wrong* call still
yields a wrong address (`target = (E8_addr + 5) + sign_extend(rel32)`; for a
first-match pattern confirm the *lowest-address* match).

**The following are NOT confirmation — each one silently lied during a real
refresh; do not substitute any of them for decompiling:**
- **Size / size-ratio.** A wrong function often has a plausible size, and inlining
  makes the right one grow 2-3x. (A `0x117`-byte hit was the *wrong* getBurnDuration
  candidate; `tryGetStateFromLegacyData` looked wrong by size yet was right.)
- **String overlap.** Only clears string-bearing functions; a no-string function or
  a generic-prologue false match has nothing to compare.
- **Vtable-slot index.** NOT version-stable — it drifts to a base-class stub
  (`PistonBlockActor::tick`'s old slot became a 1-byte stub; the real tick was found
  by content). Re-align by neighbour identity, then **decompile the slot you pick**.
- **Address neighbourhood / cluster.** Clusters move between builds
  (`tryGetStateFromLegacyData` resolved into the campfire cluster and was correct).
- **A unique byte pattern.** Uniqueness proves the bytes are rare, not that they are
  the intended function.

So the rule is: locate → **decompile the candidate** → confirm the behaviour matches
the old named DB → only then trust it. When refreshing across a bump, decompile
**every** resolved entry (successes included), not just the failures: a stale
pattern can silently resolve to the wrong function, and unlike a clean miss that
fails loudly, a wrong resolve is invisible until you read the code. Every relocate
recipe ends with "confirm vs the old named DB."

## Pitfalls (each cost real time)

- **Search exact keys NULL-TERMINATED.** For an NBT / DB / registry / command key,
  always append the `00` - a bare substring collides with longer ids and misleads
  (`"scoreboard\0"` is unique; bare `scoreboard` also hits `scoreboardId`,
  `commands.scoreboard.players`). The null pins the *end*.
- **`get_string` under-reports length.** IDA may define the string item shorter
  than the real literal (`"1 startSleepIn"` for `"1 startSleepInBed"`). Author the
  search hex from `get_bytes(stringAddr, N)`, reading past the visible end to the
  `00`.
- **Hold a candidate? Read its PROLOGUE before chasing its callers.** A ctor is
  unmistakable from its first ~15 instructions: it `call`s the base-class ctor,
  then `lea rax, <vtable>; mov [this], rax` (the vtable store), then stores its
  args into members. Don't burn calls walking the call graph around a candidate
  you can identify directly.
- **Runtime-init HashedStrings dead-end the string->global walk.** When a class's
  ids are `HashedString` globals copied from the registry at runtime (not static
  pointers to the `.rdata` id), the id string is referenced only by registration
  (as stack temporaries) and never reaches the target - prefer the vtable scan.
  (Registration does bake the **FNV id hashes** inline (`mov rax, <hash>`), which
  are invariant anchors if a chain leads through it.)
- **An anchor string can be DELETED between builds.** A `validate*` label vanished
  entirely one bump while its sibling label survived, and the function was alive
  and well. Before concluding "the function is gone", sweep `.rdata`/`.data` for
  near-miss ASCII, then switch tactic (vtable slot, constant intersection).
  Rewrite the recipe when this happens - it is now permanently wrong.
- **RVA-delta banding is worthless when the linker reshuffles link order.** One
  bump moved seven correct answers by -18 MB to +5.8 MB, with functions of the
  same class moving in opposite directions. Deltas and address proximity are not
  evidence; body/behaviour is the only arbiter.
- **A "perfect size match" is a trap.** A candidate sized right next to the old
  target turned out to be the old target's *caller*, proven by its string set.
  Never accept a size-based pick without a string/constant check.
- **A string mapping to >1 function** must be disambiguated by **name-independent**
  tells only - never by a named callee (it's `sub_` in the unnamed DB):
  unreferenced copies drop out (only matches with a code xref are candidates); a
  caller/callee pair orders them (target is the callee, often smaller); control-
  flow shape / size / basic-block count (a loop vs straight-line); vtable presence
  (a virtual override has a data xref from a vtable, a static helper doesn't).
  Tag the relocate block `- N xrefs` so the next person knows it isn't a clean 1:1.

## Authoring a locate recipe

The comment block above an entry's `pattern =` is **navigation documentation,
derived purely from the old named DB** (not a measurement - you don't need the new
binary to write it). It is **pure numbered steps - `# 1.`, `# 2.`, `# 3.` ... and
nothing else**: no header, no provenance line, no prose paragraphs. The reader has
*this* skill for the general tactics, so keep each step terse and
function-specific. Trace a stable anchor through the call chain in the reference
and write the steps so an agent can replay them in the unnamed target:

- **Step 1 names the anchor** - the exact string bytes + `= "..."` gloss (with its
  `00` terminator) and its uniqueness, or the magic constant / vtable anchor.
- For each **caller -> target hop**, give **local landmarks at the call site**
  (the constant stored right after, the `if`/`cmp` it sits in, the arg immediate),
  not just "it calls the target".
- **Make a foreseen drift its own step** ("X is inlined into Y in newer builds;
  the chain shortens - xref the anchor one level"; "skip the string route - the
  ids are runtime-init HashedStrings; use the vtable scan").
- **Close with the structural-similarity check** vs the old named DB (size, args,
  return shape, own callees) - the concrete confirmation, and how you tell the
  right `call` from a sibling call.

Three buckets decide the approach: **unique-string** (write the string block) /
**generic-only** (has strings, none usable) / **none** - the latter two fall back
to a call-site in a named caller, the packet vtable, or the vtable slot map.

## When you can't re-find it this pass

If an entry is confirmed wrong but you can't relocate it yet, do not leave a
silently-mis-resolving pattern. The `update-signatures` skill covers neutralizing
it with a non-matching sentinel; keep the numbered recipe above it - that block
is the recipe for the eventual re-find.

## Related skills

- **update-signatures** - once located + confirmed, cut the byte `pattern` (direct
  prologue / call-site / data global) and verify with `dump_symbols`. The
  survival/uniqueness rules for pattern *selection* live there.
- **build-ida-db** - produce the `.i64` databases this skill reads.
- **bump-bds** - the broader version-bump workflow that drives this.

Record durable, tooling-level lessons (new anchor idioms, idalib quirks) back into
this skill; keep per-version specifics in commit messages / CHANGELOG.
