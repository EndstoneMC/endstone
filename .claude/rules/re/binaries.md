---
paths:
  - "scripts/configs/**"
---

# Reading BDS binaries

## Vtables

- Use Linux RTTI vtables as the bulk oracle for vtable shape; Windows Minecraft code has no RTTI.
- Find an Itanium vtable through its RTTI: mangled name in `.rodata`, then the `.data.rel.ro` qword pointing at it, then the code pointers after the slot equal to the `type_info` address.
- Search the typeinfo name on its trailing NUL and validate it through the typeinfo that points at it; the name is not always NUL-preceded.
- Rebuild `.data.rel.ro` from `.rela.dyn` `R_X86_64_RELATIVE` addends before reading PIE vtable slots; `offset_to_top` is not relocated.
- Parse `.rela.dyn` as 24-byte records and keep `info & 0xffffffff == 8`; `lief` `Binary.relocations` is empty on the stripped ELF.
- Split a vtable group by `offset_to_top` to isolate secondary-base sub-tables.
- Read the whole primary vtable: base slots repeat one shared stub, and a derived slot may be a tiny forwarder that tail-calls the base body.
- End an Itanium vtable run at the next vtable's header, a zero `offset_to_top` followed by a typeinfo; RTTI-less function-pointer tables follow vtables.
- A derived vtable is never shorter than its primary base's; use that to reject false length changes.
- `__vmi_class_type_info` turning into `__si_class_type_info` means a base was removed.
- `offset_to_top` of a secondary base is a size oracle for the primary subobject.
- A per-class length sweep cannot see a net-zero change; when a base's length changes, re-check every deriver by slot alignment.
- Run a per-slot `difflib.SequenceMatcher` alignment over every declared class and report only insert and delete opcodes.
- An abstract base whose vtable is never emitted is missing from a length sweep; read a changed deriver as a change somewhere in the hierarchy and walk up.
- Scan `**/*.cpp` as well as headers for the classes Endstone declares.
- Tag each slot `P` pure, `T` this-adjusting thunk, `R` repeat or `.` other; equal tag strings rule out a same-count shuffle.
- Build a recompilation-robust slot signature from the first six instructions, keeping mnemonic and register class and dropping immediates and displacements.
- Map a slot to its header declaration by counting virtuals, and anchor the count on a hooked virtual with a known slot.
- A header may omit Linux-only virtuals, so the cumulative count can fall short; match local neighbours such as a const and non-const overload pair returning the same `this+N` subobject.
- Find a Windows vtable without RTTI as a `.rdata` run of ICF-folded `lea rax,[rcx+d]; ret` stubs; they are 16-aligned with no `.pdata` record, so a `.pdata` filter drops them.
- Identify the class of an RTTI-less vtable from its dtor slot.
- End a Windows vtable where the next qword's address is the target of a `lea` plus `mov [reg],rax` vfptr store; MSVC packs vftables back to back.
- A displacement proves slot order, not semantics.
- Index one virtual in a large Windows vtable by walking back from the `.rdata` qword that holds it, calibrating the offset against the last PDB-bearing release.
- An ICF-folded const and non-const pair shows as two adjacent slots sharing a target.
- A vtable slot offset is not version-stable; pin two string-locatable virtuals far apart to measure the drift.
- Locate an insertion with a diagonal scan of `old[i]` against `new[i]` and `new[i+1]`, skipping 1-3 byte stubs.
- Compare vtable size across builds; a shrink means a virtual was dropped.
- A derived forwarder should be the target's only direct caller in both builds.
- Compare `.data.rel.ro` as a qword array index by index across Linux builds, accepting only the known section shifts; if it stays aligned end to end no pointer was inserted or removed.
- On Windows classify each `.rdata` qword (into `.text`, into `.rdata`, zero, plain data) and compare the total code-pointer count across builds.
- When vtable counts differ, key every vtable by RTTI name, record each class's slot-run lengths and diff the name-keyed maps; template instantiations appearing and vanishing move the global count with no class changing shape.

## Member layout and sizes

