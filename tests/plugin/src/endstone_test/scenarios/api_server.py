import contextlib
import gzip
import importlib
import os
import shutil
import site
import sys
import tempfile
import threading
import zipfile
from pathlib import Path

from endstone import Logger
from endstone.command import BlockCommandSender, CommandExecutor
from endstone.event import PluginDisableEvent, PluginEnableEvent
from endstone.metrics import Metrics

from endstone_test.harness import Context, scenario
from endstone_test.plugin import HARNESS_STARTS

FIXTURE = "test_fixture"
ADDON = "test_addon"
FUNCTION = "endstone_cov_reload"
PACK = Path("development_behavior_packs") / "endstone_test"
PLUGINS_STASH = "_endstone_test_api_server_plugins"
CYCLE_STASH = "_endstone_test_api_server_cycle"


def fixture(ctx: Context):
    plugin = ctx.server.plugin_manager.get_plugin(FIXTURE)
    assert plugin is not None, f"the {FIXTURE} plugin is not loaded"
    return plugin


def mentions(lines: list[str], text: str) -> list[str]:
    return [line for line in lines if text in line]


@scenario("api.server.logger", budget=400)
async def logger_levels(ctx: Context) -> None:
    logger = fixture(ctx).logger
    levels = list(Logger.Level)
    names = ["trace", "debug", "info", "warning", "error", "critical"]
    original = next(level for level in levels if logger.is_enabled_for(level))
    try:
        for level in levels:
            logger.set_level(level)
            for probe in levels:
                assert logger.is_enabled_for(probe) == (probe >= level), (level, probe)

        logger.set_level(Logger.Level.TRACE)
        lines = await ctx.log_of(lambda: [getattr(logger, name)(f"cov logger {name}") for name in names])
        for name in names:
            assert mentions(lines, f"cov logger {name}"), f"logger.{name} wrote nothing at TRACE: {lines}"

        logger.set_level(Logger.Level.ERROR)
        lines = await ctx.log_of(lambda: [getattr(logger, name)(f"cov logger {name}") for name in names])
        for name in names:
            written = bool(mentions(lines, f"cov logger {name}"))
            assert written == (name in ("error", "critical")), f"logger.{name} at ERROR wrote={written}: {lines}"

        logger.set_level(Logger.Level.CRITICAL)
        lines = await ctx.log_of(lambda: [getattr(logger, name)(f"cov logger {name}") for name in names])
        assert [name for name in names if mentions(lines, f"cov logger {name}")] == ["critical"], lines
    finally:
        logger.set_level(original)
    assert logger.is_enabled_for(original)


@scenario("api.server.metrics", budget=400)
async def metrics_hooks(ctx: Context) -> None:
    manager = ctx.server.plugin_manager
    plugin = fixture(ctx)
    metrics = Metrics(plugin, 1)
    try:
        assert metrics.service_enabled is True

        lines = await ctx.log_of(lambda: metrics.log_info("cov metrics info"))
        info = mentions(lines, "cov metrics info")
        assert info and "TestFixture" in info[0], lines

        lines = await ctx.log_of(lambda: metrics.log_error("cov metrics error", RuntimeError("cov boom")))
        error = mentions(lines, "cov metrics error: cov boom")
        assert error and "TestFixture" in error[0], lines

        ran = []
        metrics.submit_task(lambda: ran.append((ctx.server.is_primary_thread, threading.get_ident())))
        assert ran == [], "submit_task ran the task before the next tick"
        await ctx.until(lambda: ran, timeout=20, what="the submitted task")
        assert ran[0][0] is True

        try:
            manager.disable_plugin(plugin)
            assert metrics.service_enabled is False
            late = []
            metrics.submit_task(lambda: late.append(1))
            await ctx.ticks(5)
            assert late == [], "a task submitted for a disabled plugin ran"
        finally:
            if not plugin.is_enabled:
                manager.enable_plugin(plugin)
        assert metrics.service_enabled is True
    finally:
        metrics.shutdown()


