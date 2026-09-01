from endstone.event import (
    BlockBreakEvent,
    BlockCookEvent,
    BlockExplodeEvent,
    BlockFormEvent,
    BlockFromToEvent,
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
            recipe_id=event.recipe.id if event.recipe else None,
            recipe_tag=event.recipe.tag if event.recipe else None,
            recipe_input_matches=event.recipe.input_choice.test(event.source)
            if event.recipe
            else False,
            recipe_result_type=str(event.recipe.result.type) if event.recipe else None,
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
        if self.due(event, CANCEL):
            self.cancelled(event, block_type=str(event.block.type))

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

    @event_handler
    def on_cauldron_level_change(self, event: CauldronLevelChangeEvent):
        self.record(
            event,
            f"{event.block} changes ({event.reason})",
            always_log=True,
            block_type=str(event.block.type),
            new_type=str(event.new_state.type),
            reason=str(event.reason),
            actor_type=None if event.actor is None else str(event.actor.type),
        )
        if self.due(event, CANCEL):
            self.cancelled(event, reason=str(event.reason))
        elif self.due(event, MUTATE):
            before = str(event.new_state.type)
            event.new_state.type = "minecraft:cauldron"
            self.mutated(event, before=before, after=str(event.new_state.type))