- Take class size from `operator new` at constructor call sites and from vector element stride.
- A `make_shared` `operator new` immediate is the control block plus `sizeof(T)`: 16 on MSVC, 24 on libc++.
- `MinecraftPackets::createPacket` is a jump table indexed by packet id: `table[id]`, then `make_packet<T>`, then `operator new`.
- Cross-check a Linux size with the sized `operator delete` in the D0 destructor.
- The ordered `operator new` immediates of a factory fingerprint it across versions; the entry that changed is the new `sizeof`.
- The allocation-failure assert spills `mov qword [rsp+0x28],N`, which is greppable as a byte pattern.
- A stack-only class has no `operator new` site; MSVC emits `operator delete(this,sizeof(T))` in vftable slot 0.
- Take slot 0 from the scalar deleting dtor body, not from the start of the over-running run.
- Sweep Linux `sizeof` off the Itanium D0, `mov esi,N; jmp sized delete`, requiring `offset_to_top` 0 and not relocated, and a D0 of at most 26 instructions that calls the slot before it.
- Scan the first 20 slots for the D0 pair.
- Sweep static asserts by mangling each asserted class to `_ZTS...` and running the D0 oracle on both the old and new binary; trust the differential, and require the oracle to reproduce the old asserted number first.
- Non-polymorphic classes are invisible to the D0 oracle; report them unresolved.
- `std::optional<T>` is self-measuring: the engaged-flag offset in `cmp byte [this+K],1` pins the member start and `sizeof(T)`.
- Diff the named previous database's ctor and dtor against the target; the first differing offset localises the change and the delta is the inserted size.
- Confirm a later anchor member moved by the same delta.
- Extract offsets with a `this`-relative store tracker seeded at `rcx`, following `mov` and `lea` copies and stack spills, and logging stores and calls with a `this`-relative `rcx`.
- A uniform delta from the first divergence to the end is one change; mixed bands are several.
- A ctor `mov qword [this+N],0` becoming `movups` proves an 8-byte append at `N+8`.
- Index-align the two dtors' sorted displacement lists instead of set-differencing them; the lists must have the same length.
- MSVC sizes: `std::string` 32 with capacity 15 at `+24` and SSO bit `0x80000000000000`, `std::vector` 24 with a sized delete from `end - begin`, `std::unordered_map` and `unordered_set` 64 with load factor `1.0f`, mask 7, 8 buckets and a 32-byte sentinel node.
- MSVC sizes: `shared_ptr` and `weak_ptr` 16, `Bedrock::NonOwnerPointer` 24, both with an atomic refcount and virtual `__on_zero`, and `unique_ptr` 8 with a virtual deleting dtor for polymorphic `T`.
- MSVC sizes: `std::function` 64, `BaseGameVersion` 32, `Core::Cache` 72, `AABB` 24, `HashedString` 48, `mce::Color` 16.
- A `unique_ptr` to `shared_ptr` flip keeps the offset and shifts every later member by 8; tell them apart by release kind (atomic refcount with virtual `__on_zero` versus inline delete), not by the delta.
- A getter returning `shared_ptr::get()` keeps working while every later member is 8 bytes out.
- After finding a `unique_ptr` to `shared_ptr` flip, look for its siblings in related ownership classes of the same release.
- A `unique_ptr<T>` can also become an inline `std::optional<T>`, and a member can relocate with `sizeof` unchanged.
- Without symbols or a locatable ctor, read the Linux dtor at vtable+16 (D1) or +24 (D0); libc++ offsets differ but member order and kinds match.
- A dtor sees only owned members; treat a dtor-only verdict as a lower bound and cross-check the ctor stores and what Endstone reads.
- A layout bug surfaces as an access violation in a trivial accessor or `NonOwnerPointer::_setControlBlock`; read the displacement from BDS's own accessor in both versions first.
- If a hook already called the original with that `this` and returned, `this` is fine and the fault is a member offset.
- A newly added `std::lock_guard` shows as inlined `_Mtx_lock` and `_Mtx_unlock`, an `INT_MAX-1` sentinel and `_Throw_Cpp_error(5/6)` and moves no member.
- A vanished `__shared_ptr_emplace<T>` means nothing calls `make_shared<T>` any more, which usually pairs with a member changing from `shared_ptr<T>` to `unique_ptr<T>`.

## Packets and cereal

