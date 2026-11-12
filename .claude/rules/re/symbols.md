---
paths:
  - "scripts/**"
  - "src/bedrock/symbol.h"
  - "src/bedrock/symbols/*.h"
  - "src/endstone/runtime/**"
---

# Symbols

## Configs and the dumper

- Define symbols in `scripts/configs/windows.toml` and `scripts/configs/linux.toml` as `[[signatures]]` with `name`, `pattern`, `relative`, `extra` and optional `offsets`, `rip_relative`, `rip_offset`.
- Keep both configs describing the same set of functions: MSVC `?...` names in `windows.toml`, Itanium `_Z...` names in `linux.toml`.
- Bump the top-level `version` of a config, `MajorVersion`, `MinorVersion` and `PatchVersion` in `src/bedrock/shared_constants.h` and the Minecraft version badge in `README.md` before running `scripts/dump_symbols.py`, even when every offset moved uniformly.
- Regenerate `src/bedrock/symbols/windows.h` and `linux.h` with `scripts/dump_symbols.py` and commit the result; edit them by hand only as the PDB bullet below says.
- Run `scripts/dump_symbols.py [INPUTS...] [--pdb PDB]`; with no inputs it scans `windows.toml` and `linux.toml`.
- Treat the dumper's pattern scan as a regex over the `.text` section only that takes the first match; report the match count, not a pass.
- Use `--pdb` only on a Windows config. Check the PDB's version equals the exe's and that it carries type records (grep a known member or enumerator name); a stale PDB resolves moved symbols to the old RVA.
- Do not regenerate `src/bedrock/symbols/<platform>.h` from an untrusted PDB. Hand-patch the one verified offset and update the config pattern for the next clean regeneration.
- Names `--pdb` cannot pin (ICF-folded, overloaded) fall back to the byte pattern.
- Resolve a Windows offset as section RVA plus symbol offset.
- Shell out to `pdbtool` for PDB symbols.
- Recover a renamed Windows symbol by grepping `pdbtool --quiet dump <pdb> psi` for `?<method>@<Class>@@`.

## Names

- Use real type names in declarations where a PDB exists; placeholder types fall back to the pattern.
- Treat `name` as a lookup key equal to `__FUNCDNAME__` of Endstone's own declaration, so its sizes, classes, order and return convention must be ABI-correct.
- Use the target's real mangled name for `name`; for a lambda body or compiler thunk the name is an arbitrary unique key, and says so in its recipe.
- Never guess a mangled name or its access/virtual letter, and never grep PDB text for one as proof. Compile the reconstructed declaration with `clang-cl /c` (MSVC) or `clang++ --target=x86_64-unknown-linux-gnu` (Itanium) and read it back with `llvm-nm`.
- Use a plain `Class::member` string as `name` for a data global or patch site, and read it with `BEDROCK_VAR`.
- Update `name` to the new mangled name whenever a hooked or `BEDROCK_CALL` function's signature changes.

## Entry lifecycle

- Resolve every consumed symbol before the build is green; an unresolved entry is dropped from the header, and `C7595` from `get_symbol` means a consumed symbol is unresolved, so resolve it instead of dropping the use.
- Read a dump miss by platform: both platforms missing is a real signature or API change, Windows only is a changed mangled name, Linux only or a Windows fallback hit on an ordinary name is a stale pattern or a different function.
- Keep zero-offset entries in the dumper output only temporarily to compile-check before every symbol resolves, and revert them before finishing.
- Check a function still exists on a bump and re-derive every hooked and `BEDROCK_CALL` declaration against the target; a split or inlined function leaves a pattern matching a sibling.
- A hook bound to a split or removed function shows a hooked return value that is never null, because a hidden-pointer `unique_ptr` return is read as the object.
- Reimplement an inlined function in `src/bedrock` instead of leaving a missing symbol, only where Endstone already tracks the layout.
- Use the full-DWARF Linux debug build of an older version to recover bodies of tiny header methods inlined in stripped builds.
- Delete an entry when its last consumer is gone: a live hook, a `BEDROCK_CALL` in a `src/bedrock` definition, or a `BEDROCK_VAR`.
- Keep an entry used only by devtools; every entry must resolve on every build.
- Audit liveness against the runtime DLL's export table, never `ENDSTONE_HOOK` markers in headers.
- When a function is inlined on one platform only and hooked, relocate the hook on that platform to the out-of-line caller and reconstruct the trigger condition.
- Re-read a mutable BDS global before writing a symbol constant into it.
- Strip `//` and `/* */` comments before counting header virtuals against a binary vtable; a virtual that became non-virtual or vanished can cancel an added one.

## Patterns

