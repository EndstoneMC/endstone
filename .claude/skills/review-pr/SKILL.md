---
name: review-pr
description: Review one open Endstone pull request that adds an API, event, enum, or feature - check it against the add-api rules, Paper/Bukkit, the real BDS headers, and IDA, then write a structured report. Use when asked to "review PR #N", "check the open PRs", "triage the event PRs", or before merging a community contribution that touches include/endstone, src/bedrock, src/endstone, or the Python bindings.
---

# Review an API / event / feature PR

One invocation reviews **one** PR. Fan out one agent per PR; a synthesis pass
collates the reports afterwards.

The bar is the **`add-api`** skill: it is the spec a feature PR is measured
against, and you MUST read it before reviewing if it is not already in context.
This skill states the review-specific obligations `add-api` does not cover.

## Normative language

Requirement keywords are used in the RFC 2119 sense and map directly onto
report severity:

| Keyword | Meaning | A violation is |
| --- | --- | --- |
| **MUST** / **MUST NOT** | absolute requirement | **Blocking** |
| **SHOULD** / **SHOULD NOT** | strong default; deviation needs a stated, accepted reason | **Should fix** |
| **MAY** | genuinely optional | at most a **Nit** |

Two kinds of clause appear:

- "**The PR MUST/SHOULD** …" - an obligation on the contribution. Breach is a
  finding against the PR.
- "**You MUST/SHOULD** …" - an obligation on you, the reviewer. Breach means
  the review is not finished.

Clauses are numbered (§3.4, §5.2) so findings can cite them and so the
synthesis pass can compare reports. Every finding in the report MUST cite the
clause it breaches.

Where a clause records a **deviation the maintainer has already approved**, it
says so explicitly. You MUST NOT raise an approved deviation as a finding.

## Scope of this review

**0.1** You MUST NOT push, comment on GitHub, or modify the PR branch unless
the user explicitly passes `--fix` or `--comment`. The default is a report.

**0.2** You MUST NOT check out, stage, stash, or edit anything tracked in the
primary checkout (`$REPO`, §1.1). Reviews run in parallel and that checkout is
shared. `git fetch` and `git worktree add` are the only commands you MAY run
against it. The single write you MAY make there is your report under the
gitignored `$REPO/.claude/notes/` (§12.1).

**0.3** Each PR MUST be reviewed in its own git worktree (§1).

## 1. Set up

**1.1** You MUST create a dedicated worktree and do all reading - and any
`--fix` work - inside it:

```shell
PR=<number>
REPO=$(git rev-parse --show-toplevel)      # never hardcode the checkout path
WT=$(mktemp -d -t "endstone-pr$PR-XXXX")/wt
git -C "$REPO" fetch origin "pull/$PR/head:pr-$PR" "develop"
git -C "$REPO" worktree add "$WT" "pr-$PR"
```

Everything after this runs with `-C "$WT"`.

```shell
gh pr view $PR --repo EndstoneMC/endstone --json title,body,isDraft,baseRefName,headRefName,commits,files
gh pr diff $PR --repo EndstoneMC/endstone > "$WT/../pr$PR.diff"
git -C "$WT" diff origin/develop...HEAD --stat
```

**1.2** You SHOULD read the PR in the worktree rather than from the diff alone.
The worktree shows the *merged* result: full context around each hunk, whether
an inserted binding really lands after its base class, what a neighbouring file
looks like at this base.

**1.3** You MUST establish the base state before reviewing content:
`git -C "$WT" log --oneline origin/develop..HEAD` exposes drive-by commits
(§2.2); `git -C "$WT" merge-base --is-ancestor origin/develop HEAD` says
whether the base is current or stale. Report it (§12).

**1.4** You MUST remove the worktree once the report is written:

```shell
git -C "$REPO" worktree remove --force "$WT"
git -C "$REPO" branch -D "pr-$PR"
```

**1.5** You MUST NOT build in the worktree unless the user asks. A build tree
per PR is expensive and no clause in this skill requires one (§11.3).

## 2. Self-containment

Check this first - it is the most common failure.

**2.1** The PR MUST be mergeable on its own, in any order, against `develop`.
It MUST NOT depend on, duplicate, or re-land another open PR's work. You MUST
test this rather than infer it - `git -C "$REPO" merge-tree origin/develop
"pr-$PR"` reports the real conflicts, so a stale base is only a finding if it
actually conflicts. You MUST then list its files against every other open PR's
files; any new file appearing in two PRs is a stacked branch presented as
independent. *(Seen in the wild: three PRs each shipping the same
`brstd/move_only_function.h`, `actor_interaction.{h,cpp}`, `shear.{h,cpp}` and
`player_shear_actor_event.h` - whichever merges first turns the other two into
conflict resolution.)* The remedy to demand: extract the shared groundwork into
its own prerequisite PR and rebase onto it, or declare the stack and set the
correct `base` on GitHub.

