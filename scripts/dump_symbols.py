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
instead, via the Windows DbgHelp API (dbghelp.dll -- no external tool). Names
the PDB can't pin to a unique address -- ICF-folded or overloaded functions --
fall back to a byte-pattern scan of the PE (read next to the PDB, or downloaded).

The signature configs live in `scripts/configs/` next to this script. Each
scanned config writes `src/bedrock/symbols/<platform>.h`, a `std::array` of
`{ mangled_name, offset }` pairs that `src/bedrock/symbol.h` includes for the
platform being built. The header is committed -- run this script to refresh it
when bumping BDS.

Dependencies are declared inline (PEP 723) and resolved by uv into an ephemeral
environment -- no manual install. Bump each config's `version` key first, then
pass the platform config(s) to scan:
    # Both platforms (defaults to configs/windows.toml + configs/linux.toml)
    uv run --script scripts/dump_symbols.py

    # Same, explicit
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
import struct
import sys
from collections import defaultdict
from ctypes import wintypes
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
CONFIGS_DIR = SCRIPT_DIR / "configs"
# Scanned when no config is passed on the command line.
DEFAULT_CONFIGS = (CONFIGS_DIR / "windows.toml", CONFIGS_DIR / "linux.toml")

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


# --- PDB symbol lookup (DbgHelp) ----------------------------------------------

# Symbols are resolved from the PDB with the Windows DbgHelp API (dbghelp.dll,
# part of Windows -- no external tool). A single `SymEnumSymbols` pass buckets
# every function by its undecorated `Scope::method` name; a config entry
# resolves when exactly one address carries that name. Overloaded functions
# share an undecorated name, so those are disambiguated with `SymFromName` (an
# exact mangled lookup, cheap once the enumeration has loaded the symbols).
# Anything the PDB can't pin to one address -- ICF-folded or overloaded
# non-public functions -- falls back to a byte-pattern scan of the PE.

_MAX_SYM_NAME = 2000
_SYMOPT_FAIL_CRITICAL_ERRORS = 0x00000200
_SYMOPT_EXACT_SYMBOLS = 0x00000400
_SYMOPT_NO_PROMPTS = 0x00080000
_UNDNAME_NAME_ONLY = 0x1000


class _SYMBOL_INFO(ctypes.Structure):
    # DbgHelp SYMBOL_INFO; `Name` is a trailing variable-length buffer.
    _fields_ = [
        ("SizeOfStruct", wintypes.ULONG),
        ("TypeIndex", wintypes.ULONG),
        ("Reserved", ctypes.c_ulonglong * 2),
        ("Index", wintypes.ULONG),
        ("Size", wintypes.ULONG),
        ("ModBase", ctypes.c_ulonglong),
        ("Flags", wintypes.ULONG),
        ("Value", ctypes.c_ulonglong),
        ("Address", ctypes.c_ulonglong),
        ("Register", wintypes.ULONG),
        ("Scope", wintypes.ULONG),
        ("Tag", wintypes.ULONG),
        ("NameLen", wintypes.ULONG),
        ("MaxNameLen", wintypes.ULONG),
        ("Name", ctypes.c_char * 1),
    ]


_NAME_OFFSET = _SYMBOL_INFO.Name.offset


def _undecorate(name: str) -> str:
    """Undecorate an MSVC symbol name to its fully-qualified `Scope::name` form."""
    buffer = ctypes.create_unicode_buffer(2048)
    if ctypes.windll.dbghelp.UnDecorateSymbolNameW(name, buffer, len(buffer), _UNDNAME_NAME_ONLY):
        return buffer.value
    return ""


