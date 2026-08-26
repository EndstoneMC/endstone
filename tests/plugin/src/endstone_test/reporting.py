import contextlib
import io
import os
import shutil

from endstone import Logger


class LoggerStream(io.TextIOBase):
    def __init__(self, logger: Logger):
        self._logger = logger
        self._pending = ""

    def writable(self) -> bool:
        return True

    def write(self, text: str) -> int:
        *lines, self._pending = (self._pending + text).split("\n")
        for line in lines:
            self._logger.info(line)
        return len(text)

    def flush(self) -> None:
        pass

    def drain(self) -> None:
        if self._pending:
            self._logger.info(self._pending)
            self._pending = ""

    def isatty(self) -> bool:
        return False


@contextlib.contextmanager
def logging_to(logger: Logger):
    stream = LoggerStream(logger)
    prefix = len(f"[00:00:00 INFO]: [{logger.name}] ")
    columns = os.environ.get("COLUMNS")
    os.environ["COLUMNS"] = str(max(shutil.get_terminal_size().columns - prefix, 40))
    try:
        with contextlib.redirect_stdout(stream), contextlib.redirect_stderr(stream):
            yield
    finally:
        stream.drain()
        if columns is None:
            os.environ.pop("COLUMNS", None)
        else:
            os.environ["COLUMNS"] = columns