A clean `merge-tree` does **not** prove two PRs compose. You MUST also check
for collisions the merge cannot see, because git resolves them silently and the
damage lands at build time or later:

- **Duplicate definitions.** Two PRs each *defining* the same free function,
  global, or out-of-line member at namespace scope in different files merge
  without a conflict and then fail to link (ODR / duplicate symbol). Identical
  *declarations* in a shared header merge fine; definitions do not.
- **Duplicate registrations.** Two PRs binding the same Python name, adding the
  same `[[signatures]]` entry, or registering the same hook merge cleanly and
  then collide at runtime or on regeneration.
- **Semantic overlap in one function.** Two PRs adding non-adjacent branches to
  the same `switch` or handler merge textually while producing an ordering or
  double-fire nobody reviewed.
- **A dependency only a sibling PR supplies.** A PR that hooks a symbol whose
  `[[signatures]]` entry lives in *another* open PR merges cleanly, builds, and
  then aborts at startup when the hook cannot resolve. You MUST check that every
  symbol the PR hooks is declared by the PR itself or already on `develop`.
  *Confirmed: a PR hooks `CauldronBlock::use` while only a sibling PR adds its
  entry - merged first, the server does not start.*

Report each of these as a §2.1 finding naming the *other* PR and the
consequence, and say which merge order is safe.

**2.2** The PR MUST NOT carry drive-by refactors. Reordering, reformatting and
renaming MUST be separate PRs. A feature PR MAY only *insert* into shared
registration lists; it MUST NOT move existing entries.

*Confirmed, and worse than it sounds: a `refactor(event): reorder ...
declarations` commit rode along in nine branches, re-created independently in
each so the SHAs all differ and git cannot dedupe them - every pair genuinely
conflicts. Its stated purpose (registering derived events after their bases)
was already true on develop, so it achieved nothing. And in at least one branch
it inverted `PlayerMoveEvent`/`PlayerJumpEvent` and
`PlayerTeleportEvent`/`PlayerPortalEvent`, which breaks `import endstone`
outright (§9.4).*

A reordering commit is therefore never a harmless nit. You MUST diff the
registration order against `develop` and confirm no base moved after a derived
class, rather than treating movement as cosmetic.

**2.3** Insertions MUST follow the established order: alphabetical for includes
in `include/endstone/endstone.hpp` and for names in `endstone/event/__init__.py`'s
`__all__`; in `src/endstone/python/event.cpp` a base class MUST be registered
before its derived classes, and everything else MUST stay put.

**2.4** The PR MUST NOT hand-edit generated files. `src/bedrock/symbols/{linux,windows}.h`
carry a `DO NOT EDIT` banner: a new symbol MUST be added to
`scripts/configs/{linux,windows}.toml` and the table regenerated. You MUST flag
any PR touching `symbols/*.h` without a matching `configs/*.toml` change. The
regenerated array-size line will still conflict between PRs; that is expected
and MUST be resolved by re-running `dump_symbols.py` after merge, never by
hand-merging offsets.

**2.5** The PR's scope MUST match its title. `feat(event): add XEvent` MUST NOT
also change `BlockFace`, add a `Player` virtual, or alter the firing conditions
of an existing event. A behaviour change to an existing event MUST be its own
PR with its own justification.

## 3. Faithfulness to Paper / Bukkit

**3.1** You MUST verify Bukkit-derived naming and semantics against the **Paper
Javadocs**, fetched - not from memory.

**3.2** Endstone says `Actor` wherever Bukkit says `Entity`. This is the one
systematic, **approved** divergence and it applies throughout: class names
(`Entity` -> `Actor`, `LivingEntity` -> `Mob`), event names
(`EntityToggleGlideEvent` -> `ActorToggleGlideEvent`), header folder
(`org.bukkit.event.entity.*` -> `include/endstone/event/actor/*`), base classes
(`EntityEvent` -> `ActorEvent`), accessors (`getEntity()` -> `getActor()`) and
the Python module. You MUST compare against the Bukkit original with that
substitution applied, and you MUST flag both directions of error: a port that
keeps `Entity` in a name, and a port that drops a Bukkit *entity* event into
the player package because it renamed the class instead of translating the
package.

**3.3** The PR MUST NOT introduce a new name for something Bukkit already
names. You MUST check for an existing Bukkit equivalent under a different name
or package before accepting an invention. `EntityToggleGlideEvent` and
`EntityToggleSwimEvent` are entity events, so `ActorToggleGlideEvent` /
`ActorToggleSwimEvent` under `event/actor/` - not `PlayerToggle*` under
`event/player/`. Crafting is `org.bukkit.event.inventory.CraftItemEvent`, not a
player-package event.

