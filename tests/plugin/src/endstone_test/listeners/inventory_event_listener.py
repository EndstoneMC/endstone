from endstone.event import (
    EnchantItemEvent,
    InventoryCloseEvent,
    InventoryOpenEvent,
    event_handler,
)

from .event_listener import EventListener


class InventoryEventListener(EventListener):
    @event_handler
    def on_inventory_open(self, event: InventoryOpenEvent):
        self.record(
            event,
            f"{event.player.name} opened an inventory of {event.inventory.size} slots",
            player=event.player.name,
            size=event.inventory.size,
        )

    @event_handler
    def on_inventory_close(self, event: InventoryCloseEvent):
        self.record(
            event,
            f"{event.player.name} closed an inventory of {event.inventory.size} slots",
            player=event.player.name,
            size=event.inventory.size,
        )

    @event_handler
    def on_enchant_item(self, event: EnchantItemEvent):
        self.record(
            event,
            f"{event.enchanter.name} enchants {event.item} using option "
            f"{event.which_button}",
            always_log=True,
            player=event.enchanter.name,
            block_type=str(event.enchant_block.type),
            item_type=str(event.item.type),
            exp_level_cost=event.exp_level_cost,
            enchants={
                str(enchantment.id): level
                for enchantment, level in event.enchants_to_add.items()
            },
            which_button=event.which_button,
        )
