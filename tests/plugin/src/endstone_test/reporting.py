import contextlib
import io

from endstone import ColorFormat, Logger


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


class LoggerReporter:
    def __init__(self, logger: Logger):
        self._logger = logger

    def pytest_runtest_logreport(self, report) -> None:
        if report.failed:
            self._report_failure(report.nodeid, report.longrepr)
        elif report.skipped:
            self._logger.info(
                f"{ColorFormat.YELLOW}SKIP{ColorFormat.RESET} {report.nodeid}"
            )
        elif report.when == "call":
            self._logger.info(
                f"{ColorFormat.GREEN}PASS{ColorFormat.RESET} {report.nodeid}"
            )

    def pytest_collectreport(self, report) -> None:
        if report.failed:
            self._report_failure(report.nodeid, report.longrepr)

    def pytest_internalerror(self, excrepr) -> None:
        self._report_failure("pytest internal error", excrepr)

    def _report_failure(self, nodeid: str, longrepr) -> None:
        self._logger.error(f"{ColorFormat.RED}FAIL{ColorFormat.RESET} {nodeid}")
        crash = getattr(longrepr, "reprcrash", None)
        message = crash.message if crash is not None else str(longrepr)
        for line in message.splitlines():
            self._logger.error(line)


@contextlib.contextmanager
def logging_to(logger: Logger):
    stream = LoggerStream(logger)
    try:
        with contextlib.redirect_stdout(stream), contextlib.redirect_stderr(stream):
            yield
    finally:
        stream.drain()
