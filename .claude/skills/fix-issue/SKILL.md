---
name: fix-issue
description: Diagnose and fix an Endstone bug report - a server crash (crash report, access violation, abort), an event or API that behaves differently from vanilla BDS or from Bukkit/Paper, an event that never fires, fires when it should not, or leaves the client out of sync when cancelled, or a Python binding that raises or returns the wrong thing. Use when asked to "fix #N", "look at this crash report", "X doesn't work like vanilla", "event Y doesn't fire", "Z crashes the server", or to work through bugs from the GitHub issue tracker. Layout/vtable drift after a BDS bump is handed off to bump-bds and reconcile-headers.
---

# Fix an Endstone bug

Almost every fix in this repository's history falls into one of the classes below,
and the fix is always the same move: find out what BDS does (for game behaviour) or
what Paper/CraftBukkit does (for API semantics), then make Endstone do exactly that.
Classify the report, read the evidence, fix the root cause, then look for the same
mistake elsewhere.

Each class cites commits as worked examples. Read them with `git show <hash>`. They
live on `develop` or the current release branch, so fetch both. Commit bodies explain
the mechanism in detail, so read the body as well as the diff.

## Ground rules

- **DO read the evidence before naming a cause.** That means the crash report, the BDS
  function decompiled at the target version, and the Paper source fetched, not
  recalled. A theory about a crash that never opened the dump is a guess.
- **DO confirm BDS behaviour by decompiling.** A function's name, size, vtable slot or
  neighbourhood does not establish what it does. The `locate-function` skill covers
  finding a function without symbols. Put the evidence in the commit body: BDS
  version, platform, address and the instruction or call that settles it
  (`4be6a165f`, `c54592af5`).
- **Know your sources.** The Linux `bedrock_server` keeps its symbol names and is the
  easy place to read a mechanism. Use the Windows build of the target version for
  offsets. `build-ida-db` builds both databases. `bedrock-headers` is NDA-restricted.
  Without access to it, the binaries are the source of truth.
- **DO mirror Paper/CraftBukkit for API semantics,** including call order, copy versus
  live semantics and which subclasses an event covers. Endstone says `Actor` wherever
  Bukkit says `Entity`.
- **DO fix the root cause, not the symptom.** Do not wrap a use-after-free in
  try/catch, null-guard a wrong offset, or add a delay to dodge an ordering problem.
  For example, resending boss bars after a dimension change on a delay never worked
  (`6e953497c`). Answering the client's own Query is what fixed it (`bf772c28a`).
- **DO sweep for siblings.** One divergence in a file means you check every function
  in it. The #539 crash fix found three more divergences in the same file
  (`4be6a165f`). A rule copied into four call sites was wrong in all four
  (`78a7ea71e`).
- **DO NOT invent.** Never write a BDS function body you cannot see. Declare it and
  resolve it by symbol instead. Never return a translation key no client resolves
  (`440bb56a8` raises instead). Never fill in a constant you cannot read
  (`29bb533c1` leaves it undefined so any use fails to link).
- **DO NOT add speculative hardening** such as locks, retries or guards for callers
  that break the main-thread-only contract. If you spot an adjacent problem, report
  it and land it separately.
- **DO NOT add public API on a released minor.** A new virtual in `include/endstone/`
  breaks every compiled C++ plugin. If the fix cannot be made without one, say so and
  leave the fix on `develop`.

## 1. Triage

### Read a crash report

Crash reports are written to `crash_reports/crash-<datetime>-server.txt` in the
server folder.

- **Turn addresses into RVAs.** Frames show the preferred base plus the RVA, so for
  `bedrock_server.exe`, `RVA = addr - 0x140000000`. For `endstone_runtime.dll`, the
  base is `0x180000000`. The same crash gives the same addresses on every run.
- **Find the faulting frame.** Filtered modules keep their indices, so frame numbers
  have gaps. The crash handler's own frames come first. The first frame after the gap
  is the faulting PC. Every address is the return address minus 1, so expect the call
  instruction one byte later.
