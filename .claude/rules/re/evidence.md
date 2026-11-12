---
paths:
  - "src/bedrock/**"
  - "src/endstone/runtime/**"
  - "scripts/**"
---

# Evidence for BDS facts

## Sources and their order

- Settle a name, member order, base class, virtual order, enum value or signature from the BDS headers first, then `EndstoneMC/protocol-docs`, then `Mojang/bedrock-protocol-docs`. Never use CloudburstMC or gophertunnel for names.
- Read a PDB type layout with `llvm-pdbutil pretty -classes -class-definitions=layout -include-types='^Name$'`, batching several types and budgeting minutes per run on a multi-GB PDB.
- Take member offsets, `sizeof`, bodies of server-only enums, vtable contents and whether a function has an out-of-line body from the binary or PDB.
- A public mangled name reveals a nested type that became global, a difference MSVC encodes and Itanium does not.
- Never use DbgHelp for type layouts; it resolves the name then fails every type query, so use it for addresses only.
- Diff the `EndstoneMC/protocol-docs` branches (one per release) before opening a binary for packet work. A packet present there is cerealised.
- Date a packet's cereal migration by decompiling its `write` in the old and new binaries; cereal display strings are present in both eras, so grepping for them proves nothing.
- Do not cite Endstone's own `src/bedrock` or core code as evidence of how BDS works. Log disagreements instead of trusting the existing reconstruction.
- Do not treat community sources (public RE projects, Java-name extrapolation, symbol-less decompiles, a neighbouring file) or string presence in the binary as evidence.
- Recover a function body only from the binary, never from a header, name, size, vtable slot or neighbours: a one-call body names its delegate by vtable slot, an ICF-folded address proves identical bodies, a `lea` before `operator<<` gives the literal.
- Read member and type names from live-referenced cereal and entt reflection registration strings, not from ICF-orphaned `currMetaMember()` strings (zero xrefs).
- Prove an RTTI-less type name through entt's one-shot type-id-to-name static initializer, matched to the code that iterates the container at its stride.
- Use `__PRETTY_FUNCTION__` literals to bridge Windows and Linux; they are identical apart from the source path.

## Which binary answers what

- Read mechanism from the Linux `bedrock_server`: it keeps symbol names and is far less inlined.
- Read offsets and layout from the Windows build closest to the target version, and back every layout with Windows and Linux evidence.
- State which IDA database and build settled each point. The target version wins on disagreement.
- dwarf2cpp dumps are template-instantiated: infer the generic template from the instantiations.
- Use a sampling profile's symbol names as guesses only. Map `bedrock_server::0x...` frames through the decimal offsets in `src/bedrock/symbols/<platform>.h` only for the build whose SHA-256 equals `extra_platform_metadata["BDS executable SHA-256"]`; without that build, disassemble the matching binary or label each name a guess from vtable, RTTI or string evidence.
- A spark `ThreadNode.children` is a flat array and `children_refs` holds indexes into it.
- A spark `times[k]` is wall time; ms per tick is `times[k] / window_stats[k].ticks`.
- Same-label sibling frames in a spark profile are separate hot program counters inside one function, not separate calls; the export drops the PC, so name no instruction.

## Confirming

- Decompile to confirm every located, refreshed or reconstructed function: match logic, distinctive constants, argument and return shape, and call graph against the named reference. Size, string overlap, vtable slot, address neighbourhood and pattern uniqueness are triage only.
- Decompile every resolved entry, not only the misses, and compare the body at the boundary in the old and new databases.
- Judge a function by its body, never by name, hexrays prototype, auto-name, callee-name overlap, RVA at the old version, RVA-delta banding or address proximity.
- Do not dismiss a candidate for a changed size or caller count: inlining can grow a function 2-3x. A wrong match looks categorically different (3-line stub, tree lookup, teardown loop, `JUMPOUT`).
- Do not accept a perfect size match without a string or constant check; it may be the target's caller.
- Anchor identity on a self-naming marker string (a `<Class>::<method>` trace literal) referenced from one site in both versions.
- Validate a hand-rolled scanner on the previous binary before trusting a negative result, and ask IDA when a plainly existing string scans as unreferenced.
- Do not assume protocol numbers increase monotonically. Collect the `cmp` immediates of `ServerNetworkHandler::_validateLoginPacket` (the version is in that set, reproduced first on the previous version), never filter by a plausible successor and never count raw dwords across `.text`; confirm with the RakNet MOTD builder.
- String-set similarity measures the function class, so a singleton-accessor family scores about 0.96 for every member; pair it with the marker string of the entry's locate recipe.
- A low structural score on Windows and Linux independently means the code changed; low on one platform means that platform's offset is wrong.
- Cross-validate a reverse-engineered payload member by member against the `bedrock-protocol` schema.
- Do not report a hooked symbol as resolving because the build compiles. Check it resolves to the intended function and not a twin, the decoration matches the real declaration, and the pattern is unique.
- Do not call an untested path verified: a vtable one slot off or a member at the wrong offset still compiles, links and passes the suite.
- Record a declaration the sources cannot settle as unverified. Do not merge it.

