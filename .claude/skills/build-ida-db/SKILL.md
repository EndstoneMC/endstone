---
name: build-ida-db
description: Download the Bedrock Dedicated Server (BDS) binary for Windows and Linux and run idat.exe headless to build the IDA database (.i64) for each, in the background, reporting progress. Use when asked to "build/create the IDA database", "make the .i64 for BDS", "download BDS and run IDA on it", or to prep databases for the update-signatures / bump-bds work.
---

# Build the IDA databases for BDS (Windows + Linux)

Downloads the BDS server binary for both platforms and runs `idat.exe` headless
on each to produce an IDA database (`.i64`). Both analyses are long-running
(hours on the ~170 MB binary, ~5 GB output each), so they run **in the
background** and this skill reports progress as it goes.

The resulting `.i64` is the input the `update-signatures` and `bump-bds` skills
consume (e.g. `../bedrock-symbols/win32_server_x64/<version>/bedrock_server.exe.i64`).

## Conventions (paths)

All commands and paths below are **relative to the repo root** (the working
directory) and use POSIX forward slashes; run the skill from the repo root so
the relative paths resolve.

| What | Where |
|---|---|
| `idat.exe` (headless IDA) | discovered at launch - see step 0 (not hardcoded) |
| BDS zip cache | `~/.bedrock_server/<platform>/bedrock-server-<ver>.zip` |
| DB output (windows) | `../bedrock-symbols/win32_server_x64/<version>/bedrock_server.exe` (+ `.i64`) |
| DB output (linux) | `../bedrock-symbols/linux_server_x64/<version>/bedrock_server` (+ `.i64`) |
| Skill helpers | `.claude/skills/build-ida-db/{fetch_bds.py,make_idb.py}` |

`<version>` is the 3-component release (e.g. `1.26.30`), read by default from
`scripts/configs/<platform>.toml`'s `version` key - the same value `bump-bds`
sets. The `bedrock-symbols` tree sits **beside** the repo (`../bedrock-symbols`),
not inside it. `fetch_bds.py` prints the extracted binary's path repo-relative
with forward slashes, so the `idat` command lines stay relative too.

## Prerequisites

- IDA Professional 9.x installed (single `idat.exe` handles both 32/64-bit and
  writes `.i64`; there is no separate `idat64.exe` in IDA 9). Its path is found
  at launch (step 0), not hardcoded.
- `uv` for the helper scripts (PEP 723 inline deps, no manual install).
- The target version must exist in `EndstoneMC/bedrock-server-data` (`fetch_bds.py`
  reads its `release/<version>/metadata.json`). The zips for the current target
  are usually already cached under `~/.bedrock_server/`.
- **Disk + RAM:** each analysis writes a ~5 GB `.i64` plus ~4 GB of unpacked
  `.id0/.id1/...` during the run, and holds the binary in memory. Two in
  parallel is fine on a roomy machine; if RAM/disk is tight, run them
  sequentially (launch linux only after windows finishes).

## Procedure

Drive this as **small, reported steps** - the user wants visible progress, not
one silent multi-hour block.

### 0. Locate idat.exe (common install locations)

IDA's install path varies by edition/version, so discover it instead of
hardcoding. List **every** `idat.exe` in the usual roots (plus one on PATH):

```bash
shopt -s nullglob
printf '%s\n' \
  "/c/Program Files/"IDA*/idat.exe \
  "/c/Program Files (x86)/"IDA*/idat.exe \
  "$HOME/AppData/Local/Programs/"IDA*/idat.exe | sort -uV
command -v idat 2>/dev/null   # also report one on PATH, if any
```

`printf` + shell globbing (not `ls`) keeps the paths clean - an aliased `ls -F`
would append a `*` classifier and break them. Then pick based on the count:

- **No candidates** -> IDA is not in a common location; ask the user for the
  `idat.exe` path (or check other drives / a portable install).
- **Exactly one** -> use it.
- **More than one** (e.g. `IDA Professional 9.0` and `9.1` side by side) ->
  **ask the user which install to use** (via AskUserQuestion; list the
  discovered paths as options) - do **not** silently auto-pick a version.

