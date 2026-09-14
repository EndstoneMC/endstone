from __future__ import annotations

from ..models import EventScenario


def build_actor_scenarios(ctx, namespace) -> list[EventScenario]:
    actor_collision = namespace['actor_collision']
    actor_damage = namespace['actor_damage']
    actor_death = namespace['actor_death']
    actor_dismount = namespace['actor_dismount']
    actor_effect = namespace['actor_effect']
    cleanup_actor_dismount = namespace['cleanup_actor_dismount']
    cleanup_actor_pickup_item = namespace['cleanup_actor_pickup_item']
    hide_player_actor = namespace['hide_player_actor']
    prepare_actor_pickup_item = namespace['prepare_actor_pickup_item']
    remove_actor = namespace['remove_actor']
    show_player_actor = namespace['show_player_actor']
    spawn_actor = namespace['spawn_actor']
    teleport_actor = namespace['teleport_actor']
    verify_actor_collision = namespace['verify_actor_collision']
    verify_actor_damage = namespace['verify_actor_damage']
    verify_actor_death = namespace['verify_actor_death']
    verify_actor_dismount = namespace['verify_actor_dismount']
    verify_actor_effect = namespace['verify_actor_effect']
    verify_actor_pickup_item = namespace['verify_actor_pickup_item']
    verify_actor_remove = namespace['verify_actor_remove']
    verify_actor_spawn = namespace['verify_actor_spawn']
    verify_actor_teleport = namespace['verify_actor_teleport']
    verify_toggle_event = namespace['verify_toggle_event']
    verify_visibility_event = namespace['verify_visibility_event']
    return [
        EventScenario(
                    "actor-spawn",
                    ("ActorSpawnEvent",),
                    spawn_actor,
                    requires_player=True,
                    verify=lambda snapshots: verify_actor_spawn(snapshots, False),
                ),
        EventScenario(
                    "actor-spawn-cancel",
                    ("ActorSpawnEvent",),
                    spawn_actor,
                    requires_player=True,
                    cancel_events=("ActorSpawnEvent",),
                    verify=lambda snapshots: verify_actor_spawn(snapshots, True),
                ),
        EventScenario(
                    "actor-teleport",
                    ("ActorTeleportEvent",),
                    teleport_actor,
                    requires_player=True,
                    verify=lambda snapshots: verify_actor_teleport(snapshots, False),
                ),
        EventScenario(
                    "actor-teleport-cancel",
                    ("ActorTeleportEvent",),
                    teleport_actor,
                    requires_player=True,
                    cancel_events=("ActorTeleportEvent",),
                    verify=lambda snapshots: verify_actor_teleport(snapshots, True),
                ),
        EventScenario(
                    "actor-remove",
                    ("ActorRemoveEvent",),
                    remove_actor,
                    requires_player=True,
                    verify=verify_actor_remove,
                ),
        EventScenario(
                    "player-hide-actor",
                    ("PlayerHideActorEvent",),
                    hide_player_actor,
                    requires_player=True,
                    verify=lambda snapshots: verify_visibility_event(
                        snapshots, "PlayerHideActorEvent"
                    ),
                ),
        EventScenario(
                    "player-show-actor",
                    ("PlayerShowActorEvent",),
                    show_player_actor,
                    requires_player=True,
                    verify=lambda snapshots: verify_visibility_event(
                        snapshots, "PlayerShowActorEvent"
                    ),
                ),
        EventScenario(
                    "actor-damage",
                    ("ActorDamageEvent",),
                    actor_damage,
                    requires_player=True,
                    verify=lambda snapshots: verify_actor_damage(snapshots, False),
                ),
        EventScenario(
                    "actor-damage-cancel",
                    ("ActorDamageEvent",),
                    actor_damage,
                    requires_player=True,
                    cancel_events=("ActorDamageEvent",),
                    verify=lambda snapshots: verify_actor_damage(snapshots, True),
                ),
        EventScenario(
                    "actor-death",
                    ("ActorDeathEvent",),
                    actor_death,
                    timeout_ticks=120,
                    requires_player=True,
                    verify=verify_actor_death,
                ),
        EventScenario(
                    "actor-effect",
                    ("ActorEffectEvent",),
                    actor_effect,
                    requires_player=True,
                    verify=lambda snapshots: verify_actor_effect(snapshots, False),
                ),
        EventScenario(
                    "actor-effect-cancel",
                    ("ActorEffectEvent",),
                    actor_effect,
                    requires_player=True,
                    cancel_events=("ActorEffectEvent",),
                    verify=lambda snapshots: verify_actor_effect(snapshots, True),
                ),
        EventScenario(
                    "actor-pickup-item",
                    ("ActorPickupItemEvent",),
                    prepare_actor_pickup_item,
                    timeout_ticks=220,
                    requires_player=True,
                    cleanup=cleanup_actor_pickup_item,
                    verify=lambda snapshots: verify_actor_pickup_item(snapshots, False),
                ),
        EventScenario(
                    "actor-pickup-item-cancel",
                    ("ActorPickupItemEvent",),
                    prepare_actor_pickup_item,
                    timeout_ticks=220,
                    requires_player=True,
                    cleanup=cleanup_actor_pickup_item,
                    cancel_events=("ActorPickupItemEvent",),
                    verify=lambda snapshots: verify_actor_pickup_item(snapshots, True),
                ),
        EventScenario(
                    "actor-collision",
                    ("ActorCollideWithActorEvent",),
                    actor_collision,
                    requires_player=True,
                    verify=lambda snapshots: verify_actor_collision(snapshots, False),
                ),
        EventScenario(
                    "actor-collision-cancel",
                    ("ActorCollideWithActorEvent",),
                    actor_collision,
                    requires_player=True,
                    cancel_events=("ActorCollideWithActorEvent",),
                    verify=lambda snapshots: verify_actor_collision(snapshots, True),
                ),
        EventScenario(
                    "actor-dismount",
                    ("ActorDismountEvent",),
                    actor_dismount,
                    timeout_ticks=100,
                    settle_ticks=6,
                    requires_player=True,
                    cleanup=cleanup_actor_dismount,
                    verify=lambda snapshots: verify_actor_dismount(snapshots, False),
                ),
        EventScenario(
                    "actor-dismount-cancel",
                    ("ActorDismountEvent",),
                    actor_dismount,
                    timeout_ticks=100,
                    settle_ticks=6,
                    requires_player=True,
                    cleanup=cleanup_actor_dismount,
                    cancel_events=("ActorDismountEvent",),
                    verify=lambda snapshots: verify_actor_dismount(snapshots, True),
                ),
        EventScenario(
                    "actor-toggle-swim",
                    ("ActorToggleSwimEvent",),
                    lambda: None,
                    driver_action="toggle-swim",
                    requires_player=True,
                    verify=lambda snapshots: verify_toggle_event(
                        snapshots, "ActorToggleSwimEvent", "is_swimming", True
                    ),
                ),
        EventScenario(
                    "actor-toggle-glide",
                    ("ActorToggleGlideEvent",),
                    lambda: None,
                    driver_action="toggle-glide",
                    requires_player=True,
                    verify=lambda snapshots: verify_toggle_event(
                        snapshots, "ActorToggleGlideEvent", "is_gliding", True
                    ),
                ),
    ]
