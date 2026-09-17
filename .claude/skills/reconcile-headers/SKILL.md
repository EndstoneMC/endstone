---
name: reconcile-headers
description: Reconcile src/bedrock against newly available reference material (reconstructed headers for the target BDS version, and/or a PDB carrying type records) after a bump that was ported from binaries alone. Use when headers or a full PDB land for a version already supported, when asked to "clear out the placeholders", "sweep the header diff", or to verify a finished bump against the real declarations.
---

# Reconcile src/bedrock with real headers

A bump ported from binaries alone (bump-bds Scenario B) leaves three kinds of debt:

- **Placeholders** - `unknown<slot>()`, `unknown_<off>_`, `std::byte x[N]`, invented struct names, each marked `// TODO(fixme): check the name`.
- **Wrong guesses** - a member order or a vtable slot that the size assert happened not to catch.
- **Missed changes** - anything the crash-driven loop never had a reason to look at.

When headers for that version arrive, all three are fixable in one pass. The placeholders are the visible part and the smallest part; **sweep the whole diff, not the `TODO(fixme)` list**.

## Scope and prerequisites

- Reconstructed headers for the version Endstone already targets, and for the version it targeted before (the two tags to diff).
- Optionally the Windows PDB for the target build. **Check whether it carries type records** (`grep -aoc 'LF_ENUM'` is not reliable; grep for a known member or enumerator name). A PDB with types settles member order, offsets, bitfield widths, enum bodies and `sizeof` - things the headers leave open.
- Read the NDA boundary in `bump-bds` first: both artifacts are private. Never copy header bodies, layouts, PDB dumps or diffs into the repo, commits, PRs or this skill.
- Work on a branch off the release branch, in its own worktree when the main checkout is in use.

## Procedure

1. **Build the actionable set.** Intersect the changed headers with Endstone's `src/bedrock`:
   - by normalized basename (lowercase, strip `_`/`-`): `BlockSource.h` <-> `block_source.h`;
   - plus interface spellings: Endstone's `<x>_interface.h` is usually the headers' `I<X>.h`;
   - plus **by declared class name**, which catches everything the basename misses - a class whose generated file was renamed, or that moved into a `.cpp`.
   A few hundred Endstone headers typically reduce to ~200 candidates, many of them false matches.
2. **Fan out.** Group the set by subsystem into batches of 10-20 files with **disjoint file lists**, one agent each, launched together. Give every agent the same written brief (rules below), its batch, and the placeholders in its files. Forbid edits outside its list: `forward.h`, the symbol configs and `src/endstone/**` call sites are handed back to the orchestrator, not edited in parallel.
3. **Audit.** Re-slice the resulting diff and hand it to a second, **read-only** wave whose only job is to prove each hunk against the headers. Parallel porting produces plausible unfounded edits; this pass is what catches them. Require severities (wrong / unfounded / risky) and specific evidence, and re-verify a finding yourself before acting on it - an auditor's confident "invented" can be wrong when the body was in fact recovered from the binary.
4. **Finish.** Regenerate both symbol tables, build, run the tests, then fix what the build finds.

## Editing rules

