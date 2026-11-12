import sys
import threading
import time
import traceback
import typing
from dataclasses import dataclass, field

from endstone.command import CommandSenderWrapper
from endstone.event import Event
from endstone.lang import Translatable

from .capture import Capture, Hub
from .control import Control
from .plot import Plot, PlotField

if typing.TYPE_CHECKING:
    from endstone_test.plugin import EndstoneTest

NORMAL = "normal"
CANCEL = "cancel"
MUTATE = "mutate"
PATHS = (NORMAL, CANCEL, MUTATE)
RELOAD_STASH = "_endstone_test_reload_stash"
DRAIN_KICK_TICKS = 100
DRAIN_FAIL_TICKS = 400


class Skip(Exception):
    pass


class HarnessError(Exception):
    pass


@dataclass
class Scenario:
    name: str
    fn: typing.Callable[["Context"], typing.Awaitable[None]]
    covers: dict[str, tuple[str, ...]]
    budget: int
    plot: bool
    enabled: bool


REGISTRY: dict[str, Scenario] = {}


def scenario(
    name: str,
    *,
    covers: dict[type[Event], tuple[str, ...] | str] | None = None,
    budget: int = 200,
    plot: bool = False,
    enabled: bool = True,
):
    """Register an async scenario. `covers` maps each event class to the paths the scenario exercises."""

    def decorator(fn):
        if name in REGISTRY:
            raise ValueError(f"duplicate scenario {name}")
        paths = {}
        for cls, value in (covers or {}).items():
            values = (value,) if isinstance(value, str) else tuple(value)
            assert all(v in PATHS for v in values), values
            paths[cls.__name__] = values
        REGISTRY[name] = Scenario(name, fn, paths, budget, plot, enabled)
        return fn

    return decorator


# =============================================================================
# Awaitables
# =============================================================================


class Wait:
    what = "a tick"

    def __await__(self):
        return (yield self)

    def start(self, tick: int) -> None:
        pass

    def poll(self, tick: int) -> tuple[bool, object]:
        raise NotImplementedError


class Ticks(Wait):
    def __init__(self, n: int):
        self.n = n
        self.what = f"{n} tick(s)"

    def start(self, tick: int) -> None:
        self.until = tick + self.n

    def poll(self, tick: int) -> tuple[bool, object]:
        return tick >= self.until, None


class Until(Wait):
    def __init__(self, predicate, timeout: int, what: str):
        self.predicate = predicate
        self.timeout = timeout
        self.what = what

    def start(self, tick: int) -> None:
        self.limit = tick + self.timeout

    def poll(self, tick: int) -> tuple[bool, object]:
        value = self.predicate()
        if value:
            return True, value
        if tick >= self.limit:
            raise TimeoutError(
                f"timed out after {self.timeout} ticks waiting for {self.what}"
            )
        return False, None


class Reply(Wait):
    def __init__(self, control: Control, call_id: int, timeout: int, what: str):
        self.control = control
        self.call_id = call_id
        self.timeout = timeout
        self.what = what

    def start(self, tick: int) -> None:
        self.limit = tick + self.timeout

    def poll(self, tick: int) -> tuple[bool, object]:
        reply = self.control.replies.pop(self.call_id, None)
        if reply is not None:
            if reply["ok"]:
                return True, reply.get("value")
            if reply.get("unavailable"):
                raise Skip(reply["error"])
            raise HarnessError(f"{self.what} failed in the runner: {reply['error']}")
        if tick >= self.limit:
            raise TimeoutError(
                f"timed out after {self.timeout} ticks waiting for {self.what}"
            )
        return False, None


# =============================================================================
# Context handed to a scenario
# =============================================================================


@dataclass
class CommandResult:
    success: bool
    messages: list[str] = field(default_factory=list)
    errors: list[str] = field(default_factory=list)

    @property
    def output(self) -> str:
        return "\n".join(self.messages + self.errors)


