"""Run endstone-test's scenarios against a scratch Endstone server and gate on the result.

    uv run --no-project --python 3.13 python <repo>/tests/plugin/src/endstone_test/runner.py         --wheel <endstone.whl> --bds-zip <bedrock-server.zip> --workdir C:/esh

It needs only the standard library to start and can be launched from any directory: every interpreter it
starts runs with PYTHONSAFEPATH=1 in the workdir, so a checkout's endstone/ package never shadows the wheel.
"""

from __future__ import annotations

import argparse
import asyncio
import datetime
import errno
import fnmatch
import glob
import inspect
import json
import os
import re
import shlex
import shutil
import socket
import subprocess
import sys
import time
import uuid
import zipfile
from pathlib import Path

STAGE_ENV = "ENDSTONE_TEST_RUNNER_STAGE"
CONTROL_ENV = "ENDSTONE_TEST_CONTROL"
RUN_ID_ENV = "ENDSTONE_TEST_RUN_ID"
CRASH_ENV = "ENDSTONE_TEST_CRASH"
API_GATE_ENV = "ENDSTONE_TEST_API_GATE"
TRANSPORT_ENV = "ENDSTONE_TEST_TRANSPORT"
API_GATE_MODES = ("enforce", "report", "off")
FIXTURE_PLUGIN = Path("fixtures") / "endstone-test-fixture"
ADDON_PLUGIN = Path("fixtures") / "endstone-test-addon"

WINDOWS = sys.platform == "win32"
ANSI = re.compile(r"\x1b\[[0-9;]*[A-Za-z]")
MARK = re.compile(r"@@harness mark (\d+) (begin|end)")
CRASH_BANNER = "=== ENDSTONE CRASHED! ==="
CONTROL_LINE_LIMIT = 256 * 1024 * 1024
ERROR_PATTERNS = [
    re.compile(p)
    for p in (
        r"Could not pass event",
        r"Could not execute task",
        r"Unhandled exception",
        r"Traceback \(most recent call last\)",
        r"Error occurred when",
        r"Could not load plugin",
        CRASH_BANNER,
    )
]
ERROR_CONTEXT_LINES = 20

SERVER_PROPERTIES = {
    "level-name": "harness",
    "level-type": "FLAT",
    "online-mode": "false",
    "allow-list": "false",
    "allow-cheats": "true",
    "transport": "raknet",
    "enable-lan-visibility": "false",
    "player-idle-timeout": "0",
}
SCRUB = (
    "worlds",
    "crash_reports",
    "logs",
    "plugins",
    ".sentry-native",
    "endstone.toml",
    ".endstone_restart",
)


def log(message: str) -> None:
    print(f"[runner] {message}", flush=True)


# =============================================================================
# Prepare: venv and server directory
# =============================================================================


def venv_python(venv: Path) -> Path:
    return venv / ("Scripts/python.exe" if WINDOWS else "bin/python")


def venv_endstone(venv: Path) -> Path:
    return venv / ("Scripts/endstone.exe" if WINDOWS else "bin/endstone")


def python_version_of(wheel: Path) -> str:
    match = re.search(r"-cp(\d)(\d+)-", wheel.name)
    if not match:
        raise SystemExit(f"cannot tell the Python version from {wheel.name}")
    return f"{match[1]}.{match[2]}"


def child_env(**extra: str) -> dict[str, str]:
    env = dict(os.environ, PYTHONSAFEPATH="1", PYTHONUNBUFFERED="1", **extra)
    env.pop(STAGE_ENV, None)
    return env


def run_tool(
    command: list[str], workdir: Path, **kwargs
) -> subprocess.CompletedProcess:
    return subprocess.run(
        command, cwd=str(workdir), env=child_env(), check=True, **kwargs
    )


def local_project_name(path: Path) -> str | None:
    pyproject = path / "pyproject.toml"
    if not pyproject.is_file():
        return None
    match = re.search(
        r'(?ms)^\[project\].*?^name\s*=\s*"([^"]+)"',
        pyproject.read_text(encoding="utf-8"),
    )
    return match[1] if match else None


def local_sources(path: Path) -> list[str]:
    """Packages a local project takes from a path under [tool.uv.sources], such as a freshly built wheel."""
    text = (path / "pyproject.toml").read_text(encoding="utf-8")
    section = re.search(r"(?ms)^\[tool\.uv\.sources\]\s*$(.*?)(?=^\[|\Z)", text)
    if not section:
        return []
    return re.findall(r"(?m)^([A-Za-z0-9_.\-]+)\s*=\s*\{[^}]*\bpath\s*=", section[1])


def wheel_name(path: Path) -> str:
    return path.name.split("-", 1)[0].replace("_", "-")


def resolve_extras(extras: list[str]) -> tuple[list[str], list[str]]:
    """Each extra as uv should see it, and the packages to reinstall so a rebuilt local artifact is picked up.

    An extra is a requirement, a local project directory, a wheel, or a glob of wheels (the newest match wins).
    """
    requirements, reinstall = [], []
    for extra in extras:
        if any(c in extra for c in "*?["):
            matches = sorted(glob.glob(extra), key=os.path.getmtime)
            if not matches:
                raise SystemExit(f"--extra {extra} matches nothing")
            extra = str(matches[-1])
        path = Path(extra)
        if not path.exists():
            requirements.append(extra)
            continue
        path = path.resolve()
        requirements.append(str(path))
        if path.is_file() and path.suffix == ".whl":
            reinstall.append(wheel_name(path))
        elif name := local_project_name(path):
            reinstall += [name, *local_sources(path)]
    return requirements, list(dict.fromkeys(reinstall))


