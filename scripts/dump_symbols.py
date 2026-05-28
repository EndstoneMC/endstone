#!/usr/bin/env -S uv run --script
# /// script
# requires-python = ">=3.10"
# dependencies = [
#     "click",
#     "colorlog",
#     "lief",
#     "requests",
#     "tomlkit",
#     "tqdm",
# ]
# ///
"""
Scan a Minecraft Bedrock Dedicated Server (BDS) binary for function signatures
and write the resolved symbol offset table consumed by Endstone's runtime
hooking layer.

How it works:
  1. Download the BDS zip for a version from the bedrock-server-data repo.
  2. Parse the server binary (PE on Windows, ELF on Linux) with lief.
  3. Pattern-match byte signatures in the `.text` section to locate offsets.
  4. Group results by C++ scope and write a per-platform symbols file.

With `--pdb`, a Windows config's symbols are resolved by name from a PDB
instead (via `pdbtool` -- install it with `cargo install pdbtool`). Names with
no public record in the PDB -- lambdas, function-local statics -- fall back to
a byte-pattern scan of the PE (read next to the PDB, or downloaded).

The signature configs live in `scripts/configs/` next to this script. Each
scanned config writes `src/bedrock/symbols/<platform>.h`, a `std::array` of
`{ mangled_name, offset }` pairs that `src/bedrock/symbol.h` includes for the
platform being built. The header is committed -- run this script to refresh it
when bumping BDS.

Dependencies are declared inline (PEP 723) and resolved by uv into an ephemeral
environment -- no manual install. Bump each config's `version` key first, then
pass the platform config(s) to scan:
    # Both platforms
    uv run --script scripts/dump_symbols.py scripts/configs/windows.toml scripts/configs/linux.toml

    # Windows only
    uv run --script scripts/dump_symbols.py scripts/configs/windows.toml

    # Windows from a PDB (resolve by name, byte-pattern fallback for the rest)
    uv run --script scripts/dump_symbols.py scripts/configs/windows.toml --pdb bedrock_server.pdb

Config format -- each `scripts/configs/<platform>.toml` has top-level `version`
and `platform` keys plus a list of `[[signatures]]`:
    name     (str)  Mangled C++ symbol name (MSVC `?...` or Itanium `_Z...`).
    pattern  (str)  Space-separated hex bytes, `?` for a wildcard byte.
    relative (bool) If false, the section base address is added to the result.
    extra    (int)  Constant added to the final resolved offset.
    offsets  (list) Optional. Byte offsets for pointer-chain dereferencing.
    rip_relative / rip_offset  Optional. Resolve x86-64 RIP-relative addressing.

Adapted from EndstoneMC/bedrock-dumper (MIT).
"""

import ctypes
import hashlib
import logging
import os
import re
import shutil
import struct
import subprocess
import sys
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path
from zipfile import ZipFile

import click
import colorlog
import lief
import requests
import tomlkit

# --- Paths --------------------------------------------------------------------

SCRIPT_DIR = Path(__file__).resolve().parent
REPO_ROOT = SCRIPT_DIR.parent
OUTPUT_DIR = REPO_ROOT / "src" / "bedrock" / "symbols"

# --- BDS download -------------------------------------------------------------

# Raw file root of the EndstoneMC/bedrock-server-data repo.
BASE_RAW_URL = "https://raw.githubusercontent.com/EndstoneMC/bedrock-server-data/v2"
# Cached server zips live here, keyed by SHA256.
CACHE_DIR = Path.home() / ".bedrock_server"
HEADERS = {
    "User-Agent": (
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
        "(KHTML, like Gecko) Chrome/129.0.0.0 Safari/537.36 Edg/129.0.0.0"
    )
}

# --- Logging ------------------------------------------------------------------

handler = colorlog.StreamHandler(sys.stdout)
handler.setFormatter(
    colorlog.ColoredFormatter(
        "%(log_color)s%(levelname)s %(message)s",
        log_colors={
            "DEBUG": "cyan",
            "INFO": "reset",
            "WARNING": "yellow",
            "ERROR": "red",
            "CRITICAL": "red,bg_white",
        },
    )
)
logging.basicConfig(level=logging.INFO, handlers=[handler])
logger = logging.getLogger(__name__)


def compute_sha256(file_path: Path) -> str:
    """Compute the SHA256 hash of a local file."""
    h = hashlib.sha256()
    with open(file_path, "rb") as f:
        for chunk in iter(lambda: f.read(8192), b""):
            h.update(chunk)
    return h.hexdigest()