@scenario("api.server.loader", budget=400)
async def loader_toggles(ctx: Context) -> None:
    manager = ctx.server.plugin_manager
    addon = manager.get_plugin(ADDON)
    assert addon is not None, f"the {ADDON} plugin is not loaded"
    loader = addon.plugin_loader
    disabled = ctx.capture(PluginDisableEvent, lambda e: e.plugin.name == ADDON)
    enabled = ctx.capture(PluginEnableEvent, lambda e: e.plugin.name == ADDON)
    try:
        assert addon.is_enabled
        loader.disable_plugin(addon)
        assert not addon.is_enabled
        assert manager.is_plugin_enabled(ADDON) is False
        assert disabled.count == 1, disabled.events

        loader.disable_plugin(addon)
        assert disabled.count == 1, "disabling a disabled plugin fired PluginDisableEvent again"

        loader.enable_plugin(addon)
        assert addon.is_enabled
        assert manager.is_plugin_enabled(ADDON) is True
        assert enabled.count == 1, enabled.events

        loader.enable_plugin(addon)
        assert enabled.count == 1, "enabling an enabled plugin fired PluginEnableEvent again"
        before = len(addon.received)
        assert ctx.command("addoncmd after").success
        assert len(addon.received) == before + 1
    finally:
        if not addon.is_enabled:
            loader.enable_plugin(addon)


@scenario("api.server.scheduler", budget=600)
async def scheduler_cancel_and_running(ctx: Context) -> None:
    scheduler = ctx.server.scheduler
    other = fixture(ctx)
    runs = {"mine": 0, "other_a": 0, "other_b": 0}

    def bump(key):
        return lambda: runs.__setitem__(key, runs[key] + 1)

    cancelled = scheduler.run_task(ctx.plugin, bump("mine"), delay=1, period=1)
    await ctx.until(lambda: runs["mine"] >= 3, timeout=40, what="the repeating task")
    assert scheduler.is_queued(cancelled.task_id)
    scheduler.cancel_task(cancelled.task_id)
    assert cancelled.is_cancelled
    assert not scheduler.is_queued(cancelled.task_id)
    frozen = runs["mine"]
    await ctx.ticks(6)
    assert runs["mine"] == frozen, "a cancelled task kept running"
    scheduler.cancel_task(cancelled.task_id)
    scheduler.cancel_task(2**31 - 1)

    a = scheduler.run_task(other, bump("other_a"), delay=1, period=1)
    b = scheduler.run_task(other, bump("other_b"), delay=1, period=1)
    mine = scheduler.run_task(ctx.plugin, bump("mine"), delay=1, period=1)
    await ctx.until(lambda: runs["other_a"] >= 2 and runs["other_b"] >= 2, timeout=40, what="the fixture's tasks")
    scheduler.cancel_tasks(other)
    assert a.is_cancelled and b.is_cancelled
    assert not mine.is_cancelled
    assert not scheduler.is_queued(a.task_id) and not scheduler.is_queued(b.task_id)
    assert scheduler.is_queued(mine.task_id)
    frozen = dict(runs)
    await ctx.ticks(6)
    assert (runs["other_a"], runs["other_b"]) == (frozen["other_a"], frozen["other_b"])
    assert runs["mine"] > frozen["mine"], "cancel_tasks stopped another plugin's task"
    assert other.is_enabled
    scheduler.cancel_task(mine.task_id)

    inside = []
    holder = {}

    def probe() -> None:
        inside.append(scheduler.is_running(holder["id"]))

    repeating = scheduler.run_task(ctx.plugin, probe, delay=2, period=3)
    holder["id"] = repeating.task_id
    assert scheduler.is_running(repeating.task_id) is False
    await ctx.until(lambda: len(inside) >= 2, timeout=40, what="the probing task")
    scheduler.cancel_task(repeating.task_id)
    assert inside[:2] == [True, True], inside
    assert scheduler.is_running(repeating.task_id) is False

    once = []

    def probe_once() -> None:
        once.append(scheduler.is_running(holder["once"]))

    single = scheduler.run_task(ctx.plugin, probe_once, delay=1)
    holder["once"] = single.task_id
    await ctx.until(lambda: once, timeout=20, what="the one-shot task")
    assert once == [True]
    await ctx.ticks(2)
    assert scheduler.is_running(single.task_id) is False
    assert scheduler.is_running(2**31 - 1) is False


