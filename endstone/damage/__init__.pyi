"""
Classes relating to damage types and sources applicable to mobs (living entities).
"""

from endstone.actor import Actor

__all__ = ["DamageSource"]

class DamageSource:
    """
    Represents a source of damage.
    """
    @property
    def type(self) -> str:
        """
        Get the damage type.
        """
        ...
    @property
    def actor(self) -> Actor:
        """
        Get the actor that caused the damage to occur.
        """
        ...
    @property
    def damaging_actor(self) -> Actor:
        """
        Get the actor that directly caused the damage.
        """
        ...
    @property
    def is_indirect(self) -> bool:
        """
        Get if this damage is indirect.
        """
        ...
    def __str__(self) -> str: ...