class Context:
    def __init__(self, driver: "Driver", scenario: Scenario, plot: Plot | None):
        self.driver = driver
        self.scenario = scenario
        self.plot = plot
        self.plugin = driver.plugin
        self.server = driver.plugin.server
        self.level = self.server.level
        self.overworld = self.level.get_dimension("overworld")
        self.captures: list[Capture] = []
        self.bots: list[str] = []
        self.info: dict[str, object] = {}

    @property
    def stash(self) -> dict:
        """State for this scenario that survives /reload, which purges every endstone_* module.

        A scenario that reloads the server runs again in the reloaded plugin; the stash tells the second run what the
        first one did. It is cleared when the scenario ends.
        """
        store = getattr(sys, RELOAD_STASH, None)
        if store is None:
            store = {}
            setattr(sys, RELOAD_STASH, store)
        return store.setdefault(self.scenario.name, {})

    def ticks(self, n: int = 1) -> Ticks:
        return Ticks(n)

    def until(self, predicate, timeout: int = 100, what: str | None = None) -> Until:
        return Until(
            predicate, timeout, what or getattr(predicate, "__name__", "a condition")
        )

    def capture(
        self,
        event: type[Event],
        where=None,
        *,
        cancel: bool = False,
        mutate=None,
        extra=None,
    ) -> Capture:
        """Record every matching event from now until the scenario ends, cancelling or mutating it in the handler."""
        capture = self.driver.hub.open(
            event.__name__, where=where, cancel=cancel, mutate=mutate, extra=extra
        )
        self.captures.append(capture)
        return capture

    def release(self, capture: Capture) -> None:
        """Stop recording and acting on events for `capture` before the scenario ends."""
        self.driver.hub.close(capture)

    def text(self, message: str | Translatable) -> str:
        if isinstance(message, Translatable):
            return self.server.language.translate(message)
        return str(message)

    def command(self, line: str) -> CommandResult:
        """Run a command as the console, capturing what it says back."""
        result = CommandResult(False)
        sender = CommandSenderWrapper(
            self.server.command_sender,
            on_message=lambda m: result.messages.append(self.text(m)),
            on_error=lambda m: result.errors.append(self.text(m)),
        )
        result.success = (
            self.server.dispatch_command(sender, line) and not result.errors
        )
        return result

    def run(self, line: str) -> CommandResult:
        """Run a setup command that must succeed."""
        result = self.command(line)
        if not result.success:
            raise HarnessError(f"/{line} failed: {result.output}")
        return result

    def block_is(self, at: str, block: str) -> bool:
        """Ask vanilla /testforblock whether the block at "x y z" is `block`."""
        return self.command(f"testforblock {at} {block}").success

    def call(self, op: str, timeout: int = 200, **payload) -> Reply:
        call_id = self.driver.control.call(op, **payload)
        return Reply(self.driver.control, call_id, timeout, op)

    async def console(self, line: str) -> None:
        """Type a line into the server console, so it arrives the way an operator's command does."""
        await self.call("console", line=line)

    async def log_of(self, action) -> list[str]:
        """Return the server log lines written while `action()` ran."""
        mark = self.driver.next_mark()
        self.plugin.logger.info(f"@@harness mark {mark} begin")
        try:
            action()
        finally:
            self.plugin.logger.info(f"@@harness mark {mark} end")
        return await self.call("log.between", mark=mark)

    async def bot(self, name: str) -> "Bot":
        await self.call("bot.spawn", bot=name, timeout=1200)
        self.bots.append(name)
        return Bot(self, name)


class Bot:
    def __init__(self, ctx: Context, name: str):
        self.ctx = ctx
        self.name = name

    def call(self, method: str, *args, reply_ticks: int = 200, **kwargs) -> Reply:
        """Call an action of this bot's endstone_client ClientInstance, such as `game_mode.use_item_on`."""
        return self.ctx.call(
            "bot.call",
            reply_ticks,
            bot=self.name,
            method=method,
            args=list(args),
            kwargs=kwargs,
        )


# =============================================================================
# Driver
# =============================================================================


@dataclass
class Running:
    scenario: Scenario
    ctx: Context
    coro: typing.Coroutine
    started: int
    wall: float
    journal_start: int
    wait: Wait | None = None


