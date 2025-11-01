from __future__ import annotations

import asyncio
import concurrent.futures
import threading
import typing
from typing import Awaitable, Optional

__all__ = ["get_loop", "submit"]

_T = typing.TypeVar("_T")


class _ThreadLoopRunner:
    def __init__(self, *, debug: Optional[bool] = None, thread_name: str = "EndstoneAsyncio"):
        self._loop: Optional[asyncio.AbstractEventLoop] = None
        self._thread: Optional[threading.Thread] = None
        self._started_evt = threading.Event()
        self._debug = debug
        self._thread_name = thread_name
        self._lock = threading.RLock()
        self._closed = False

    def _lazy_init(self) -> None:
        with self._lock:
            if self._closed:
                raise RuntimeError("Runner is closed")

            if self._loop is not None:
                return

            loop = asyncio.new_event_loop()
            if self._debug is not None:
                loop.set_debug(self._debug)
            self._loop = loop

            def _thread_main():
                asyncio.set_event_loop(loop)
                self._started_evt.set()
                loop.run_forever()

            t = threading.Thread(target=_thread_main, name=self._thread_name, daemon=True)
            t.start()
            self._thread = t
            self._started_evt.wait()

    def shutdown(self, *, join_timeout: Optional[float] = None) -> None:
        with self._lock:
            if self._loop is None:
                self._closed = True
                return

            loop = self._loop
            thread = self._thread

            async def _shutdown():
                current = asyncio.current_task()
                to_cancel = [t for t in asyncio.all_tasks() if t is not current]
                for t in to_cancel:
                    t.cancel()
                if to_cancel:
                    await asyncio.gather(*to_cancel, return_exceptions=True)
                await loop.shutdown_asyncgens()
                await loop.shutdown_default_executor()

            # run cleanup
            fut = asyncio.run_coroutine_threadsafe(_shutdown(), loop)
            try:
                fut.result()
            except Exception:  # noqa
                pass

            loop.call_soon_threadsafe(loop.stop)  # noqa
            if thread and thread.is_alive():
                thread.join(join_timeout)

            if not loop.is_closed():
                loop.close()

            self._loop = None
            self._thread = None
            self._closed = True

    def submit(self, coro: typing.Awaitable[_T]) -> concurrent.futures.Future[_T]:
        if not asyncio.iscoroutine(coro):
            raise TypeError(f"a coroutine object is required, got {coro!r}")
        self._lazy_init()
        return asyncio.run_coroutine_threadsafe(coro, self._loop)

    def get_loop(self) -> asyncio.AbstractEventLoop:
        self._lazy_init()
        return self._loop  # type: ignore[return-value]


_lock = threading.RLock()
_runner: Optional[_ThreadLoopRunner] = None


def _ensure_runner() -> _ThreadLoopRunner:
    global _runner
    with _lock:
        if _runner is None:
            _runner = _ThreadLoopRunner()
        _runner._lazy_init()  # noqa
        return _runner


def get_loop() -> asyncio.AbstractEventLoop:
    """Return the background event loop (starts it if needed)."""
    return _ensure_runner().get_loop()


def submit(coro: Awaitable[_T]) -> concurrent.futures.Future[_T]:
    """
    Submit an awaitable to the background loop, returns a concurrent.futures.Future.
    """
    return _ensure_runner().submit(coro)
