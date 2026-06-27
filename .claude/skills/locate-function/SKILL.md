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

## Confirm the target (always)

Resolve the candidate's `VA = 0x140000000 + RVA` and diff it against the old
namesake. Size **may** differ (inlining); what should line up is **args / return
shape / own callees / distinctive constants / control-flow shape**. For a
call-site or string anchor, confirm the resolve **TARGET**, not just that a
pattern is unique: a unique pattern at the *wrong* call still yields a wrong
address. Compute `target = (E8_addr + 5) + sign_extend(rel32)` and check it equals
the function's entry VA (for a first-match pattern, confirm the *lowest-address*
match resolves there). Every relocate recipe ends with "confirm vs the old named
DB."

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
