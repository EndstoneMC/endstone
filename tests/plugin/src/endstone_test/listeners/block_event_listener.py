from endstone.event import (
    BlockBreakEvent,
    BlockCookEvent,
    BlockExplodeEvent,
    BlockFormEvent,
    BlockFromToEvent,
    BlockGrowEvent,
    BlockPistonExtendEvent,
    BlockPistonRetractEvent,
    BlockPlaceEvent,
    CauldronLevelChangeEvent,
    LeavesDecayEvent,
    event_handler,
)

from endstone_test.checks import CANCEL, MUTATE

from .event_listener import EventListener


class BlockEventListener(EventListener):
    @event_handler
    def on_block_grow(self, event: BlockGrowEvent):
        cancelled_before = event.is_cancelled
        if self.plugin.should_cancel_event("BlockGrowEvent"):
            event.cancel()
        self.record(
            event,
            f"{event.block} grows into {event.new_state.type}",
            always_log=True,
            block_type=str(event.block.type),
            new_type=str(event.new_state.type),
            xyz=(event.block.x, event.block.y, event.block.z),
            cancelled_before=cancelled_before,
            cancelled=event.is_cancelled,
        )

    @event_handler
    def on_cauldron_level_change(self, event: CauldronLevelChangeEvent):
        cancelled_before = event.is_cancelled
        if self.plugin.should_cancel_event("CauldronLevelChangeEvent"):
            event.cancel()
        self.record(
            event,
            f"{event.block} cauldron changes due to {event.reason}",
            always_log=True,
            block_type=str(event.block.type),
            actor_type=str(event.actor.type) if event.actor is not None else None,
            reason=str(event.reason),
            new_type=str(event.new_state.type),
            cancelled_before=cancelled_before,
            cancelled=event.is_cancelled,
        )
        if self.due(event, CANCEL):
            self.cancelled(event, reason=str(event.reason))
        elif self.due(event, MUTATE):
            before = str(event.new_state.type)
            event.new_state.type = "minecraft:cauldron"
            self.mutated(event, before=before, after=str(event.new_state.type))

    @event_handler
    def on_block_break(self, event: BlockBreakEvent):
        cancelled_before = event.is_cancelled
        if self.plugin.should_cancel_event("BlockBreakEvent"):
            event.cancel()
        self.record(
            event,
            f"{event.player.name} breaks a block {event.block}",
            player=event.player.name,
            block_type=str(event.block.type),
            xyz=(event.block.x, event.block.y, event.block.z),
            cancelled_before=cancelled_before,
            cancelled=event.is_cancelled,
        )

    @event_handler
    def on_block_cook(self, event: BlockCookEvent):
        cancelled_before = event.is_cancelled
        if self.plugin.should_cancel_event("BlockCookEvent"):
            event.cancel()
        self.record(
            event,
            f"{event.block} cooked an item {event.source} -> {event.result}",
            block_type=str(event.block.type),
            source_type=str(event.source.type),
            result_type=str(event.result.type),
            recipe_id=event.recipe.id if event.recipe else None,
            recipe_tag=event.recipe.tag if event.recipe else None,
            recipe_input_matches=event.recipe.input_choice.test(event.source)
            if event.recipe
            else False,
            recipe_result_type=str(event.recipe.result.type) if event.recipe else None,
            cancelled_before=cancelled_before,
            cancelled=event.is_cancelled,
        )

    @event_handler
    def on_block_explode(self, event: BlockExplodeEvent):
        cancelled_before = event.is_cancelled
        if self.plugin.should_cancel_event("BlockExplodeEvent"):
            event.cancel()
        self.record(
            event,
            f"{event.block} exploded, destroying {len(event.block_list)} block(s)",
            block_type=str(event.block.type),
            block_count=len(event.block_list),
            cancelled_before=cancelled_before,
            cancelled=event.is_cancelled,
        )

    @event_handler
    def on_block_form(self, event: BlockFormEvent):
        cancelled_before = event.is_cancelled
        if self.plugin.should_cancel_event("BlockFormEvent"):
            event.cancel()
        self.record(
            event,
            f"{event.block} formed into {event.new_state.type}",
            block_type=str(event.block.type),
            new_type=str(event.new_state.type),
            xyz=(event.block.x, event.block.y, event.block.z),
            cancelled_before=cancelled_before,
            cancelled=event.is_cancelled,
        )
        if self.due(event, CANCEL):
            self.cancelled(event, block_type=str(event.block.type))

    @event_handler
    def on_block_from_to(self, event: BlockFromToEvent):
        cancelled_before = event.is_cancelled
        if self.plugin.should_cancel_event("BlockFromToEvent"):
            event.cancel()
        self.record(
            event,
            f"{event.block} flows to {event.to_block}",
            block_type=str(event.block.type),
            to_block_type=str(event.to_block.type),
            xyz=(event.block.x, event.block.y, event.block.z),
            cancelled_before=cancelled_before,
            cancelled=event.is_cancelled,
        )

    @event_handler
    def on_block_place(self, event: BlockPlaceEvent):
        cancelled_before = event.is_cancelled
        if self.plugin.should_cancel_event("BlockPlaceEvent"):
            event.cancel()
        self.record(
            event,
            f"{event.player.name} places {event.block_placed} against "
            f"{event.block_against} (replacing {event.block_replaced_state})",
            player=event.player.name,
            block_type=str(event.block.type),
            block_placed_type=str(event.block_placed.type),
            block_replaced_type=str(event.block_replaced_state.type),
            block_against_type=str(event.block_against.type),
            cancelled_before=cancelled_before,
            cancelled=event.is_cancelled,
        )

    @event_handler
    def on_piston_extend(self, event: BlockPistonExtendEvent):
        cancelled_before = event.is_cancelled
        if self.plugin.should_cancel_event("BlockPistonExtendEvent"):
            event.cancel()
        self.record(
            event,
            f"Piston ({event.block}) extends towards {event.direction}",
            always_log=True,
            block_type=str(event.block.type),
            direction=str(event.direction),
            cancelled_before=cancelled_before,
            cancelled=event.is_cancelled,
        )

    @event_handler
    def on_piston_retract(self, event: BlockPistonRetractEvent):
        cancelled_before = event.is_cancelled
        if self.plugin.should_cancel_event("BlockPistonRetractEvent"):
            event.cancel()
        self.record(
            event,
            f"Piston ({event.block}) retracts towards {event.direction}",
            always_log=True,
            block_type=str(event.block.type),
            direction=str(event.direction),
            cancelled_before=cancelled_before,
            cancelled=event.is_cancelled,
        )

    @event_handler
    def on_leaves_decay(self, event: LeavesDecayEvent):
        cancelled_before = event.is_cancelled
        if self.plugin.should_cancel_event("LeavesDecayEvent"):
            event.cancel()
        self.record(
            event,
            f"Leaves ({event.block}) decayed",
            always_log=True,
            block_type=str(event.block.type),
            cancelled_before=cancelled_before,
            cancelled=event.is_cancelled,
        )