def prepare(args: argparse.Namespace) -> None:
    workdir: Path = args.workdir
    venv = workdir / "venv"
    if not venv_python(venv).exists():
        log(f"creating venv {venv}")
        run_tool(
            ["uv", "venv", str(venv), "--python", python_version_of(args.wheel)],
            workdir,
        )

    extras, reinstall = resolve_extras(args.extra)
    packages = [str(args.wheel), str(args.plugin), *extras]
    if not args.bench:
        packages.append(str(args.plugin / FIXTURE_PLUGIN))
        packages.append(str(args.plugin / ADDON_PLUGIN))
    log("installing the endstone wheel, endstone-test and extras")
    command = ["uv", "pip", "install", "--python", str(venv_python(venv)), *packages]
    for name in ["endstone-test", "endstone-test-fixture", "endstone-test-addon", *reinstall]:
        command += ["--reinstall-package", name]
    for links in args.find_links:
        command += ["--find-links", str(Path(links).resolve())]
    run_tool(command, workdir)
    if args.bench:
        installed = run_tool(
            ["uv", "pip", "list", "--python", str(venv_python(venv))],
            workdir,
            capture_output=True,
            text=True,
        ).stdout
        test_plugins = [
            n
            for n in ("endstone-test", "endstone-test-fixture")
            if re.search(rf"(?m)^{n}\s", installed)
        ]
        if test_plugins:
            run_tool(
                [
                    "uv",
                    "pip",
                    "uninstall",
                    "--python",
                    str(venv_python(venv)),
                    *test_plugins,
                ],
                workdir,
            )

    server: Path = args.server_dir
    executable = server / ("bedrock_server.exe" if WINDOWS else "bedrock_server")
    marker = server / ".bds-zip"
    source = Path(args.bds_zip).name
    if not executable.exists() or not marker.exists() or marker.read_text(encoding="utf-8") != source:
        with zipfile.ZipFile(args.bds_zip) as archive:
            longest = max(len(name) for name in archive.namelist())
            if WINDOWS and len(str(server)) + longest >= 259:
                raise SystemExit(
                    f"--server-dir is too long for BDS's {longest}-character paths; use a short one"
                )
            log(f"extracting {args.bds_zip} into {server}")
            archive.extractall(server)
        marker.write_text(source, encoding="utf-8")

    for name in SCRUB:
        path = server / name
        if path.is_dir():
            shutil.rmtree(path)
        elif path.exists():
            path.unlink()

    pack = Path(__file__).parent / "assets" / "behavior_pack"
    shutil.copytree(
        pack,
        server / "development_behavior_packs" / "endstone_test",
        dirs_exist_ok=True,
    )
    header = json.loads((pack / "manifest.json").read_text(encoding="utf-8"))["header"]
    world = server / "worlds" / SERVER_PROPERTIES["level-name"]
    world.mkdir(parents=True)
    (world / "world_behavior_packs.json").write_text(
        json.dumps([{"pack_id": header["uuid"], "version": header["version"]}]),
        encoding="utf-8",
    )

    properties = dict(
        SERVER_PROPERTIES,
        transport=args.transport,
        **{
            "server-port": str(args.port),
            "server-portv6": str(args.port + 1),
            "enable-lan-visibility": "true" if args.transport == "nethernet" else "false",
        },
    )
    write_properties(server / "server.properties", properties)

    version = run_tool(
        [
            str(venv_python(venv)),
            "-c",
            "import endstone; print(endstone.__minecraft_version__)",
        ],
        workdir,
        capture_output=True,
        text=True,
    ).stdout.strip()
    (server / "version.txt").write_text(version, encoding="utf-8")
    log(
        f"server ready: BDS {version}, ports {args.port}/{args.port + 1}, control {args.port + 2}"
    )


def pytest_target(spec: str) -> str:
    """A tests/ file, optionally with ::test, as the dotted --pyargs name pytest resolves inside the server."""
    path, _, names = spec.replace("\\", "/").partition("::")
    path = (
        path.split("endstone_test/tests/", 1)[-1]
        .removeprefix("tests/")
        .removesuffix(".py")
    )
    target = "endstone_test.tests." + path.strip("/").replace("/", ".")
    return f"{target}::{names}" if names else target


def repo_tests(args: argparse.Namespace) -> str | None:
    folder = args.plugin.parent
    return str(folder) if any(folder.glob("test_*.py")) else None


def pytest_options(args: argparse.Namespace) -> dict:
    return {
        "targets": [pytest_target(spec) for spec in args.pytest_file],
        "args": shlex.split(args.pytest_args),
        "repo_tests": repo_tests(args),
    }


def write_properties(path: Path, values: dict[str, str]) -> None:
    lines = path.read_text(encoding="utf-8").splitlines()
    pending = dict(values)
    for i, line in enumerate(lines):
        key = line.split("=", 1)[0].strip()
        if not line.startswith("#") and key in pending:
            lines[i] = f"{key}={pending.pop(key)}"
    lines += [f"{key}={value}" for key, value in pending.items()]
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


# =============================================================================
# Bots: the only code that knows endstone_client
# =============================================================================


class BotUnavailable(Exception):
    pass


CONNECT_TIMEOUT = 60
LEAVE_TIMEOUT = 15


class Bots:
    def __init__(
        self,
        host: str,
        port: int,
        transport: str = "raknet",
        protocol: int | None = None,
    ):
        self.address = (host, port)
        self.transport = transport
        self.protocol = protocol
        self.bots: dict[str, object] = {}

    async def spawn(self, name: str) -> dict:
        try:
            from endstone_client import ClientInstance
        except ImportError as e:
            raise BotUnavailable(
                "endstone_client is not installed in the runner's environment"
            ) from e
        if name in self.bots:
            raise ValueError(f"a bot named {name} is already connected")
        try:
            bot = ClientInstance(name)
        except ImportError as e:
            raise BotUnavailable(f"endstone_client cannot start: {e}") from e
        try:
            if self.transport == "nethernet":
                await asyncio.wait_for(
                    bot.connect_nethernet(
                        f"http://{self.address[0]}:{self.address[1]}",
                        protocol=self.protocol,
                    ),
                    CONNECT_TIMEOUT,
                )
            else:
                await asyncio.wait_for(
                    bot.connect_to_third_party_server(
                        *self.address, protocol=self.protocol
                    ),
                    CONNECT_TIMEOUT,
                )
            await bot.wait_for_player_ready()
        except BaseException:
            await asyncio.wait_for(bot.request_leave_game(), LEAVE_TIMEOUT)
            raise
        self.bots[name] = bot
        return {"name": name}

    async def call(self, name: str, method: str, args: list, kwargs: dict) -> object:
        result = self.bots[name].call(method, *args, **kwargs)
        if inspect.isawaitable(result):
            result = await result
        return jsonable(result)

    async def close(self, name: str) -> None:
        bot = self.bots.pop(name, None)
        if bot is not None:
            await asyncio.wait_for(bot.request_leave_game(), LEAVE_TIMEOUT)

    async def close_all(self) -> None:
        for name in list(self.bots):
            await self.close(name)


def jsonable(value: object) -> object:
    if value is None or isinstance(value, (bool, int, float, str)):
        return value
    if isinstance(value, (list, tuple)):
        return [jsonable(v) for v in value]
    if isinstance(value, dict):
        return {str(k): jsonable(v) for k, v in value.items()}
    if hasattr(value, "to_json"):
        return value.to_json()
    return repr(value)


# =============================================================================
# RakNet unconnected ping
# =============================================================================

RAKNET_MAGIC = bytes.fromhex("00ffff00fefefefefdfdfdfd12345678")


async def raknet_ping(host: str, port: int, timeout: float = 2.0) -> str | None:
    """Send one unconnected ping and return the server's advertisement, or None when no pong arrives."""
    loop = asyncio.get_running_loop()
    pong: asyncio.Future = loop.create_future()

    class Protocol(asyncio.DatagramProtocol):
        def datagram_received(self, data: bytes, addr) -> None:
            if data[:1] == bytes([0x1C]) and not pong.done():
                pong.set_result(data)

    transport, _ = await loop.create_datagram_endpoint(
        Protocol, remote_addr=(host, port)
    )
    try:
        sent = int(time.monotonic() * 1000)
        transport.sendto(
            bytes([0x01])
            + sent.to_bytes(8, "big")
            + RAKNET_MAGIC
            + uuid.uuid4().bytes[:8]
        )
        data = await asyncio.wait_for(pong, timeout)
    except asyncio.TimeoutError:
        return None
    finally:
        transport.close()
    length = int.from_bytes(data[33:35], "big")
    return data[35 : 35 + length].decode("utf-8", errors="replace")


