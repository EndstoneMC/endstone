"""
Classes relating to the boss bars that appear at the top of the player's screen.
"""

import enum

from endstone import Player

__all__ = ["BarColor", "BarFlag", "BarStyle", "BossBar"]

class BarColor(enum.Enum):
    PINK = 0
    BLUE = 1
    RED = 2
    GREEN = 3
    YELLOW = 4
    PURPLE = 5
    REBECCA_PURPLE = 6
    WHITE = 7

class BarFlag(enum.Enum):
    DARKEN_SKY = 0
    """
    Darkens the sky like during fighting a wither.
    """
    CREATE_FOG = 1
    """
    Creates fog around the world.
    """

class BarStyle(enum.Enum):
    SOLID = 0
    """
    Makes the boss bar solid (no segments)
    """
    SEGMENTED_6 = 1
    """
    Splits the boss bar into 6 segments
    """
    SEGMENTED_10 = 2
    """
    Splits the boss bar into 10 segments
    """
    SEGMENTED_12 = 3
    """
    Splits the boss bar into 12 segments
    """
    SEGMENTED_20 = 4
    """
    Splits the boss bar into 20 segments
    """

class BossBar:
    """
    Represents a boss bar that is displayed to players.
    """
    @property
    def title(self) -> str:
        """
        The title of this boss bar.
        """
        ...
    @title.setter
    def title(self, arg1: str) -> None: ...
    @property
    def color(self) -> BarColor:
        """
        The color of this boss bar.
        """
        ...
    @color.setter
    def color(self, arg1: BarColor) -> None: ...
    @property
    def style(self) -> BarStyle:
        """
        The style of this boss bar.
        """
        ...
    @style.setter
    def style(self, arg1: BarStyle) -> None: ...
    @property
    def progress(self) -> float:
        """
        The progress of the bar between 0.0 and 1.0.
        """
        ...
    @progress.setter
    def progress(self, arg1: float) -> None: ...
    @property
    def is_visible(self) -> bool:
        """
        If the boss bar is displayed to attached players.
        """
        ...
    @is_visible.setter
    def is_visible(self, arg1: bool) -> None: ...
    def has_flag(self, flag: BarFlag) -> bool:
        """
        Checks whether this boss bar has the passed flag set.
        """
        ...
    def add_flag(self, flag: BarFlag) -> None:
        """
        Adds an optional flag to this boss bar.
        """
        ...
    def remove_flag(self, flag: BarFlag) -> None:
        """
        Removes an existing flag on this boss bar.
        """
        ...
    def add_player(self, player: Player) -> None:
        """
        Adds the player to this boss bar causing it to display on their screen.
        """
        ...
    def remove_player(self, player: Player) -> None:
        """
        Removes the player from this boss bar causing it to be removed from their screen.
        """
        ...
    def remove_all(self) -> None:
        """
        Removes all players from this boss bar.
        """
        ...
    @property
    def players(self) -> list[Player]:
        """
        Returns all players viewing this boss bar.
        """
        ...