class _PdbSymbols:
    """A DbgHelp session over a PDB + its PE, for resolving symbols by name."""

    def __init__(self, pdb_path: Path, pe_path: Path) -> None:
        self._dh = ctypes.WinDLL("dbghelp.dll")
        kernel32 = ctypes.WinDLL("kernel32.dll")
        kernel32.GetCurrentProcess.restype = wintypes.HANDLE
        self._dh.SymSetOptions.argtypes = [wintypes.DWORD]
        self._dh.SymSetOptions.restype = wintypes.DWORD
        self._dh.SymInitialize.argtypes = [wintypes.HANDLE, wintypes.LPCSTR, wintypes.BOOL]
        self._dh.SymInitialize.restype = wintypes.BOOL
        self._dh.SymLoadModuleEx.argtypes = [
            wintypes.HANDLE, wintypes.HANDLE, wintypes.LPCSTR, wintypes.LPCSTR,
            ctypes.c_ulonglong, wintypes.DWORD, ctypes.c_void_p, wintypes.DWORD,
        ]
        self._dh.SymLoadModuleEx.restype = ctypes.c_ulonglong
        self._dh.SymFromName.argtypes = [wintypes.HANDLE, wintypes.LPCSTR, ctypes.POINTER(_SYMBOL_INFO)]
        self._dh.SymFromName.restype = wintypes.BOOL
        self._enum_cb = ctypes.WINFUNCTYPE(
            wintypes.BOOL, ctypes.POINTER(_SYMBOL_INFO), wintypes.ULONG, ctypes.c_void_p
        )
        self._dh.SymEnumSymbols.argtypes = [
            wintypes.HANDLE, ctypes.c_ulonglong, wintypes.LPCSTR, self._enum_cb, ctypes.c_void_p,
        ]
        self._dh.SymEnumSymbols.restype = wintypes.BOOL
        self._dh.SymCleanup.argtypes = [wintypes.HANDLE]
        self._dh.SymCleanup.restype = wintypes.BOOL

        self._proc = kernel32.GetCurrentProcess()
        self._dh.SymSetOptions(_SYMOPT_EXACT_SYMBOLS | _SYMOPT_FAIL_CRITICAL_ERRORS | _SYMOPT_NO_PROMPTS)
        if not self._dh.SymInitialize(self._proc, str(pdb_path.parent).encode(), False):
            raise click.ClickException(f"SymInitialize failed for {pdb_path}")
        self._base = self._dh.SymLoadModuleEx(self._proc, None, str(pe_path).encode(), None, 0, 0, None, 0)
        if self._base == 0:
            self._dh.SymCleanup(self._proc)
            raise click.ClickException(f"SymLoadModuleEx failed for {pe_path}")

    def enum_named(self, wanted: set[bytes]) -> dict[bytes, set[int]]:
        """
        One pass over every module symbol, returning {name -> {RVA}} for symbols
        whose (undecorated) name is in `wanted`. A `NameLen` pre-check skips the
        string copy for the millions of non-matching symbols.
        """
        want_lens = {len(name) for name in wanted}
        found: dict[bytes, set[int]] = defaultdict(set)
        base = self._base

        @self._enum_cb
        def callback(psym, _size, _ctx):
            sym = psym.contents
            if sym.NameLen in want_lens:
                name = ctypes.string_at(ctypes.addressof(sym) + _NAME_OFFSET, sym.NameLen)
                if name in wanted:
                    found[name].add(sym.Address - base)
            return True

        self._dh.SymEnumSymbols(self._proc, base, b"*", callback, None)
        return found

    def address_of(self, name: str) -> int | None:
        """RVA of the symbol with this exact (mangled) name, or None."""
        buffer = (ctypes.c_byte * (ctypes.sizeof(_SYMBOL_INFO) + _MAX_SYM_NAME))()
        sym = ctypes.cast(buffer, ctypes.POINTER(_SYMBOL_INFO))
        sym.contents.SizeOfStruct = ctypes.sizeof(_SYMBOL_INFO)
        sym.contents.MaxNameLen = _MAX_SYM_NAME
        if self._dh.SymFromName(self._proc, name.encode(), sym):
            return sym.contents.Address - self._base
        return None

    def close(self) -> None:
        self._dh.SymCleanup(self._proc)


