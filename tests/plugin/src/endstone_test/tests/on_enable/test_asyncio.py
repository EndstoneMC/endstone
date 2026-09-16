import asyncio
import time

import endstone.asyncio
import pytest


def test_concurrent_sleeps_do_not_block():
    async def sleeper(n: int):
        await asyncio.sleep(0.1)
        return n

    async def concurrent():
        return await asyncio.gather(*[sleeper(n) for n in range(3)])

    start = time.perf_counter()
    future = endstone.asyncio.submit(concurrent())

    assert future.result(timeout=10) == [0, 1, 2]
    elapsed = time.perf_counter() - start
    assert 0.09 <= elapsed < 0.3, f"Concurrent sleeps took too long: {elapsed:.3f}s"


def test_submit_rejects_non_coroutines():
    """submit() should raise TypeError when given a non-coroutine."""
    with pytest.raises(TypeError):
        endstone.asyncio.submit(123)  # noqa

    def gen():
        # generator, not coroutine
        yield 1

    with pytest.raises(TypeError):
        endstone.asyncio.submit(gen())  # noqa


def test_exceptions_propagate_to_caller():
    """Exceptions raised in the coroutine are delivered to the future/result()."""

    async def raise_error():
        await asyncio.sleep(0)
        raise ValueError("boom")

    fut = endstone.asyncio.submit(raise_error())
    with pytest.raises(ValueError, match="boom"):
        fut.result(timeout=5)


def test_submit_can_handle_large_number_of_tasks_without_starvation():
    """Scheduling many tasks should work without starving the loop."""

    async def work(i):
        await asyncio.sleep(0.01)
        return i * i

    N = 100  # noqa
    futures = [endstone.asyncio.submit(work(i)) for i in range(N)]
    results = [f.result(timeout=3) for f in futures]
    assert results == [i * i for i in range(N)]
