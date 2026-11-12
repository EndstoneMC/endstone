import pytest
from endstone import event as events

CANCELLABLE = [
    "ActorChangeBlockEvent",
    "ActorCollideWithActorEvent",
    "ActorDamageEvent",
    "ActorDismountEvent",
    "ActorEffectEvent",
    "ActorPickupItemEvent",
    "EnchantItemEvent",
    "PlayerArmSwingEvent",
    "PlayerArmorStandManipulateEvent",
    "PlayerBucketActorEvent",
    "PlayerPickupArrowEvent",
    "PlayerPickupExperienceEvent",
    "PlayerShearActorEvent",
    "PlayerToggleFlightEvent",
    "PlayerToggleSneakEvent",
    "PlayerToggleSprintEvent",
    "PrepareItemEnchantEvent",
]

NOT_CANCELLABLE = [
    "ActorToggleGlideEvent",
    "ActorToggleSwimEvent",
    "PlayerBedLeaveEvent",
    "PlayerDeathEvent",
    "PlayerDimensionChangeEvent",
    "PlayerExpChangeEvent",
    "PlayerInputEvent",
    "PlayerJoinEvent",
    "PlayerLevelChangeEvent",
    "PlayerQuitEvent",
    "PlayerRecipeBookSettingsChangeEvent",
    "PlayerRespawnEvent",
    "PlayerRiptideEvent",
    "PlayerToggleCrawlEvent",
]


@pytest.mark.parametrize("event_name", CANCELLABLE)
def test_cancellable(event_name: str) -> None:
    """Verify the events that stop the behaviour they announce are cancellable."""
    event_cls = getattr(events, event_name)
    assert hasattr(event_cls, "cancel")
    assert hasattr(event_cls, "is_cancelled")
    assert issubclass(event_cls, events.Cancellable)


@pytest.mark.parametrize("event_name", NOT_CANCELLABLE)
def test_not_cancellable(event_name: str) -> None:
    """Verify the events that only report a state change are not cancellable."""
    event_cls = getattr(events, event_name)
    assert not hasattr(event_cls, "cancel")
    assert not hasattr(event_cls, "is_cancelled")
    assert not issubclass(event_cls, events.Cancellable)


def test_prepare_item_enchant_is_an_inventory_event() -> None:
    assert issubclass(events.PrepareItemEnchantEvent, events.InventoryEvent)