A matching *name* is not a matching *port*. You MUST check the upstream
hierarchy too, and port the base along with the subclass or say why not.
*Confirmed: Paper's `PlayerArmSwingEvent` is real, but it extends
`org.bukkit.event.player.PlayerAnimationEvent` (Cancellable,
`getAnimationType()`) and only adds `getHand()`. A PR shipped the subclass
deriving straight from `PlayerEvent`, so the name looked right while the
hierarchy, the cancellability and both accessors were missing.*

**3.4** Where Bukkit has no equivalent, you MUST say so explicitly in the
report, and the new shape SHOULD follow Bukkit style: an actor-vs-actor event
derives from `ActorEvent` and exposes `getActor()` plus a named counterpart,
not a `std::vector` of two.

**3.5** The header path MUST mirror the Bukkit package, and the Python module
SHOULD follow the same grouping.

**3.6** Accessors SHOULD match Paper's. `PlayerRiptideEvent` exposes the
trident (`getItem()`), not an invented `isRiptiding()`. Where the Bedrock
signal supports only a subset, the PR SHOULD port that subset rather than
invent a different surface.

**3.7** Mutability SHOULD match Paper's. `PlayerExpChangeEvent` has
`getAmount()` *and* `setAmount()`; a read-only port is not the same API.

**3.8 (approved deviation)** An event MAY ship non-cancellable where Paper's is
`Cancellable`. Bedrock does not always offer a cancel point at the moment
Endstone can reach, a read-only event is still useful, and adding
`Cancellable<...>` later is purely additive. You MUST NOT raise this as a
finding. You SHOULD note the gap - that Paper cancels this and Endstone does
not yet, and whether a cancel point exists in BDS - in the Trigger line or
under Questions. The opposite error is blocking (§7).

## 4. Faithfulness to BDS (`src/bedrock`)

The contributor could not check this and you can. That asymmetry is the whole
job of this section. Maintainers have the BDS headers and the IDA databases
(§4.6); the community does not. Whatever a community PR puts in
`src/bedrock` came from something weaker - a public reverse-engineering project
(often stale by several BDS versions, and wrong in ways that look right), a
guess extrapolated from Java/Bukkit naming, a decompiler with no symbols, or
another Endstone file it pattern-matched. None of those are evidence.

**4.1** You MUST treat every declaration the PR adds or changes under
`src/bedrock` as unverified until you have personally checked it against our
sources. You MUST check them all; spot-checking does not satisfy this clause.

**4.2** You MUST NOT accept a reconstruction because it is plausible, because
it compiles, because its `static_assert` passes, or because it resembles the
file next to it. A wrong class, a wrong vtable slot, a wrong ref-vs-pointer or
an extra virtual all compile, link and pass every size assertion, then silently
return wrong data or dispatch the wrong function at runtime - surfacing months
later as a one-platform bug report with no stack trace pointing here.

**4.3** You SHOULD spend the review budget here first. A large `src/bedrock`
diff outranks the public header, the bindings and the hygiene pass. A PR that
rewrites or extends existing reconstructions warrants more scrutiny than one
adding a new file: you MUST re-verify the lines it *touched*, not only the
lines it *added*, and confirm that any existing member it moved, retyped or
reordered is still correct afterwards.

**4.4** Verified is not the same as verifiable. Where our sources cannot settle
a declaration, you MUST record it as unverified in the report and state that it
MUST NOT merge in that state. You MUST NOT approve it on trust.

**4.5** You MUST verify with our sources and report without them. NDA header
text MUST NOT appear in a PR comment, a commit message, or any report leaving
the machine. Cite as "confirmed against the BDS headers", or by IDA address
plus the single decompiled line that settles the point.

**4.6** Sources, in this order. Both live beside the checkout as sibling
working directories - resolve them, do not hardcode a path:

```shell
SRC=$(dirname "$REPO")
HEADERS="$SRC/bedrock-headers"             # BDS headers (NDA, §4.5)
SYMBOLS="$SRC/bedrock-symbols"             # IDA databases, per platform/version
TARGET=$(grep -m1 '^version' "$REPO/scripts/configs/windows.toml")   # the build Endstone targets
ls "$SYMBOLS"/*/*/                          # what is actually built, before picking one
```

