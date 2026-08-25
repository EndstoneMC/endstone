from endstone.event import (
    InventoryCloseEvent,
    InventoryInteractEvent,
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
    def on_inventory_interact(self, event: InventoryInteractEvent):
        self.record(
            event,
            f"{event.who_clicked.name} interacted with an inventory of {event.inventory.size} slots",
            always_log=True,
            player=event.who_clicked.name,
            size=event.inventory.size,
        )
