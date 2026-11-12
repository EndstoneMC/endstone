---
name: upgrade-bds
description: Moves Endstone to a new Bedrock Dedicated Server version by building IDA databases, regenerating the symbol offset tables, porting src/bedrock to the new ABI and proving it on a live server. Use when asked to bump the BDS version, add support for BDS X.Y.Z, port a new BDS release, refresh or re-cut the signatures, build the IDA database, reconcile headers that arrived late, or resume a partial bump branch.
---

# Upgrade BDS

Read first: `.claude/rules/re/` (all files), `.claude/rules/dev/hooks.md`, `.claude/rules/dev/threading.md`, `.claude/rules/github.md`.

1. Pick the mode: headers present for the target is headers mode, absent is binaries mode, a patch release of the same minor is a symbol refresh (steps 1-9 and 14-15 only).
2. Confirm the target is in `EndstoneMC/bedrock-server-data` `versions.json`; create `feat/<NN.NN>-support` from the release branch when asked; on a resumed branch build early and treat every earlier stage as unfinished.
3. Find `idat.exe` under the IDA install folders or on PATH; ask the user when none or several are found.
4. Fetch binaries in the foreground: `uv run --script .claude/skills/upgrade-bds/fetch_bds.py --platform windows`, then `--platform linux`; the last stdout line is the extracted path.
5. Build each database in the background, one job per platform: `"<IDAT>" -A -c [-Opdb:off] -L"<DIR>/idat.log" -S".claude/skills/upgrade-bds/make_idb.py" "<BIN>"`; on a failure show the log tail and do not rerun without asking.
6. While databases build, bump the BDS version surfaces that `re/symbols.md` lists.
7. Dump per platform: `uv run --script scripts/dump_symbols.py scripts/configs/<platform>.toml` and list every warning.
8. Open the previous named database and the target as separate sessions and have `reverse-engineer` locate and re-cut each stale entry, one at a time.
9. Re-run both dumpers until `N/N items successful` with no hard-failure warning and report each entry as kept, re-cut or unfound.
10. Port `src/bedrock` one class at a time by fanning `bedrock-developer` agents over disjoint batches of 10-20 files with one shared brief.
11. In headers mode follow the header diff stage by stage; in binaries mode follow the Linux RTTI vtable diff, constructor reads, asserted `sizeof` values and the `protocol-docs` diff.
12. When headers or a PDB land for an already ported version, sweep the whole diff and have `code-reviewer` audit each hunk read-only.
13. When `NetworkProtocolVersion` moved, update it and follow `add-protocol` in bedrock-protocol.
14. Regenerate both symbol tables, run `verify-change` and check each hooked mangled name against its declaration.
15. Run `validate-change` on Windows, Linux and NetherNet, iterating each crash from its frame.
16. Add one `### Added` CHANGELOG line; commit only when the user asks.
17. Report deviations, what remains unverified and edits handed back.