@scenario("api.server.reload_data", plot=True, budget=600)
async def reload_data(ctx: Context) -> None:
    function = PACK / "functions" / f"{FUNCTION}.mcfunction"
    assert function.is_file(), f"the test behavior pack has no {function.resolve()}"
    original = function.read_text(encoding="utf-8")
    at = ctx.plot.at(*ctx.plot.middle)
    try:
        assert ctx.command(f"function {FUNCTION}").success
        assert ctx.block_is(at, "air")

        function.write_text(f"setblock {at} gold_block\n", encoding="utf-8")
        assert ctx.command(f"function {FUNCTION}").success
        assert ctx.block_is(at, "air"), "an edited function file was read before reload_data"

        ctx.server.reload_data()
        await ctx.until(
            lambda: (ctx.command(f"function {FUNCTION}"), ctx.block_is(at, "gold_block"))[1],
            timeout=300,
            what="the edited function to run",
        )

        ctx.run(f"scriptevent endstone_test:setblock {at} stone")
        await ctx.until(lambda: ctx.block_is(at, "stone"), timeout=40, what="the script event after reload_data")
    finally:
        function.write_text(original, encoding="utf-8")
        ctx.command(f"setblock {at} air")
        ctx.server.reload_data()


class BlockWatcher(CommandExecutor):
    def __init__(self):
        CommandExecutor.__init__(self)
        self.seen = []

    def on_command(self, sender, command, args) -> bool:
        if isinstance(sender, BlockCommandSender):
            block = sender.block
            self.seen.append(
                {
                    "name": sender.name,
                    "position": (block.x, block.y, block.z),
                    "type": str(block.type),
                    "dimension": str(block.dimension.id),
                    "args": list(args),
                }
            )
        return True


@scenario("api.server.block_command_sender", plot=True, budget=400)
async def block_command_sender(ctx: Context) -> None:
    command = ctx.plugin.get_command("testoptmsg")
    watcher = BlockWatcher()
    command.executor = watcher
    at = ctx.plot.at(*ctx.plot.middle)
    try:
        ctx.run(f"structure load endstone_test:command_block {at}")
        await ctx.until(lambda: watcher.seen, timeout=100, what="the command block to run its command")
    finally:
        command.executor = None
        ctx.command(f"setblock {at} air")
    ctx.info["seen"] = watcher.seen
    seen = watcher.seen[0]
    assert seen["position"] == ctx.plot.pos(*ctx.plot.middle), seen
    assert seen["type"] == "minecraft:command_block", seen
    assert seen["dimension"] == str(ctx.overworld.id), seen
    assert seen["args"] == ["covblock"], seen


@contextlib.contextmanager
def detached_stdin():
    if sys.platform == "win32":
        import ctypes
        import msvcrt

        kernel = ctypes.windll.kernel32
        kernel.GetStdHandle.restype = ctypes.c_void_p
        kernel.SetStdHandle.argtypes = [ctypes.c_ulong, ctypes.c_void_p]
        previous = kernel.GetStdHandle(-10)
        with open(os.devnull) as nul:
            handle = msvcrt.get_osfhandle(nul.fileno())
            os.set_handle_inheritable(handle, True)
            kernel.SetStdHandle(-10, handle)
            try:
                yield
            finally:
                kernel.SetStdHandle(-10, previous)
    else:
        saved = os.dup(0)
        nul = os.open(os.devnull, os.O_RDONLY)
        os.dup2(nul, 0)
        try:
            yield
        finally:
            os.dup2(saved, 0)
            os.close(saved)
            os.close(nul)


def build_plugin_wheel(folder: Path, name: str) -> Path:
    dist = f"endstone_{name}"
    info = f"{dist}-0.1.0.dist-info"
    wheel = folder / f"{dist}-0.1.0-py3-none-any.whl"
    cls = "".join(part.capitalize() for part in name.split("_"))
    source = (
        "from endstone.plugin import Plugin\n\n\n"
        f"class {cls}(Plugin):\n"
        '    api_version = "0.12"\n\n'
        "    def __init__(self):\n"
        "        super().__init__()\n"
        "        self.loaded_times = 0\n"
        "        self.enabled_times = 0\n\n"
        "    def on_load(self):\n"
        "        self.loaded_times += 1\n\n"
        "    def on_enable(self):\n"
        "        self.enabled_times += 1\n"
    )
    files = {
        f"{dist}/__init__.py": source,
        f"{info}/METADATA": (
            f"Metadata-Version: 2.1\nName: endstone-{name.replace('_', '-')}\nVersion: 0.1.0\n"
            "Summary: Built by endstone-test\n"
        ),
        f"{info}/WHEEL": "Wheel-Version: 1.0\nGenerator: endstone-test\nRoot-Is-Purelib: true\nTag: py3-none-any\n",
        f"{info}/entry_points.txt": f"[endstone]\n{name} = {dist}:{cls}\n",
    }
    files[f"{info}/RECORD"] = "".join(f"{path},,\n" for path in [*files, f"{info}/RECORD"])
    with zipfile.ZipFile(wheel, "w") as archive:
        for path, text in files.items():
            archive.writestr(path, text)
    return wheel