Carry the chosen `idat.exe` path into step 4 by substituting it literally
(quoted - the path has spaces); shell state does not persist between tool calls.

### 1. Resolve the version

Read `version` from `scripts/configs/windows.toml` (and confirm `linux.toml`
matches). Override with an explicit version only if the user names one.

### 2. Fetch + extract both binaries

```bash
uv run --script .claude/skills/build-ida-db/fetch_bds.py --platform windows
uv run --script .claude/skills/build-ida-db/fetch_bds.py --platform linux
```

Each reuses the cached zip when its SHA256 matches, else downloads it, then
extracts the server binary into `../bedrock-symbols/<arch>/<version>/`. The
**last stdout line is the extracted binary path** (repo-relative, forward
slashes) - capture it for the next step. These are quick (seconds when cached);
run them in the foreground.

### 3. (Optional) named database - drop the PDB/symbols beside the binary first

A bare analysis of the stripped server binary yields a database with **no
function names** - usable for raw disassembly but a non-starter for
`update-signatures` (which resolves symbols by mangled name). To get the named
DB those skills need, place the matching debug artifact **next to the extracted
binary before step 4** so IDA auto-applies it during analysis:

- Windows: `bedrock_server.pdb` beside `bedrock_server.exe`.
- Linux: the server binary is stripped; there is no public symbol file, so the
  linux DB stays nameless (this matches how the toolchain already treats it).

If you have the matching `bedrock_server.pdb`, drop it beside the binary first -
step 4 detects it automatically and leaves the PDB provider on so names load.
Keep the PDB and its contents out of the repo. If no PDB is available, proceed
without it (step 4 adds `-Opdb:off`) and note the DB will be unnamed.

### 4. Launch idat headless in the background, one per platform

For each platform, launch with the **Bash tool's `run_in_background: true`** so
the harness tracks it and notifies on completion. Use the `idat.exe` path chosen
in step 0 (quote it - the path has spaces) and point `-L` at a log file beside
the binary:

**Decide `-Opdb:off` by whether a PDB sits beside the binary** - add it only
when there is *no* PDB; with a PDB present it must stay off-the-command-line so
IDA loads the PDB for names. The launch snippet checks and sets it itself:

```bash
# <IDAT> = idat.exe chosen in step 0; <BIN> = path printed by fetch_bds.py; <DIR> = its parent
shopt -s nullglob
pdbs=("<DIR>"/*.pdb)
PDBOFF="-Opdb:off"; [ ${#pdbs[@]} -gt 0 ] && PDBOFF=""   # PDB present -> let IDA load it
"<IDAT>" -A -c $PDBOFF -L"<DIR>/idat.log" -S".claude/skills/build-ida-db/make_idb.py" "<BIN>"
```

- `-A` autonomous (no dialogs), `-c` create a fresh DB (overwrites any existing
  one), `-S` runs `make_idb.py` which waits for auto-analysis, saves, and quits,
  `-L` captures the message log for progress. This avoids the giant `.asm`
  byproduct that `idat -B` would produce.
- **Why the `-Opdb:off` check matters.** With **no PDB** beside the binary, the
  PE's embedded PDB reference makes IDA probe `msdl.microsoft.com`, 404, then
  throw a **blocking "Do you want to browse for the pdb file on disk?" prompt**
  that hangs headless `idat` forever (`-A` auto-answers Yes -> a file dialog with
  no stdin -> stuck). `-Opdb:off` turns the PDB provider off so this never
  happens. With a **PDB present**, `-Opdb:off` would also skip the local PDB
  (provider off = loads nothing), so it is omitted; the provider then finds the
  local PDB first and never reaches the network/browse fallback. (A bare ELF -
  the linux binary - has no PDB reference, so the flag is a harmless no-op there;
  the check simply adds it.)
- Launch windows and linux as **two separate background jobs** (or sequentially
  if RAM/disk is tight - see Prerequisites).

### 5. Report progress while they run