1. **The BDS headers** (`$HEADERS`) - `grep -rn` the symbol.
2. **IDA** via the `ida-pro` idalib MCP, for anything the headers cannot answer:
   a function *body*, a call path, a vtable slot, an inlined-away helper. You
   MUST pick the database by role, not by memorised version - the built set
   changes as BDS moves:
   - **mechanism** (what a function does, its call path): the `linux_server_x64`
     `*_symbols.debug.i64`. It carries symbol names and is far less inlined.
   - **offsets and layout** (member order, sizes, vtable slots): the
     `win32_server_x64` database whose version is closest to `TARGET`.

   You SHOULD open each once and batch lookups; the databases are multi-GB and
   are shared across parallel reviews, so call `idb_list` and reuse an existing
   session before calling `idb_open`. You MUST state in the report which
   database and which build settled each point (§4.8).

**4.7** You MUST NOT infer a function body from a header or from a name.
Decompile it.

**4.8** Per declaration, you MUST check all of the following and record in the
report which source settled each:

- **Right class.** A member on the wrong class still compiles and links.
  *Confirmed: BDS has `Actor::isSwimming() const` and `Player::isCrawling()`; a
  PR declared both on `Mob`.*
- **Right call path.** *`Actor::isSwimming` decompiles to
  `SynchedActorDataAccess::getActorFlag(ctx, 57)`, not `Actor::getStatusFlag` -
  both return the same bool today; only one is what BDS does.*
- **Right signature** - reference vs pointer, `const`-qualification, `class` vs
  `struct`, enum underlying type, member declaration order, virtual count and
  order.
- **Right BDS version.** Our headers and each IDA database are pinned to a
  build; `scripts/configs/*.toml` names the version Endstone targets. A struct
  correct in an older build and wrong in the target one is the classic
  public-RE failure. Where sources disagree across versions the target version
  wins, and you MUST state which build you verified in.
- **Existing reconstructions the PR leans on.** Where the feature reads a
  member or calls a virtual already in the tree, you MUST confirm that one too.
  Prior misreconstructions are real here (`add-api` documents
  `getDefaultBiome` / `getDefaultBiomeId`) and a PR built on one inherits its
  bug.

**4.9** Reconstructions MUST be minimal: only the members the feature needs, in
their correct place in the layout. A whole dumped struct MUST NOT be pasted in,
and BDS methods nothing calls MUST NOT be carried.

**4.10** The `// Endstone` marker MUST appear on everything invented or
changed, and MUST NOT appear on a faithful re-declaration. The dangerous
inverse is an invented mechanism sitting unmarked inside a bedrock namespace,
reading as real BDS API - *confirmed: a `thread_local SkipPushBypassScope`
declared inside `namespace PushableByEntityUtility`*. Invented plumbing MUST
live in `src/endstone/runtime/bedrock_hooks/`, not in `src/bedrock`.

**4.11** A re-implemented BDS function MUST match BDS's algorithm and call
path, not merely its result. It SHOULD NOT hand-roll a substitute for a real
container call - *a linear scan over `recipes_by_net_id_` comparing `raw_id`
where the map has `find()` is both a divergence and an O(n)-per-craft cost*.

**4.12** Layout MUST be pinned with `BEDROCK_STATIC_ASSERT_SIZE` /
`static_assert`, with no explicit offsets and no manual padding, and
`NO_UNIQUE_ADDRESS` on empty members.

You MUST NOT treat a passing assertion as evidence. It is self-referential: the
number is written by the same author as the members, so it asserts what the PR
believes, not what BDS has. It proves neither arrangement (two swapped members,
or one wrong type padded back to the same total, assert clean) nor magnitude (a
reconstruction that omits half the class asserts its own truncated size
happily). *Confirmed: a PR asserted 24 bytes for a type BDS lays out at 240,
and 24 for another that is 80 - both asserts passed.*

You MUST therefore source every asserted number from the headers or IDA
yourself, on both ABIs, and record it in the report (§12.1). An assert whose
value you did not independently confirm is unverified under §4.4.

**4.13** A reconstructed packet SHOULD declare only its layout - the payload
member, `serialization_mode`, and the size assert. It SHOULD NOT override
`getId`, `getName`, `write` or `_read`. Endstone never constructs these by
value; it `static_cast`s a packet BDS already built, so the overrides are dead
weight that emit a vtable in our TU and drag in `<stdexcept>` for bodies that
only throw. 29 of the 37 packet headers already do it this way - `EmotePacket`
is the model, and it is consumed exactly the same way from
`EndstonePlayer::handlePacket`. The exception is a packet Endstone genuinely
constructs and sends, which needs whatever BDS's own type provides.

Style note that comes up in the same files: prefer an inline expression, or a
plain `const auto` local named for the value, over a one-use helper lambda or
helper function. Match what the surrounding function already does.

## 5. New BDS symbols