def download_file(url: str, dest_path: Path) -> Path:
    """Download a file to dest_path, showing a progress bar when possible."""
    dest_path.parent.mkdir(parents=True, exist_ok=True)

    with requests.get(url, headers=HEADERS, stream=True) as r:
        r.raise_for_status()
        total_size = int(r.headers.get("content-length", 0))

        with open(dest_path, "wb") as f:
            if total_size:
                from tqdm import tqdm

                with tqdm(
                    total=total_size,
                    unit="B",
                    unit_scale=True,
                    desc=f"Downloading {dest_path.name}",
                ) as pbar:
                    for chunk in r.iter_content(chunk_size=8192):
                        if chunk:
                            f.write(chunk)
                            pbar.update(len(chunk))
            else:
                for chunk in r.iter_content(chunk_size=8192):
                    if chunk:
                        f.write(chunk)

    return dest_path


def download_server(version: str, platform: str) -> Path:
    """
    Download (or reuse a cached copy of) the BDS server zip for a release
    version. A cached file is reused only when its SHA256 matches metadata.
    """
    url = f"{BASE_RAW_URL}/release/{version}/metadata.json"
    resp = requests.get(url, headers=HEADERS)
    resp.raise_for_status()
    info = resp.json().get("binary", {}).get(platform)
    if not info or "url" not in info or "sha256" not in info:
        raise KeyError(f"No {platform} binary info for version {version}")

    download_url = info["url"]
    expected_hash = info["sha256"]
    dest_path = CACHE_DIR / platform / os.path.basename(download_url)

    if dest_path.exists() and compute_sha256(dest_path).lower() == expected_hash.lower():
        logger.info(f"Using cached {platform} binary: {dest_path}")
        return dest_path

    return download_file(download_url, dest_path)


# --- C++ scope parsing --------------------------------------------------------


def get_scopes_msvc(mangled: str) -> str | None:
    """Extract the enclosing C++ scope from an MSVC-mangled name."""
    # Example: ?registerOverloadInternal@CommandRegistry@@AEAAX...
    if not mangled.startswith("?"):
        return None
    i = 1
    if mangled[i] == "?":
        i += 1
        if i < len(mangled) and mangled[i] in "01":
            i += 1
        else:
            return None
    else:
        try:
            i = mangled.index("@", 1) + 1
        except ValueError:
            return None

    rest = mangled[i:]
    end = rest.find("@@")
    if end == -1:
        return None
    scopes = [s for s in rest[:end].split("@") if s]
    if not scopes:
        return None

    return "::".join(reversed(scopes))


def get_scopes_itanium(mangled: str, innermost: bool = False) -> str | None:
    """Extract the enclosing C++ scope from an Itanium-mangled name."""
    if not mangled.startswith("_Z"):
        return None
    i = 2
    if i >= len(mangled) or mangled[i] != "N":
        return None
    i += 1
    while i < len(mangled) and mangled[i] in "KVRO":
        i += 1

    names = []
    saw_ctor_dtor = False

    while i < len(mangled) and mangled[i] != "E":
        if mangled[i].isdigit():
            n = 0
            while i < len(mangled) and mangled[i].isdigit():
                n = n * 10 + (ord(mangled[i]) - 48)
                i += 1
            if i + n > len(mangled):
                return None
            ident = mangled[i : i + n]
            i += n
            names.append(ident)

            if i < len(mangled) and mangled[i] == "I":
                depth = 1
                i += 1
                while i < len(mangled) and depth:
                    if mangled[i] == "I":
                        depth += 1
                    elif mangled[i] == "E":
                        depth -= 1
                    elif mangled[i].isdigit():
                        j, ln = i, 0
                        while j < len(mangled) and mangled[j].isdigit():
                            ln = ln * 10 + (ord(mangled[j]) - 48)
                            j += 1
                        i = j + ln
                        continue
                    i += 1
            continue

        if mangled[i] in "CD":
            saw_ctor_dtor = True
            i += 1
            if i < len(mangled) and mangled[i] in "0123":
                i += 1
            continue

        return None

    if i >= len(mangled) or mangled[i] != "E":
        return None
    if len(names) < 1:
        return None

    if saw_ctor_dtor:
        return names[-1] if innermost else "::".join(names)
    # Regular member: drop the unqualified name at the end.
    if len(names) < 2:
        return None
    return names[-2] if innermost else "::".join(names[:-1])


# --- Signature scanning -------------------------------------------------------


@dataclass
class Signature:
    name: str
    relative: bool
    extra: int
    pattern: str
    offsets: list[int] = None
    rip_relative: bool = False
    rip_offset: int = 0


