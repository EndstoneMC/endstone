import asyncio
from threading import Thread

__ALL__ = ["run"]

_loop = asyncio.new_event_loop()


def _start_event_loop(loop: asyncio.AbstractEventLoop):
    asyncio.set_event_loop(loop)
    loop.run_forever()


_thread = Thread(target=_start_event_loop, args=(_loop,), daemon=True)
_thread.start()


def run(coro):
    """
    Submit a coroutine to the shared background event loop.

    Args:
        coro: The coroutine to schedule.

    Returns:
        A concurrent.futures.Future representing the execution of the coroutine.
    """

    # Schedule the coroutine thread-safely and return its Future
    return asyncio.run_coroutine_threadsafe(coro, _loop)
