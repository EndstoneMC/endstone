class Logger:
    def set_level(self, level: int) -> None:
        """
        Set the logging level for this logger instance.

        Args:
            level: The logging level.
        """
        ...
    def is_enabled_for(self, level: int) -> bool:
        """
        Check if logging is enabled for the specified level.

        Args:
            level: The logging level to check.

        Returns:
            bool: True if logging is enabled for the specified level, False otherwise.
        """
    def trace(self, msg: str) -> None:
        """
        Log a message at the TRACE level.

        Args:
            msg (str): The message to log.
        """
        ...
    def debug(self, msg: str) -> None:
        """
        Log a message at the DEBUG level.

        Args:
            msg (str): The message to log.
        """
        ...
    def info(self, msg: str) -> None:
        """
        Log a message at the INFO level.

        Args:
            msg (str): The message to log.
        """
        ...
    def warning(self, msg: str) -> None:
        """
        Log a message at the WARNING level.

        Args:
            msg (str): The message to log.
        """
        ...
    def error(self, msg: str) -> None:
        """
        Log a message at the ERROR level.

        Args:
            msg (str): The message to log.
        """
        ...
    def critical(self, msg: str) -> None:
        """
        Log a message at the CRITICAL level.

        Args:
            msg (str): The message to log.
        """
        ...
    @property
    def name(self) -> str:
        """
        Returns:
            str: The name of the logger.
        """
        ...
    TRACE: int
    DEBUG: int
    INFO: int
    WARNING: int
    ERROR: int
    CRITICAL: int