# =============================================================================
# NetherNet LAN discovery ping
# =============================================================================

LAN_DISCOVERY_PORT = 7551
GAME_MODE_NAMES = {0: "Survival", 1: "Creative", 2: "Adventure", 6: "Spectator"}


def udp_port_owners(port: int) -> set[int]:
    """Pids that hold a UDP socket on this port, from netstat."""
    out = subprocess.run(
        ["netstat", "-ano", "-p", "UDP"], capture_output=True, text=True
    ).stdout
    owners = set()
    for line in out.splitlines():
        cols = line.split()
        if len(cols) >= 4 and cols[0] == "UDP" and cols[1].rsplit(":", 1)[-1] == str(port):
            owners.add(int(cols[-1]))
    return owners


def read_varint(data: bytes, at: int) -> tuple[int, int]:
    value = shift = 0
    while True:
        byte = data[at]
        at += 1
        value |= (byte & 0x7F) << shift
        if not byte & 0x80:
            return value, at
        shift += 7


def read_zigzag(data: bytes, at: int) -> tuple[int, int]:
    value, at = read_varint(data, at)
    return (value >> 1) ^ -(value & 1), at


def read_string(data: bytes, at: int) -> tuple[str, int]:
    length, at = read_varint(data, at)
    return data[at : at + length].decode("utf-8", errors="replace"), at + length


def server_data_to_advertisement(host_id: str, advertisement: bytes) -> str:
    """Render a NetherNet ServerData reply (version 7) as the RakNet pong string the scenarios read."""
    data = bytes.fromhex(advertisement.decode("ascii"))
    at = 1
    name, at = read_string(data, at)
    protocol, at = read_zigzag(data, at)
    version, at = read_string(data, at)
    level, at = read_string(data, at)
    players, at = read_zigzag(data, at)
    max_players, at = read_zigzag(data, at)
    game_type, at = read_zigzag(data, at)
    mode = GAME_MODE_NAMES.get(game_type, str(game_type))
    return ";".join(
        ["MCPE", name, str(protocol), version, str(players), str(max_players), host_id, level, mode]
    )


async def wait_for_signaling_port(port: int, timeout: float = 90.0) -> None:
    """Wait until the NetherNet signaling port can be bound again, as the CLI checks before it starts BDS."""
    dualstack = socket.has_dualstack_ipv6()
    family = socket.AF_INET6 if dualstack else socket.AF_INET
    deadline = time.monotonic() + timeout
    waited = False
    while True:
        try:
            socket.create_server(("", port), family=family, dualstack_ipv6=dualstack).close()
            return
        except OSError as e:
            if e.errno != errno.EADDRINUSE or time.monotonic() > deadline:
                return
        if not waited:
            log(f"waiting for port {port} to be released before restarting")
            waited = True
        await asyncio.sleep(1)


async def signaling_ping(host: str, port: int, timeout: float = 5.0) -> dict | None:
    """`GET /v1/join` on the NetherNet signaling port; None when the server closes without answering."""
    reader, writer = await asyncio.wait_for(asyncio.open_connection(host, port), timeout)
    try:
        writer.write(f"GET /v1/join HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n".encode())
        await writer.drain()
        data = await asyncio.wait_for(reader.read(), timeout)
    finally:
        writer.close()
    if not data:
        return None
    head, _, body = data.partition(b"\r\n\r\n")
    return {"status": int(head.split(b" ", 2)[1]), "body": body.decode("utf-8", errors="replace")}


async def nethernet_ping(server_pid: int, timeout: float = 3.0) -> str | None:
    """Send one LAN discovery request to the server on loopback and return its advertisement, or None."""
    import nethernet

    if sys.platform == "win32":
        owners = udp_port_owners(LAN_DISCOVERY_PORT)
        if owners != {server_pid}:
            raise RuntimeError(
                f"UDP {LAN_DISCOVERY_PORT} is held by pids {sorted(owners)}, not only the test server {server_pid}"
            )
    async for host in nethernet.discover(
        port=0,
        bind_host="127.0.0.1",
        broadcast_host="127.0.0.1",
        broadcast_port=LAN_DISCOVERY_PORT,
        timeout=timeout,
    ):
        return server_data_to_advertisement(str(host.network_id), host.advertisement)
    return None


# =============================================================================
# Server thread CPU sampling
# =============================================================================

SERVER_THREAD_NAME = "MC_SERVER"


class ThreadClock:
    """CPU seconds spent so far by one thread of the bedrock_server process.

    Windows charges thread times only at clock interrupts, so a thread that sleeps between them reads as idle;
    cycle counts are exact, and are turned into seconds with a rate measured on a spinning thread. Linux reads
    the scheduler's nanosecond run time.
    """

    def __init__(self, pid: int, tid: int):
        self.pid = pid
        self.tid = tid

    def seconds(self) -> float:
        if WINDOWS:
            return _windows_thread_cycles(self.tid) / _cycles_per_second()
        return (
            int(
                Path(f"/proc/{self.pid}/task/{self.tid}/schedstat")
                .read_text()
                .split()[0]
            )
            / 1e9
        )


def bedrock_pid(launcher_pid: int) -> int | None:
    """The bedrock_server process somewhere below the launcher."""
    children: dict[int, list[tuple[int, str]]] = {}
    for pid, parent, name in _processes():
        children.setdefault(parent, []).append((pid, name))
    pending = [launcher_pid]
    while pending:
        for pid, name in children.get(pending.pop(), []):
            if name in ("bedrock_server", "bedrock_server.exe"):
                return pid
            pending.append(pid)
    return None


def _processes():
    if WINDOWS:
        yield from _windows_processes()
        return
    for stat in Path("/proc").glob("[0-9]*/stat"):
        try:
            parent = int(stat.read_text().rsplit(")", 1)[1].split()[1])
            name = os.path.basename(os.readlink(stat.parent / "exe"))
        except OSError:
            continue
        yield int(stat.parent.name), parent, name


def named_server_thread(pid: int) -> int | None:
    """The thread BDS names MC_SERVER; on Linux its children inherit the name, so take the lowest tid."""
    if WINDOWS:
        named = [
            tid
            for tid in _windows_threads(pid)
            if _windows_thread_description(tid) == SERVER_THREAD_NAME
        ]
    else:
        named = []
        for task in Path(f"/proc/{pid}/task").iterdir():
            try:
                if (task / "comm").read_text().strip() == SERVER_THREAD_NAME:
                    named.append(int(task.name))
            except OSError:
                continue
    return min(named) if named else None


