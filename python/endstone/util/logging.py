import collections.abc

from endstone._logger import LogLevel
from endstone._logger import Logger as BaseLogger

__all__ = ["Logger", "LogLevel"]


class Logger(BaseLogger):
    def __init__(self, name: str) -> None:
        BaseLogger.__init__(self, name)

    def log(self, level, msg, *args):
        if args and len(args) == 1 and isinstance(args[0], collections.abc.Mapping) and args[0]:
            args = args[0]

        msg = str(msg)
        if args:
            msg = msg % args

        BaseLogger.log(self, level, msg)