- Read the cereal manager's switch-on-int op function for packet layout: case 4 clones with `operator new(SIZE)`. Read the assembly, not the decompile.
- Read widths from the case 1 and 2 assembly: `movss` is a 4-byte float, `movzx byte` with `mov [dst],al` a 1-byte member, `lea; lea; call` a non-trivial member.
- Adjacent 1-byte stores mean a member narrowed; no store in the new copy means it was removed.
- Hex-Rays mixes pointer units, and 8-byte alignment rounding can hide narrowing or removal, so never trust size alone.
- Before the cereal migration `getSerializationMode` and `setSerializationMode` are stubs, folded onto `disallowBatching` on Windows and `return 0` on Linux; after it they read and write a new 4-byte member initialised to `CerealOnly` (5).
- Without a PDB, `getId`, `getName` and the mode stubs sit as a cluster beside the `"<Name>Packet"` literal on Linux; on Windows ICF folds the mode stubs elsewhere, at vftable slots 12 and 13.
- A packet in a side-by-side mode keeps its hand-written `write`, which reads every member at its real offset with the cereal name in the adjacent register and covers PODs and raw pointers a dtor misses.
- Take member names from the registration and offsets from the ctor; registration `lea` order is wire order, not always declaration order.
- `createPacket`'s `make_shared` gives the default-member map and the initial `SerializationMode`; a packet with a hand-written `write` branch starts at `SideBySide_LogOnMismatch` (1), not `CerealOnly`.
- `cerealizer<T>::bind` gives the wire field list.
- The documented wire type gives the C++ enum width: a 1-byte enum is `uint8` and a 4-byte enum is `uvarint32`; confirm with the `cereal::internal::TypeSchema<T>` string.
- The `make_shared` zero-init ends at the payload dsize with a trailing unaligned store; an odd end offset means the last member is 1 byte wide.
- Payload ctors are the member oracle when there is no hand-written `write`; reach them from the `operator new` site.
- A migration can reorder members into wire order, so a tail append does not mean nothing before it moved.
- A migration can hoist a scalar out of a container and leave an always-empty vector.
- The cereal early-out of `writeWithSerializationMode`, `serialize(ctx, this+N, stream)`, gives the payload base `N`.
- An out-of-line ctor taking the payload by value is the complete member map, and `dst - src` is the payload base.
- The trailing `operator delete` loop of that function gives a vector's element stride.
- A member absent from the cereal registration can still gate the write; per-member getters test `payload->type & bit`.
- The packet-specific accessor thunks, the 3rd and 4th `lea rax, sub_` of a member's registration block, carry offset, stride and presence; the generic entt meta thunks carry nothing.
- A `mov [rsp+10h],rdx; push...; lea rbp,[rdx+N]` block before a function is an MSVC EH funclet; `N` is a stack offset.
- Name an unnamed payload member from the offset of its first owned member: the payload dtor's last call, then its first non-trivial teardown, matched against the last PDB-bearing release.
- Corroborate with the callee's xref count: dozens for a binary-wide type, three for a packet-local one.

## Event variants

- Recover a variant from BDS dispatch `movsx rdx,[rcx+OFF]; inc rdx; call extractor`: `OFF` is the storage size and the extractor's case count is the alternative count.
- Each extractor case opens with `cmp byte [r8+K],0/1`; `K` is that alternative's value size and the largest `K` is the size driver.
- Fingerprint a case by `_InterlockedIncrement` for a `WeakRef` or `NonOwnerPointer` and by `memcpy` with a `0xAAAA...AB` multiply for a string or vector.
- Diff against the previous named database's extractor, whose copy-ctors label every old slot.
- A non-size-driver alternative can be an empty struct or named placeholder without changing the variant.
- On Linux, read `mov eax,[event+OFF]; cmp rax,-1; lea rcx,[rip+TABLE]; call [rcx+rax*8]`; `OFF` is the discriminant and `TABLE` holds one thunk per alternative.
- Bound the visit table by the next rip-referenced address, not the next non-code qword, and validate on the previous binary.
- Sweep variants per bump by scanning `.text` for `mov e?x,[reg+N]` followed by `mov e?x,0xffffffff` and diffing the set of `N` across binaries.
- Match each `Script*GameplayHandler` vtable slot's offset with clang's last `__index_t __index` from `-fdump-record-layouts`.
- A vtable change on a `Script*GameplayHandler` is usually a new overload; compare the discriminant offset and bounded count of the hooked slot.
- Confirm live by breaking on the hooked `handleEvent`, copying `byte [event+OFF_real]` into `byte [event+OFF_endstone]` and running a full start and stop.

## Locating without names

