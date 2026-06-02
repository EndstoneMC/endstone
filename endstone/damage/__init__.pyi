"""
Classes relating to damage types and sources applicable to mobs (living entities).
"""

from endstone.actor import Actor

__all__ = [
    "DamageSource",
]

class DamageSource:
    """
    Represents a source of damage.
    """
    @property
    def type(self) -> str:
        """
        The damage type.
        """
        ...
    @property
    def actor(self) -> Actor:
        """
        The actor that caused the damage to occur.

        Not to be confused with `damaging_actor`, the returned actor is the actor to which the damage is
        ultimately attributed if the receiver is killed. If, for example, the receiver was damaged by a projectile, the
        shooter/thrower would be returned.
        """
        ...
    @property
    def damaging_actor(self) -> Actor:
        """
        The actor that directly caused the damage.

        Not to be confused with `actor`, the returned actor is the actor that actually inflicted the
        damage. If, for example, the receiver was damaged by a projectile, the projectile would be returned.
        """
        ...
    @property
    def is_indirect(self) -> bool:
        """
        Whether this damage is indirect.

        Damage is considered indirect if `actor` is not equal to `damaging_actor`. This will be the case, for
        example, if a skeleton shot an arrow or a player threw a potion.
        """
        ...
    def __str__(self) -> str: ...
