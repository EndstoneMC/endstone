from endstone.event import (
    EnchantItemEvent,
    InventoryCloseEvent,
    InventoryInteractEvent,
    InventoryOpenEvent,
    PrepareItemEnchantEvent,
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
    def on_inventory_interact(self, event: InventoryInteractEvent):
        self.record(
            event,
            f"{event.who_clicked.name} interacted with an inventory of {event.inventory.size} slots",
            always_log=True,
            player=event.who_clicked.name,
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

    @event_handler
    def on_prepare_item_enchant(self, event: PrepareItemEnchantEvent):
        self.record(
            event,
            f"{event.enchanter.name} prepares enchanting offers for {event.item}",
            always_log=True,
            player=event.enchanter.name,
            inventory_size=event.inventory.size,
            block_type=str(event.enchant_block.type),
            item_type=str(event.item.type),
            offers=[
                None
                if offer is None
                else {
                    "cost": offer.cost,
                    "enchants": {
                        str(enchantment.id): level
                        for enchantment, level in offer.enchants.items()
                    },
                }
                for offer in event.offers
            ],
            enchantment_bonus=event.enchantment_bonus,
        )