A `[[signatures]]` entry in `scripts/configs/{windows,linux}.toml` asks the
project to take on permanent, recurring cost. Each entry must be re-found on
every BDS bump, on both platforms; when its pattern goes stale the release is
blocked until someone re-locates the function by hand (`update-signatures`,
then `locate-function`). Worse, a stale pattern does not always fail loudly -
`find_signature` takes the lowest `.text` hit, so an over-generic pattern
silently binds the wrong function. One symbol is a maintenance subscription,
not a line of config.

**5.1** The PR MUST justify every new symbol. You MUST work the ladder below
and stop at the first rung that works, and the report MUST state which rung the
PR landed on and why the rungs above it were rejected. "It was easier" is not a
reason.

1. **A BDS gameplay event already carries the signal.** Grep
   `src/bedrock/world/events/*_events.h`. Zero new symbols, and it fires on
   every path BDS itself considers the action. You MUST try this first.
2. **An already-resolved anchor reaches it.** A member read costs nothing -
   reconstruct the struct. If the target is called from a function already in
   the table, hook the caller.

   **A vtable slot does NOT count as free, and SHOULD NOT be preferred over a
   signature.** `vhook::create<Ordinal>` needs a per-platform ordinal, and a
   wrong or drifted one fails **silently** - it dispatches the wrong function
   with no error. A byte pattern that stops matching fails **loudly**, at
   `dump_symbols.py` time, and on Windows `--pdb` resolves the name directly.
   So prefer a `[[signatures]]` entry with a proper §5.4 recipe over a vtable
   hook, even though the ladder would otherwise call the vtable cheaper -
   maintainability wins over symbol count here. *Confirmed: PR #489 shipped
   Windows ordinal 109, which is `Actor::openContainerComponent`, where the
   target `getInteraction` is 117; it compiled, linked and asserted clean.*
   Reserve `vhook` for cases where no stable pattern can be cut, and say so.
3. **Re-implement it in `src/bedrock`.** A non-virtual function whose body you
   can read MAY be reconstructed faithfully (§4.11) instead of resolved,
   trading a maintenance subscription for a one-time correctness risk - usually
   the better trade for small, stable functions.
4. **One wide chokepoint instead of N narrow ones.** Where a feature needs
   three symbols, you SHOULD look for the single function upstream of all
   three.
5. **Add the symbol**, having written down why 1-4 do not work.

**5.2** A symbol MUST NOT be added for a read-only accessor, for convenience
over an existing reconstruction, or for something an existing hook already
sees.

**5.3** The `name` MUST be the target's real mangled symbol, confirmed against
the headers or the binary - never a descriptive name the author made up for it.
`dump_symbols.py --pdb` resolves Windows entries **by name from the PDB** and
only falls back to the byte-pattern scan; a fabricated name silently forfeits
that path forever, and it also makes the entry unauditable, since no later
reviewer can grep for something BDS never called. Where the target has no name
of its own - a lambda body, a compiler-generated thunk - the PR MUST say so and
mark it, rather than dressing it as a member function.

*Confirmed: an entry named `executeBucketEntityUse` is really
`ItemStack::useOn` - an exported name on Windows, so the invention traded a
name lookup for a permanent pattern dependency. Another, `executeInteraction`,
is an unnamed lambda's `_Do_call`.*

**5.4** Every `[[signatures]]` entry MUST carry a numbered locate recipe in the
comment block directly above it (house style: the existing entries in
`scripts/configs/windows.toml`). The recipe MUST:

- give **numbered steps** a reader with a *fresh, unnamed, no-PDB* database can
  follow from nothing;
- anchor each step on something **version-stable** - a unique string literal
  and its xref, a vtable slot counted from a named virtual, a call from an
  already-located function, a distinctive immediate. It MUST NOT anchor on an
  absolute RVA/VA, "the Nth function", a file offset, or a raw prologue alone;
  a prologue pattern with no recipe above it is unfindable the moment it
  breaks;
- **confirm identity independently** in a final step - the body, its callees,
  marker strings in its call closure, or a cross-check against the old named
  database. Shape, size and alignment all silently agree with the wrong
  function;
- state how **uniqueness** was verified, and warn if the target sits in a
  family of byte-identical siblings;
- exist in **both** `windows.toml` and `linux.toml`;
- have been cut against the BDS version in the file's `version =` line. A
  comment citing a different database version than the table targets is a
  finding: the pattern was never validated against the version that ships.

## 6. Interception point and trigger

**6.1** The PR MUST catch the signal at the correct layer. This is a
correctness requirement, not a style preference. In descending preference:

1. **BDS gameplay handler** (`script_*_gameplay_handler.cpp`) - required
   whenever BDS dispatches its own event. `src/bedrock/world/events/*_events.h`
   is the catalogue and you MUST grep it before accepting anything else. Where
   `PlayerAddExpEvent` or `PlayerSwingStartEvent` exists, a packet hook for the
   same signal is the wrong layer: it misses every non-packet path (commands,
   dispensers, redstone, scripts) and fires before BDS has ruled the action
   legal.
