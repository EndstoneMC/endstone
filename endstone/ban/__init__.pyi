"""
Classes relevant to bans.
"""

import datetime
import uuid

__all__ = ["BanEntry", "IpBanEntry", "IpBanList", "PlayerBanEntry", "PlayerBanList"]

class BanEntry:
    """
    A single entry from a ban list.
    """
    @property
    def created(self) -> datetime.datetime:
        """
        Gets or sets the date this ban entry was created.
        """
        ...
    @created.setter
    def created(self, arg1: datetime.datetime) -> None: ...
    @property
    def source(self) -> str:
        """
        Gets or sets the source of this ban.
        """
        ...
    @source.setter
    def source(self, arg1: str) -> None: ...
    @property
    def expiration(self) -> datetime.datetime | None:
        """
        Gets or sets the date this ban expires on.
        """
        ...
    @expiration.setter
    def expiration(self, arg1: datetime.datetime | None) -> None: ...
    @property
    def reason(self) -> str:
        """
        Gets or sets the reason for this ban.
        """
        ...
    @reason.setter
    def reason(self, arg1: str) -> None: ...

class IpBanEntry(BanEntry):
    """
    Represents a ban entry for an IP address.
    """
    def __init__(self, address: str) -> None: ...
    @property
    def address(self) -> str:
        """
        Gets the banned IP address.
        """
        ...

class IpBanList:
    """
    Represents a ban list containing banned IP addresses.
    """
    def get_ban_entry(self, address: str) -> IpBanEntry:
        """
        Gets a BanEntry by IP address.
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
        Adds a ban to this list, or updates an existing one.
        """
        ...
    def is_banned(self, address: str) -> bool:
        """
        Checks if a BanEntry exists for the target by IP address.
        """
        ...
    def remove_ban(self, address: str) -> None:
        """
        Removes an IP address from the ban list.
        """
        ...
    @property
    def entries(self) -> list[IpBanEntry]:
        """
        Gets a vector of pointers to entries in the ban list.
        """
        ...

class PlayerBanEntry(BanEntry):
    """
    Represents a ban entry for a player.
    """
    def __init__(self, name: str, uuid: uuid.UUID | None = None, xuid: str | None = None) -> None: ...
    @property
    def name(self) -> str:
        """
        Gets the banned player's name.
        """
        ...
    @property
    def unique_id(self) -> uuid.UUID | None:
        """
        Gets the banned player's unique ID, or None if not available.
        """
        ...
    @property
    def xuid(self) -> str | None:
        """
        Gets the banned player's Xbox user ID (XUID), or None if not available.
        """
        ...

class PlayerBanList:
    """
    Represents a ban list containing banned players.
    """
    def get_ban_entry(self, name: str, uuid: uuid.UUID | None = None, xuid: str | None = None) -> PlayerBanEntry:
        """
        Gets a BanEntry by player name, UUID, or XUID.
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
        Adds a ban to this list, or updates an existing one.
        """
        ...
    def is_banned(self, name: str, uuid: uuid.UUID | None = None, xuid: str | None = None) -> bool:
        """
        Checks if a BanEntry exists for the target by name, UUID, or XUID.
        """
        ...
    def remove_ban(self, name: str, uuid: uuid.UUID | None = None, xuid: str | None = None) -> None:
        """
        Removes a player from the ban list by name, UUID, or XUID.
        """
        ...
    @property
    def entries(self) -> list[PlayerBanEntry]:
        """
        Gets a vector of pointers to entries in the ban list.
        """
        ...