- **Use the exception code to pick a class:**

  | Code | Means | Go to |
  | --- | --- | --- |
  | `0xC0000005`, right after a BDS bump | wrong offset | §2 |
  | `0xC0000005` in a BDS frame, ticks after a disconnect, unload or reload | freed object | §3, §5, §11 |
  | `0x40000015` (`STATUS_FATAL_APP_EXIT`) | uncaught C++ throw; frame #0 is the throw site | §4 |
  | `0xC0000409`, or a fail-fast with no useful stack | a call landed in the wrong virtual (`a6e36ac3b`) | §2 |
  | `bad_variant_access` at shutdown | event variant drift | `bump-bds` |

- **Resolve `??` frames** against the PDB when that version ships one. Otherwise use
  the named Linux build or the `.pdata` function bounds.

### Classify a behaviour report

| Report | Section |
| --- | --- |
| An event never fires, fires on only some paths, or fires when it should not | §6 |
| Cancelling did nothing visible, the item vanished, or a side effect still happened | §7 |
| An API returns garbage | §2 |
| An API returns a plausible but wrong value | §5, §8 |
| Data is lost after a read-modify-write | §8 |
| Works on Windows but not Linux, or the reverse | §9 |
| Fails only from Python | §10 |
| Breaks only after `/reload` or a plugin disable | §11 |
| A client can crash or hang the server | §12 |

### Reproduce

- **Read the code at the reported release, not at `HEAD`.** The branch may already
  target a newer BDS, so compare offsets with `git show v<version>:src/bedrock/symbols/<platform>.h`.
- **Test the symptom, not the cause the reporter suggests.** Reports often blame
  "missing symbol bindings" or hooks without evidence.
- **Drive a headless server from its console before forming a theory.** Copy the
  server folder to a scratch directory and give it:
  - a fresh `level-type=FLAT` world;
  - `online-mode=false` and `allow-list=false`;
  - unused ports.

  Then pipe commands into `endstone -s <copy> -y --no-interactive`:
  - `tickingarea add` to load chunks;
  - `fill` or `setblock` to build the scene;
  - `summon`, or a powered dispenser for projectiles;
  - `testforblock`, or `testfor @e[...,x=,y=,z=,dx=,dy=,dz=]`, to read the result.

  Pipe the same script into the bare `bedrock_server` for a vanilla baseline.
- **Use a clean environment.** A dev venv loads every plugin installed in it as an
  entry point. For a clean run, and for Linux, `pip install endstone==<version>` in a
  `python` container. Point it at the extracted BDS zip with a matching `version.txt`
  so it skips the download.
- **Paths a player drives need a client.** Write the repro as an in-server test in the
  `endstone_test` plugin; find it with `git ls-files '*endstone_test/__init__.py'`.
  An opped player runs `/test`. `ctest` and `pytest` never load BDS. You cannot join a
  server or issue `/test`, so ask the user.
- **If it does not reproduce, change no code.** Ask the reporter for:
  - their OS and container;
  - the output of `/plugins` and any add-ons in the world;
  - the log around one occurrence;
  - whether plain BDS behaves the same.

## 2. `src/bedrock` disagrees with the binary (layout, vtable, enum drift)

This is the largest class by count. A reconstruction compiles whether or not it
matches BDS. A wrong offset reads a neighbouring field, and a missing virtual
dispatches to the wrong function.

- **Field offsets.** A member dropped from LevelChunk made `Block::getBiome` return
  an unrelated biome or crash (`950af2d8a`).
- **Enum values.** A new game rule shifted `SHOW_DEATH_MESSAGES` (`e66d9065e`).
  `ChunkState` lost two states, so `>= Loaded` never passed and `ChunkLoadEvent` went
  silent (`4c673704c`). `BlockProperty` was renumbered (`39bb2498a`). `Undead` lacked
  its `Mob` bit (`5a4d5dc5e`). Check every enumerator's value, not just the count.