if WINDOWS:
    import ctypes
    from ctypes import wintypes

    _kernel32 = ctypes.WinDLL("kernel32", use_last_error=True)
    _kernel32.CreateToolhelp32Snapshot.restype = wintypes.HANDLE
    _kernel32.OpenThread.restype = wintypes.HANDLE
    _kernel32.GetThreadDescription.argtypes = [
        wintypes.HANDLE,
        ctypes.POINTER(ctypes.c_wchar_p),
    ]

    class _ThreadEntry(ctypes.Structure):
        _fields_ = [
            ("size", wintypes.DWORD),
            ("usage", wintypes.DWORD),
            ("tid", wintypes.DWORD),
            ("pid", wintypes.DWORD),
            ("base_priority", wintypes.LONG),
            ("delta_priority", wintypes.LONG),
            ("flags", wintypes.DWORD),
        ]

    class _ProcessEntry(ctypes.Structure):
        _fields_ = [
            ("size", wintypes.DWORD),
            ("usage", wintypes.DWORD),
            ("pid", wintypes.DWORD),
            ("heap_id", ctypes.c_void_p),
            ("module_id", wintypes.DWORD),
            ("threads", wintypes.DWORD),
            ("parent", wintypes.DWORD),
            ("base_priority", wintypes.LONG),
            ("flags", wintypes.DWORD),
            ("exe", ctypes.c_wchar * 260),
        ]

    def _snapshot(flag: int, entry_type, first, following):
        handle = _kernel32.CreateToolhelp32Snapshot(flag, 0)
        entry = entry_type()
        entry.size = ctypes.sizeof(entry)
        ok = first(handle, ctypes.byref(entry))
        while ok:
            yield entry
            ok = following(handle, ctypes.byref(entry))
        _kernel32.CloseHandle(handle)

    def _windows_processes():
        for entry in _snapshot(
            0x2, _ProcessEntry, _kernel32.Process32FirstW, _kernel32.Process32NextW
        ):
            yield entry.pid, entry.parent, entry.exe

    def _windows_threads(pid: int) -> list[int]:
        return [
            e.tid
            for e in _snapshot(
                0x4, _ThreadEntry, _kernel32.Thread32First, _kernel32.Thread32Next
            )
            if e.pid == pid
        ]

    def _open_thread(tid: int):
        handle = _kernel32.OpenThread(0x0800, False, tid)
        if not handle:
            raise OSError(f"cannot open thread {tid}: error {ctypes.get_last_error()}")
        return handle

    def _windows_thread_description(tid: int) -> str | None:
        try:
            handle = _open_thread(tid)
        except OSError:
            return None
        text = ctypes.c_wchar_p()
        try:
            if _kernel32.GetThreadDescription(handle, ctypes.byref(text)) < 0:
                return None
            value = text.value
            _kernel32.LocalFree(text)
            return value
        finally:
            _kernel32.CloseHandle(handle)

    _kernel32.QueryThreadCycleTime.argtypes = [
        wintypes.HANDLE,
        ctypes.POINTER(ctypes.c_uint64),
    ]
    _kernel32.GetCurrentThread.restype = wintypes.HANDLE

    def _cycles(handle) -> int:
        cycles = ctypes.c_uint64()
        if not _kernel32.QueryThreadCycleTime(handle, ctypes.byref(cycles)):
            raise OSError(
                f"QueryThreadCycleTime failed: error {ctypes.get_last_error()}"
            )
        return cycles.value

    def _windows_thread_cycles(tid: int) -> int:
        handle = _open_thread(tid)
        try:
            return _cycles(handle)
        finally:
            _kernel32.CloseHandle(handle)

    _rate: list[float] = []

    def _cycles_per_second() -> float:
        """Cycles per second of a thread that runs the whole time: the fastest of a few short spins."""
        if not _rate:
            me = _kernel32.GetCurrentThread()
            rates = []
            for _ in range(20):
                start_cycles, start = _cycles(me), time.perf_counter()
                while time.perf_counter() - start < 0.01:
                    pass
                rates.append(
                    (_cycles(me) - start_cycles) / (time.perf_counter() - start)
                )
            _rate.append(max(rates))
        return _rate[0]


class Sampler:
    """Records the server thread's CPU milliseconds per wall-clock second until stopped."""

    def __init__(self, clock: ThreadClock, how: str):
        self.clock = clock
        self.how = how
        self.samples: list[float] = []
        self.task = asyncio.create_task(self.run())

    async def run(self) -> None:
        last_cpu, last_wall = self.clock.seconds(), time.monotonic()
        while True:
            await asyncio.sleep(1)
            cpu, wall = self.clock.seconds(), time.monotonic()
            self.samples.append(round((cpu - last_cpu) / (wall - last_wall) * 1000, 1))
            last_cpu, last_wall = cpu, wall

    def stop(self) -> dict:
        self.task.cancel()
        samples = self.samples
        return {
            "thread": self.clock.tid,
            "identified_by": self.how,
            "cpu_ms_per_s": samples,
            "mean_cpu_ms_per_s": round(sum(samples) / len(samples), 1)
            if samples
            else None,
        }


# =============================================================================
# Run: server lives, control channel, log watch
# =============================================================================


