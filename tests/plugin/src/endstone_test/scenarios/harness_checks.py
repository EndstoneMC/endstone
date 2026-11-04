import ctypes
import os
import time

from endstone_test.harness import Context, scenario

CRASH_ENV = "ENDSTONE_TEST_CRASH"


@scenario("harness.crash", enabled=os.environ.get(CRASH_ENV) == "1")
async def crash(ctx: Context) -> None:
    """Crash the server on purpose, so the runner's crash handling can be shown. Off unless ENDSTONE_TEST_CRASH=1."""
    await ctx.ticks(1)
    ctypes.c_char.from_address(0).value = b"x"


@scenario("harness.server_thread_cpu", budget=600)
async def server_thread_cpu(ctx: Context) -> None:
    """The runner's CPU sampler must see work done on the server thread, and only that."""
    await ctx.call("perf.start")
    started = time.perf_counter()
    await ctx.ticks(80)
    tps = 80 / (time.perf_counter() - started)
    idle = await ctx.call("perf.stop")
    ctx.info["idle_tps"] = round(tps, 1)
    assert tps > 18, (
        f"the server ran at {tps:.1f} ticks per second before the probe: {idle}"
    )

    await ctx.call("perf.start")
    started = time.perf_counter()
    spun = 0.0
    end = ctx.driver.tick + 80
    while ctx.driver.tick < end:
        spin = time.perf_counter()
        while time.perf_counter() - spin < 0.025:
            pass
        spun += time.perf_counter() - spin
        await ctx.ticks(1)
    elapsed = time.perf_counter() - started
    busy = await ctx.call("perf.stop")

    expected = spun / elapsed * 1000
    extra = busy["mean_cpu_ms_per_s"] - idle["mean_cpu_ms_per_s"]
    ctx.info["perf"] = {"idle": idle, "busy": busy, "spun_ms_per_s": round(expected, 1)}
    ctx.info["busy_tps"] = round(80 / elapsed, 1)
    assert abs(extra - expected) < 0.2 * expected, (
        f"spinning {expected:.0f} ms/s on the server thread read as {extra:.0f}"
    )


@scenario("harness.control.large_end")
async def large_end(ctx: Context) -> None:
    """A verdict carrying megabytes of info must still reach the runner; a 64 KB line cap once lost it."""
    ctx.info["payload"] = "x" * (8 * 1024 * 1024)
    await ctx.ticks(1)


@scenario("harness.control.reserved_info_keys")
async def reserved_info_keys(ctx: Context) -> None:
    """Info keys named like envelope or result fields must not change the verdict or break the message."""
    ctx.info.update(
        type="end",
        scenario="someone.else",
        result={},
        status="failed",
        detail="not a failure",
    )
    await ctx.ticks(1)


@scenario("harness.control.unreadable_line")
async def unreadable_line(ctx: Context) -> None:
    """The runner must charge an unreadable control line to the running scenario, naming it and the size."""
    junk = b"{this is not json\n"
    ctx.driver.control.send_raw(junk)
    problems = await ctx.call("control.acknowledge")
    ctx.info["problems"] = problems
    assert problems, "the runner did not notice an unreadable control line"
    assert any(
        ctx.scenario.name in p and f"{len(junk)} bytes" in p for p in problems
    ), problems