- **Virtual count.** A missing virtual sent `/status` into the wrong function
  (`9dcc3272b`).
- **Objects Endstone builds and hands to BDS.** `BlockChangeContext` gained a bool,
  so BDS read past every Endstone-built instance (`39bb2498a`).
- **A hook bound to a function that no longer exists.** 1.26.51 split `_loadMapData`
  into `_deserializeMapData` and `_publishMapData`. The bump kept the old name and cut
  a pattern that matched `_deserializeMapData`, which returns a `unique_ptr` through a
  hidden pointer. The hook took that pointer for the map, so it was never null and the
  map id came from the caller's stack. Every deferred `MapInitializeEvent` task then
  re-queued itself (#537, `7ad256cc1`). A hooked return value that is never null is
  the tell.

Detect drift with `bump-bds` and sweep it against headers with `reconcile-headers`.
Model members with their real types, never with manual padding, and pin sizes with
`BEDROCK_STATIC_ASSERT_SIZE(T, win, linux)`. Derive the Linux size separately rather
than copying the Windows one, for two reasons:

- libc++'s `std::string` is 24 bytes against MSVC's 32.
- Itanium packs a derived class into its base's tail padding (`4fe727f20`,
  `d46e8473b`).

## 3. A wrapper outlives the BDS object (stale reference)

BDS frees and recreates objects on its own schedule, and a plugin, especially a
Python one, may hold an Endstone wrapper forever. Any wrapper that caches a raw
pointer or reference into BDS is a use-after-free waiting to happen.

- Block states cached `::BlockActor` and `BlockSource` (`ceb33198f`).
- Inventories cached `::Container` (`72d9d79c8`).
- `MapView` held a `MapItemSavedData&`, which BDS erases and recreates per id
  (`5a49f8bda`).
- `Effect` stored an `Identifier`, which is a pair of `string_view`s pointing into
  the pybind11 caster's temporary buffer (`e4b5cbe4f`).

Fixes follow from that:

- **DO store an identity** (an id, a position plus dimension, or an entity handle)
  and resolve it on every access. Throw `"Trying to access a <thing> that is no
  longer valid."` when it is gone. Check the type when re-resolving, because a sign
  replaced by a chest must raise, not reinterpret the bytes (`ceb33198f`).
- **DO capture once, at construction,** any value that must survive teardown (the
  player address in `14da6800e`).
- **DO own strings** in public value types.
- **DO NOT cache** `BlockSource`, `Container`, `BlockActor`, `MapItemSavedData` or
  `LevelChunk` pointers in a wrapper.

## 4. An exception escapes into BDS

Nothing up a BDS call stack catches. A throw out of a hook, or out of core code that
BDS called, aborts the server with `0x40000015`. Calls a plugin makes are already
wrapped, so this only bites core-initiated paths.

- **Core fan-outs over plugin-held objects must tolerate a dead handle.** A retained
  `Player` crashed `broadcast` (`d5ff6d115`). Now `sendMessage` does nothing on a
  dead handle, and `isOp` falls back to the last known value. Identity getters
  (`getName`, `getUniqueId`) still throw by design.
- **DO NOT `std::abort()` or fall off the end of a non-void function**
  (`811e211ac`). Throw where the plugin can catch it.
- **Report "not found" the way the return type allows,** such as a null or an
  invalid handle, and let the caller decide what absence means (`addd3f478`).
  Refuse to construct a wrapper over a null handle (`4ae5123bd`). Never dereference
  a missed lookup (`c4c683ed4`).

## 5. A reimplementation in `src/bedrock` diverges from BDS

`src/bedrock` contains real bodies of BDS functions that run in-process: inlined
helpers, hashers, operators, and functions reimplemented so they can be hooked. They
drift silently.

