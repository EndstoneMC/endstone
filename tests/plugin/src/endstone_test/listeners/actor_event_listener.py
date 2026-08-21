from endstone.event import (
    ActorChangeBlockEvent,
    ActorCollideWithActorEvent,
    ActorDamageEvent,
    ActorDeathEvent,
    ActorDismountEvent,
    ActorEffectEvent,
    ActorExplodeEvent,
    ActorKnockbackEvent,
    ActorPickupItemEvent,
    ActorRemoveEvent,
    ActorSpawnEvent,
    ActorTeleportEvent,
    ActorToggleGlideEvent,
    ActorToggleSwimEvent,
    event_handler,
)

from .event_listener import EventListener


class ActorEventListener(EventListener):
    @event_handler
    def on_actor_change_block(self, event: ActorChangeBlockEvent):
        self.record(
            event,
            f"{event.actor.name} ({event.actor.type}) changes {event.block}",
            always_log=True,
            actor_type=str(event.actor.type),
            block_type=str(event.block.type),
            xyz=(event.block.x, event.block.y, event.block.z),
        )

    @event_handler
    def on_actor_collide_with_actor(self, event: ActorCollideWithActorEvent):
        actors = event.actors
        self.record(
            event,
            f"{actors[0].name} collides with {actors[1].name}",
            actor_count=len(actors),
            actor_types=tuple(str(actor.type) for actor in actors),
            runtime_ids=tuple(actor.runtime_id for actor in actors),
        )

    @event_handler
    def on_actor_damage(self, event: ActorDamageEvent):
        self.record(
            event,
            f"{event.actor.name} ({event.actor.type}) hurt "
            f"(source: {event.damage_source}, damage: {event.damage}).",
            actor_type=str(event.actor.type),
            damage=event.damage,
            damage_type=event.damage_source.type,
            is_cancellable=hasattr(event, "cancel"),
        )

    @event_handler
    def on_actor_death(self, event: ActorDeathEvent):
        self.record(
            event,
            f"{event.actor.name} ({event.actor.type}) died "
            f"(source: {event.damage_source}).",
            actor_type=str(event.actor.type),
            damage_type=event.damage_source.type,
        )

    @event_handler
    def on_actor_dismount(self, event: ActorDismountEvent):
        self.record(
            event,
            f"{event.actor.name} ({event.actor.type}) dismounts "
            f"{event.vehicle.name} ({event.vehicle.type})",
            always_log=True,
            actor_type=str(event.actor.type),
            vehicle_type=str(event.vehicle.type),
            is_self_ride=event.actor.runtime_id == event.vehicle.runtime_id,
        )

    @event_handler
    def on_actor_effect(self, event: ActorEffectEvent):
        effect = event.effect
        self.record(
            event,
            f"{event.actor.name} ({event.actor.type}) effect {event.action}: {effect}",
            actor_type=str(event.actor.type),
            action=event.action.name,
            effect_type=str(effect.type),
            duration=effect.duration,
            amplifier=effect.amplifier,
            infinite=effect.infinite,
        )

    @event_handler
    def on_actor_explode(self, event: ActorExplodeEvent):
        self.record(
            event,
            f"{event.actor.name} ({event.actor.type}) exploded.",
            actor_type=str(event.actor.type),
            block_count=len(event.block_list),
            x=event.location.x,
            y=event.location.y,
            z=event.location.z,
        )

    @event_handler
    def on_actor_knockback(self, event: ActorKnockbackEvent):
        self.record(
            event,
            f"{event.actor.name} ({event.actor.type}) is knocked by {event.knockback}",
            actor_type=str(event.actor.type),
            knockback=(event.knockback.x, event.knockback.y, event.knockback.z),
            has_source=event.source is not None,
        )

    @event_handler
    def on_actor_pickup_item(self, event: ActorPickupItemEvent):
        self.record(
            event,
            f"{event.actor.name} ({event.actor.type}) picks up {event.amount} x "
            f"{event.item.item_stack}",
            always_log=True,
            actor_type=str(event.actor.type),
            item_type=str(event.item.item_stack.type),
            amount=event.amount,
            stack_amount=event.item.item_stack.amount,
        )

    @event_handler
    def on_actor_remove(self, event: ActorRemoveEvent):
        self.record(
            event,
            f"{event.actor.name} ({event.actor.type}) is removed from the world.",
            actor_type=str(event.actor.type),
        )

    @event_handler
    def on_actor_spawn(self, event: ActorSpawnEvent):
        self.record(
            event,
            f"{event.actor.name} ({event.actor.type}) just spawned "
            f"- {event.actor.__class__.__name__}.",
            actor_type=str(event.actor.type),
            python_type=event.actor.__class__.__name__,
        )

    @event_handler
    def on_actor_teleport(self, event: ActorTeleportEvent):
        self.record(
            event,
            f"{event.actor.name} ({event.actor.type}) teleported from "
            f"{event.from_location} to {event.to_location}",
            actor_type=str(event.actor.type),
            from_xyz=(
                event.from_location.x,
                event.from_location.y,
                event.from_location.z,
            ),
            to_xyz=(event.to_location.x, event.to_location.y, event.to_location.z),
        )

    @event_handler
    def on_actor_toggle_glide(self, event: ActorToggleGlideEvent):
        self.record(
            event,
            f"{event.actor.name} ({event.actor.type}) gliding -> {event.is_gliding}",
            always_log=True,
            actor_type=str(event.actor.type),
            runtime_id=event.actor.runtime_id,
            is_gliding=event.is_gliding,
            actor_is_gliding=event.actor.is_gliding,
        )

    @event_handler
    def on_actor_toggle_swim(self, event: ActorToggleSwimEvent):
        self.record(
            event,
            f"{event.actor.name} ({event.actor.type}) swimming -> {event.is_swimming}",
            actor_type=str(event.actor.type),
            runtime_id=event.actor.runtime_id,
            is_swimming=event.is_swimming,
            actor_is_swimming=event.actor.is_swimming,
        )