Don't poll on a timer - when a background job exits, the harness re-invokes you.
Between events, check progress on demand with cheap reads (don't open the `.i64`
in the idalib MCP while idat holds it):

- **CPU delta - the reliable "alive and working" signal.** Sample each idat
  PID's `TotalProcessorTime` ~8 s apart; an active analysis adds roughly a full
  core's worth (e.g. +8 s over 8 s). A near-**zero** increase means it stalled
  (e.g. blocked on a prompt - see the PDB gotcha), *not* that it's done.
- **Log tail** - phase lines (`Reading ... directory`, `Parsing .pdata ...`) plus
  `make_idb.py`'s own `[make_idb] ...` markers. The linux run reaches
  `[make_idb] load complete; waiting for auto-analysis...` quickly. Note `-L`
  **appends** across reruns, so read from the last `Loading file` header. A
  benign `Syntax error near: <END>` around import/`ws2_32` type loading is IDA
  noise, not a failure.
- **Do NOT use `.id1` disk size for live progress.** IDA memory-maps the b-tree
  and only flushes on save, so a perfectly healthy run shows a *static* `.id1`
  for long stretches - it looks identical to a hang. Use CPU + the log instead.

Completion = the process exits 0, the log ends with `[make_idb] saved; exiting.`,
and a finalized `.i64` exists (the unpacked `.id*` may remain). Report per
platform: queued -> analyzing (latest log phase + CPU climbing) -> saved.

### 6. Completion

A platform is done when its idat process exits 0, `idat.log` ends with
`[make_idb] saved; exiting.`, and `bedrock_server[.exe].i64` exists in the dest
dir. Report the two final `.i64` paths and sizes. If a run exits non-zero or the
log stops mid-phase, surface the tail of `idat.log` - don't claim success.

## Gotchas

- **`idat.exe`, not `ida.exe`** - `idat` is the text-mode/headless launcher;
  `ida.exe` is the GUI and will pop a window. IDA 9 unified the bitness, so
  `idat.exe` produces `.i64` for the 64-bit server binaries automatically.
- **A nameless DB is expected without a PDB.** The server binaries are stripped.
  Step 3 is what makes the Windows DB useful downstream; skip it only if the
  user just wants raw disassembly.
- **Don't open the new `.i64` in the idalib MCP while idat is writing it**, and
  don't run two idat processes against the *same* DB path. The Windows DB path
  collides with whatever the idalib MCP may already have open from a prior
  `update-signatures` run - if `-c` fails to delete the old DB, an idalib worker
  is likely holding it; close/unbind it first (see the `update-signatures`
  skill's dirty-DB recovery).
- **It is genuinely slow.** Full auto-analysis of a 170 MB binary is multi-hour
  and the `.i64` exceeds 5 GB. This is why it runs in the background; set
  expectations with the user up front.
- **PDB browse-prompt hang (Windows) - the big one.** The PE embeds a PDB
  reference, so without `-Opdb:off` IDA probes `msdl.microsoft.com`, 404s (Mojang
  PDBs aren't published there), then throws a **blocking "browse for the pdb file
  on disk?" prompt**. Headless `idat -A` auto-answers Yes and opens a file dialog
  that waits on stdin forever - the process hangs with **CPU at ~0** (the tell:
  a log ending at that prompt and no CPU progress; a frozen `.id1` is *not* the
  tell - healthy runs freeze it too). The fix is
  `-Opdb:off` (provider off), per `cfg/pdb.cfg`'s documented `-Opdb:off|msdia|pdbida`
  switch. `_NT_SYMBOL_PATH=` empty only suppresses the *network* leg, not the
  browse prompt - use `-Opdb:off`. With a real local PDB staged beside the binary,
  the provider finds it first and never reaches the prompt, so leave it on.
- **Re-runs:** `-c` recreates from scratch. To resume/keep an existing DB
  instead, drop `-c` and point idat at the `.i64` rather than the binary.

## Record findings here

Append durable, tooling-level lessons (idat flag quirks, analysis-time figures,
new platform dirs) to the Gotchas. Keep per-version specifics in commits.
