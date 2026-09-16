from endstone.event import (
    BlockBreakEvent,
    BlockCookEvent,
    BlockPistonExtendEvent,
    BlockPistonRetractEvent,
    BlockPlaceEvent,
    LeavesDecayEvent,
    event_handler,
)
from endstone.inventory import ItemStack

from .event_listener import EventListener


class BlockEventListener(EventListener):
    @event_handler
    def on_block_break(self, event: BlockBreakEvent):
        self.plugin.on_event_triggered(
            event, f"{event.player.name} breaks a block {event.block}"
        )

    @event_handler
    def on_block_cook(self, event: BlockCookEvent):
        self.plugin.on_event_triggered(
            event, f"{event.block} cooked an item {event.source}"
        )
        if event.result.type == "minecraft:cooked_cod":
            event.result = ItemStack("minecraft:apple")

    @event_handler
    def on_block_placed(self, event: BlockPlaceEvent):
        self.plugin.on_event_triggered(
            event,
            f"{event.player.name} places a {event.block_placed_state} against {event.block_against} (was {event.block})",
        )

    @event_handler
    def on_piston_extend(self, event: BlockPistonExtendEvent):
        self.plugin.on_event_triggered(
            event, f"Piston ({event.block}) expands towards {event.direction}", True
        )
        # event.cancel()

    @event_handler
    def on_piston_retract(self, event: BlockPistonRetractEvent):
        self.plugin.on_event_triggered(
            event, f"Piston ({event.block}) retracts towards {event.direction}", True
        )
        # event.cancel()

    @event_handler
    def on_leaves_decay(self, event: LeavesDecayEvent):
        self.plugin.on_event_triggered(event, f"Leaves ({event.block}) decayed", True)
