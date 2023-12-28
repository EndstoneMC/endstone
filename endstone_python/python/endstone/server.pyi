from endstone.logger import Logger

class Server:
    """
    Represents a server implementation.
    """

    @property
    def logger(self) -> Logger:
        """
        Returns:
            Logger: The primary logger associated with this server instance.
        """
        ...
    @property
    def version(self) -> str:
        """
        Returns:
            str: The version string of this server implementation.
        """
        ...
    @property
    def minecraft_version(self) -> str:
        """
        Returns:
            str: The Minecraft version that this server is running.
        """
        ...
