import pytest

from endstone_test.recorder import EventRecorder


def test_actor_spawn(recorder: EventRecorder) -> None:
    """Verify ActorSpawnEvent names a namespaced actor type."""
    for snapshot in recorder.require("ActorSpawnEvent"):
        assert ":" in snapshot["actor_type"]
        assert snapshot["python_type"]


def test_actor_remove(recorder: EventRecorder) -> None:
    """Verify ActorRemoveEvent names a namespaced actor type."""
    assert ":" in recorder.require("ActorRemoveEvent")[0]["actor_type"]


def test_actor_damage(recorder: EventRecorder) -> None:
    """Verify ActorDamageEvent reports a positive damage and a source type."""
    for snapshot in recorder.require("ActorDamageEvent"):
        assert snapshot["damage"] >= 0
        assert snapshot["damage_type"]


def test_actor_damage_is_cancellable(recorder: EventRecorder) -> None:
    """Verify ActorDamageEvent is delivered as a cancellable event."""
    assert all(s["is_cancellable"] for s in recorder.require("ActorDamageEvent"))


def test_actor_death(recorder: EventRecorder) -> None:
    """Verify ActorDeathEvent reports the damage source type."""
    snapshot = recorder.require("ActorDeathEvent")[0]
    assert ":" in snapshot["actor_type"]
    assert snapshot["damage_type"]


def test_player_death_also_fires_actor_death(recorder: EventRecorder) -> None:
    """Verify a player death reaches ActorDeathEvent, since Player is a Mob."""
    if recorder.count("PlayerDeathEvent"):
        types = {s["actor_type"] for s in recorder.require("ActorDeathEvent")}
        assert "minecraft:player" in types


def test_actor_knockback(recorder: EventRecorder) -> None:
    """Verify ActorKnockbackEvent carries a three-component knockback vector."""
    for snapshot in recorder.require("ActorKnockbackEvent"):
        assert len(snapshot["knockback"]) == 3


def test_actor_teleport(recorder: EventRecorder) -> None:
    """Verify ActorTeleportEvent moves the actor somewhere else."""
    assert (
        recorder.require("ActorTeleportEvent")[0]["from_xyz"]
        != recorder.snapshots("ActorTeleportEvent")[0]["to_xyz"]
    )


def test_actor_explode(recorder: EventRecorder) -> None:
    """Verify ActorExplodeEvent reports the explosion location and block list."""
    snapshot = recorder.require("ActorExplodeEvent")[0]
    assert snapshot["block_count"] >= 0
    assert isinstance(snapshot["y"], float)


def test_actor_change_block(recorder: EventRecorder) -> None:
    """Verify ActorChangeBlockEvent names the actor and the block it changes."""
    snapshot = recorder.require("ActorChangeBlockEvent")[0]
    assert ":" in snapshot["actor_type"]
    assert ":" in snapshot["block_type"]
    assert len(snapshot["xyz"]) == 3


def test_actor_collide_with_actor(recorder: EventRecorder) -> None:
    """Verify ActorCollideWithActorEvent pairs exactly two distinct actors."""
    for snapshot in recorder.require("ActorCollideWithActorEvent"):
        assert snapshot["actor_count"] == 2
        assert len(set(snapshot["runtime_ids"])) == 2
        assert all(":" in actor_type for actor_type in snapshot["actor_types"])


def test_actor_dismount(recorder: EventRecorder) -> None:
    """Verify ActorDismountEvent names a vehicle other than the rider."""
    for snapshot in recorder.require("ActorDismountEvent"):
        assert ":" in snapshot["vehicle_type"]
        assert not snapshot["is_self_ride"]


def test_actor_effect(recorder: EventRecorder) -> None:
    """Verify ActorEffectEvent describes the effect being applied."""
    for snapshot in recorder.require("ActorEffectEvent"):
        assert ":" in snapshot["effect_type"]
        assert snapshot["amplifier"] >= 0


def test_actor_effect_is_only_ever_added(recorder: EventRecorder) -> None:
    """Verify ActorEffectEvent only reports ADDED, the one action the server fires."""
    assert {s["action"] for s in recorder.require("ActorEffectEvent")} == {"ADDED"}


def test_actor_effect_duration_matches_infinite(recorder: EventRecorder) -> None:
    """Verify an infinite effect reports no duration and a finite one does."""
    for snapshot in recorder.require("ActorEffectEvent"):
        assert (snapshot["duration"] is None) == snapshot["infinite"]


def test_actor_pickup_item(recorder: EventRecorder) -> None:
    """Verify ActorPickupItemEvent picks no more than the stack holds."""
    for snapshot in recorder.require("ActorPickupItemEvent"):
        assert ":" in snapshot["item_type"]
        assert 0 < snapshot["amount"] <= snapshot["stack_amount"]


def test_actor_pickup_item_is_never_a_player(recorder: EventRecorder) -> None:
    """Verify ActorPickupItemEvent leaves players to PlayerPickupItemEvent."""
    for snapshot in recorder.require("ActorPickupItemEvent"):
        assert snapshot["actor_type"] != "minecraft:player"


def test_actor_toggle_glide(recorder: EventRecorder) -> None:
    """Verify ActorToggleGlideEvent fires before the new state is applied."""
    for snapshot in recorder.require("ActorToggleGlideEvent"):
        assert isinstance(snapshot["is_gliding"], bool)
        assert snapshot["actor_is_gliding"] is not snapshot["is_gliding"]


def test_actor_toggle_swim(recorder: EventRecorder) -> None:
    """Verify ActorToggleSwimEvent fires before the new state is applied."""
    for snapshot in recorder.require("ActorToggleSwimEvent"):
        assert isinstance(snapshot["is_swimming"], bool)
        assert snapshot["actor_is_swimming"] is not snapshot["is_swimming"]


@pytest.mark.parametrize(
    "event_name", ["ActorToggleGlideEvent", "ActorToggleSwimEvent"]
)
def test_toggle_alternates_per_actor(recorder: EventRecorder, event_name: str) -> None:
    """Verify a toggle event is not fired twice running for one actor's state."""
    field = "is_gliding" if "Glide" in event_name else "is_swimming"
    per_actor: dict[int, list[bool]] = {}
    for snapshot in recorder.require(event_name):
        per_actor.setdefault(snapshot["runtime_id"], []).append(snapshot[field])
    for states in per_actor.values():
        assert all(a is not b for a, b in zip(states, states[1:]))


@pytest.mark.parametrize(
    "event_name",
    [
        "ActorChangeBlockEvent",
        "ActorCollideWithActorEvent",
        "ActorDismountEvent",
        "ActorEffectEvent",
        "ActorPickupItemEvent",
    ],
)
def test_cancellable(event_name: str) -> None:
    """Verify the actor events that stop the behaviour they announce are cancellable."""
    from endstone import event as event_module

    event_cls = getattr(event_module, event_name)
    assert hasattr(event_cls, "cancel")
    assert hasattr(event_cls, "is_cancelled")


@pytest.mark.parametrize(
    "event_name", ["ActorToggleGlideEvent", "ActorToggleSwimEvent"]
)
def test_not_cancellable(event_name: str) -> None:
    """Verify the actor events that only report a state change are not cancellable."""
    from endstone import event as event_module

    event_cls = getattr(event_module, event_name)
    assert not hasattr(event_cls, "cancel")
    assert not hasattr(event_cls, "is_cancelled")