class Run:
    def __init__(self, args: argparse.Namespace):
        self.args = args
        self.run_id = (
            args.run_id
            or datetime.datetime.now().strftime("%Y%m%d-%H%M%S-") + uuid.uuid4().hex[:6]
        )
        self.server_dir: Path = args.server_dir
        self.pytest = pytest_options(args)
        if self.pytest["targets"] or self.pytest["args"]:
            args.select = args.select or ["pytest.*"]
        self.full = not args.select
        self.control_errors: list[str] = []
        self.broken: dict[str, list[str]] = {}
        self.reloaded_by: str | None = None
        self.restart_after_reload = False
        self.deadline = time.monotonic() + args.timeout
        self.bots = Bots("127.0.0.1", args.port, args.transport, args.protocol)
        self.calls: set[asyncio.Task] = set()
        self.log_path = args.workdir / "logs" / f"{self.run_id}.log"

        self.available: list[str] | None = None
        self.remaining: list[str] | None = None
        self.current: str | None = None
        self.lives: list[dict] = []
        self.scenarios: dict[str, dict] = {}
        self.log_errors: list[dict] = []
        self.coverage: dict | None = None
        self.api_manifest: dict | None = None
        self.api_hits: dict[str, set[str]] = {}
        self.pending_context: list[tuple[dict, int]] = []
        self.marks: dict[int, list[str]] = {}
        self.mark_done: dict[int, asyncio.Event] = {}

        self.server_thread: int | None = None
        self.sampler: Sampler | None = None
        self.proc: asyncio.subprocess.Process | None = None
        self.writer: asyncio.StreamWriter | None = None
        self.hello = asyncio.Event()
        self.done = asyncio.Event()
        self.last_activity = time.monotonic()

    # ---- lifecycle -----------------------------------------------------------

    async def main(self) -> int:
        self.log_path.parent.mkdir(parents=True, exist_ok=True)
        self.log_file = self.log_path.open("w", encoding="utf-8")
        control = await asyncio.start_server(
            self.on_connect, "127.0.0.1", self.args.port + 2, limit=CONTROL_LINE_LIMIT
        )
        log(f"run {self.run_id}, log {self.log_path}")
        try:
            life, barren = 0, 0
            while True:
                before = len(self.remaining or [])
                outcome = await self.live(life)
                life += 1
                if outcome == "watchdog" or not self.remaining:
                    break
                barren = barren + 1 if len(self.remaining) == before else 0
                if barren > self.args.max_restarts:
                    log(
                        f"giving up: {barren} server lives in a row ended without running a scenario"
                    )
                    break
                if self.args.transport == "nethernet":
                    await wait_for_signaling_port(self.args.port)
                log(
                    f"restarting the server for {len(self.remaining)} remaining scenario(s)"
                )
        finally:
            control.close()
            await self.bots.close_all()
            self.log_file.close()
        return self.report()

    async def live(self, index: int) -> str:
        life = {
            "index": index,
            "outcome": None,
            "exit_code": None,
            "crash_reports": [],
            "scenario": None,
        }
        self.lives.append(life)
        self.hello.clear()
        self.done.clear()
        self.current = None
        self.reloaded_by = None
        self.restart_after_reload = False
        self.writer = None
        self.server_thread = None
        reports_before = set(self.crash_reports())

        env = child_env(
            **{CONTROL_ENV: f"127.0.0.1:{self.args.port + 2}", RUN_ID_ENV: self.run_id}
        )
        if self.args.crash_test:
            env[CRASH_ENV] = "1"
        env[API_GATE_ENV] = self.args.api_gate
        env[TRANSPORT_ENV] = self.args.transport

        command = [
            str(venv_endstone(self.args.workdir / "venv")),
            "-s",
            str(self.server_dir),
            "-y",
            "--no-interactive",
        ]
        self.proc = await asyncio.create_subprocess_exec(
            *command,
            stdin=asyncio.subprocess.PIPE,
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.STDOUT,
            cwd=str(self.args.workdir),
            env=env,
            start_new_session=not WINDOWS,
        )
        log(f"server life {index} started (pid {self.proc.pid})")
        pump = asyncio.create_task(self.pump(self.proc))
        started = time.monotonic()
        self.last_activity = started
        outcome = None
        stopping_since = None

        while self.proc.returncode is None:
            try:
                await asyncio.wait_for(asyncio.shield(self.proc.wait()), timeout=1)
                break
            except asyncio.TimeoutError:
                pass
            now = time.monotonic()
            if now > self.deadline:
                outcome = "watchdog"
                log(f"wall-clock watchdog fired after {self.args.timeout}s")
                self.kill()
            elif self.done.is_set():
                if stopping_since is None:
                    stopping_since = now
                    self.send_console("stop")
                elif now - stopping_since > self.args.stop_timeout:
                    outcome = "stop-timeout"
                    log("server did not stop in time")
                    self.kill()
            elif not self.hello.is_set() and now - started > self.args.startup_timeout:
                outcome = "no-plugin"
                log(
                    f"endstone-test did not connect within {self.args.startup_timeout}s"
                )
                self.kill()
            elif (
                self.hello.is_set()
                and now - self.last_activity > self.args.hang_timeout
            ):
                outcome = "hang"
                log(f"no word from the plugin for {self.args.hang_timeout}s")
                self.kill()

        await self.proc.wait()
        try:
            await asyncio.wait_for(pump, timeout=10)
        except asyncio.TimeoutError:
            log("the server's output stayed open after it exited")
        await self.bots.close_all()
        life["exit_code"] = self.proc.returncode
        life["crash_reports"] = [
            str(p) for p in self.crash_reports() if p not in reports_before
        ]
        if outcome is None:
            if life["crash_reports"]:
                outcome = "crash"
            elif self.done.is_set() and self.proc.returncode == 0:
                outcome = "finished"
            else:
                outcome = "exited"
        life["outcome"] = outcome
        log(f"server life {index} ended: {outcome}, exit code {self.proc.returncode}")

        if outcome != "finished":
            victim = self.current
            life["scenario"] = victim
            if victim is not None:
                self.scenarios[victim] = {
                    "status": "crashed" if outcome == "crash" else "error",
                    "detail": f"server {outcome} while the scenario was running (exit code {self.proc.returncode})",
                    "crash_reports": life["crash_reports"],
                }
                if self.remaining and victim in self.remaining:
                    self.remaining.remove(victim)
        return outcome

    def kill(self) -> None:
        if self.proc is not None:
            kill_tree(self.proc)

    def crash_reports(self) -> list[Path]:
        folder = self.server_dir / "crash_reports"
        return sorted(folder.glob("*.txt")) if folder.exists() else []

    def send_console(self, line: str) -> None:
        if (
            self.proc is not None
            and self.proc.stdin is not None
            and not self.proc.stdin.is_closing()
        ):
            self.proc.stdin.write((line + "\n").encode())

    # ---- server output -------------------------------------------------------

    async def pump(self, proc: asyncio.subprocess.Process) -> None:
        assert proc.stdout is not None
        while True:
            raw = await proc.stdout.readline()
            if not raw:
                return
            line = ANSI.sub("", raw.decode("utf-8", errors="replace")).rstrip("\r\n")
            self.log_file.write(line + "\n")
            if self.args.verbose:
                print(line, flush=True)
            self.on_line(line)

    def on_line(self, line: str) -> None:
        for entry, left in list(self.pending_context):
            entry["context"].append(line)
            self.pending_context.remove((entry, left))
            if left > 1:
                self.pending_context.append((entry, left - 1))

        mark = MARK.search(line)
        if mark:
            mark_id, edge = int(mark[1]), mark[2]
            if edge == "begin":
                self.marks[mark_id] = []
            else:
                self.mark_done.setdefault(mark_id, asyncio.Event()).set()
            return
        for lines in self.marks.values():
            lines.append(line)

        if any(pattern.search(line) for pattern in ERROR_PATTERNS):
            entry = {"scenario": self.current, "line": line, "context": []}
            self.log_errors.append(entry)
            self.pending_context.append((entry, ERROR_CONTEXT_LINES))

    # ---- control channel -----------------------------------------------------

    async def on_connect(
        self, reader: asyncio.StreamReader, writer: asyncio.StreamWriter
    ) -> None:
        self.writer = writer
        while True:
            try:
                line = await reader.readline()
            except ValueError:
                self.bad_control_line(
                    f"a control line over the {CONTROL_LINE_LIMIT}-byte limit"
                )
                continue
            except (ConnectionError, asyncio.IncompleteReadError):
                return
            if not line:
                return
            self.last_activity = time.monotonic()
            try:
                message = json.loads(line)
            except ValueError as e:
                self.bad_control_line(
                    f"an unreadable control line of {len(line)} bytes ({e})"
                )
                continue
            await self.on_message(message)

    def bad_control_line(self, what: str) -> None:
        """A message from the plugin was lost: fail the scenario that was running, never let it pass or vanish."""
        scenario = self.current
        problem = f"{what} arrived while {scenario or 'no scenario'} was running"
        log(problem)
        self.control_errors.append(problem)
        if scenario is not None:
            self.broken.setdefault(scenario, []).append(problem)

    def lost_end(self, name: str) -> None:
        """Record a scenario whose end message never arrived as an error."""
        problems = self.broken.get(name) or ["its end message never reached the runner"]
        self.scenarios[name] = {"status": "error", "detail": "\n".join(problems)}
        if self.remaining and name in self.remaining:
            self.remaining.remove(name)
        log(f"< {name}: error -- {problems[0]}")

    def send(self, message: dict) -> None:
        if self.writer is not None and not self.writer.is_closing():
            self.writer.write((json.dumps(message) + "\n").encode())

    async def on_message(self, message: dict) -> None:
        kind = message["type"]
        if kind == "hello":
            if message.get("run_id") != self.run_id:
                log(f"ignoring a plugin from another run: {message.get('run_id')}")
                return
            if self.remaining is None:
                self.available = message["scenarios"]
                self.coverage = message.get("coverage")
                self.api_manifest = message.get("api")
                self.remaining = self.select(self.available)
                log(
                    f"{len(self.remaining)} of {len(self.available)} scenario(s) selected"
                )
            if self.hello.is_set():
                self.reloaded_by = self.current
                self.lives[-1]["reloaded_by"] = self.current
                log(f"the plugin was reloaded during {self.current}")
            self.send(
                {
                    "type": "plan",
                    "scenarios": self.remaining,
                    "full": self.full,
                    "pytest": self.pytest,
                }
            )
            self.hello.set()
        elif kind == "begin":
            if self.current is not None and self.current != message["scenario"]:
                self.lost_end(self.current)
            self.current = message["scenario"]
            log(f"> {self.current}")
        elif kind == "end":
            name = message["scenario"]
            result = dict(message["result"])
            if self.broken.get(name):
                if result["status"] in ("passed", "skipped"):
                    result["status"] = "error"
                result["detail"] = "\n".join(
                    [*self.broken[name], result.get("detail") or ""]
                ).strip()
            self.scenarios[name] = result
            if self.remaining and name in self.remaining:
                self.remaining.remove(name)
            self.current = None
            if (
                self.reloaded_by is not None
                and not self.restart_after_reload
                and not self.args.keep_reloaded_server
            ):
                self.restart_after_reload = True
                log(
                    f"stopping the server after {name}: later scenarios get a fresh one, not the reloaded one"
                )
                self.send(
                    {
                        "type": "plan",
                        "scenarios": [],
                        "full": self.full,
                        "pytest": self.pytest,
                    }
                )
            status = result["status"]
            detail = result.get("detail") or ""
            log(
                f"< {name}: {status}"
                + (
                    f" -- {detail.splitlines()[0]}"
                    if detail and status != "passed"
                    else ""
                )
            )
        elif kind == "call":
            call = asyncio.create_task(self.on_call(message))
            self.calls.add(call)
            call.add_done_callback(self.calls.discard)
        elif kind == "thread":
            self.server_thread = message["native_id"]
            pid = bedrock_pid(self.proc.pid)
            named = named_server_thread(pid) if pid else None
            self.lives[-1]["server_thread"] = {
                "pid": pid,
                "plugin": self.server_thread,
                SERVER_THREAD_NAME: named,
            }
            if named != self.server_thread:
                log(
                    f"the plugin runs on thread {self.server_thread}, but {SERVER_THREAD_NAME} is {named}"
                )
        elif kind == "api_hits":
            for key, sources in message["hits"].items():
                self.api_hits.setdefault(key, set()).update(sources)
            self.lives[-1]["api_recorder_errors"] = message.get("errors", 0)
            if message.get("last_error"):
                self.lives[-1]["api_recorder_last_error"] = message["last_error"]
        elif kind == "done":
            if self.current is not None:
                self.lost_end(self.current)
                self.current = None
            self.done.set()

    async def on_call(self, message: dict) -> None:
        op = message["op"]
        try:
            if op == "console":
                self.send_console(message["line"])
                value = None
            elif op == "log.between":
                mark_id = message["mark"]
                await asyncio.wait_for(
                    self.mark_done.setdefault(mark_id, asyncio.Event()).wait(),
                    timeout=30,
                )
                value = self.marks.pop(mark_id, [])
                self.mark_done.pop(mark_id, None)
            elif op == "perf.start":
                if self.sampler is not None:
                    self.sampler.stop()
                self.sampler = Sampler(*self.server_clock())
                value = {
                    "thread": self.sampler.clock.tid,
                    "identified_by": self.sampler.how,
                }
            elif op == "perf.stop":
                value = self.sampler.stop() if self.sampler else None
                self.sampler = None
            elif op == "control.acknowledge":
                value = self.broken.pop(self.current, [])
                for problem in value:
                    self.control_errors.remove(problem)
            elif op == "ping":
                if self.args.transport == "nethernet":
                    value = await nethernet_ping(bedrock_pid(self.proc.pid))
                else:
                    value = await raknet_ping("127.0.0.1", self.args.port)
            elif op == "signaling_ping":
                if self.args.transport != "nethernet":
                    raise BotUnavailable("the signaling ping needs --transport nethernet")
                value = await signaling_ping("127.0.0.1", self.args.port)
            elif op == "bot.spawn":
                value = await self.bots.spawn(message["bot"])
            elif op == "bot.call":
                value = await self.bots.call(
                    message["bot"],
                    message["method"],
                    message.get("args", []),
                    message.get("kwargs", {}),
                )
            elif op == "bot.jump_taps":
                value = self.bots.bots[message["bot"]].local_player._jump_presses.extend(message["presses"])
            elif op == "bot.flight_burst":
                burst_player = self.bots.bots[message["bot"]].local_player

                async def burst(player=burst_player, request=message):
                    await asyncio.sleep(request["delay"])
                    if not hasattr(player, "_press_toggle"):
                        player._jump_presses.extend((True, False, True, *request["after"]))
                        return
                    player._jump_presses.extend((True, False, True))
                    player._press_toggle = ("FLYING", request["flying"])
                    while player._jump_presses or player._press_toggle is not None:
                        await asyncio.sleep(0.002)
                    player._jump_presses.extend(request["after"])

                asyncio.ensure_future(burst())
                value = None
            elif op == "bot.close":
                value = await self.bots.close(message["bot"])
            else:
                raise ValueError(f"unknown op {op}")
            reply = {"type": "reply", "id": message["id"], "ok": True, "value": value}
        except BotUnavailable as e:
            reply = {
                "type": "reply",
                "id": message["id"],
                "ok": False,
                "unavailable": True,
                "error": str(e),
            }
        except Exception as e:  # noqa: BLE001
            reply = {
                "type": "reply",
                "id": message["id"],
                "ok": False,
                "error": f"{type(e).__name__}: {e}",
            }
        self.send(reply)

    def server_clock(self) -> tuple[ThreadClock, str]:
        pid = bedrock_pid(self.proc.pid)
        if pid is None:
            raise RuntimeError("no bedrock_server process under the launcher")
        if self.server_thread is not None:
            return ThreadClock(pid, self.server_thread), "the plugin's scheduler thread"
        named = named_server_thread(pid)
        if named is None:
            raise RuntimeError(
                f"no thread named {SERVER_THREAD_NAME} in bedrock_server"
            )
        return ThreadClock(pid, named), f"the thread named {SERVER_THREAD_NAME}"

    def select(self, available: list[str]) -> list[str]:
        if not self.args.select:
            return list(available)
        return [
            name
            for name in available
            if any(fnmatch.fnmatchcase(name, p) for p in self.args.select)
        ]

    # ---- result --------------------------------------------------------------

    def api_coverage(self) -> dict | None:
        manifest = self.api_manifest
        if manifest is None:
            return None
        members = manifest.get("members", {})
        waivers = manifest.get("waived", {})
        table: dict[str, dict] = {}
        missing: list[str] = []
        covered = waived = 0
        for key, kind in sorted(members.items()):
            entry = {"kind": kind}
            sources = sorted(self.api_hits.get(key, ()))
            if sources:
                entry.update(status="covered", sources=sources)
                covered += 1
            elif key in waivers:
                entry.update(status="waived", reason=waivers[key])
                waived += 1
            else:
                entry["status"] = "missing"
                missing.append(f"{key} ({kind}) never ran")
            table[key] = entry
        problems = list(manifest.get("problems", []))
        errors = sum(life.get("api_recorder_errors", 0) for life in self.lives)
        if errors:
            last = next(
                (life["api_recorder_last_error"] for life in reversed(self.lives) if life.get("api_recorder_last_error")),
                "",
            )
            problems.append(f"the recorder caught {errors} internal error(s), the last {last}")
        missing += problems
        return {
            "mode": self.args.api_gate,
            "summary": f"{covered}/{len(members)} members covered, {waived} waived, {len(missing)} missing",
            "missing": missing,
            "problems": problems,
            "stale_waivers": sorted(key for key in waivers if self.api_hits.get(key)),
            "unknown_hits": sorted(set(self.api_hits) - set(members)),
            "members": table,
        }

    def report(self) -> int:
        failures: list[str] = []
        selected = self.select(self.available or [])
        for name in selected:
            result = self.scenarios.get(name)
            if result is None:
                failures.append(f"{name}: never ran")
            elif result["status"] == "skipped":
                if self.full:
                    failures.append(
                        f"{name}: skipped in a full run ({result.get('detail')})"
                    )
            elif result["status"] != "passed":
                failures.append(f"{name}: {result['status']}")
            info = (result or {}).get("info") or {}
            for test in info.get("skipped_tests", []) if result and self.full else []:
                failures.append(f"{name}: pytest skipped {test}")
        for problem in self.control_errors:
            failures.append(f"control channel: {problem}")
        for entry in self.log_errors:
            failures.append(
                f"log error during {entry['scenario'] or 'no scenario'}: {entry['line']}"
            )
        for life in self.lives:
            if life["outcome"] not in ("finished",):
                failures.append(f"server life {life['index']}: {life['outcome']}")
        if self.available is None:
            failures.append("the plugin never connected")
        if self.full and self.coverage and self.coverage.get("missing"):
            for item in self.coverage["missing"]:
                failures.append(f"coverage: {item}")
        api = self.api_coverage()
        if self.full and self.args.api_gate == "enforce":
            if api is None:
                failures.append("api coverage: the plugin sent no API manifest")
            for item in (api or {}).get("missing", []):
                failures.append(f"api coverage: {item}")

        result = {
            "run_id": self.run_id,
            "full": self.full,
            "select": self.args.select,
            "pytest": self.pytest,
            "passed": not failures,
            "failures": failures,
            "scenarios": {
                name: self.scenarios.get(name, {"status": "never ran"})
                for name in selected
            },
            "lives": self.lives,
            "log_errors": self.log_errors,
            "control_errors": self.control_errors,
            "coverage": self.coverage,
            "api_coverage": api,
            "log": str(self.log_path),
        }
        path = self.args.workdir / f"results-{self.run_id}.json"
        path.write_text(json.dumps(result, indent=2), encoding="utf-8")

        counts: dict[str, int] = {}
        for name in selected:
            status = self.scenarios.get(name, {}).get("status", "never ran")
            counts[status] = counts.get(status, 0) + 1
        print()
        print(
            f"run {self.run_id}: "
            + ", ".join(f"{n} {s}" for s, n in sorted(counts.items()))
        )
        if self.coverage:
            print(f"coverage: {self.coverage.get('summary')}")
        if api is not None:
            print(f"api coverage: {api['summary']}")
        for failure in failures[:50]:
            print(f"  FAIL {failure}")
        if len(failures) > 50:
            print(f"  ... and {len(failures) - 50} more")
        print(f"result: {'PASS' if not failures else 'FAIL'} ({path})")
        return 0 if not failures else 1


