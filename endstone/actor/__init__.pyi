"""
Classes relating to actors (entities) that can exist in a world, including all players, monsters, projectiles, etc.
"""

import typing

from endstone.attribute import AttributeInstance
from endstone.command import CommandSender
from endstone.inventory import ItemStack
from endstone.level import Dimension, Level, Location
from endstone.util import Vector

__all__ = ["Actor", "Item", "Mob"]

class Actor(CommandSender):
    """
    Represents a base actor in the level.
    """
    @property
    def type(self) -> str:
        """
        Gets the type of the actor.
        """
        ...
    @property
    def runtime_id(self) -> int:
        """
        Returns the runtime id for this actor.
        """
        ...
    @property
    def location(self) -> Location:
        """
        Gets the actor's current position.
        """
        ...
    @property
    def velocity(self) -> Vector:
        """
        Gets this actor's current velocity.
        """
        ...
    @property
    def is_on_ground(self) -> bool:
        """
        Returns true if the actor is supported by a block, i.e. on ground.
        """
        ...
    @property
    def is_in_water(self) -> bool:
        """
        Returns true if the actor is in water.
        """
        ...
    @property
    def is_in_lava(self) -> bool:
        """
        Returns true if the actor is in lava.
        """
        ...
    @property
    def level(self) -> Level:
        """
        Gets the current Level this actor resides in.
        """
        ...
    @property
    def dimension(self) -> Dimension:
        """
        Gets the current Dimension this actor resides in.
        """
        ...
    def set_rotation(self, yaw: float, pitch: float) -> None:
        """
        Sets the actor's rotation.
        """
        ...
    @typing.overload
    def teleport(self, location: Location) -> bool:
        """
        Teleports this actor to the given location.
        """
        ...
    @typing.overload
    def teleport(self, target: Actor) -> bool:
        """
        Teleports this actor to the target Actor.
        """
        ...
    @property
    def id(self) -> int:
        """
        Returns a unique id for this actor.
        """
        ...
    def remove(self) -> None:
        """
        Remove this actor from the level.
        """
        ...
    @property
    def is_valid(self) -> bool:
        """
        Returns false if the entity has died, been despawned for some other reason, or has not been added to the level.
        """
        ...
    @property
    def is_dead(self) -> bool:
        """
        Returns true if this actor has been marked for removal.
        """
        ...
    @property
    def scoreboard_tags(self) -> list[str]:
        """
        Returns a list of scoreboard tags for this actor.
        """
        ...
    def add_scoreboard_tag(self, tag: str) -> bool:
        """
        Adds a tag to this actor.
        """
        ...
    def remove_scoreboard_tag(self, tag: str) -> bool:
        """
        Removes a given tag from this actor.
        """
        ...
    @property
    def is_name_tag_visible(self) -> bool:
        """
        Gets or sets if the actor's name tag is visible or not.
        """
        ...
    @is_name_tag_visible.setter
    def is_name_tag_visible(self, arg1: bool) -> None: ...
    @property
    def is_name_tag_always_visible(self) -> bool:
        """
        Gets or sets if the actor's name tag is always visible or not.
        """
        ...
    @is_name_tag_always_visible.setter
    def is_name_tag_always_visible(self, arg1: bool) -> None: ...
    @property
    def name_tag(self) -> str:
        """
        Gets or sets the current name tag of the actor.
        """
        ...
    @name_tag.setter
    def name_tag(self, arg1: str) -> None: ...
    @property
    def score_tag(self) -> str:
        """
        Gets or sets the current score tag of the actor.
        """
        ...
    @score_tag.setter
    def score_tag(self, arg1: str) -> None: ...
    def has_attribute(self, attribute: str) -> bool:
        """
        Check the attribute is present in the object.
        """
        ...
    def get_attribute(self, attribute: str) -> AttributeInstance:
        """
        Gets the specified attribute instance from the object. This instance will be backed directly to the object and any changes will be visible at once.
        """
        ...
    @property
    def attributes(self) -> list[AttributeInstance]:
        """
        Gets all attribute instances from the object. This instance will be backed directly to the object and any changes will be visible at once.
        """
        ...

class Mob(Actor):
    """
    Represents a mobile entity (i.e. living entity), such as a monster or player.
    """
    @property
    def is_gliding(self) -> bool:
        """
        Checks to see if an actor is gliding, such as using an Elytra.
        """
        ...
    @property
    def health(self) -> int:
        """
        Gets or sets the entity's health from 0 to its max possible value, where 0 is dead.
        """
        ...
    @health.setter
    def health(self, arg1: int) -> None: ...
    @property
    def max_health(self) -> int:
        """
        Gets or sets the maximum health this entity has.
        """
        ...
    @max_health.setter
    def max_health(self, arg1: int) -> None: ...

class Item(Actor):
    """
    Represents a base actor in the level.
    """
    @property
    def item_stack(self) -> ItemStack:
        """
        Gets or sets the item stack associated with this item drop.
        """
        ...
    @item_stack.setter
    def item_stack(self, arg1: ItemStack) -> None: ...
    @property
    def pickup_delay(self) -> int:
        """
        Gets or sets the delay before this Item is available to be picked up by players.
        """
        ...
    @pickup_delay.setter
    def pickup_delay(self, arg1: int) -> None: ...
    @property
    def is_unlimited_lifetime(self) -> bool:
        """
        Gets or sets if this Item lives forever
        """
        ...
    @is_unlimited_lifetime.setter
    def is_unlimited_lifetime(self, arg1: bool) -> None: ...
    @property
    def thrower(self) -> int | None:
        """
        Gets or sets the thrower of this item.
        """
        ...
    @thrower.setter
    def thrower(self, arg1: int | None) -> None: ...