def _locate_pe(pdb_path: Path, config: dict) -> Path:
    """Path to the Windows PE -- next to the PDB, else downloaded and extracted."""
    exe_name = str(config.get("executable", "bedrock_server.exe"))
    local = pdb_path.parent / exe_name
    if local.is_file():
        return local
    logger.info(f"{exe_name} not found next to the PDB; downloading")
    zip_path = download_server(str(config["version"]), "windows")
    dest = zip_path.parent / exe_name
    if not dest.is_file():
        with ZipFile(zip_path, "r") as zf:
            zf.extract(exe_name, zip_path.parent)
    return dest


def _scan_pe_fallback(
    pdb_path: Path, config: dict, unresolved: list, result: dict[str, int]
) -> None:
    """
    Byte-pattern scan the PE for `unresolved` config entries -- functions the
    PDB can't pin to one address (ICF-folded, or overloaded non-publics).
    Updates `result` in place.
    """
    pe_path = _locate_pe(pdb_path, config)
    logger.info(f"Byte-pattern fallback for {len(unresolved)} symbol(s) via {pe_path}")
    text_section = lief.parse(pe_path.read_bytes()).get_section(".text")
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

    A single DbgHelp `SymEnumSymbols` pass matches each entry by its undecorated
    name; overloaded names (shared undecorated form) are disambiguated by their
    exact mangled name. Functions the PDB can't pin to one address -- ICF-folded,
    or overloaded non-publics -- fall back to a byte-pattern scan of the PE.
    Symbols that no path resolves map to 0.
    """
    if sys.platform != "win32":
        raise click.ClickException("--pdb requires Windows; PDB lookup uses the DbgHelp API")

    names = [str(entry["name"]) for entry in config["signatures"]]
    pe_path = _locate_pe(pdb_path, config)
    logger.info(f"Loading {pdb_path.name} via DbgHelp")
    syms = _PdbSymbols(pdb_path, pe_path)
    result = {name: 0 for name in names}
    try:
        # Match every entry by its undecorated name in a single enumeration.
        pretty = {name: (_undecorate(name) or name).encode() for name in names}
        logger.info(f"Scanning module symbols for {len(names)} entries")
        addrs = syms.enum_named(set(pretty.values()))

        ambiguous = []
        for name in names:
            hits = addrs.get(pretty[name], set())
            if len(hits) == 1:
                result[name] = next(iter(hits))
            elif len(hits) > 1:
                ambiguous.append(name)

        # Overloaded names resolve to several addresses; pick the exact one by
        # its mangled name (fast now that the enumeration has loaded symbols).
        for name in ambiguous:
            rva = syms.address_of(name)
            if rva is not None:
                result[name] = rva
            else:
                logger.warning(
                    f"Ambiguous symbol with no public record: {name} "
                    f"({_undecorate(name)}) -- leaving for byte-pattern fallback"
                )
    finally:
        syms.close()

    for name in names:
        if result[name]:
            logger.info(f"Found symbol: {name} => 0x{result[name]:x}")

    # Anything the PDB can't pin to one address falls back to a byte-pattern scan.
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
    type=click.Path(exists=True, dir_okay=False, path_type=Path),
)
@click.option(
    "--pdb",
    "pdb_path",
    type=click.Path(exists=True, dir_okay=False, path_type=Path),
    default=None,
    help="PDB for the Windows binary. When given, the windows config's symbols "
    "are resolved by name from the PDB via DbgHelp; names it can't pin uniquely "
    "fall back to a byte-pattern scan of the PE (read next to the PDB, or downloaded).",
)
def main(inputs: tuple[Path, ...], pdb_path: Path | None) -> None:
    if not inputs:
        inputs = DEFAULT_CONFIGS
        logger.info("No config given; scanning default configs: windows.toml, linux.toml")

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
                logger.warning(
                    f"--pdb applies only to a windows config; scanning {config_path} by signature"
                )

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