def find_signature(section: lief.Section, sig: Signature) -> int:
    """Resolve a single signature to an offset within the binary."""
    logger.debug(f"Begin scan {sig.name}")
    logger.debug(f"Searching for pattern: {sig.pattern}")

    pattern = ""
    for s in sig.pattern.split():
        pattern += "\\x" + s if s != "?" else "."
    byte_pattern = pattern.encode("utf-8")

    mem = section.content
    base_addr = section.virtual_address

    match = re.search(byte_pattern, mem.tobytes(), re.DOTALL)
    if not match:
        raise NameError(f"Pattern not found: {sig.pattern} for {sig.name}")
    addr = match.start()

    logger.debug(f"Pattern found at: 0x{addr:x} (+ base = 0x{(addr + base_addr):x})")

    for i, o in enumerate(sig.offsets or []):
        logger.debug(f"Offset {i}: ptr: 0x{addr:x} offset: 0x{o:x}")
        pos = addr + o
        addr = struct.unpack_from("<I", mem, pos)[0]
        logger.debug(f"Offset {i}: => 0x{addr:x}")

    if sig.rip_relative:
        logger.debug(f"rip_relative: addr 0x{addr:x} + rip_offset 0x{sig.rip_offset:x}")
        addr = addr + sig.rip_offset
        rip = struct.unpack_from("<i", mem, addr)[0]
        logger.debug(f"rip_relative: addr 0x{addr:x} + rip 0x{rip:x} + 4")
        addr = addr + rip + 4
        logger.debug(f"rip_relative: addr => 0x{addr:x}")

    logger.debug(f"Adding extra: 0x{sig.extra:x}")
    addr = addr + sig.extra

    if not sig.relative:
        logger.debug(f"Not relative, addr 0x{addr:x} + base 0x{base_addr:x}")
        addr = addr + base_addr

    return addr


def scan_signatures(section: lief.Section, config: dict) -> dict[str, int]:
    """Resolve every signature in a platform config. Unresolved signatures map to 0."""
    signatures = config["signatures"]
    logger.info(f"Starting signature scanning: {len(signatures)} items")
    result = {}

    for signature_dict in signatures:
        sig = Signature(**signature_dict)
        try:
            offset = find_signature(section, sig)
            logger.info(f"Found signature: {sig.name} => 0x{offset:x}")
        except NameError as e:
            offset = 0
            logger.error(e)

        result[sig.name] = offset

        if offset % 8 != 0:
            logger.warning(f"Offset 0x{offset:x} is not divisible by 8.")

    found = len([v for v in result.values() if v != 0])
    logger.info(f"Finished signature scanning: {found}/{len(signatures)} items successful")
    return result


# --- PDB symbol lookup --------------------------------------------------------

# Symbol offsets are read from a PDB with Microsoft's `pdbtool` (from the pdb-rs
# project, https://github.com/microsoft/pdb-rs -- `cargo install pdbtool`). It
# dumps the public symbol table in seconds, where dbghelp takes minutes on a
# large PDB. PDBs are a Windows-only debug format, all that --pdb targets.

# `pdbtool dump <pdb> psi` line: "... S_PUB32: [ssss:oooooooo], flags: ..., NAME"
_PUBLIC_RE = re.compile(r"S_PUB32: \[([0-9a-fA-F]+):([0-9a-fA-F]+)\], flags: [0-9a-fA-F]+, (.+)")
# `pdbtool dump <pdb> sections` line: "s [ssss:00000000] rva: rrrrrrrr + ..."
_SECTION_RE = re.compile(r"^s \[([0-9a-fA-F]+):[0-9a-fA-F]+\] rva: ([0-9a-fA-F]+) ")
_UNDNAME_NAME_ONLY = 0x1000


def _find_pdbtool() -> str:
    """Locate the pdbtool executable on PATH or in the default cargo bin dir."""
    found = shutil.which("pdbtool")
    if found:
        return found
    cargo_bin = Path.home() / ".cargo" / "bin" / "pdbtool.exe"
    if cargo_bin.is_file():
        return str(cargo_bin)
    raise click.ClickException("pdbtool not found -- install it with `cargo install pdbtool`")


def _pdbtool_lines(pdbtool: str, pdb_path: Path, command: str):
    """Run `pdbtool dump <pdb> <command>` and yield its stdout lines."""
    proc = subprocess.Popen(
        [pdbtool, "--quiet", "dump", str(pdb_path), command],
        stdout=subprocess.PIPE,
        text=True,
        encoding="utf-8",
        errors="replace",
    )
    try:
        yield from proc.stdout
    finally:
        proc.stdout.close()
        if proc.wait() != 0:
            raise click.ClickException(f"pdbtool dump {command} failed for {pdb_path}")