2. **A function hook** on the real BDS call - correct where there is no
   gameplay event but there is a single chokepoint.
3. **`EndstonePlayer::handlePacket`** - last resort, for client intent with no
   server-side action behind it.

**6.2** The PR MUST NOT touch raw packet bytes, in **either** direction. Both
halves duplicate BDS's own wire handling, break silently on a protocol bump,
and cannot be unit-tested.

- **Inbound:** MUST NOT hand-parse payload bytes. *A hand-written varint reader
  for `ItemStackRequest` in `batched_network_peer.cpp` duplicated BDS's own
  deserialization; its `10`/`11` constants were cereal variant indices, not the
  `ItemStackRequestActionType` values they looked like, and would have shifted
  silently on any change to the variant list.* BDS's
  `ItemStackRequestAction*` types exist and MUST be used.
- **Outbound:** MUST NOT compose a packet by writing fields into a
  `BinaryStream`, and MUST NOT send raw bytes. Reconstruct the packet's layout
  under `src/bedrock/network/packet/` (§4.13), build it through the factory,
  assign the payload, and send it:

  ```cpp
  auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::Text);
  auto &pk = static_cast<TextPacket &>(*packet);
  pk.payload = { /* typed fields */ };
  getHandle().sendNetworkPacket(*packet);
  ```

  Hand-writing the wire format hardcodes field order, widths and varint
  encoding that BDS owns and revises. A reconstructed payload gets those from
  the compiler and fails at build time when the layout moves, instead of
  emitting a malformed packet at runtime.

  **The one sanctioned exception** is a payload whose types would drag in a
  whole subsystem for no gain. `EndstonePlayer::spawnParticle` hand-writes
  SpawnParticleEffect deliberately, because reconstructing it faithfully means
  reconstructing Molang. That is accepted and MUST NOT be raised as a finding.
  A PR MAY invoke the same exception, but it MUST name the subsystem it is
  avoiding and why the trade is worth it - "it was easier" is not that. Absent
  such a justification, the rule stands.

**6.3** The trigger SHOULD match Paper's, not merely the name. Firing semantics
are part of the contract plugin authors rely on. You MUST determine where Paper
actually calls the event - the source, not the Javadoc. Almost none fire on
packet arrival: the dominant pattern is `CraftEventFactory.callXEvent(...)`
invoked from the `Item`/`Block`/`Mob`/`ServerPlayer` method performing the
action, after Vanilla's checks and before it commits the mutation. Where Paper
does hook `ServerGamePacketListenerImpl`, it is because the signal is pure
client intent. Which of the two Paper does decides the correct rung of §6.1,
and the report MUST say which.

**6.4** You MUST then locate the Bedrock counterpart of *that routine*, not the
packet preceding it. The question to answer: at the moment Paper fires, what
has already been decided and what has not yet happened?

**6.5** Each of the following is a finding in its own right - firing **too
early**, typically a packet hook standing in for a gameplay call:

- fires for actions the server then rejects (out of reach, wrong game mode,
  spectator, cooldown, ability disabled, item not really held, block not really
  breakable, target already dead);
- fires more than once per logical action, or every tick a held input flag
  stays set rather than on the transition;
- carries state read *before* the action, so `getItem()` / `getBlock()`
  describe the pre-check world;
- cancellation drops a packet the client already applied locally, desyncing it.

**6.6** Firing **too late** is equally a finding: the mutation has happened, so
a `Cancellable` event cannot honour its contract and a `set*` mutator writes
where nothing reads. A cancellable event MUST map to a *Before*-equivalent
point; BDS's own `...BeforeEvent`/`...AfterEvent` split is the same
distinction.

**6.7** The report MUST state what the chosen site over-fires and under-fires
relative to Paper. Paper's events fire on every causing path - a dispenser
filling a bucket, a command setting a spawn point, a script or add-on, another
plugin calling the API - while a packet hook sees only the client path.

**6.8** Where the site is wrong, you SHOULD locate the right one rather than
only naming the fault. This is also the search that answers §5.1:

1. **Name the moment** from Paper: "fires after X is validated, before Y is
   applied."
2. **Find the Bedrock routine that applies Y**, anchoring on something it
   provably touches - the sound or particle it plays, the stat it increments,
   the loot or spawn call it makes, the `setBlock`/`remove`/`add` it performs,
   a unique format string. Search strings, take the xref.
3. **Walk callers** (`xrefs_to`) upward until every path funnels through one
   function. That chokepoint - not the leaf, not the packet - is the candidate.
   Several surviving callers mean the event belongs one level higher.