def remove_installed(name: str) -> None:
    root = Path("plugins") / ".local"
    if not root.is_dir():
        return
    for found in [*root.rglob(f"endstone_{name}"), *root.rglob(f"endstone_{name}-*")]:
        shutil.rmtree(found, ignore_errors=True)


def check_restored(ctx: Context, stash: dict) -> None:
    manager = ctx.server.plugin_manager
    if "error" in stash:
        raise AssertionError(stash["error"])
    for name in ("test", FIXTURE, ADDON):
        assert manager.is_plugin_enabled(name), f"{name} is not enabled afterwards"
    assert all(p.is_enabled for p in manager.plugins), [p.name for p in manager.plugins if not p.is_enabled]
    assert not [p.name for p in manager.plugins if p.name.startswith("covplug_")]


@scenario("api.server.plugins_replace", budget=3000)
async def plugins_replace(ctx: Context) -> None:
    manager = ctx.server.plugin_manager
    stash = getattr(sys, PLUGINS_STASH, None)
    if stash is not None:
        delattr(sys, PLUGINS_STASH)
        check_restored(ctx, stash)
        assert ctx.server.get_plugin_command("fixturecmd") is not None
        assert stash["loaded"] == ["covplug_a", "covplug_b", "covplug_c"], stash
        ctx.info["before_reload"] = stash
        return

    work = Path(tempfile.mkdtemp(prefix="endstone_cov_"))
    cleared = False
    outcome = {}
    try:
        folder = work / "dir"
        folder.mkdir()
        wheel_a = build_plugin_wheel(work, "covplug_a")
        build_plugin_wheel(folder, "covplug_b")
        wheel_c = build_plugin_wheel(work, "covplug_c")

        prefix = os.path.join("plugins", ".local")
        for site_dir in site.getsitepackages(prefixes=[prefix]):
            os.makedirs(site_dir, exist_ok=True)
        importlib.invalidate_caches()
        with detached_stdin():
            a = manager.load_plugin(str(wheel_a))
        assert a is not None and a.name == "covplug_a"
        assert manager.get_plugin("covplug_a") is a
        assert a in manager.plugins
        assert a.loaded_times == 1 and a.enabled_times == 0 and not a.is_enabled
        assert manager.is_plugin_enabled("covplug_a") is False
        assert Path(a.data_folder).parent == work
        manager.enable_plugin(a)
        assert a.is_enabled and a.enabled_times == 1

        assert manager.load_plugin(str(work / "unsupported.txt")) is None

        with detached_stdin():
            from_dir = manager.load_plugins(str(folder))
        assert [p.name for p in from_dir] == ["covplug_b"], from_dir
        assert manager.get_plugin("covplug_b") is from_dir[0]
        assert not from_dir[0].is_enabled
        assert Path(from_dir[0].data_folder).parent == folder

        with detached_stdin():
            from_files = manager.load_plugins([str(wheel_c)])
        assert [p.name for p in from_files] == ["covplug_c"], from_files
        assert manager.get_plugin("covplug_c") is from_files[0]
        assert manager.load_plugins([str(work / "unsupported.txt")]) == []

        manager.enable_plugins()
        assert all(p.is_enabled for p in manager.plugins), [p.name for p in manager.plugins if not p.is_enabled]
        assert from_dir[0].enabled_times == 1 and from_files[0].enabled_times == 1
        assert a.enabled_times == 1, "enable_plugins enabled an enabled plugin again"

        outcome["loaded"] = [p.name for p in manager.plugins if p.name.startswith("covplug_")]
        remove_installed("covplug_a")
        remove_installed("covplug_b")
        remove_installed("covplug_c")
        assert manager.permissions

        cleared = True
        manager.clear_plugins()
        assert manager.plugins == []
        assert manager.get_plugin(FIXTURE) is None and manager.get_plugin("covplug_a") is None
        assert not a.is_enabled
        assert manager.permissions == set()
        assert manager.load_plugins(str(folder)) == []
        assert manager.load_plugins([str(wheel_c)]) == []
    except BaseException as e:
        outcome["error"] = f"{type(e).__name__}: {e}"
        if not cleared:
            raise
    finally:
        remove_installed("covplug_a")
        remove_installed("covplug_b")
        remove_installed("covplug_c")
        shutil.rmtree(work, ignore_errors=True)
    setattr(sys, PLUGINS_STASH, outcome)
    ctx.server.reload()
    await ctx.ticks(1200)
    raise AssertionError("the plugin was not reloaded")


