"""
Classes relevant to bans.
"""

import datetime
import uuid

__all__ = [
    "BanEntry",
    "IpBanEntry",
    "IpBanList",
    "PlayerBanEntry",
    "PlayerBanList",
]

class BanEntry:
    """
    A single entry from a ban list.
    """
    @property
    def created(self) -> datetime.datetime:
        """
        The date this ban entry was created.
        """
        ...
    @created.setter
    def created(self, arg1: datetime.datetime) -> None: ...
    @property
    def source(self) -> str:
        """
        The source of this ban.
        """
        ...
    @source.setter
    def source(self, arg1: str) -> None: ...
    @property
    def expiration(self) -> datetime.datetime | None:
        """
        The date this ban expires on, or `None` for no defined end date.
        """
        ...
    @expiration.setter
    def expiration(self, arg1: datetime.datetime | None) -> None: ...
    @property
    def reason(self) -> str:
        """
        The reason for this ban.
        """
        ...
    @reason.setter
    def reason(self, arg1: str) -> None: ...

class IpBanEntry(BanEntry):
    """
    Represents an entry for a banned IP address.
    """
    def __init__(self, address: str) -> None: ...
    @property
    def address(self) -> str:
        """
        The banned IP address.
        """
        ...

class IpBanList:
    """
    Represents a ban list, containing banned IP addresses.
    """
    def get_ban_entry(self, address: str) -> IpBanEntry | None:
        """
        Gets a `BanEntry` by IP address.

        Args:
            address: The IP address to search for.

        Returns:
            The corresponding entry, or `None` if none found.
        """
        ...
    def add_ban(
        self,
        address: str,
        reason: str | None = None,
        expires: datetime.datetime | None = None,
        source: str | None = None,
    ) -> IpBanEntry:
        """
        Adds a ban to this list. If a previous ban exists, this will update the previous entry.

        Args:
            address: The IP address of the target.
            reason: The reason for the ban, `None` indicates implementation default.
            expires: The date for the ban's expiration (unban), or `None` to imply forever.
            source: The source of the ban, `None` indicates implementation default.

        Returns:
            The entry for the newly created ban, or the entry for the (updated) previous ban.
        """
        ...
    def is_banned(self, address: str) -> bool:
        """
        Checks if a `BanEntry` exists for the target, indicating an active ban status.

        Args:
            address: The IP address to find.

        Returns:
            `True` if a `BanEntry` exists for the target, indicating an active ban status, `False` otherwise.
        """
        ...
    def remove_ban(self, address: str) -> None:
        """
        Removes the specified IP address from this list, therefore indicating a "not banned" status.

        Args:
            address: The IP address to remove from this list.
        """
        ...
    @property
    def entries(self) -> list[IpBanEntry]:
        """
        A vector containing pointers to every entry tracked by this list.
        """
        ...

class PlayerBanEntry(BanEntry):
    """
    Represents an entry for a banned player.
    """
    def __init__(self, name: str, uuid: uuid.UUID | None = None, xuid: str | None = None) -> None: ...
    @property
    def name(self) -> str:
        """
        The banned player's name.
        """
        ...
    @property
    def unique_id(self) -> uuid.UUID | None:
        """
        The banned player's unique id, or `None` if not available.
        """
        ...
    @property
    def xuid(self) -> str | None:
        """
        The banned player's xbox user id (xuid), or `None` if not available.
        """
        ...

class PlayerBanList:
    """
    Represents a ban list, containing banned players.
    """
    def get_ban_entry(self, name: str, uuid: uuid.UUID | None = None, xuid: str | None = None) -> PlayerBanEntry | None:
        """
        Gets a `BanEntry` by player name, UUID, or XUID.

        Args:
            name: The player name to search for.
            uuid: The UUID of the player to search for, `None` if not used.
            xuid: The XUID of the player to search for, `None` if not used.

        Returns:
            The corresponding entry, or `None` if none found.
        """
        ...
    def add_ban(
        self,
        name: str,
        uuid: uuid.UUID | None = None,
        xuid: str | None = None,
        reason: str | None = None,
        expires: datetime.datetime | None = None,
        source: str | None = None,
    ) -> PlayerBanEntry:
        """
        Adds a ban to this list. If a previous ban exists, this will update the previous entry.

        Args:
            name: The name of the target.
            uuid: The UUID of the target, `None` if not used.
            xuid: The XUID of the target, `None` if not used.
            reason: The reason for the ban, `None` indicates implementation default.
            expires: The date for the ban's expiration (unban), or `None` to imply forever.
            source: The source of the ban, `None` indicates implementation default.

        Returns:
            The entry for the newly created ban, or the entry for the (updated) previous ban.
        """
        ...
    def is_banned(self, name: str, uuid: uuid.UUID | None = None, xuid: str | None = None) -> bool:
        """
        Checks if a `BanEntry` exists for the target, indicating an active ban status.

        Args:
            name: The player name to find.
            uuid: The UUID of the target, `None` if not used.
            xuid: The XUID of the target, `None` if not used.

        Returns:
            `True` if a `BanEntry` exists for the target, indicating an active ban status, `False` otherwise.
        """
        ...
    def remove_ban(self, name: str, uuid: uuid.UUID | None = None, xuid: str | None = None) -> None:
        """
        Removes the specified player from this list, therefore indicating a "not banned" status.

        Args:
            name: The player name to remove from this list.
            uuid: The unique id of the player, `None` if not used.
            xuid: The xbox user id (xuid) of the player, `None` if not used.
        """
        ...
    @property
    def entries(self) -> list[PlayerBanEntry]:
        """
        A vector containing pointers to every entry tracked by this list.
        """
        ...
