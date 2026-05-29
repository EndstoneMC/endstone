"""
Classes relating to manage the client side score display system.
"""

import enum
import typing

from endstone import Player
from endstone.actor import Actor

__all__ = [
    "Criteria",
    "DisplaySlot",
    "Objective",
    "ObjectiveSortOrder",
    "RenderType",
    "Score",
    "Scoreboard",
]

class RenderType(enum.Enum):
    """
    Controls the way in which an Objective is rendered on the client side.
    """

    INTEGER = 0
    HEARTS = 1

class DisplaySlot(enum.Enum):
    """
    Locations for displaying objectives to the player
    """

    BELOW_NAME = 0
    """
    Displays the score below the player's name.
    """
    PLAYER_LIST = 1
    """
    Displays the score in the player list on the pause screen.
    """
    SIDE_BAR = 2
    """
    Displays the score on the side of the player's screen.
    """

class ObjectiveSortOrder(enum.Enum):
    """
    Represents the sort order of objectives on a DisplaySlot.
    """

    ASCENDING = 0
    """
    Sorts the objectives in the ascending order
    """
    DESCENDING = 1
    """
    Sorts the objectives in the descending order
    """

class Scoreboard:
    """
    Represents a scoreboard.
    """
    def add_objective(
        self,
        name: str,
        criteria: Criteria.Type,
        display_name: str | None = None,
        render_type: RenderType = RenderType.INTEGER,
    ) -> Objective:
        """
        Registers an Objective on this Scoreboard.

        Args:
            name: Name of the Objective.
            criteria: Criteria type for the Objective.
            display_name: Name displayed to players for the Objective. Defaults to ``name``.
            render_type: Manner of rendering the Objective.

        Returns:
            The newly registered Objective.
        """
        ...
    @typing.overload
    def get_objective(self, name: str) -> Objective:
        """
        Gets an Objective on this Scoreboard by name.

        Args:
            name: Name of the Objective.

        Returns:
            The Objective, or None if it does not exist.
        """
        ...
    @typing.overload
    def get_objective(self, slot: DisplaySlot) -> Objective:
        """
        Gets the Objective currently displayed in a DisplaySlot on this Scoreboard.

        Args:
            slot: The DisplaySlot.

        Returns:
            The Objective currently displayed, or None if nothing is displayed in that DisplaySlot.
        """
        ...
    @property
    def objectives(self) -> list[Objective]:
        """
        A list of all Objectives on this Scoreboard.
        """
        ...
    def get_objectives_by_criteria(self, criteria: Criteria.Type) -> list[Objective]:
        """
        Gets all Objectives of a Criteria on the Scoreboard.

        Args:
            criteria: Criteria type to search by.

        Returns:
            A list of Objectives using the specified Criteria.
        """
        ...
    def get_scores(self, entry: Player | Actor | str) -> list[Score]:
        """
        Gets all scores for an entry on this Scoreboard.

        Args:
            entry: The entry whose scores are being retrieved.

        Returns:
            A list of all scores tracked for the entry.
        """
        ...
    def reset_scores(self, entry: Player | Actor | str) -> None:
        """
        Removes all scores for an entry on this Scoreboard.

        Args:
            entry: The entry to drop all current scores for.
        """
        ...
    @property
    def entries(self) -> list[Player | Actor | str]:
        """
        A list of all entries tracked by this Scoreboard.
        """
        ...
    def clear_slot(self, slot: DisplaySlot) -> None:
        """
        Clears any objective in the specified slot.

        Args:
            slot: The slot to remove objectives from.
        """
        ...

class Objective:
    """
    Represents an objective on a scoreboard that can show scores specific to entries.
    """
    @property
    def name(self) -> str:
        """
        The name of this Objective.
        """
        ...
    @property
    def display_name(self) -> str:
        """
        The name displayed to players for this objective.
        """
        ...
    @display_name.setter
    def display_name(self, arg1: str) -> None: ...
    @property
    def criteria(self) -> Criteria:
        """
        The criteria this objective tracks.
        """
        ...
    @property
    def is_modifiable(self) -> bool:
        """
        True if the objective's scores can be modified directly by a plugin.
        """
        ...
    @property
    def scoreboard(self) -> Scoreboard:
        """
        The scoreboard to which this objective is attached.
        """
        ...
    def unregister(self) -> None:
        """
        Unregisters this objective from the associated Scoreboard.
        """
        ...
    @property
    def is_displayed(self) -> bool:
        """
        True if the objective is currently displayed in a slot.
        """
        ...
    @property
    def display_slot(self) -> DisplaySlot | None:
        """
        The display slot this objective is displayed at.
        """
        ...
    @display_slot.setter
    def display_slot(self, arg1: DisplaySlot | None) -> None: ...
    @property
    def sort_order(self) -> ObjectiveSortOrder | None:
        """
        The sort order for this objective.
        """
        ...
    @sort_order.setter
    def sort_order(self, arg1: ObjectiveSortOrder) -> None: ...
    def set_display(self, slot: DisplaySlot | None, order: ObjectiveSortOrder = ObjectiveSortOrder.ASCENDING) -> None:
        """
        Sets the display slot and sort order for this objective.

        This will remove it from any other display slot.

        Args:
            slot: The display slot where this objective should be displayed.
            order: The sort order for this objective in the display slot.
        """
        ...
    @property
    def render_type(self) -> RenderType:
        """
        The manner in which this objective will be rendered.
        """
        ...
    def get_score(self, entry: Player | Actor | str) -> Score:
        """
        Gets an entry's Score for this objective.

        Args:
            entry: Entry for the Score.

        Returns:
            Score tracking the Objective and entry specified.
        """
        ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...

class Criteria:
    """
    Represents a scoreboard criteria.
    """
    class Type(enum.Enum):
        """
        Represents a scoreboard criteria.
        """

        DUMMY = 0
        """
        The dummy criteria. Not changed by the server.
        """

    DUMMY = Type.DUMMY
    @property
    def name(self) -> str:
        """
        The name of this criteria (its unique id).
        """
        ...
    @property
    def is_read_only(self) -> bool:
        """
        True if this criteria is read only.

        If read only, scoreboards with this criteria cannot have their scores changed.
        """
        ...
    @property
    def default_render_type(self) -> RenderType:
        """
        The RenderType used by default for this criteria.
        """
        ...

class Score:
    """
    Represents a score for an objective on a scoreboard.
    """
    @property
    def entry(self) -> Player | Actor | str:
        """
        The entry being tracked by this Score.
        """
        ...
    @property
    def value(self) -> int:
        """
        The current score.
        """
        ...
    @value.setter
    def value(self, arg1: int) -> None: ...
    @property
    def is_score_set(self) -> bool:
        """
        True if this score has been set at any point in time.
        """
        ...
    @property
    def objective(self) -> Objective:
        """
        The Objective being tracked by this Score.
        """
        ...
    @property
    def scoreboard(self) -> Scoreboard:
        """
        The scoreboard for the associated objective.
        """
        ...