## Crash reports

- Read the crash report, the decompiled BDS function at the target version, and the minidump or IDA database before naming a cause.- Frames print the preferred base plus an RVA, which is the return address minus 1. The first frame after a gap in the indices is the faulting PC.
- An RVA is the address minus `0x140000000` for `bedrock_server.exe` and `0x180000000` for `endstone_runtime.dll`; the same crash gives the same addresses every run.
- Resolve a `??` frame through the PDB, else the named Linux build, else `.pdata` bounds matched by rip-relative strings and instruction shape.
- `0xC0000005` right after a bump means a wrong offset; in a BDS frame ticks after disconnect, unload or reload it means a freed object.
- `0x40000015` is an uncaught throw with frame #0 at the throw site. `0xC0000409` is a fail-fast: a call into the wrong virtual, or terminate from an uncaught exception.
- `bad_variant_access` at `event.visit` means event variant drift and ends as terminate `0x40000015` then exit `0xFFFF7001`; it fires only when a covered event dispatches, so a server that runs fine and crashes on `stop` is the classic symptom.
- Bisect a fail-fast with no crash report using `fprintf(stderr)` and `fflush`: one marker per top-level stage, then one per statement; two builds usually name the bad dispatch, whose concrete class goes to the Linux RTTI vtable diff.
- Read the code at the reported release (`git show v<ver>:src/bedrock/symbols/<platform>.h`), not HEAD, and test the symptom rather than the reporter's suspected cause.

## Reviewing src/bedrock

- Open the header declaration for every `src/bedrock` declaration a change adds, retypes, moves or reorders, existing members included. Never claim a declaration follows the headers without opening it, and never accept one because it is plausible.
- Check each declaration for the right class, call path, signature (the `re/reconstruction.md` list), BDS version, and the reconstructions the feature leans on.
- Confirm the namespace and members of a candidate header match; do not trust it by name alone (`Function.h`, `Result.h`, `*Data`).
- Compare Endstone against the target header directly, not only the version diff. Report pre-existing drift that another Endstone class overrides separately, and do not port what Endstone does not declare.
- Build the actionable header set as the changed headers intersected with `src/bedrock` by normalised basename (lowercase, `_` and `-` stripped), by interface spelling (`<x>_interface.h` is `I<X>.h`) and by declared class name.
- Skip client-only trees when sweeping a header diff against `src/bedrock`.
- Strip noise before reading a header diff: versioned-namespace churn, template-instantiation lists, lambda source locations, parameter-name-only changes and declaration reordering; in template-heavy utility headers the real diff is usually empty.
- A file shown as deleted often only moved; diff the explicit blob paths in both revisions before calling a type gone or new.

## The NDA boundary

- The BDS headers (`bedrock-headers`), the PDB, generated dumps, decompiler output and header diffs are under NDA and not every collaborator can open them.
- Never copy header bodies, class definitions, member lists, full layouts, PDB dumps, symbol listings or diffs into the repo, commits, PRs, issues, logs, skills or rules.
- Write `src/bedrock` as a minimal hand-written reimplementation in Endstone naming. Mangled names, symbol offsets and general ABI technique are allowed.
- Cite a private source as "confirmed against the BDS headers", or as an IDA address plus the single decompiled line.
- Without header access, derive names and layout from the binaries, a named older reference database, `EndstoneMC/protocol-docs` and `bedrock-protocol`. Record what you cannot confirm as unverified and ask a maintainer to confirm it against the headers before merge.
- Use an older header snapshot only to look up a symbol removed in the current version, and name the snapshot branch when recording the result.
