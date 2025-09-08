"""
Classes relating to manage the client side score display system.
"""

from __future__ import annotations

import enum
import typing

from endstone import Player
from endstone.actor import Actor

__all__: list[str] = ["Criteria", "DisplaySlot", "Objective", "ObjectiveSortOrder", "RenderType", "Score", "Scoreboard"]

class Criteria:
    """
    Represents a scoreboard criteria.
    """

    class Type(enum.Enum):
        """
        Represents a scoreboard criteria.
        """

        DUMMY: typing.ClassVar[Criteria.Type]  # value = <Type.DUMMY: 0>

    DUMMY: typing.ClassVar[Criteria.Type]  # value = <Type.DUMMY: 0>

    @property
    def default_render_type(self) -> RenderType: ...
    @property
    def is_read_only(self) -> bool: ...
    @property
    def name(self) -> str: ...

class DisplaySlot(enum.Enum):
    """
    Locations for displaying objectives to the player
    """

    BELOW_NAME: typing.ClassVar[DisplaySlot]  # value = <DisplaySlot.BELOW_NAME: 0>
    PLAYER_LIST: typing.ClassVar[DisplaySlot]  # value = <DisplaySlot.PLAYER_LIST: 1>
    SIDE_BAR: typing.ClassVar[DisplaySlot]  # value = <DisplaySlot.SIDE_BAR: 2>

class Objective:
    """
    Represents an objective on a scoreboard that can show scores specific to entries.
    """

    __hash__: typing.ClassVar[None] = None

    def __eq__(self, arg0: Objective) -> bool: ...
    def __ne__(self, arg0: Objective) -> bool: ...
    def get_score(self, entry: Player | Actor | str) -> Score:
        """
        Gets an entry's Score for this objective
        """

    def set_display(
        self,
        slot: DisplaySlot | None,
        order: ObjectiveSortOrder | None = None,
    ) -> None:
        """
        Sets the display slot and sort order for this objective. This will remove it from any other display slot.
        """

    def unregister(self) -> None:
        """
        Unregisters this objective from the associated Scoreboard.
        """

    @property
    def criteria(self) -> Criteria:
        """
        Gets the criteria this objective tracks
        """

    @property
    def display_name(self) -> str:
        """
        Gets or sets the name displayed to players for this objective
        """

    @display_name.setter
    def display_name(self, arg1: str) -> None: ...
    @property
    def display_slot(self) -> DisplaySlot | None:
        """
        Gets or sets the display slot this objective is displayed at
        """

    @display_slot.setter
    def display_slot(self, arg1: DisplaySlot | None) -> None: ...
    @property
    def is_displayed(self) -> bool:
        """
        Gets if the objective is currently displayed in a slot.
        """

    @property
    def is_modifiable(self) -> bool:
        """
        Gets if the objective's scores can be modified directly by a plugin
        """

    @property
    def name(self) -> str:
        """
        Gets the name of this Objective
        """

    @property
    def render_type(self) -> RenderType:
        """
        Gets the manner in which this objective will be rendered.
        """

    @property
    def scoreboard(self) -> Scoreboard:
        """
        Gets the scoreboard to which this objective is attached
        """

    @property
    def sort_order(self) -> ObjectiveSortOrder | None:
        """
        Gets or sets the sort order for this objective
        """

    @sort_order.setter
    def sort_order(self, arg1: ObjectiveSortOrder) -> None: ...

class ObjectiveSortOrder(enum.Enum):
    """
    Represents the sort order of objectives on a DisplaySlot.
    """

    ASCENDING: typing.ClassVar[ObjectiveSortOrder]  # value = <ObjectiveSortOrder.ASCENDING: 0>
    DESCENDING: typing.ClassVar[ObjectiveSortOrder]  # value = <ObjectiveSortOrder.DESCENDING: 1>

class RenderType(enum.Enum):
    """
    Controls the way in which an Objective is rendered on the client side.
    """

    HEARTS: typing.ClassVar[RenderType]  # value = <RenderType.HEARTS: 1>
    INTEGER: typing.ClassVar[RenderType]  # value = <RenderType.INTEGER: 0>

class Score:
    """
    Represents a score for an objective on a scoreboard.
    """

    @property
    def entry(self) -> Player | Actor | str:
        """
        Gets the entry being tracked by this Score
        """

    @property
    def is_score_set(self) -> bool:
        """
        Shows if this score has been set at any point in time.
        """

    @property
    def objective(self) -> Objective:
        """
        Gets the Objective being tracked by this Score.
        """

    @property
    def scoreboard(self) -> Scoreboard:
        """
        Gets the scoreboard for the associated objective.
        """

    @property
    def value(self) -> int:
        """
        Gets or sets the current score.
        """

    @value.setter
    def value(self, arg1: typing.SupportsInt) -> None: ...

class Scoreboard:
    """
    Represents a scoreboard
    """

    def add_objective(
        self,
        name: str,
        criteria: Criteria.Type,
        display_name: str,
        render_type: RenderType = RenderType.INTEGER,
    ) -> Objective:
        """
        Registers an Objective on this Scoreboard with a name displayed to players
        """

    def clear_slot(self, slot: DisplaySlot) -> None:
        """
        Clears any objective in the specified slot
        """

    @typing.overload
    def get_objective(self, name: str) -> Objective:
        """
        Gets an Objective on this Scoreboard by name
        """

    @typing.overload
    def get_objective(self, slot: DisplaySlot) -> Objective:
        """
        Gets the Objective currently displayed in a DisplaySlot on this Scoreboard
        """

    def get_objectives_by_criteria(self, criteria: Criteria.Type) -> list[Objective]:
        """
        Gets all Objectives of a Criteria on the Scoreboard
        """

    def get_scores(self, entry: Player | Actor | str) -> list[Score]:
        """
        Gets all scores for an entry on this Scoreboard
        """

    def reset_scores(self, entry: Player | Actor | str) -> None:
        """
        Removes all scores for an entry on this Scoreboard
        """

    @property
    def entries(self) -> list[Player | Actor | str]:
        """
        Gets all entries tracked by this Scoreboard
        """

    @property
    def objectives(self) -> list[Objective]:
        """
        Gets all Objectives on this Scoreboard
        """