- Read a candidate's prologue before chasing callers: a ctor calls the base ctor, then `lea rax,vtable; mov [this],rax`, then stores its arguments.
- Assume a missing callee or caller was inlined up and xref the surviving anchor one level; the target balloons in size and inherits the hop's callees and strings.
- Pin an indirect hop with a local landmark: a constant `mov`'d after the call, an enclosing `cmp, imm` or an argument immediate.
- A derived override tail-calls its base through a 5-byte thunk.
- A sibling-shared caller places the target at the neighbouring call.
- Walk down a forwarder chain from a string at the chain entry's call site, taking the overload the thunk calls.
- Reach a packet's vtable from `"<Name>Packet\0"`: its code xref is the tiny `getName`, whose address is a vtable slot; walk to the vtable start and xref it.
- The vtable's code xrefs are the ctor, the dtor and any inline builder; take the non-ctor non-dtor one.
- Windows PE raw file offsets differ from RVAs by a section delta; never scan raw bytes assuming they match.
- Without IDA, find the string in the new binary and scan executable sections for rip refs where `disp + offset_of_disp == target_rva - section_va - 4`, checking `-8` for a trailing imm32.
- Map hits to functions through `.pdata`; the same identity finds direct `call rel32`, and an absolute 8-byte `.rdata` scan finds vtable slots.
- On Linux the rip-ref identity is `(v32 + p) mod 2^32 == (target - 4) mod 2^32`; filter a preceding `E8` for callers.
- Prove a target by exhaustive constant intersection: scan 2-3 distinctive immediates as raw little-endian bytes across `.text` and require the same cardinality in both binaries with one member anchored.
- Fingerprint a call graph by the multiset of its callees' `.pdata` extents.
- A `.pdata` entry with `UNW_FLAG_CHAININFO` is a chunk; resolve it to its primary `RUNTIME_FUNCTION`.
- Build a direct-call index from `E8` targets filtered to `.pdata` BeginAddresses.
- Intersect the callee sets of uniquely-anchored caller counterparts; a byte-for-byte mapped caller pins the call at the same relative offset.
- A self-naming assert string `"<ret> Class::method(args)"` states the return type, so grep it to confirm a signature.
- Linux has no named database on either side: run the config's pattern against the baseline database with `find_bytes`, decompile both and diff.
- Identity carries across a bump through EnTT component-lookup masks and the `0x3FFFF` and `0x7FF` entity-id masks.
- Constant folding can replace a static read with a literal, and some vtable offsets shift while others hold.
- A runtime-init `HashedString` id dead-ends the string-to-global walk; use the vtable scan, since FNV id hashes are inlined and invariant.
- Disambiguate a string mapping to several functions by name-independent tells: a code xref, caller and callee order, loop versus straight-line shape, and a vtable data xref.
- Tag a recipe `- N xrefs` when its string is not a clean 1:1.
- Try locate anchors in this order: direct string, a string-locatable neighbour, a magic constant, a packet vtable through its RTTI name, a vtable slot map, a data global through its initializer.

## Checking a resolved offset

- Exact-match each resolved Windows RVA against a `BeginAddress` of the exception directory (data directory 3, 12-byte `RUNTIME_FUNCTION` records); an RVA that is not one is not a function start.
- A tiny tail-call thunk may have no `RUNTIME_FUNCTION`; check the previous known-good version also lacked one, then follow the `jmp` and compare the target.
- A unique pattern landing a few bytes inside the right function passes every flow check and fails only the exact function-start match; require match count 1 and match address equal to the function start.
- Check a Linux offset against `.eh_frame_hdr`: its sorted table holds `<i4 initial_location, i4 fde_ptr>` per function, relative to the header address; exact-match `initial_location`.
- Read a Linux function's exact length from its FDE (`pc_begin` pcrel at fde+8, `pc_range` at fde+12) and assert `pc_begin` equals the table value.
- Confirm the Linux offset sits at the same slot of the same class in both binaries.
- A function with zero callers and zero vtable slots may be reachable only through an adjustor thunk; follow the thunk before calling it unreachable.
- Compare the ASCII and UTF-16 strings reached by rip-relative references inside the function extent; a genuine match gives a near-identical or superset set, large disjointness is a false match, and a genuine match losing old anchors is not routine.
- Weight strings by distinctiveness: a string present in more than 0.2% of a few thousand sampled functions is generic, and the allocator strings (`void *operator new(size_t)`, `We failed to allocate %zu bytes.`, `pointer || size == 0`, the `MemoryOperators.cpp` path) are always dropped.
- For a string-less function compare the string set of its call closure 2-4 levels deep.
- Verify a data global by section membership plus the reference population; globals have no `.pdata`, prologue, strings or alignment guarantee.
- Sort resolved offsets by RVA and print `new - old` on a hotfix: insertions step up with RVA, a negative delta or flipped order marks a wrong match; PE sections are page-aligned, so a global's delta of 0 while code shifted is correct, and a reshuffled link order makes the bands a hint, never an arbiter.
- Compare about 256 bytes at the new and old offset and require at least 90% byte equality; `.bss` data has no file content, so use section membership and delta.

## Notifications

- A `dispatch<...>` instantiation is per-signature and normally has one caller; equal caller sets across versions prove an unmoved publish site.
- Read the thin notifier, not the publisher.
- Find the notifier's caller through the literal `tryChangeState(expected,desired)` pair to order against a state field.
- BDS discards the CAS result and publishes outside the lock, so an Endstone-side `state >= X` gate is unsound.
