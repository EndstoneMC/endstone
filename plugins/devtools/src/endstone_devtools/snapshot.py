"""Best-effort snapshot of the server's registries as plain, picklable data.

Only what the public registry API exposes today is included; sections grow as the
API does. Everything returned here must stay picklable (plain dicts/lists/scalars)
so it can cross the process boundary to the GUI unchanged — no live server objects.
"""

from __future__ import annotations

from typing import TYPE_CHECKING

from endstone.actor import ActorType
from endstone.block import BlockType
from endstone.enchantments import Enchantment
from endstone.inventory import ItemType

if TYPE_CHECKING:
    from endstone import Server


def collect(server: Server) -> dict:
    """Snapshot every registry we can read. Must run on the server thread."""
    return {
        "blocks": [_block(b) for b in server.get_registry(BlockType)],
        "items": [_item(i) for i in server.get_registry(ItemType)],
        "enchantments": [_enchantment(e) for e in server.get_registry(Enchantment)],
        "actors": [_actor(a) for a in server.get_registry(ActorType)],
    }


def _block(b) -> dict:
    return {
        "id": str(b.id),
        "translation_key": b.translation_key,
        "has_item_type": b.has_item_type,
    }


def _item(i) -> dict:
    return {
        "id": str(i.id),
        "translation_key": i.translation_key,
        "max_stack_size": i.max_stack_size,
        "max_durability": i.max_durability,
    }


def _enchantment(e) -> dict:
    return {
        "id": str(e.id),
        "translation_key": e.translation_key,
        "start_level": e.start_level,
        "max_level": e.max_level,
    }


def _actor(a) -> dict:
    return {
        "id": str(a.id),
        "translation_key": a.translation_key,
    }
