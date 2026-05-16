---
name: bump-bds-version
description: Update Endstone to support a new Bedrock Dedicated Server (BDS) version - regenerate the symbol offset tables and fix the symbols whose ABI changed. Use when bumping the supported BDS version (e.g. "add support for BDS 1.26.x", "bump the BDS version").
---

# Bump Endstone to a new BDS version

Endstone hooks the BDS binary at fixed offsets, so each supported BDS version
needs its own symbol offset table. This skill regenerates those tables and
walks the fixes needed for symbols whose ABI changed.

## How the symbol pipeline works

```
scripts/configs/{windows,linux}.toml          signature configs, hand-maintained
        |  scripts/dump_symbols.py
        v
src/bedrock/symbol_generator/symbols/{windows,linux}.toml   resolved offsets
        |  symbol_generator (CMake target)
        v
bedrock_symbols.generated.h                    std::array of name -> offset
```

`src/bedrock/symbol.h` `get_symbol()` looks a symbol up by `__FUNCDNAME__` - the
mangled name of Endstone's own declaration in `src/bedrock/`. The symbol-table
key IS the signature of Endstone's reimplementation. Unresolved symbols are
written as `0` and skipped by the generator (that hook is disabled; the build
still succeeds).

## Prerequisites

- `uv` - runs `dump_symbols.py` (PEP 723 inline deps, no manual install).
- `pdbtool` - `cargo install pdbtool` (Microsoft pdb-rs). Reads the Windows PDB.
- The Windows BDS PDB (`bedrock_server.pdb`) for the target version.
- The target version must be published in the `EndstoneMC/bedrock-server-data`
  repo - the Linux path downloads the binary from it. Check its `versions.json`.

## Procedure

### 1. Branch

Branch off the current release branch (e.g. `v0.11`):
`git checkout -b feat/<NN.NN>-support` (naming follows `feat/26.10-support`).

### 2. Bump the config versions

Set `version = "<X.Y.Z>"` in both `scripts/configs/windows.toml` and
`scripts/configs/linux.toml`. Use the 3-component release string exactly as
listed in bedrock-server-data `versions.json` (e.g. `1.26.20`).

### 3. Regenerate the symbol tables

Run in the background - these take 25 s to a few minutes:

- Windows (reads the PDB, resolves by name):
  `uv run --script scripts/dump_symbols.py scripts/configs/windows.toml --pdb <path>/bedrock_server.pdb`
- Linux (downloads the BDS, scans byte signatures):
  `uv run --script scripts/dump_symbols.py scripts/configs/linux.toml`

Each rewrites `src/bedrock/symbol_generator/symbols/<platform>.toml`. The log
ends with `Finished ...: N/68 ... resolved`.

### 4. Triage the failures

The two platforms fail for different reasons - do not read a `0` the same way:

- **Windows (PDB, by name):** a miss means the mangled name is no longer in the
  PDB, i.e. the function's signature changed (MSVC mangling encodes the full
  signature, including the return type). A Windows hit is name-verified.
- **Linux (byte-pattern scan):** a miss means the `pattern` in
  `configs/linux.toml` no longer matches the new binary. The function usually
  still exists - the pattern just went stale. A Linux hit is a pattern match
  *labelled* with the config name; it is not name-verified, so a hit on a
  changed-signature function can be wrong.

Cross-reference the two failure lists:

- Failed on **both** -> almost certainly a real signature/API change.
- Failed on **Windows only** -> mangled name changed. If only the return type
  changed, the Linux Itanium name is unaffected (Itanium mangling omits return
  types) and Linux still resolves it.
- Failed on **Linux only** -> stale byte pattern; the function is fine, re-
  extract the signature.

### 5. Fix the changed symbols

For each symbol that genuinely changed:

1. Find the new signature in the PDB: `pdbtool dump <pdb> psi`, grep for the
   `?name@Class@@` prefix (stable across signature changes), decode old vs new.
2. Update the matching declaration in `src/bedrock/` to the new ABI. This is
   what makes `__FUNCDNAME__` produce the new mangled name.
3. Update the `name` in `scripts/configs/windows.toml` (MSVC mangling) and
   `scripts/configs/linux.toml` (Itanium mangling).
4. For Linux byte-pattern misses, re-extract a `pattern` from the new ELF
   (IDA / Ghidra) and update `configs/linux.toml`.
5. Update affected hook code in `src/endstone/runtime/bedrock_hooks/`.

Symbols that are not public - function-local statics like `getI18n::result`,
some data - never appear in `pdbtool ... psi`. They stay on the byte-signature
path; resolve them with a `pattern`, not a PDB lookup.

### 6. Re-run, build, verify

Re-run the dumper until everything resolves (or the remaining `0`s are
understood and intentional). Build (`cmake --build --preset conan-release`),
run the tests.

### 7. Finish

Add a CHANGELOG entry under the new version (user-visible, e.g. "Added support
for BDS X.Y.Z"). Commit and open the PR.

## Gotchas

- **lief's free build cannot read PDBs.** `lief.pdb.load()` fails with
  "DebugInfo are not available for this build" - PDB/DWARF support is
  LIEF-extended (licensed). This is why the Windows path shells out to
  `pdbtool`.
- **dbghelp is too slow.** Loading a ~2 GB BDS PDB through dbghelp takes
  minutes; `pdbtool dump ... psi` takes ~25 s.
- **PowerShell `>` writes UTF-16.** A redirected `pdbtool` dump will not grep
  with ASCII patterns - capture via the script's subprocess (UTF-8), or run
  `iconv -f UTF-16 -t UTF-8` on the file first.
- Windows symbol offsets are RVAs: `RVA = section RVA + symbol offset`
  (`dump_symbols.py` reads both from `pdbtool`).

## Record findings here

Append durable, version-independent lessons to **Gotchas** as the process
teaches them. Keep per-version specifics (which overload was chosen, what
replaced a removed function) in commit messages and the CHANGELOG, not here.