- `removeFromObjective` never erased the score, which left a freed
  `IdentityDefinition*` that the next save walked (#539, `4be6a165f`).
- `std::hash<ActorUniqueID>` masked 24 bits where BDS masks 32. Every lookup into a
  BDS-owned map missed, and a re-queuing task took 95% of the tick (`c54592af5`).
- `Actor::hasType` compared the whole value instead of `TypeMask`, so
  `PlayerPickupArrowEvent` never fired (`8824fae10`).
- An `operator==` branched on the wrong type, and a constructor left half the key
  uninitialised (`78a7ea71e`).
- Block lists copied from `LeavesBlock::randomTick` fell behind, so poplar leaves
  decayed next to their trunk (`43d537e60`).
- The NBT reader had five divergences, including swallowed load errors
  (`bed530ed2`).

Fix it by decompiling the BDS function at the target version and lining it up
against ours. Check:

- the order of operations;
- masks and integer widths;
- which branch is taken;
- what is initialised;
- whether a key is copied to a local before a loop that frees its storage (an
  aliasing bug).

Then sweep the rest of the file. Where BDS exports the function, call it by symbol
instead of keeping a copy: the hand-cloned `BlockActor::loadStatic` had dropped two
steps (#503). Re-check lists and tables copied from BDS on every bump, because they
go stale without a compile error.

## 6. An event fires at the wrong time, or never

The hook point defines what the event means.

**It never fires:**
- **BDS inlined the hooked function.** When the movement-correction system was
  inlined, `PlayerMoveEvent` moved to the `PlayerAuthInputPacket` handler
  (`83759305d`). Check that the hook still resolves to a function that runs.
- **An enum or mask diverges** (§2, §5).
- **A subclass Bukkit covers is skipped.** `ActorDeathEvent` ignored players although
  `PlayerDeathEvent` derives from it (`2fad9f9eb`).
- **The BDS event has no cancel channel.** In that case, own the packet handler and
  fire before the vanilla call (`0fbddfeec`).

**It covers only one path:**
- **DO hook the function every path funnels through,** the one BDS itself calls, not
  the client packet behind one of the paths. `PlayerBedLeaveEvent` fired from the
  leave button only. Hooking `Player::stopSleepInBed` covers morning, a broken bed and
  every other wake-up (`52ed4043d`).

**It fires when it should not:**
- **Query calls.** `InteractComponent::getInteraction` also runs with `mNoCapture` set,
  just to fill in the interact text. Gate on `shouldCapture()` (`cf3cb4113`).
- **No-op inputs:** the same slot (`e399567bc`), an empty item (#528, `a4965c076`), or
  a book with no user data (`dcae14bfd`).
- **After an earlier event was cancelled.** No `PlayerTeleportEvent` after a cancelled
  `PlayerMoveEvent` (`660113a22`), and no `ActorRemoveEvent` after a cancelled
  `ActorSpawnEvent` (`074281ec6`).
- **Off the server thread.** Synchronous events are rejected there. `BlockFormEvent`
  flooded the log during chunk instaticking until it was guarded on
  `!isInstaticking()` (`3946f6420`).

**It reports the wrong payload:**
- **DO observe the value, never derive it.** `PlayerLevelChangeEvent` computed the old
  level from the requested delta, which a clamp makes unrecoverable. Reading the level
  on either side of `Player::addLevels` fixed it (`4cd803f16`).
- **Read the field the server itself uses.** The bed comes from `BED_POSITION` entity
  data, not from the respawn point (`52ed4043d`).

Mechanics:
- To re-enter BDS without re-firing your own event, call
  `ENDSTONE_HOOK_CALL_ORIGINAL` directly.
- To pass one bit from core to a hook, tag the entity with an empty component from
  `flag_components.h`. Do not add a `thread_local` or a helper namespace.
- On `develop`, packet hooks are `EndstonePacketHandler` specializations in
  `bedrock_hooks/packet.cpp`. Call `handle()` to forward to the vanilla handler, or
  return without calling it to drop the packet.

## 7. Cancelling leaves the client out of sync, or side effects still happen

The client predicts its own actions, and server-authoritative inventory has often
committed the change before the event fires. Skipping the BDS call is not enough.

- **Dropped items.** The item stack request had already taken the stack, so a refused
  drop left the item nowhere. Put it back in the slot (#375, `279558cb2`).
- **Book edits.** Re-setting an identical stack is a no-op, because
  `setPlayerContainer` returns early on a match. Clear the slot and then put the book
  back, which forces a resync. With text filtering on, BDS applies the edit later from
  a callback, so only patch the slot when it holds what you predicted (`dcae14bfd`).
- **Movement.** A cancelled position-only move sends a correction packet. A cancelled
  jump or rotation needs a teleport (`83759305d`).
- **Damage.** Knockback was still applied after the damage event was cancelled
  (`7181ee3a7`). Lowered damage bypassed invulnerability frames (`ad4d3ed51`).
- **Commands.** A cancelled command was still logged (`0719ae587`).

After any cancel fix, check in game that the client shows the server's state and
that every follow-on effect is suppressed: knockback, logs, broadcasts and dependent
events. Resync through a vanilla path such as a forced set or a correction packet,
not raw bytes.

## 8. The right object, the wrong value

- **Wrong overload.** `Potion::getDescriptionId()` returns a raw token, and the
  `PotionType` overload builds the lang key (`ee0304bea`). `BlockType` returned the
  base id where `buildDescriptionId()` is what clients resolve (`04cead1ab`).
- **Semantics read backwards.** `ChunkSource::LoadMode::None` means "do not load"
  (`66e2774e9`).
- **Disjoint storage.** BDS keeps players in a vector separate from
  `ActorManager::mEntities`, so `getActors()` never saw them (`a55ff5803`).
- **Transport assumptions.** Downcasting to RakNet types on NetherNet read a member
  that object does not have (`4b84239a5`). Go through the base types both transports
  share.
- **Truthiness on a valid zero.** `if (tag.getInt(k))` is false for `ORIGINAL`. Use
  `contains()` (`04cead1ab`).
- **Lossy round-trips.** Rebuilding a list from scratch dropped each book page's photo
  and filtered text (`918e6ad72`). It also dropped a map's marker flag (`d93f56fcc`)
  and server-list fields Endstone does not model (`35e3c33a2`). **DO rewrite only the
  fields you model and pass everything else through.**
- **Vanilla settings ignored:** `SHOW_DEATH_MESSAGES` (`bfdc73067`),
  `SEND_COMMAND_FEEDBACK` (`064027b78`), the `damage_sensor` component (`da68c2947`).
- **Bukkit semantics.** Paper's metadata getters return copies (`dcae14bfd`).
  `isValid()` means `isAlive() && valid`, and `getHandle()` still reaches a removed
  actor (`7e8d31f8b`). On quit the order is the event, then the disconnect, then the
  message (`d5ff6d115`).

## 9. Works on one platform only

- **Key functions and vtable emission.** If a class's only virtual is the one you
  hook, the hook becomes its key function and clang emits the vtable in our object.
  That drags in base destructors that are never defined, so define the base destructor
  out of line (`75feefb7d`) or declare a key function (`9ea8c42bf`). MSVC never emits
  that vtable, so Windows links fine and Linux does not.
- **Unqualified virtual calls inside a reimplementation.** On Itanium such a call
  dispatches through our vtable at the wrong slot. Qualify it (`260121626`).
- **Virtuals introduced in a secondary base.** `ENDSTONE_HOOK_CALL_ORIGINAL` must name
  the declaring base's member pointer, and MSVC and Itanium adjust `this` differently.
  Prefer calling the virtual from an already-hooked non-virtual function.
- **Type identity across shared objects.** On Linux, `std::type_index` and typeinfo
  addresses differ between `libendstone_runtime.so`, the Python extension and every
  plugin. Compare mangled names instead (`c4c683ed4`). Never `dynamic_cast` across
  plugins.
- **Static initialisation order.** A namespace-scope `std::thread::id` was re-zeroed
  after the library constructor recorded it. Use a function-local static
  (`0c5fd2d64`).
- **Includes.** clang-cl does not pull standard headers in transitively, so include
  what you use.

## 10. Python binding bugs

- **Wrong or missing bindings.** `disable_plugin` was bound to `enablePlugin`
  (`70969ca0d`). Some APIs were documented but never bound (`e8910bde2`). An
  unterminated `)doc"` swallowed the next `.def`, which still compiled (`27cc2e04d`).
- **Bases and registration order.** Register the C++ base, or `isinstance` disagrees
  with the C++ hierarchy (`e8910bde2`). Register a base before its subclasses. When
  two classes reference each other, declare both `py::class_` handles before adding
  any `.def`.
- **Downcasts.** Add new subclasses to `polymorphic_type_hook` (`9fbb1974c`).
- **Casters.** A caster must throw for a right-typed but wrong-valued argument, and
  return `false` only for a type mismatch, so overload resolution still works. Never
  use `PyErr_SetString` followed by `return false` (`580153955`).
- **Unregistered types.** Cast through the bound type: `std::ref(*this)` deduced the
  unbound `EndstoneServer` (`dccb890f4`).
- **Hashing.** `__hash__` must agree with `__eq__`. Types that compare equal to their
  id string hash that string (`32c6d3fc5`).
- **Reference cycles.** A `std::function` holding a Python callable that captures the
  wrapper is never collected. Clear it on disconnect (`52990325c`).
- **Stubs.** Regenerate them after any binding change, because CI fails on a stale
  `.pyi`.

## 11. Plugin lifetime and `/reload`

- **Release plugin-supplied code before its module is unloaded.** Anything holding
  code a plugin supplied (a `std::function`, a service, a metrics chart, a task or an
  event handler) must be released first. Unregister these on disable (`36da53f9b`,
  `492f3175c`). In `reload()`, clear them before `clearPlugins()`, because that call
  unloads every C++ plugin module. A late release jumps into unmapped code.
- **Destroy loaders in reverse load order** (`9244bd1bb`).
- **Never hold the scheduler lock and a task's lock at once.** Collect the targets
  under the lock, then act on them after releasing it (`62f8d63c8`).

## 12. A client can crash or hang the server

- **Untrusted input needs BDS's bounds.** Input that reaches an Endstone
  reimplementation must be bounded the way BDS bounds it: NBT nesting depth
  (`9462d2e9c`), certificate chain sizes (`da9b2316c`), ACK/NAK ranges (`f6f095398`).
  When BDS already bounds the input itself, put the guard in the `raknet` recipe
  patch rather than in a hook.
- **Close connections with `NetworkSystem::setCloseConnection`,** never
  `closeConnection`, which runs `onConnectionClosed` a second time over freed objects
  (`ccdef427f`).
- **Never hand-write wire bytes.** Reconstruct the packet and build it with
  `MinecraftPackets::createPacket`.

## 13. Land the fix

- **Branch.** Fix on the line where users see the bug, then `cherry-pick -x` to the
  other line. Before porting back, check that the commits the fix builds on exist
  there too. If the port needs an API or ABI change, leave the fix on `develop`.
- **Verify.** Build both platforms when a layout changed, because the size asserts are
  the Linux check. Run `ctest` and `pytest`, run the in-server `/test`, and run the
  reporter's repro in game. Say what you verified and what you did not.
- **Commit.** Write a `fix(<scope>): <what works now>` subject. Write the body in
  prose: what was wrong and what users saw, the mechanism, the BDS evidence (version
  and address), and the change. End with `Fixes #N` or `Closes #N`. Never add a
  `Co-Authored-By` line for Claude.
- **CHANGELOG.** Add one user-visible `Fixed` line ending in `(#N)`. Add nothing for
  internal-only fixes or for regressions that were never released.
- **Record what you learned.** When a fix teaches a new class or a new tell, add it to
  this skill.