def _load_public_symbols(pdbtool: str, pdb_path: Path) -> dict[str, int]:
    """Return {decorated name -> RVA} for every public symbol in the PDB."""
    section_rva = {}
    for line in _pdbtool_lines(pdbtool, pdb_path, "sections"):
        match = _SECTION_RE.match(line)
        if match:
            section_rva[int(match.group(1), 16)] = int(match.group(2), 16)

    publics = {}
    for line in _pdbtool_lines(pdbtool, pdb_path, "psi"):
        match = _PUBLIC_RE.search(line)
        if match:
            base = section_rva.get(int(match.group(1), 16))
            if base is not None:
                publics[match.group(3).rstrip()] = base + int(match.group(2), 16)
    return publics


def _undecorate(name: str) -> str:
    """Undecorate an MSVC symbol name to its fully-qualified `Scope::name` form."""
    buffer = ctypes.create_unicode_buffer(2048)
    if ctypes.windll.dbghelp.UnDecorateSymbolNameW(name, buffer, len(buffer), _UNDNAME_NAME_ONLY):
        return buffer.value
    return ""


def _scan_pe_fallback(pdb_path: Path, config: dict, unresolved: list, result: dict[str, int]) -> None:
    """
    Byte-pattern scan the PE for `unresolved` config entries -- symbols the PDB
    has no public record of (lambdas, function-local statics). Updates `result`
    in place. The PE is read from next to the PDB when present, else downloaded.
    """
    exe_name = str(config.get("executable", "bedrock_server.exe"))
    local_exe = pdb_path.parent / exe_name
    if local_exe.is_file():
        logger.info(f"Byte-pattern fallback for {len(unresolved)} symbol(s) via {local_exe}")
        raw = local_exe.read_bytes()
    else:
        logger.info(f"Byte-pattern fallback for {len(unresolved)} symbol(s); downloading PE")
        zip_path = download_server(str(config["version"]), "windows")
        with ZipFile(zip_path, "r") as zf:
            with zf.open(exe_name) as fh:
                raw = fh.read()

    text_section = lief.parse(raw).get_section(".text")
    if text_section is None:
        raise click.ClickException("No .text section in the PE for the byte-pattern fallback")

    for entry in unresolved:
        sig = Signature(**entry)
        try:
            offset = find_signature(text_section, sig)
        except NameError as e:
            logger.error(f"Fallback scan failed: {e}")
            continue
        result[sig.name] = offset
        logger.info(f"Found signature (fallback): {sig.name} => 0x{offset:x}")


def scan_pdb(pdb_path: Path, config: dict) -> dict[str, int]:
    """
    Resolve every config symbol by name from a PDB, returning name -> RVA.

    Each entry's `name` is looked up in the PDB's public symbol table. Names the
    PDB has no public record for -- lambdas, function-local statics -- fall back
    to a byte-pattern scan of the PE via `find_signature`. Symbols that neither
    path resolves map to 0.
    """
    if sys.platform != "win32":
        raise click.ClickException("--pdb requires Windows; PDB lookup uses pdbtool and dbghelp")

    names = [str(entry["name"]) for entry in config["signatures"]]
    pdbtool = _find_pdbtool()
    logger.info(f"Reading public symbols from {pdb_path.name} via pdbtool")
    publics = _load_public_symbols(pdbtool, pdb_path)
    logger.info(f"Loaded {len(publics)} public symbols; resolving {len(names)}")

    result = {name: publics.get(name, 0) for name in names}

    # Data symbols are listed by their pretty `Scope::member` name, not a
    # mangled key. Undecorate the few publics with a matching leaf to find them.
    misses = [n for n, rva in result.items() if rva == 0 and not n.startswith("?")]
    if misses:
        leaf_prefixes = tuple(f"?{n.rsplit('::', 1)[-1]}@" for n in misses)
        wanted = set(misses)
        for decorated, rva in publics.items():
            if decorated.startswith(leaf_prefixes) and _undecorate(decorated) in wanted:
                result[_undecorate(decorated)] = rva

    for name in names:
        rva = result[name]
        if rva:
            logger.info(f"Found symbol: {name} => 0x{rva:x}")
        else:
            logger.error(f"Symbol not found in PDB: {name}")

    # Names the PDB has no public record for fall back to a byte-pattern scan.
    unresolved = [entry for entry in config["signatures"] if result[str(entry["name"])] == 0]
    if unresolved:
        _scan_pe_fallback(pdb_path, config, unresolved, result)

    found = len([v for v in result.values() if v != 0])
    logger.info(f"Finished symbol resolution: {found}/{len(names)} symbols resolved")
    return result