class Driver:
    """Runs the planned scenarios one after another, resuming the current one once per server tick."""

    def __init__(self, plugin: "EndstoneTest", control: Control, hub: Hub):
        self.plugin = plugin
        self.control = control
        self.hub = hub
        self.plots = PlotField()
        self.tick = 0
        self.plan: list[str] | None = None
        self.pytest_options: dict = {}
        self.current: Running | None = None
        self.setup: typing.Coroutine | None = None
        self.setup_wait: Wait | None = None
        self.ready = False
        self.finished = False
        self.draining: list[str] = []
        self.drain_from = 0
        self.marks = 0
        self.results: dict[str, dict] = {}
        self.task = plugin.server.scheduler.run_task(
            plugin, self.on_tick, delay=1, period=1
        )

    def next_mark(self) -> int:
        self.marks += 1
        return self.marks

    def on_tick(self) -> None:
        self.tick += 1
        try:
            self.step()
        except Exception:  # noqa: BLE001
            detail = traceback.format_exc()
            running, self.current = self.current, None
            if running is None:
                self.plugin.logger.error(f"Harness error:\n{detail}")
                for name in self.plan or []:
                    self.report(
                        name,
                        "error",
                        f"harness error before the scenario ran:\n{detail}",
                        0,
                    )
                self.plan = []
                self.finish()
                return
            self.plugin.logger.error(
                f"Harness error in {running.scenario.name}:\n{detail}"
            )
            running.coro.close()
            for capture in running.ctx.captures:
                self.hub.close(capture)
            self.report(
                running.scenario.name,
                "error",
                f"harness error:\n{detail}",
                self.tick - running.started,
            )

    def step(self) -> None:
        for message in self.control.pump():
            if message["type"] == "plan":
                self.plan = list(message["scenarios"])
                self.pytest_options = message.get("pytest") or {}
        if self.control.closed:
            self.plugin.logger.error("The harness runner closed the control channel.")
            self.task.cancel()
            return
        if self.tick % 100 == 0:
            self.control.send({"type": "alive", "tick": self.tick})
        if self.plan is None or self.finished:
            return
        if not self.ready:
            self.run_setup()
            return
        if self.current is None and self.draining and self.drain():
            return
        if self.current is None:
            self.start_next()
        if self.current is not None:
            self.advance(self.current)

    # ---- harness setup -------------------------------------------------------

    def run_setup(self) -> None:
        if self.setup is None:
            self.control.send(
                {"type": "thread", "native_id": threading.get_native_id()}
            )
            self.setup = self.plots.setup(SetupContext(self))
            value, error = None, None
        else:
            try:
                ready, value = self.setup_wait.poll(self.tick)
                error = None
            except Exception as e:  # noqa: BLE001
                ready, value, error = True, None, e
            if not ready:
                return
        try:
            self.setup_wait = (
                self.setup.throw(error) if error else self.setup.send(value)
            )
            self.setup_wait.start(self.tick)
        except StopIteration:
            self.ready = True
            for capture in self.hub.startup.values():
                self.hub.close(capture)
        except Exception:  # noqa: BLE001
            detail = traceback.format_exc()
            self.plugin.logger.error(f"Harness setup failed:\n{detail}")
            for name in self.plan:
                self.report(name, "error", f"harness setup failed:\n{detail}", 0)
            self.finish()

    def drain(self) -> bool:
        """Hold the next scenario until the previous one's bots are gone from the server."""
        online = [n for n in self.draining if self.plugin.server.get_player(n) is not None]
        if not online:
            self.draining = []
            return False
        waited = self.tick - self.drain_from
        if waited >= DRAIN_FAIL_TICKS:
            raise HarnessError(f"{online} still online {waited} ticks after their bots were closed")
        if waited == DRAIN_KICK_TICKS:
            self.plugin.logger.error(f"Harness: {online} still online {waited} ticks after bot.close, kicking")
            for name in online:
                SetupContext(self).command(f"kick {name}")
        return True

    # ---- scenarios -----------------------------------------------------------

    def start_next(self) -> None:
        while self.plan:
            name = self.plan.pop(0)
            found = REGISTRY.get(name)
            if found is None or not found.enabled:
                self.control.send({"type": "begin", "scenario": name})
                self.report(name, "error", "no such scenario in this plugin build", 0)
                continue
            plot = self.plots.next() if found.plot else None
            ctx = Context(self, found, plot)
            self.control.send({"type": "begin", "scenario": name})
            coro = self.body(ctx)
            self.current = Running(
                found, ctx, coro, self.tick, time.monotonic(), len(self.hub.journal)
            )
            self.resume(self.current, None, None)
            return
        self.finish()

    async def body(self, ctx: Context) -> None:
        if ctx.plot is not None:
            await self.plots.prepare(ctx, ctx.plot)
        await ctx.scenario.fn(ctx)

    def advance(self, running: Running) -> None:
        used = self.tick - running.started
        if used > running.scenario.budget:
            what = running.wait.what if running.wait else "nothing"
            self.conclude(
                running,
                "failed",
                f"tick budget of {running.scenario.budget} exceeded waiting for {what}",
            )
            return
        error = next(
            (c.error for c in running.ctx.captures if c.error is not None), None
        )
        if error is not None:
            for capture in running.ctx.captures:
                capture.error = None
            self.resume(running, None, error)
            return
        try:
            ready, value = running.wait.poll(self.tick)
        except BaseException as e:  # noqa: BLE001
            self.resume(running, None, e)
            return
        if ready:
            self.resume(running, value, None)

    def resume(
        self, running: Running, value: object, error: BaseException | None
    ) -> None:
        while True:
            try:
                wait = (
                    running.coro.throw(error)
                    if error is not None
                    else running.coro.send(value)
                )
            except StopIteration:
                self.conclude(running, "passed", "")
                return
            except Skip as e:
                self.conclude(running, "skipped", str(e))
                return
            except AssertionError as e:
                self.conclude(running, "failed", describe_failure(e))
                return
            except Exception as e:  # noqa: BLE001
                self.conclude(running, "error", describe_failure(e))
                return
            if not isinstance(wait, Wait):
                error = HarnessError(
                    f"scenario awaited {wait!r}, which is not a harness Wait"
                )
                continue
            running.wait = wait
            wait.start(self.tick)
            try:
                ready, value = wait.poll(self.tick)
                error = None
            except BaseException as e:  # noqa: BLE001
                ready, value, error = True, None, e
            if not ready:
                return

    def conclude(self, running: Running, status: str, detail: str) -> None:
        self.current = None
        running.coro.close()
        ctx = running.ctx
        getattr(sys, RELOAD_STASH, {}).pop(ctx.scenario.name, None)
        violations = self.hub.journal_violations(running.journal_start)
        if violations:
            if status in ("passed", "skipped"):
                status, detail = "failed", ""
            detail = "\n".join([detail, *violations]).strip()
        for capture in ctx.captures:
            self.hub.close(capture)
        for name in ctx.bots:
            self.control.call("bot.close", bot=name)
        self.draining = [n for n in ctx.bots if self.plugin.server.get_player(n) is not None]
        self.drain_from = self.tick
        if ctx.plot is not None:
            self.plots.release(ctx, ctx.plot)
        events = {}
        for capture in ctx.captures:
            events.setdefault(capture.name, []).extend(capture.events)
        ticks = self.tick - running.started
        self.report(
            ctx.scenario.name,
            status,
            detail,
            ticks,
            wall=round(time.monotonic() - running.wall, 3),
            events=events,
            info=ctx.info,
        )

    def report(
        self,
        name: str,
        status: str,
        detail: str,
        ticks: int,
        wall: float | None = None,
        events: dict | None = None,
        info: dict | None = None,
    ) -> None:
        """Send the verdict. The scenario's own `info` travels in its own key, so no key in it can shadow a field."""
        self.results[name] = {"status": status, "detail": detail}
        colour = {"passed": "§a", "skipped": "§e"}.get(status, "§c")
        line = f"{colour}[{status.upper()}]§r {name}" + (
            f" -- {detail.splitlines()[0]}" if detail else ""
        )
        self.plugin.logger.info(line)
        self.control.send(
            {
                "type": "end",
                "scenario": name,
                "result": {
                    "status": status,
                    "detail": detail,
                    "ticks": ticks,
                    "wall": wall,
                    "events": events or {},
                    "info": info or {},
                },
            }
        )

    def finish(self) -> None:
        self.finished = True
        self.plots.teardown(SetupContext(self))
        counts: dict[str, int] = {}
        for result in self.results.values():
            counts[result["status"]] = counts.get(result["status"], 0) + 1
        self.plugin.logger.info(
            "Scenarios: " + ", ".join(f"{n} {s}" for s, n in sorted(counts.items()))
        )
        self.control.send({"type": "done"})
        self.task.cancel()


class SetupContext(Context):
    def __init__(self, driver: Driver):
        dummy = Scenario("harness.setup", lambda ctx: None, {}, 0, False, True)
        super().__init__(driver, dummy, None)


def describe_failure(error: BaseException) -> str:
    frames = traceback.extract_tb(error.__traceback__)
    lines = [f"{type(error).__name__}: {error}".rstrip(": ")]
    for frame in frames:
        path = frame.filename.replace("\\", "/")
        if "endstone_test/" in path and "/harness/" not in path:
            lines.append(
                f"  at {path.rsplit('endstone_test/', 1)[-1]}:{frame.lineno}: {frame.line}"
            )
    if len(lines) == 1:
        lines += traceback.format_exception(error)
    return "\n".join(lines)