4. **Read above and below the candidate.** The right site has BDS's validation
   above it and the mutation below it. Checks below means too early; the write
   above means too late.
5. **Check the cheap sites first** - is the candidate virtual? does it already
   dispatch a BDS gameplay event? is it already called from a function Endstone
   hooks? Only a candidate surviving all three justifies a new symbol.
6. **Confirm coverage** - walk callers once more and confirm the non-player
   paths reach it.

Report the outcome either way, with the BDS function, its address or mangled
name, and its cost. A better site identified but not implemented is still the
most useful thing in the review.

**6.9** An event on a hot path MUST be skipped when no plugin is listening.
Guard it with `EndstoneServer::getEndstonePluginManager()`'s
`isEventRegistered<EventType>()`, placed early enough to skip the work that
builds the event, not just the dispatch - the handle lookups, the `Block`
allocations, the address formatting. Treat a site as hot when it runs per tick,
per packet, per actor pair or per block update. Paper marks the same sites with
`getHandlerList().getRegisteredListeners().length != 0`; when the counterpart
event carries that guard upstream, the Endstone port MUST carry it too.

**6.10** A guard MUST NOT change behaviour when a listener is present, and MUST
NOT skip work the server needs regardless. Where a hook does something besides
firing the event - patching an outbound packet, keeping a cached value current -
the guard belongs after that work, or the condition must exclude it.

## 7. The cancel contract

An event need not be cancellable (§3.8). But where the PR *declares* it
`Cancellable`:

**7.1** Cancelling MUST actually prevent the action, on every path that fires
the event. A half-working cancel is worse than none: plugin authors write
against it and the failure is silent and situational.

**7.2** You MUST trace what happens on `e.isCancelled()`. Returning `false`
from `handlePacket` drops the packet - the report MUST say whether that leaves
the client desynced and whether the PR corrects it back.

**7.3** Corrupting data to force a failure MUST NOT be accepted as
cancellation. *Overwriting a recipe net-id with `0xFF...0x7F` so BDS rejects it
makes the server fail the action rather than decline it, and leaves the
client's inventory inconsistent.*

**7.4** A documented hole is still a hole. *A doc comment saying cancellation
"prevents the spawn change on supported native paths" while `/spawnpoint` "may
still report success" describes a contract that does not hold.* The PR MUST
either cover the path or drop `Cancellable` and ship read-only - the latter is
a legitimate landing state (§3.8), not a rejection.

**7.5** A PR that fires a `Cancellable` event and ignores the result MUST
either drop the base class or wire the cancel up.

## 8. Public API and ABI

`include/endstone/` is header-only; C++ plugins compile against it and load as
shared libraries.

**8.1** A new pure virtual on an existing polymorphic class breaks ABI for
every already-built plugin, and inserting it mid-class shifts every later
vtable slot. A new virtual MUST be appended at the end of the class, and the
CHANGELOG entry MUST be prefixed `**BREAKING**:`. A mid-class virtual insertion
is blocking.

**8.2** New enums MUST be `CamelCase` in C++ and `UPPER_CASE` in Python via
`py::native_enum(...).export_values().finalize()`.

**8.3** Doc comments MUST follow the **`cpp-docs`** skill (no `@brief`,
Paper-derived wording); Python docstrings MUST follow **`python-doc`**.

**8.4** You SHOULD check whether the event carries a `Reason`/`Cause` enum that
Paper has and the PR dropped - *Paper's `PlayerSetSpawnEvent` has `RESET` and a
notify-player flag*.

## 9. Bindings and stubs

**9.1** Every new public class and property MUST have a `py::class_` /
`def_property_readonly` in `src/endstone/python/*.cpp`, `snake_case`, mirroring
the C++ getter.

**9.2** A `Cancellable<...>` event MUST list `ICancellable` in its `py::class_`
bases, or `is_cancelled` silently disappears from Python.

**9.3** `endstone/**/__init__.pyi` and `__all__` MUST be regenerated
(`scripts/stubgen.py`), never hand-written, and the diff MUST contain nothing
but the addition.

**9.4** A binding registered before its base class is a runtime import error;
you MUST check ordering (§2.3).

## 10. Repo hygiene

**10.1** Every new file MUST carry the full 13-line Apache header. Truncated
headers stopping after the license URL are a recurring defect - you SHOULD diff
the header against a sibling file.

**10.2** The trailing semicolon on `ENDSTONE_EVENT(X)` is **not** a finding in
either direction. The tree is split (51 headers with, 8 without) so there is no
convention to breach; raising it just adds noise to every report. You MAY note
it once, as a house-style question for the maintainer to settle, and MUST NOT
file it as Blocking or Should-fix.

**10.3** The PR MUST add a user-facing CHANGELOG bullet under `## [Unreleased]`
in the correct Keep-a-Changelog group. Internal implementation notes
(reconstruction, layout) MUST NOT appear there.