@scenario("api.server.plugins_cycle", budget=3000)
async def plugins_cycle(ctx: Context) -> None:
    manager = ctx.server.plugin_manager
    scheduler = ctx.server.scheduler
    stash = getattr(sys, CYCLE_STASH, None)
    if stash is not None:
        delattr(sys, CYCLE_STASH)
        check_restored(ctx, stash)
        assert stash["disabled"] == stash["names"] and stash["enabled"] == stash["names"], stash
        ctx.info["cycle"] = stash
        return

    plugin = fixture(ctx)
    plugins = list(manager.plugins)
    names = [p.name for p in plugins]
    assert all(p.is_enabled for p in plugins)
    ran = []
    task = scheduler.run_task(plugin, lambda: ran.append(1), delay=1, period=1)
    await ctx.until(lambda: ran, timeout=20, what="the fixture's task")
    before = (plugin.enabled_times, plugin.disabled_times)
    outcome = {"names": names}
    starts = getattr(sys, HARNESS_STARTS, 0)
    try:
        manager.disable_plugins()
        outcome["disabled"] = [p.name for p in plugins if not p.is_enabled]
        assert plugin.disabled_times == before[1] + 1
        assert task.is_cancelled and not scheduler.is_queued(task.task_id)
        manager.disable_plugins()
        assert plugin.disabled_times == before[1] + 1, "disable_plugins disabled a disabled plugin again"
        manager.enable_plugins()
        outcome["enabled"] = [p.name for p in plugins if p.is_enabled]
        assert plugin.enabled_times == before[0] + 1
        assert ctx.server.plugin_manager.get_plugin(FIXTURE) is plugin
    except BaseException as e:
        outcome["error"] = f"{type(e).__name__}: {e}"
    finally:
        setattr(sys, CYCLE_STASH, outcome)
        manager.enable_plugins()
    if getattr(sys, HARNESS_STARTS, 0) == starts:
        delattr(sys, CYCLE_STASH)
        raise AssertionError(outcome.get("error", "the harness plugin was not enabled again"))
    await ctx.ticks(1200)
    raise AssertionError("the plugin was not re-enabled")


@scenario("api.server.shutdown", budget=1200)
async def shutdown(ctx: Context) -> None:
    started = ctx.driver.tick
    ctx.server.shutdown()
    ctx.driver.report(ctx.scenario.name, "passed", "", ctx.driver.tick - started, info=ctx.info)
    ctx.driver.control.send({"type": "done"})
    await ctx.ticks(1200)
    raise AssertionError("the server kept running after Server.shutdown")


def shutdown_trail() -> str:
    rotated = sorted((Path("logs")).glob("*.log.gz"), key=lambda p: p.stat().st_mtime)
    if not rotated:
        return ""
    with gzip.open(rotated[-1], "rt", encoding="utf-8", errors="replace") as stream:
        log = stream.read()
    stopped = log.rfind("Server stop requested.")
    return log[stopped:] if stopped >= 0 else ""


@scenario("api.server.shutdown_after", budget=400)
async def shutdown_after(ctx: Context) -> None:
    def stopped_cleanly() -> bool:
        trail = shutdown_trail()
        return "[Test] Disabling test " in trail and "[TestAddon] Disabling test_addon" in trail

    assert stopped_cleanly(), f"Server.shutdown did not stop the plugins: {shutdown_trail()!r}"
