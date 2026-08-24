from endstone.event import (
    BlockBreakEvent,
    BlockCookEvent,
    BlockExplodeEvent,
    BlockFormEvent,
    BlockFromToEvent,
    BlockPistonExtendEvent,
    BlockPistonRetractEvent,
    BlockPlaceEvent,
    LeavesDecayEvent,
    event_handler,
)

from .event_listener import EventListener


class BlockEventListener(EventListener):
    @event_handler
    def on_block_break(self, event: BlockBreakEvent):
        self.record(
            event,
            f"{event.player.name} breaks a block {event.block}",
            player=event.player.name,
            block_type=str(event.block.type),
            xyz=(event.block.x, event.block.y, event.block.z),
        )

    @event_handler
    def on_block_cook(self, event: BlockCookEvent):
        self.record(
            event,
            f"{event.block} cooked an item {event.source} -> {event.result}",
            block_type=str(event.block.type),
            source_type=str(event.source.type),
            result_type=str(event.result.type),
        )

    @event_handler
    def on_block_explode(self, event: BlockExplodeEvent):
        self.record(
            event,
            f"{event.block} exploded, destroying {len(event.block_list)} block(s)",
            block_type=str(event.block.type),
            block_count=len(event.block_list),
        )

    @event_handler
    def on_block_form(self, event: BlockFormEvent):
        self.record(
            event,
            f"{event.block} formed into {event.new_state.type}",
            block_type=str(event.block.type),
            new_type=str(event.new_state.type),
            xyz=(event.block.x, event.block.y, event.block.z),
        )

    @event_handler
    def on_block_from_to(self, event: BlockFromToEvent):
        self.record(
            event,
            f"{event.block} flows to {event.to_block}",
            block_type=str(event.block.type),
            to_block_type=str(event.to_block.type),
        )

    @event_handler
    def on_block_place(self, event: BlockPlaceEvent):
        self.record(
            event,
            f"{event.player.name} places {event.block_placed} against "
            f"{event.block_against} (replacing {event.block_replaced_state})",
            player=event.player.name,
            block_type=str(event.block.type),
            block_placed_type=str(event.block_placed.type),
            block_replaced_type=str(event.block_replaced_state.type),
            block_against_type=str(event.block_against.type),
        )

    @event_handler
    def on_piston_extend(self, event: BlockPistonExtendEvent):
        self.record(
            event,
            f"Piston ({event.block}) extends towards {event.direction}",
            always_log=True,
            block_type=str(event.block.type),
            direction=str(event.direction),
        )

    @event_handler
    def on_piston_retract(self, event: BlockPistonRetractEvent):
        self.record(
            event,
            f"Piston ({event.block}) retracts towards {event.direction}",
            always_log=True,
            block_type=str(event.block.type),
            direction=str(event.direction),
        )

    @event_handler
    def on_leaves_decay(self, event: LeavesDecayEvent):
        self.record(
            event,
            f"Leaves ({event.block}) decayed",
            always_log=True,
            block_type=str(event.block.type),
        )