**10.4** The PR MUST NOT add comments. The repo has a no-comments policy;
explanatory comments, rationale narration and "LLM notes" are all findings.

**10.5** Code MUST be clang-format clean at the 120-column limit, with
`lower_case_` privates and `camelBack` methods.

**10.6** Commits MUST use `type(scope): summary` and MUST NOT carry a
`Co-Authored-By` line.

**10.7** Draft PRs SHOULD still be reviewed, and the report MUST lead with the
fact that they are drafts.

**10.8** The PR MUST NOT declare free helper functions in a core header. This is
cheap to check and easy to miss, because each one looks harmless on its own:
grep `src/endstone/core/**.h` for declarations at namespace scope. Before this
rule there were none, so any hit is the PR's.

Two shapes cover almost every case:

- **Converting between a bedrock type and its Endstone wrapper** belongs on the
  wrapper as a `static fromMinecraft(...)` / `toMinecraft(...)`. That is what
  `EndstoneItemStack`, `EndstoneSkin`, `EndstoneGameMode` and
  `EndstoneAttributeInstance` already do, and it is what CraftBukkit does from
  the other side - it patches `toBukkitRecipe(NamespacedKey)` onto the NMS
  `Recipe` rather than adding a loose converter.
- **A function used once** belongs inline at its call site, or in an anonymous
  namespace in the `.cpp` if it is genuinely local. Hoisting it into a header
  widens the API for no caller.

*Confirmed: a recipe PR grew `makeRecipe`, `makeRecipes`, `findRecipe` and
`makeIngredient` as free functions in two core headers. `makeRecipes` and
`makeIngredient` had one call site each, `findRecipe` two in the same function.
They collapsed into one `EndstoneRecipeData::fromMinecraft` pair plus an inlined
loop, and the two `.cpp` files they lived in disappeared.*

This is the review-side check for the **Helpers** rule in `add-api`; §4.13
carries the same point for one-use lambdas inside a function.

## 11. Evidence discipline

**11.1** Every finding MUST cite both sides: where it is - `path/to/file.h:42`,
a line in the PR - and why it is wrong: a Paper Javadoc URL, a path under
`$HEADERS`, an IDA address plus the decompiled line, or a clause of `add-api`. It MUST also cite the clause of this skill it breaches.

**11.2** A claim you cannot source MUST go under Questions, not Findings. You
SHOULD NOT pad the report - three sourced blockers beat twenty stylistic
guesses.

**11.3** Where a claim rests on a build succeeding or a value returned at
runtime, you MUST say so. You cannot build the PR and you cannot run a server
(see the `/test` note in `add-api`).

## 12. Report

**12.1** You MUST write the report to `$REPO/.claude/notes/review-pr-<N>.md`
and summarise the verdict in your reply. Resolve `$REPO` (§1.1) rather than
writing a path relative to the worktree - §1.4 deletes the worktree, taking any
report inside it. `.claude/notes/` is gitignored, so writing there does not
breach §0.2. One file per PR, named for the PR, so a synthesis pass can collate
the set.

```markdown
# PR #<N> - <title> (@<author>)

**Verdict:** request changes | approve with nits | approve
**Scope:** <one line - what it adds, which layers it touches>
**Base:** current develop | stale (<detail>) | stacked on #<M>
**New symbols:** none | <N> (<names>) - ladder rung <n>, <why 1-4 were rejected>
**Trigger:** <where the PR fires it> vs Paper's <where Paper fires it> - match |
over-fires on <...> | under-fires on <...>; better site: <BDS fn, or "none found">

## Blocking (MUST)
- **<one-line claim>** - `file:line`, §<clause>. <why, with source>. Fix: <what to do>.

## Should fix (SHOULD)
- ...

## Nits (MAY)
- ...

## Bedrock verification
<one row per declaration the PR adds or changes under src/bedrock>
- `Type::member` - ok | wrong (<what>) | unverified - headers | IDA <addr> | neither

## Questions
- <anything unverifiable without a build or a live server, plus every
  `src/bedrock` declaration our sources could not settle>

## Cross-PR
- <files this PR shares with other open PRs, and the merge-order consequence>
```

**12.2** Severity MUST follow the keyword of the clause breached, per the table
in *Normative language*. A MUST breach is Blocking even if it looks small; a
SHOULD breach is not Blocking even if it looks large.

**12.3** You MUST NOT report an approved deviation as a finding. Today that
means: `Actor` where Bukkit says `Entity` (§3.2), and an event shipped
read-only where Paper's is cancellable (§3.8). The latter SHOULD be noted in
the Trigger line or under Questions so it can be revisited when demand appears.
