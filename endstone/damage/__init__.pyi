"""
Classes relating to damage types and sources applicable to mobs (living entities).
"""

from __future__ import annotations

import endstone._python.actor

__all__: list[str] = ["DamageSource"]

class DamageSource:
    """
    Represents a source of damage.
    """
    def __str__(self) -> str: ...
    @property
    def actor(self) -> endstone._python.actor.Actor:
        """
        Get the actor that caused the damage to occur.
        """
    @property
    def damaging_actor(self) -> endstone._python.actor.Actor:
        """
        Get the actor that directly caused the damage.
        """
    @property
    def is_indirect(self) -> bool:
        """
        Get if this damage is indirect.
        """
    @property
    def type(self) -> str:
        """
        Get the damage type.
        """
