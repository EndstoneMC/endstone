from endstone.enchantments import Enchantment, EnchantmentOffer
from endstone.event import (
    EnchantItemEvent,
    InventoryCloseEvent,
    InventoryInteractEvent,
    InventoryOpenEvent,
    PrepareItemEnchantEvent,
    event_handler,
)

from endstone_test.checks import CANCEL, MUTATE

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
        if self.due(event, CANCEL):
            self.cancelled(event, player=event.player.name, size=event.inventory.size)

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
        if self.due(event, CANCEL):
            self.cancelled(event, item_type=str(event.item.type))
        elif self.due(event, MUTATE):
            before = event.exp_level_cost
            event.exp_level_cost = 1
            event.enchants_to_add = {Enchantment.UNBREAKING: 3}
            self.mutated(
                event,
                cost_before=before,
                cost_after=event.exp_level_cost,
                enchants_after={
                    str(e.id): level for e, level in event.enchants_to_add.items()
                },
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
        if self.due(event, CANCEL):
            self.cancelled(event, item_type=str(event.item.type))
        elif self.due(event, MUTATE):
            offer = EnchantmentOffer({Enchantment.UNBREAKING: 1}, 1)
            event.offers = [offer, offer, offer]
            self.mutated(event, costs=[o.cost for o in event.offers if o])