# =============================================================================
# Bench: a join storm against Endstone or vanilla BDS, no plugin involved
# =============================================================================


class Bench:
    def __init__(self, args: argparse.Namespace):
        self.args = args
        self.run_id = (
            args.run_id
            or datetime.datetime.now().strftime("%Y%m%d-%H%M%S-") + uuid.uuid4().hex[:6]
        )
        self.server_dir: Path = args.server_dir
        self.bots = Bots("127.0.0.1", args.port, args.transport, args.protocol)
        self.started = asyncio.Event()
        self.joins: list[dict] = []

    async def main(self) -> int:
        log_path = self.args.workdir / "logs" / f"bench-{self.run_id}.log"
        log_path.parent.mkdir(parents=True, exist_ok=True)
        if self.args.vanilla:
            executable = self.server_dir / (
                "bedrock_server.exe" if WINDOWS else "bedrock_server"
            )
            command, cwd = [str(executable)], self.server_dir
        else:
            venv = self.args.workdir / "venv"
            command, cwd = (
                [
                    str(venv_endstone(venv)),
                    "-s",
                    str(self.server_dir),
                    "-y",
                    "--no-interactive",
                ],
                self.args.workdir,
            )
        env = child_env()
        proc = await asyncio.create_subprocess_exec(
            *command,
            stdin=asyncio.subprocess.PIPE,
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.STDOUT,
            cwd=str(cwd),
            env=env,
            start_new_session=not WINDOWS,
        )
        with log_path.open("w", encoding="utf-8") as log_file:
            pump = asyncio.create_task(self.pump(proc, log_file))
            try:
                result = await self.storm(proc)
            finally:
                await self.bots.close_all()
                if proc.returncode is None:
                    proc.stdin.write(b"stop\n")
                    try:
                        await asyncio.wait_for(proc.wait(), self.args.stop_timeout)
                    except asyncio.TimeoutError:
                        kill_tree(proc)
                pump.cancel()

        build = "vanilla" if self.args.vanilla else self.args.wheel.name
        result.update(run_id=self.run_id, build=build, log=str(log_path))
        path = self.args.workdir / f"bench-{self.run_id}.json"
        path.write_text(json.dumps(result, indent=2), encoding="utf-8")
        joined = sum(1 for j in self.joins if j["ok"])
        print(f"bench {self.run_id} ({build}): {joined}/{self.args.bots} bots joined")
        print(
            f"  server thread: idle {result['idle']['mean_cpu_ms_per_s']} ms/s, storm {result['storm']['mean_cpu_ms_per_s']} ms/s"
        )
        print(f"  {path}")
        return 0 if joined == self.args.bots else 1

    async def storm(self, proc) -> dict:
        await asyncio.wait_for(self.started.wait(), self.args.startup_timeout)
        pid = proc.pid if self.args.vanilla else bedrock_pid(proc.pid)
        tid = named_server_thread(pid)
        if tid is None:
            raise RuntimeError(
                f"no thread named {SERVER_THREAD_NAME} in bedrock_server"
            )
        clock = ThreadClock(pid, tid)
        how = f"the thread named {SERVER_THREAD_NAME}"
        await asyncio.sleep(self.args.bench_settle)
        sampler = Sampler(clock, how)
        await asyncio.sleep(self.args.bench_idle)
        idle = sampler.stop()
        sampler = Sampler(clock, how)
        await asyncio.gather(*(self.join(f"Bench{i}") for i in range(self.args.bots)))
        await asyncio.sleep(self.args.bench_seconds)
        return {
            "bots": self.args.bots,
            "joins": self.joins,
            "idle": idle,
            "storm": sampler.stop(),
        }

    async def join(self, name: str) -> None:
        start = time.monotonic()
        try:
            await self.bots.spawn(name)
            self.joins.append(
                {"bot": name, "ok": True, "seconds": round(time.monotonic() - start, 2)}
            )
        except Exception as e:  # noqa: BLE001
            self.joins.append(
                {"bot": name, "ok": False, "error": f"{type(e).__name__}: {e}"}
            )

    async def pump(self, proc, log_file) -> None:
        while raw := await proc.stdout.readline():
            line = ANSI.sub("", raw.decode("utf-8", errors="replace")).rstrip("\r\n")
            log_file.write(line + "\n")
            if "Server started." in line:
                self.started.set()


