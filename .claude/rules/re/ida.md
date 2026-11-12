---
paths:
  - "scripts/configs/**"
---

# IDA and idalib

Tool names below are the ida-pro plugin's `mcp__plugin_ida-pro_idalib__*` tools without the prefix; `list_instances` and `select_instance` are `mcp__plugin_ida-pro_gui__*`.

## Sessions

- Load every idalib tool you need with one `ToolSearch` select call.
- Drive one idalib worker sequentially; never fan subagents or parallel calls onto it.
- Issue calls back-to-back once the database is open. On "No database bound", reopen and rebind, and check liveness with `idalib_current` or `idalib_list`.
- Run `idalib_list` and read `idb_path` before every session to confirm which database is the target and which is the reference.
- Attach to an existing session with `idalib_list` and `idalib_switch`, or the `database:` parameter. Never kill an idalib worker.
- Move the unpacked `.id0`, `.id1`, `.id2`, `.nam` and `.til` files to a backup folder only when nothing holds the `.i64`. If it is still locked, use another database or ask.
- When a worker may not be killed, copy the packed `.i64` to a scratch folder, open the copy and delete it afterwards.
- Name the process holding the unpacked files with the Restart Manager API, and move them all or none.
- Do not try to adopt a foreign worker; a second frontend sees 0 sessions.
- Open the existing PDB-named `.i64`, not the raw `.exe`; without a `.pdb`, `make_signature_for_function` fails by mangled name.
- Expect the first `idalib_open` of a freshly built multi-GB `.i64` to report failure while the worker keeps unpacking. Wait until `.id0` stops growing and CPU drops, then reopen.
- Do not wait on a target database still in auto-analysis; work in the reference database meanwhile.
- Keep the old named database open beside the new unnamed one as the oracle, as two `idalib` sessions or two GUI instances via `list_instances` and `select_instance`.
- Measure a worker's memory with `PrivateMemorySize`, never `WorkingSetSize`.

## Building databases

- Build headless databases with `idat.exe`, never `ida.exe`; IDA 9 has no `idat64.exe`, one `idat.exe` handles both bitnesses and writes `.i64`.
- Log `idat` with `-L`, never `-B`, which writes a giant `.asm`.
- Keep databases under `bedrock-symbols/<arch>/<version>/` beside the repo, `<arch>` being `win32_server_x64` or `linux_server_x64`.
- Fetch a preview binary by hand, check its SHA-256 and extract it to `bedrock-symbols/<arch>/<X.Y.Z.N>/`; `fetch_bds.py` reads only release metadata.
- Pass `-Opdb:off` to `idat` only when no `.pdb` sits beside the binary; relaunch with it when `idat` sits at a PDB prompt with CPU near 0 and the log ending at the prompt.
- Ignore `Syntax error near: <END>` log lines around import or `ws2_32` types.
- Resume or keep a database by dropping `-c` and pointing `idat` at the `.i64` instead of the binary.
- Never open a `.i64` in idalib while `idat` is writing it, and never run two `idat` processes on one database path.
- Judge `idat` progress by per-process CPU delta and the tail of the `-L` log, not by `.id1` size.
- Treat a build as done only when `idat` exits 0, the log ends `[make_idb] saved; exiting.` and the `.i64` exists.
- Match any PDB to the exe's RSDS GUID and age and copy it beside the exe before launch; BDS zips never ship one and without it the Windows database has no names. Keep PDBs out of the repo.
- Treat Linux databases as nameless unless a full-DWARF debug database exists for the target or an older patch of the same minor.

## Querying

- Short string literals are referenced through `movups xmm0,[rip+disp32]` (`0F 10 05`) and stored with `0F 11`, not only `lea`; ask IDA for xrefs before calling a recipe stale.
- In a hand-written rip-relative scanner the disp32 starts after the ModRM byte.
- Do not use `search_text` on BDS databases. Convert the string to ASCII hex and use `find_bytes`.
- `get_string` under-reports length; author the search hex from `get_bytes` past the visible end to the `00`.
- `imports_query`, `func_query` and `list_globals` filters are globs; `find_regex` is a real regex.
- `lookup_funcs` and `decompile` need the exact mangled name.
- `decompile` truncates near 44 KB and hides inlined `EventCoordinator::sendX`; find dispatchers with `xrefs_to` on the `_Func_impl_no_alloc` lambda vtable from `list_globals`.
- Reach a specific instruction with `disasm(func, offset, max_instructions)` paging or `xrefs_to` on the import thunk.
- Use `func_profile(include_lists:true)` to triage a candidate; `include_lists:false` gives only `string_ref_count`. It never confirms.
- Check `get_bytes` before condemning an address `func_profile` calls "Not a function"; a tail-call thunk reads `48 89 D1 E9 ... CC`.
- Check `ida_funcs.get_func(ea).start_ea == ea` before treating an address as a function.
- `rename` every located helper to its mangled name in the target database as you go.

## Signatures

- Run `make_signature*` about 4 at a time, big functions solo, and retry once on the 60 s timeout; a timeout is indeterminate.
- Do not use `find_xref_signatures` on hot, virtual or factory targets. Use `xrefs_to(VA, limit:6)` then `make_signature([sites], max_length:16)`.
- Use `make_signature_for_function(name, format:"ida")` for direct patterns and check that `addr == 0x140000000 + src/bedrock/symbols/windows.h RVA`.
- Use `make_signature_for_range(start, end, wildcard_operands:true)` to encode a fixed range with no uniqueness requirement.
- When `make_signature` times out, hand-build it: `get_bytes` at the call site, assemble, then `find_bytes`.
- Widen each `?` of a toml pattern to `??` before `find_bytes`; one match keeps the entry, zero or two mark it stale.
- When every entry reads stale, run `get_bytes(VA, 16)` on one entry to confirm the bytes really changed before trusting the batch.