# --- CLI ----------------------------------------------------------------------


@click.command()
@click.argument(
    "inputs",
    nargs=-1,
    required=True,
    type=click.Path(exists=True, dir_okay=False, path_type=Path),
)
@click.option(
    "--pdb",
    "pdb_path",
    type=click.Path(exists=True, dir_okay=False, path_type=Path),
    default=None,
    help="PDB for the Windows binary. When given, the windows config's symbols "
    "are resolved by name from the PDB; names the PDB lacks fall back to a "
    "byte-pattern scan of the PE (read next to the PDB, or downloaded).",
)
def main(inputs: tuple[Path, ...], pdb_path: Path | None) -> None:
    for config_path in inputs:
        with config_path.open("r") as f:
            config = tomlkit.load(f)
        version = str(config["version"])
        platform = str(config["platform"])
        if platform not in ("windows", "linux"):
            raise click.ClickException(f"{config_path}: unknown platform '{platform}'")

        if pdb_path is not None and platform == "windows":
            logger.info(f"Resolving {platform} symbols from {pdb_path}")
            sigs = scan_pdb(pdb_path, config)
        else:
            if pdb_path is not None:
                logger.warning(f"--pdb applies only to a windows config; scanning {config_path} by signature")

            zip_path = download_server(version, platform)

            binary_name = "bedrock_server" if platform == "linux" else "bedrock_server.exe"
            with ZipFile(zip_path, "r") as zf:
                with zf.open(binary_name) as fh:
                    logger.info(f"Reading {platform} binary from {zip_path}")
                    raw = fh.read()

            binary = lief.parse(raw)
            if isinstance(binary, lief.PE.Binary):
                assert platform == "windows", "Platform mismatch."
            elif isinstance(binary, lief.ELF.Binary):
                assert platform == "linux", "Platform mismatch."
            else:
                raise ValueError("Unsupported binary type.")

            text_section = binary.get_section(".text")
            assert text_section is not None, "No .text section found."

            sigs = scan_signatures(text_section, config)

        get_scope = get_scopes_msvc if platform == "windows" else get_scopes_itanium
        group_by_scope = defaultdict(dict)
        for name, offset in sigs.items():
            group_by_scope[get_scope(name)][name] = offset

        output_file = OUTPUT_DIR / f"{platform}.h"
        write_symbols_header(output_file, platform, version, group_by_scope)
        logger.info(f"Wrote {output_file}")


# --- Header writer ------------------------------------------------------------


def write_symbols_header(
    output_file: Path,
    platform: str,
    version: str,
    group_by_scope: dict[str | None, dict[str, int]],
) -> None:
    """
    Write `std::array<std::pair<std::string_view, std::size_t>, N> symbols`
    grouped by C++ scope, matching what the old `symbol_generator` produced.
    Unresolved (`0`) entries are dropped -- `symbol.h`'s `consteval`
    `get_symbol()` then throws at compile time for any consumer that names them.
    """
    non_zero = sum(1 for sigs in group_by_scope.values() for v in sigs.values() if v != 0)

    lines: list[str] = []
    lines.append(f"// Generated by scripts/dump_symbols.py for BDS {version} ({platform}). DO NOT EDIT.")
    lines.append("// NOLINTBEGIN")
    lines.append("")
    lines.append("#pragma once")
    lines.append("")
    lines.append("#include <array>")
    lines.append("#include <cstddef>")
    lines.append("#include <string_view>")
    lines.append("#include <utility>")
    lines.append("")
    lines.append("namespace endstone::runtime {")
    lines.append("")
    lines.append(
        f"static constexpr std::array<std::pair<std::string_view, std::size_t>, {non_zero}> symbols = {{{{"
    )
    for scope, scoped_sigs in sorted(group_by_scope.items(), key=lambda x: x[0] or ""):
        if scope is not None:
            lines.append(f"    // {scope}")
        for name, offset in sorted(scoped_sigs.items(), key=lambda x: x[0]):
            if offset == 0:
                continue
            lines.append(f'    {{"{name}", {offset}}},')
    lines.append("}};")
    lines.append("")
    lines.append("}  // namespace endstone::runtime")
    lines.append("")
    lines.append("// NOLINTEND")
    lines.append("")

    output_file.parent.mkdir(parents=True, exist_ok=True)
    output_file.write_text("\n".join(lines), encoding="utf-8")


if __name__ == "__main__":
    main()
