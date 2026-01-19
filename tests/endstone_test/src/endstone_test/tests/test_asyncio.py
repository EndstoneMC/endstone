import asyncio
import concurrent.futures
import threading

import endstone.asyncio
import pytest

# =============================================================================
# Section 1: get_loop Tests
# =============================================================================


def test_get_loop_returns_event_loop() -> None:
    """Verify get_loop returns an asyncio event loop."""
    loop = endstone.asyncio.get_loop()
    assert isinstance(loop, asyncio.AbstractEventLoop)


def test_get_loop_returns_running_loop() -> None:
    """Verify the returned event loop is running."""
    loop = endstone.asyncio.get_loop()
    assert loop.is_running()


def test_get_loop_runs_on_background_thread() -> None:
    """Verify the event loop runs on a separate thread, not the main thread."""
    main_thread_id = threading.current_thread().ident
    endstone.asyncio.get_loop()  # ensure loop is created

    captured_thread_id = None

    async def capture_thread():
        nonlocal captured_thread_id
        captured_thread_id = threading.current_thread().ident

    future = endstone.asyncio.submit(capture_thread())
    future.result(timeout=5)

    assert captured_thread_id is not None
    assert captured_thread_id != main_thread_id


def test_get_loop_returns_same_loop() -> None:
    """Verify get_loop returns the same event loop on subsequent calls."""
    loop1 = endstone.asyncio.get_loop()
    loop2 = endstone.asyncio.get_loop()
    assert loop1 is loop2


# =============================================================================
# Section 2: submit Tests
# =============================================================================


def test_submit_returns_future() -> None:
    """Verify submit returns a concurrent.futures.Future."""

    async def simple_coro():
        return 42

    future = endstone.asyncio.submit(simple_coro())
    assert isinstance(future, concurrent.futures.Future)
    result = future.result(timeout=5)
    assert result == 42


def test_submit_executes_coroutine() -> None:
    """Verify submit actually executes the coroutine."""
    executed = []

    async def mark_executed():
        executed.append(True)
        return "done"

    future = endstone.asyncio.submit(mark_executed())
    result = future.result(timeout=5)

    assert result == "done"
    assert executed == [True]


def test_submit_with_async_sleep() -> None:
    """Verify async sleep works correctly."""

    async def sleep_and_return():
        await asyncio.sleep(0.1)
        return "after_sleep"

    future = endstone.asyncio.submit(sleep_and_return())
    result = future.result(timeout=5)
    assert result == "after_sleep"


def test_submit_propagates_exception() -> None:
    """Verify exceptions from coroutines are propagated through the future."""

    async def raise_error():
        raise ValueError("test error")

    future = endstone.asyncio.submit(raise_error())

    with pytest.raises(ValueError, match="test error"):
        future.result(timeout=5)


def test_submit_multiple_concurrent_tasks() -> None:
    """Verify multiple coroutines can run concurrently."""
    import time

    results = []

    async def task(value, delay):
        await asyncio.sleep(delay)
        results.append(value)
        return value

    start_time = time.monotonic()

    future1 = endstone.asyncio.submit(task(1, 0.2))
    future2 = endstone.asyncio.submit(task(2, 0.1))
    future3 = endstone.asyncio.submit(task(3, 0.05))

    # Wait for all to complete
    result1 = future1.result(timeout=5)
    result2 = future2.result(timeout=5)
    result3 = future3.result(timeout=5)

    elapsed = time.monotonic() - start_time

    assert result1 == 1
    assert result2 == 2
    assert result3 == 3
    # Due to different delays, order should be 3, 2, 1
    assert results == [3, 2, 1]
    # If concurrent, elapsed should be ~0.2s (longest delay)
    # If sequential, it would be ~0.35s (0.2 + 0.1 + 0.05)
    assert elapsed < 0.3, f"Tasks should run concurrently, but took {elapsed:.2f}s"


def test_submit_rejects_non_coroutine() -> None:
    """Verify submit raises TypeError for non-coroutine arguments."""

    def regular_function():
        return 42

    with pytest.raises(TypeError):
        endstone.asyncio.submit(regular_function())  # noqa


def test_submit_with_awaited_coroutines() -> None:
    """Verify coroutines can await other coroutines."""

    async def inner():
        await asyncio.sleep(0.01)
        return 10

    async def outer():
        result = await inner()
        return result * 2

    future = endstone.asyncio.submit(outer())
    result = future.result(timeout=5)
    assert result == 20


# =============================================================================
# Section 3: Thread Safety Tests
# =============================================================================


def test_submit_from_multiple_threads() -> None:
    """Verify submit can be called safely from multiple threads."""
    results = []
    lock = threading.Lock()

    async def task(value):
        await asyncio.sleep(0.01)
        return value

    def submit_from_thread(value):
        future = endstone.asyncio.submit(task(value))
        result = future.result(timeout=5)
        with lock:
            results.append(result)

    threads = [threading.Thread(target=submit_from_thread, args=(i,)) for i in range(5)]
    for t in threads:
        t.start()
    for t in threads:
        t.join(timeout=10)

    assert sorted(results) == [0, 1, 2, 3, 4]


def test_get_loop_from_multiple_threads() -> None:
    """Verify get_loop returns the same loop from multiple threads."""
    loops = []
    lock = threading.Lock()

    def get_loop_from_thread():
        loop = endstone.asyncio.get_loop()
        with lock:
            loops.append(loop)

    threads = [threading.Thread(target=get_loop_from_thread) for _ in range(5)]
    for t in threads:
        t.start()
    for t in threads:
        t.join(timeout=10)

    # All threads should get the same loop
    assert len(loops) == 5
    assert all(loop is loops[0] for loop in loops)