- **DO replace a placeholder with the real type**, and delete its `TODO(fixme)` and its evidence comment. A placeholder that carries a justification comment is still a placeholder: `void *[4]` for two smart pointers has the same size and the same ABI, so swapping in the real types costs nothing and removes a trap.
- **DO NOT invent a body.** Declare the function and leave it undefined; that is the house convention for BDS functions Endstone never calls. Recover a real body only from the binary: a one-call body names its delegate by vtable slot, an ICF-folded address proves two bodies are identical, and a `lea` before an `operator<<` gives the literal.
- **DO keep a byte stand-in when only one platform's size can be proven**, e.g. a large third-party struct Endstone never reads. Say so in the report with the evidence; that is more honest than a copy that is wrong on the other ABI.
- **DO diff vtables mechanically** - extract the `virtual` lines from both sides, reduce to method names, diff the lists. Order is the ABI, and eyeballing misses a shifted slot. Count them.
- **DO check whether a virtual became non-virtual**, or disappeared. A removed slot balances an "added" one, and a headerless bump often invented a placeholder to keep a count that was already wrong.
- **DO re-derive a size assert rather than edit it.** A small member in the padding before an 8-aligned member changes nothing. Conversely a passing assert proves nothing about order: two trailing members of different sizes often swap without changing `sizeof`.
- **DO check `class` vs `struct`** on forward declarations and aliases. It is invisible on Itanium but changes MSVC mangling, so a hooked signature can stop matching.
- **DO port an enum's shifted tail by inserting at the documented position** and shifting the rest - never by appending. These values go on the wire.
- **DO NOT fix pre-existing drift that another Endstone class overrides.** Changing a base signature breaks the `override` below it. Report it separately; a sweep that also lands unrelated corrections is hard to review.
- **DO NOT port what Endstone does not declare**: nested helpers, statics, new API it never calls. Narrow by omitting, never by changing what is kept.
- **DO watch for the knock-on edit**: a member rename breaks call sites outside `src/bedrock`; an added pure virtual makes a derived class abstract; a pImpl conversion breaks every by-value embedder and needs the special members declared and defined in the sibling `.cpp`; a new const or reference member deletes the implicit default constructor, so declare the constructor the headers show.

## Reading the diff

- **DO NOT trust candidate matches by name.** Generic names (`Function.h`, `Result.h`, `Connector.h`, `*Data`) collide across unrelated subsystems, and versioned and versionless variants of the same type coexist. Confirm the namespace and the member list.
- **DO strip the noise before reading**: template-instantiation lists, lambda source locations, parameter-name-only changes, declaration reordering, and types regrouped into another generated file. In template-heavy utility headers the real diff is usually empty.
- **DO diff a moved file by explicit blob paths** in both revisions; a path-limited diff shows it as a whole new file and hides the real changes inside.
- **DO compare Endstone against the target header directly, not only the version diff.** Drift that predates the bump shows up no other way, and it is common on inline getters and on interfaces nothing derives from.

## What the PDB settles that headers do not

- **Member order and offsets**: `llvm-pdbutil pretty -classes -class-definitions=layout -include-types='^Name$'` prints every offset, bitfield width and the real `sizeof`. Batch several types per run; budget minutes on a multi-GB PDB.
- **Enum bodies for code the headers do not cover** (dedicated-server-only types): the enumerators sit next to the mangled type name in the type records.
- **Signatures**, including const-ness and return type, from the public mangled name - which also reveals a nested type that became global, the one difference MSVC encodes and Itanium does not.
- **Vtables**: when a class has no vtable symbol, scan `.rdata` for pointers to one of its known methods and read the slots around it.
- **DO NOT use DbgHelp for type layouts** - it resolves the name and then fails on every type query. Use it for addresses only.

## Traps

- **Name-only symbol resolution hides signature changes.** The Windows dumper matches a config entry by its *undecorated* name, so a hook whose parameters changed still "resolves" - to a function with a different signature. After the sweep, grep the PDB for each hooked entry's exact mangled string; the ones with no public record need the header diff to verify them instead.
- **A hand-written Endstone helper on a reconstructed struct is not BDS API.** If BDS moved a payload to a different serialization mechanism, our own codec for it stays. Mark such members `// Endstone` so the next sweep does not "port" them away.
- **Faithfulness has a limit where the type never crosses the ABI.** A struct that only Endstone fills and drains gains nothing from a lossy upstream type; matching the name while keeping a type that preserves behaviour is the better trade. Deciding this belongs to the human - flag it, do not silently regress a released behaviour.
- **`sed -i` on a CRLF file from Git Bash rewrites the whole file to LF.** Use the editing tool, or re-normalize and verify.
- **A stale `// +offset` annotation** survives several versions. Check it before trusting or preserving it.
