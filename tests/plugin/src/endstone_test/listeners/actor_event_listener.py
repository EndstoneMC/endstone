from endstone.actor import Item
from endstone.event import (
    ActorDamageEvent,
    ActorDeathEvent,
    ActorExplodeEvent,
    ActorKnockbackEvent,
    ActorRemoveEvent,
    ActorSpawnEvent,
    ActorTeleportEvent,
    event_handler,
)

from .event_listener import EventListener


class ActorEventListener(EventListener):
    @event_handler
    def on_actor_damaged(self, event: ActorDamageEvent):
        self.plugin.on_event_triggered(
            event,
            f"{event.actor.name} ({event.actor.type}) hurt (source: {event.damage_source}, damage: {event.damage}).",
        )

    @event_handler
    def on_actor_death(self, event: ActorDeathEvent):
        self.plugin.on_event_triggered(
            event,
            f"{event.actor.name} ({event.actor.type}) died (source: {event.damage_source}).",
        )

    @event_handler
    def on_actor_explode(self, event: ActorExplodeEvent):
        self.plugin.on_event_triggered(
            event, f"{event.actor.name} ({event.actor.type}) exploded."
        )

    @event_handler
    def on_actor_knockback(self, event: ActorKnockbackEvent):
        self.plugin.on_event_triggered(
            event,
            f"{event.actor.name} ({event.actor.type}) is knocked by {event.knockback}",
        )

    @event_handler
    def on_actor_removed(self, event: ActorRemoveEvent):
        self.plugin.on_event_triggered(
            event, f"{event.actor.name} ({event.actor.type}) is removed from the world."
        )

    @event_handler
    def on_actor_spawned(self, event: ActorSpawnEvent):
        self.plugin.on_event_triggered(
            event,
            f"{event.actor.name} ({event.actor.type}) just spawned - {event.actor.__class__}.",
            always_log=isinstance(event.actor, Item),
        )

    @event_handler
    def on_actor_teleport(self, event: ActorTeleportEvent):
        self.plugin.on_event_triggered(
            event,
            f"{event.actor.name} ({event.actor.type}) teleported from {event.from_location} to {event.to_location}",
        )