def kill_tree(proc) -> None:
    if proc.returncode is not None:
        return
    if WINDOWS:
        subprocess.run(
            ["taskkill", "/T", "/F", "/PID", str(proc.pid)], capture_output=True
        )
    else:
        import signal

        os.killpg(proc.pid, signal.SIGKILL)


# =============================================================================
# Entry point
# =============================================================================


def parse_args(argv: list[str]) -> argparse.Namespace:
    source = Path(__file__).resolve().parents[2]
    default_plugin = source if (source / "pyproject.toml").exists() else None
    parser = argparse.ArgumentParser(
        prog="runner.py", description=__doc__.splitlines()[0]
    )
    parser.add_argument(
        "--wheel", type=Path, required=True, help="the endstone wheel to test"
    )
    parser.add_argument(
        "--bds-zip", type=Path, required=True, help="the bedrock-server zip to run"
    )
    parser.add_argument(
        "--workdir",
        type=Path,
        required=True,
        help="scratch directory for the venv and server",
    )
    parser.add_argument(
        "--server-dir",
        type=Path,
        help="directory the server runs in (default: <workdir>/server)",
    )
    parser.add_argument(
        "--plugin",
        type=Path,
        default=default_plugin,
        help="endstone-test source to install",
    )
    parser.add_argument(
        "--extra",
        action="append",
        default=[],
        help="another package for the venv: a requirement, a project directory, a wheel or a wheel glob",
    )
    parser.add_argument(
        "--find-links",
        action="append",
        default=[],
        help="a directory of wheels uv may resolve dependencies from",
    )
    parser.add_argument(
        "--port",
        type=int,
        default=19600,
        help="server port; +1 is IPv6, +2 the control channel",
    )
    parser.add_argument(
        "--transport",
        choices=("raknet", "nethernet"),
        default="raknet",
        help="transport the server listens on and the bots connect over",
    )
    parser.add_argument(
        "--select", action="append", default=[], help="scenario glob; a partial run"
    )
    parser.add_argument(
        "--timeout",
        type=float,
        default=1800,
        help="wall-clock limit for the whole run, seconds",
    )
    parser.add_argument("--startup-timeout", type=float, default=300)
    parser.add_argument("--hang-timeout", type=float, default=300)
    parser.add_argument("--stop-timeout", type=float, default=60)
    parser.add_argument(
        "--max-restarts",
        type=int,
        default=3,
        help="server lives in a row that may end without running a scenario; a crash in a scenario always restarts",
    )
    parser.add_argument(
        "--crash-test", action="store_true", help="enable the deliberate crash scenario"
    )
    parser.add_argument(
        "--keep-reloaded-server",
        action="store_true",
        help="carry on in a server a scenario reloaded, instead of restarting it after that scenario",
    )
    parser.add_argument(
        "--bench",
        action="store_true",
        help="join --bots bots at once and sample the server thread",
    )
    parser.add_argument(
        "--bots", type=int, default=10, help="bots in a --bench join storm"
    )
    parser.add_argument(
        "--bench-settle",
        type=float,
        default=10,
        help="seconds after startup before sampling",
    )
    parser.add_argument(
        "--bench-idle", type=float, default=10, help="seconds sampled before the storm"
    )
    parser.add_argument(
        "--bench-seconds",
        type=float,
        default=60,
        help="seconds sampled after every bot joined",
    )
    parser.add_argument(
        "--vanilla",
        action="store_true",
        help="with --bench, run the BDS from the zip without Endstone",
    )
    parser.add_argument(
        "--pytest-file",
        action="append",
        default=[],
        help="run only this test file of tests/, e.g. test_chunk.py or player/test_ability.py::test_x",
    )
    parser.add_argument(
        "--pytest-args",
        default="",
        help="extra pytest arguments as one string; write it with '=' since it starts with a dash: --pytest-args='-k chunk'",
    )
    parser.add_argument(
        "--api-gate",
        choices=API_GATE_MODES,
        default="enforce",
        help="record which API members run; enforce fails a full run on any missing member, report only reports, "
        "off does not record",
    )
    parser.add_argument(
        "--protocol",
        type=int,
        help="network protocol the bots connect with, instead of detecting it",
    )
    parser.add_argument("--run-id")
    parser.add_argument("--verbose", action="store_true", help="echo the server log")
    args = parser.parse_args(argv)
    if args.plugin is None:
        parser.error(
            "--plugin is required when the runner is not started from the endstone-test source tree"
        )
    if args.server_dir is None:
        args.server_dir = args.workdir / "server"
    for name in ("wheel", "bds_zip", "workdir", "server_dir", "plugin"):
        setattr(args, name, getattr(args, name).resolve())
    return args


def main(argv: list[str] | None = None) -> int:
    argv = sys.argv[1:] if argv is None else argv
    args = parse_args(argv)
    if os.environ.get(STAGE_ENV) != "run":
        args.workdir.mkdir(parents=True, exist_ok=True)
        prepare(args)
        env = dict(child_env(), **{STAGE_ENV: "run"})
        command = [
            str(venv_python(args.workdir / "venv")),
            str(Path(__file__).resolve()),
            *argv,
            "--plugin",
            str(args.plugin),
        ]
        return subprocess.run(command, env=env).returncode
    return asyncio.run(Bench(args).main() if args.bench else Run(args).main())


if __name__ == "__main__":
    sys.exit(main())