- Keep a byte pattern on every entry.
- Give each pattern exactly one match; the dumper takes the first match and warns `Ambiguous pattern` when matches differ.
- Read many matches collapsing to one address as harmless and n matches resolving to n addresses as `.text` order deciding; landing on the old offset in the previous binary validates the pattern, not the first-match choice.
- Read a prologue miss as the function's own codegen changing and a call-site miss as a caller changing; neither is a signature change until the Windows mangled name says so.
- A `?` masks a byte but cannot absorb an insertion such as a new `push r12`, xmm spills or `83 EC imm8` widening to `81 EC imm32`.
- Recompute the rel32 target of a unique call-site pattern; it may sit on a sibling call and resolve a different function.
- Twin copies differing only inside the rel32 cannot be separated by a prologue; keep the call-site form and tell them apart by caller count.
- A release with no PDB resolves every Windows entry by pattern and Linux always does, so no entry is name-verified and a stale pattern can land on a similar prologue.
- Cut a pattern from the function's `.pdata` `BeginAddress` (Linux FDE `initial_location`), never from a guessed prologue.
- Wildcard exactly the displacement and immediate bytes reported by capstone's `insn.encoding`, keeping every opcode and ModRM byte.
- Grow a pattern one instruction at a time until a whole-binary scan returns one match, strip trailing wildcards, then add one instruction past the minimal unique point so it ends on fixed bytes.
- Treat `Ambiguous pattern for <name>` and `Offset 0x... is not divisible by 8` as hard failures; the second means a prologue pattern still carries `rip_relative` and `rip_offset`.
- Pick the most survivable form: a unique `E8` call-site pattern at least a third shorter than the prologue, then a minimal unique prologue, then anything long.
- Take a call site over an `xmm`-save prologue (`44 0F 29` runs) whenever it is unique.
- Prefer an `E8` pattern with concrete anchor bytes after the call; avoid all-wildcard call chains, `E9`-then-`CC` padding and patterns spanning `CC` padding into the next prologue.
- Retry a call site that is not unique at 32 bytes, then try more call sites with a higher `xrefs_to` limit.
- For a virtual, most xrefs are data and the few code xrefs are devirtualised direct calls.
- Keep a prologue pattern for virtual or thunk-only targets, generic call-site idioms such as shared_ptr release chains or the security-cookie epilogue, and when the shortest unique call site is longer than the prologue.
- Accept a first-match call-site pattern only when its lowest-address hit resolves to the target and it is half the prologue length or less; direct and data-global patterns must be unique.
- Cap a call-site pattern at 32 bytes and prefer anchors with ASCII string immediates or enum/command literals over register-dependent bytes.
- Anchor byte-identical singleton accessors on a caller whose tail is a string immediate or a unique string, never on the prologue; confirm by marker strings in the call closure, never by shape, and cross-check the pattern on the previous two releases.
- Set `rip_offset` from the pattern's first instruction: `E8`/`E9` is 1, `48 8D 05/0D` and `mov [rip+x]` is 3, `FF 15` is 2; confirm that `E8` address + 5 + rel32 equals the entry VA and recompute it whenever the pattern changes.
- For a pre-call anchor such as `lea r8,[rip]` or `mov r9d,<enum imm>`, set `rip_offset` to the call address minus the pattern start plus 1.
- Anchor on a `movabs` string literal of a core enum or command name, and check which call it feeds since stack slots get reused.
- Never use a forward capture-all of the three-argument setup shape; it precedes any such call.
- Anchor a `forEach` or `function_ref` call on `lea rax,[invoke_fn]; mov [rbp+a],rax; lea rax,[ctx]` before the call and extend one instruction past it.
- Re-cut a rip entry as a rip entry before falling back to a prologue.
- Delete `rip_relative` and `rip_offset` when a pattern becomes a prologue pattern.
- Anchor an array-global pattern on the inlined bounds-checked indexed read, not on an initializer store: the unity static-initializer (`_GLOBAL__sub_I_unity_*`) locates the cluster of globals, not the array base.
- Tell an array base from a neighbouring static by xref shape: the base has hundreds of indexed loads, a neighbour a handful of `cmp reg,[rip+d]` compares and `mov qword [rip+d],0` teardown stores.
- Record in the recipe the caller an array-global pattern was cut from.
- Re-cut patterns from the target binary's own bytes per platform, wildcarding codegen-volatile bytes; never reuse the other platform's pattern or nudge an old one.
- Re-cut a virtual's pattern from its vtable slot (Linux `_ZTV`, Windows via the constructor's `__vftable` store), read the slot's prologue and wildcard only displacements and immediates.
- Neutralise a confirmed-wrong pattern you cannot re-find with the sentinel `0B 50 1E 7E DE CE A5 ED` so it fails loudly; keep its recipe.
- Never run `make_signature` on `?executeWorldInteraction@InventoryTransaction@@...`, an arbitrary name hooking a lambda; check its pattern with `find_bytes`.

## Recipes

- Write a numbered locate recipe in the comment block above every entry in both configs, with `# function prologue` or the anchor kind as the pattern's one-line description.
- Write the recipe as pure numbered steps `# 1.`, `# 2.` with no header or prose: step 1 names the anchor with its exact bytes including `00`, a gloss and its uniqueness, each hop gives call-site landmarks, a foreseen drift is its own step, and the last step is an independent identity check against the old named database. State how uniqueness was verified, warning when the target has byte-identical siblings, and cut the recipe against the config's `version`.
- Pick the recipe approach by bucket: unique string, generic-only strings (call site in a named caller, packet vtable or vtable slot map), or none.
- Write each recipe from the named reference database; only the new pattern and its address check need the target database.
- Anchor recipes on version-stable things: a unique string with xref such as a validation label, specific log or error message, command or translation key or NBT key, a vtable slot of a named virtual, a call from an already-located function, or a distinctive immediate.
- Never anchor on boilerplate strings (`NonOwnerPointer` asserts, source paths, `"%s"`); search exact keys NUL-terminated.
- Treat an unfindable recipe step as drift, not as the function being gone.
- A changed `__PRETTY_FUNCTION__` return type confirms identity and marks the config's mangled name stale.
- A rare `NonOwnerPointer<X>::_get()` instantiation identifies a function by what it touches.
