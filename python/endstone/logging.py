import collections.abc

import endstone._logger
from endstone._logger import LogLevel

__all__ = ["Logger", "LogLevel"]


def get_logger(name: str):
    return Logger(name)


class Logger:
    def __init__(self, name: str):
        self._logger = endstone._logger.Logger.get_logger(name)

    def log(self, level: LogLevel, msg: str, *args) -> None:
        if args and len(args) == 1 and isinstance(args[0], collections.abc.Mapping) and args[0]:
            args = args[0]

        msg = str(msg)
        if args:
            msg = msg % args

        self._logger.log(level, msg)

    def verbose(self, msg: str, *args) -> None:
        self.log(LogLevel.VERBOSE, msg, *args)

    def info(self, msg: str, *args) -> None:
        self.log(LogLevel.INFO, msg, *args)

    def warning(self, msg: str, *args) -> None:
        self.log(LogLevel.WARNING, msg, *args)

    def error(self, msg: str, *args) -> None:
        self.log(LogLevel.ERROR, msg, *args)

    def set_level(self, level: LogLevel) -> None:
        self._logger.set_level(level)

    def get_name(self) -> str:
        return self._logger.get_name()
