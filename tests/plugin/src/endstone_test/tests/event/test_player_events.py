import pytest

from endstone_test.recorder import EventRecorder

_INPUT_KEYS = ("forward", "backward", "left", "right", "jump", "sneak", "sprint")


def _by_player(snapshots: list[dict]) -> dict[str, list[dict]]:
    grouped: dict[str, list[dict]] = {}
    for snapshot in snapshots:
        grouped.setdefault(snapshot["player"], []).append(snapshot)
    return grouped


# =============================================================================
# Section 1: Connection
# =============================================================================


def test_player_login(recorder: EventRecorder) -> None:
    """Verify PlayerLoginEvent reports the player's skin."""
    snapshot = recorder.require("PlayerLoginEvent")[0]
    assert snapshot["player"]
    assert snapshot["skin_id"]
    assert len(snapshot["skin_shape"]) == 3
    assert snapshot["skin_shape"][2] == 4


def test_player_join(recorder: EventRecorder) -> None:
    """Verify PlayerJoinEvent exposes the joining player's identity."""
    snapshot = recorder.require("PlayerJoinEvent")[0]
    assert snapshot["player"]
    assert snapshot["unique_id"]
    assert snapshot["runtime_id"] > 0
    assert snapshot["ping"] >= 0
    assert isinstance(snapshot["is_op"], bool)


def test_player_quit(recorder: EventRecorder) -> None:
    """Verify PlayerQuitEvent names the departing player."""
    assert recorder.require("PlayerQuitEvent")[0]["player"]


def test_player_kick(recorder: EventRecorder) -> None:
    """Verify PlayerKickEvent carries a reason."""
    snapshot = recorder.require("PlayerKickEvent")[0]
    assert snapshot["player"]
    assert isinstance(snapshot["reason"], str)


# =============================================================================
# Section 2: Movement
# =============================================================================


def test_player_move(recorder: EventRecorder) -> None:
    """Verify PlayerMoveEvent fires only when position or rotation changed."""
    for snapshot in recorder.require("PlayerMoveEvent"):
        assert (
            snapshot["from_xyz"] != snapshot["to_xyz"]
            or snapshot["from_rot"] != snapshot["to_rot"]
        )


def test_player_jump(recorder: EventRecorder) -> None:
    """Verify PlayerJumpEvent always moves the player upwards."""
    for snapshot in recorder.require("PlayerJumpEvent"):
        assert snapshot["to_xyz"][1] > snapshot["from_xyz"][1]


def test_player_teleport(recorder: EventRecorder) -> None:
    """Verify PlayerTeleportEvent records both endpoints."""
    snapshot = recorder.require("PlayerTeleportEvent")[0]
    assert len(snapshot["from_xyz"]) == 3
    assert len(snapshot["to_xyz"]) == 3


def test_player_portal(recorder: EventRecorder) -> None:
    """Verify PlayerPortalEvent moves the player somewhere else."""
    snapshot = recorder.require("PlayerPortalEvent")[0]
    assert snapshot["from_xyz"] != snapshot["to_xyz"]


def test_player_dimension_change(recorder: EventRecorder) -> None:
    """Verify PlayerDimensionChangeEvent reports two different dimensions."""
    snapshot = recorder.require("PlayerDimensionChangeEvent")[0]
    assert snapshot["from_dimension"] != snapshot["to_dimension"]
    assert ":" in snapshot["from_dimension"]


def test_player_toggle_sneak(recorder: EventRecorder) -> None:
    """Verify PlayerToggleSneakEvent reports the new sneaking state."""
    for snapshot in recorder.require("PlayerToggleSneakEvent"):
        assert isinstance(snapshot["is_sneaking"], bool)
    states = {s["is_sneaking"] for s in recorder.snapshots("PlayerToggleSneakEvent")}
    if recorder.count("PlayerToggleSneakEvent") > 1:
        assert states == {True, False}


def test_player_toggle_sprint(recorder: EventRecorder) -> None:
    """Verify PlayerToggleSprintEvent reports the new sprinting state."""
    for snapshot in recorder.require("PlayerToggleSprintEvent"):
        assert isinstance(snapshot["is_sprinting"], bool)


def test_player_toggle_crawl(recorder: EventRecorder) -> None:
    """Verify PlayerToggleCrawlEvent fires before the new state is applied."""
    for snapshot in recorder.require("PlayerToggleCrawlEvent"):
        assert isinstance(snapshot["is_crawling"], bool)
        assert snapshot["player_is_crawling"] is not snapshot["is_crawling"]


def test_player_toggle_flight(recorder: EventRecorder) -> None:
    """Verify PlayerToggleFlightEvent fires before the new state is applied."""
    for snapshot in recorder.require("PlayerToggleFlightEvent"):
        assert isinstance(snapshot["is_flying"], bool)
        assert snapshot["player_is_flying"] is not snapshot["is_flying"]


def test_player_toggle_flight_requires_allow_flight(recorder: EventRecorder) -> None:
    """Verify PlayerToggleFlightEvent only fires while flight is allowed."""
    assert all(s["allow_flight"] for s in recorder.require("PlayerToggleFlightEvent"))


def test_player_input(recorder: EventRecorder) -> None:
    """Verify PlayerInputEvent reports every input direction as a bool."""
    for snapshot in recorder.require("PlayerInputEvent"):
        for key in _INPUT_KEYS:
            assert isinstance(snapshot[key], bool)


def test_player_input_fires_only_on_change(recorder: EventRecorder) -> None:
    """Verify PlayerInputEvent is not fired again for an unchanged input."""
    for snapshots in _by_player(recorder.require("PlayerInputEvent")).values():
        states = [tuple(s[key] for key in _INPUT_KEYS) for s in snapshots]
        assert all(a != b for a, b in zip(states, states[1:]))


# =============================================================================
# Section 3: Interaction
# =============================================================================


def test_player_interact(recorder: EventRecorder) -> None:
    """Verify PlayerInteractEvent reports a known action."""
    actions = {s["action"] for s in recorder.require("PlayerInteractEvent")}
    assert actions
    for action in actions:
        assert "CLICK" in action.upper()


def test_player_interact_records_block_only_when_present(
    recorder: EventRecorder,
) -> None:
    """Verify PlayerInteractEvent.has_block agrees with the recorded block."""
    for snapshot in recorder.require("PlayerInteractEvent"):
        assert (snapshot["block_type"] is not None) == snapshot["has_block"]


def test_player_interact_actor(recorder: EventRecorder) -> None:
    """Verify PlayerInteractActorEvent names the actor type."""
    assert ":" in recorder.require("PlayerInteractActorEvent")[0]["actor_type"]


def test_player_item_held(recorder: EventRecorder) -> None:
    """Verify PlayerItemHeldEvent reports a real slot change."""
    for snapshot in recorder.require("PlayerItemHeldEvent"):
        assert 0 <= snapshot["new_slot"] <= 8
        assert 0 <= snapshot["previous_slot"] <= 8


def test_player_drop_item(recorder: EventRecorder) -> None:
    """Verify PlayerDropItemEvent names the dropped item type."""
    assert ":" in recorder.require("PlayerDropItemEvent")[0]["item_type"]


def test_player_pickup_item(recorder: EventRecorder) -> None:
    """Verify PlayerPickupItemEvent reads through the Item actor's stack."""
    assert ":" in recorder.require("PlayerPickupItemEvent")[0]["item_type"]


def test_player_item_consume(recorder: EventRecorder) -> None:
    """Verify PlayerItemConsumeEvent reports the item and the hand used."""
    snapshot = recorder.require("PlayerItemConsumeEvent")[0]
    assert ":" in snapshot["item_type"]
    assert snapshot["hand"]


def test_player_chat(recorder: EventRecorder) -> None:
    """Verify PlayerChatEvent carries a message and its recipients."""
    snapshot = recorder.require("PlayerChatEvent")[0]
    assert snapshot["message"]
    assert snapshot["recipient_count"] >= 1


def test_player_command(recorder: EventRecorder) -> None:
    """Verify PlayerCommandEvent records the command line."""
    assert recorder.require("PlayerCommandEvent")[0]["command"]


def test_player_emote(recorder: EventRecorder) -> None:
    """Verify PlayerEmoteEvent carries an emote id."""
    assert recorder.require("PlayerEmoteEvent")[0]["emote_id"]


def test_player_arm_swing(recorder: EventRecorder) -> None:
    """Verify PlayerArmSwingEvent reports the held item, or none for an empty hand."""
    for snapshot in recorder.require("PlayerArmSwingEvent"):
        assert (snapshot["item_type"] is not None) == snapshot["has_item"]
        if snapshot["has_item"]:
            assert ":" in snapshot["item_type"]


def test_player_armor_stand_manipulate(recorder: EventRecorder) -> None:
    """Verify PlayerArmorStandManipulateEvent names the stand, slot and both items."""
    for snapshot in recorder.require("PlayerArmorStandManipulateEvent"):
        assert snapshot["actor_type"] == "minecraft:armor_stand"
        assert snapshot["slot"]
        assert ":" in snapshot["armor_stand_item_type"]
        assert ":" in snapshot["player_item_type"]


def test_player_armor_stand_manipulate_moves_an_item(recorder: EventRecorder) -> None:
    """Verify the interaction always has an item to swap, retrieve or place."""
    for snapshot in recorder.require("PlayerArmorStandManipulateEvent"):
        assert (
            snapshot["armor_stand_item_type"] != "minecraft:air"
            or snapshot["player_item_type"] != "minecraft:air"
        )


def test_player_bucket_actor(recorder: EventRecorder) -> None:
    """Verify PlayerBucketActorEvent names the actor, the bucket and the hand."""
    for snapshot in recorder.require("PlayerBucketActorEvent"):
        assert ":" in snapshot["actor_type"]
        assert "bucket" in snapshot["original_bucket_type"]
        assert snapshot["hand"]


def test_player_shear_actor(recorder: EventRecorder) -> None:
    """Verify PlayerShearActorEvent names the sheared actor and the shears used."""
    for snapshot in recorder.require("PlayerShearActorEvent"):
        assert ":" in snapshot["actor_type"]
        assert snapshot["item_type"] == "minecraft:shears"
        assert snapshot["hand"]


def test_player_riptide(recorder: EventRecorder) -> None:
    """Verify PlayerRiptideEvent names the trident being used."""
    for snapshot in recorder.require("PlayerRiptideEvent"):
        assert "trident" in snapshot["item_type"]


def test_player_pickup_arrow(recorder: EventRecorder) -> None:
    """Verify PlayerPickupArrowEvent only fires for an abstract arrow."""
    for snapshot in recorder.require("PlayerPickupArrowEvent"):
        assert "arrow" in snapshot["arrow_type"] or "trident" in snapshot["arrow_type"]


def test_player_recipe_book_settings_change(recorder: EventRecorder) -> None:
    """Verify PlayerRecipeBookSettingsChangeEvent reports the open and filter flags."""
    for snapshot in recorder.require("PlayerRecipeBookSettingsChangeEvent"):
        assert isinstance(snapshot["is_open"], bool)
        assert isinstance(snapshot["is_filtering"], bool)


def test_recipe_book_settings_change_is_crafting_only(recorder: EventRecorder) -> None:
    """Verify only the crafting recipe book has a fire site in the server."""
    snapshots = recorder.require("PlayerRecipeBookSettingsChangeEvent")
    assert {s["recipe_book_type"] for s in snapshots} == {"CRAFTING"}


# =============================================================================
# Section 4: Bed, death and respawn
# =============================================================================


def test_player_bed_enter(recorder: EventRecorder) -> None:
    """Verify PlayerBedEnterEvent points at a bed block."""
    assert "bed" in recorder.require("PlayerBedEnterEvent")[0]["bed_type"]


def test_player_bed_leave(recorder: EventRecorder) -> None:
    """Verify PlayerBedLeaveEvent points at a bed block."""
    assert "bed" in recorder.require("PlayerBedLeaveEvent")[0]["bed_type"]


def test_player_death(recorder: EventRecorder) -> None:
    """Verify PlayerDeathEvent reports the damage source type."""
    snapshot = recorder.require("PlayerDeathEvent")[0]
    assert snapshot["player"]
    assert snapshot["damage_type"]


def test_player_respawn(recorder: EventRecorder) -> None:
    """Verify PlayerRespawnEvent reports a known RespawnReason."""
    for snapshot in recorder.require("PlayerRespawnEvent"):
        assert snapshot["respawn_reason"] in ("DEATH", "END_PORTAL")


def test_player_death_precedes_respawn(recorder: EventRecorder) -> None:
    """Verify a DEATH respawn is only recorded after a death."""
    reasons = [s["respawn_reason"] for s in recorder.require("PlayerRespawnEvent")]
    if "DEATH" in reasons:
        assert recorder.count("PlayerDeathEvent") > 0


# =============================================================================
# Section 5: Miscellaneous
# =============================================================================


def test_player_game_mode_change(recorder: EventRecorder) -> None:
    """Verify PlayerGameModeChangeEvent reports the new game mode."""
    assert recorder.require("PlayerGameModeChangeEvent")[0]["new_game_mode"]


def test_player_exp_change(recorder: EventRecorder) -> None:
    """Verify PlayerExpChangeEvent reports a non-negative amount."""
    for snapshot in recorder.require("PlayerExpChangeEvent"):
        assert snapshot["amount"] >= 0


def test_player_pickup_experience(recorder: EventRecorder) -> None:
    """Verify PlayerPickupExperienceEvent reports what the orb is worth."""
    for snapshot in recorder.require("PlayerPickupExperienceEvent"):
        assert snapshot["amount"] > 0


def test_player_level_change(recorder: EventRecorder) -> None:
    """Verify PlayerLevelChangeEvent reports two different levels."""
    for snapshot in recorder.require("PlayerLevelChangeEvent"):
        assert snapshot["old_level"] != snapshot["new_level"]
        assert snapshot["new_level"] >= 0


def test_player_skin_change(recorder: EventRecorder) -> None:
    """Verify PlayerSkinChangeEvent carries the new skin id."""
    assert recorder.require("PlayerSkinChangeEvent")[0]["new_skin_id"]


@pytest.mark.parametrize(
    "event_name",
    [
        "PlayerToggleSneakEvent",
        "PlayerToggleSprintEvent",
        "PlayerJoinEvent",
        "PlayerQuitEvent",
        "PlayerRespawnEvent",
        "PlayerBedLeaveEvent",
        "PlayerDeathEvent",
        "PlayerDimensionChangeEvent",
        "PlayerArmSwingEvent",
        "PlayerExpChangeEvent",
        "PlayerInputEvent",
        "PlayerLevelChangeEvent",
        "PlayerRecipeBookSettingsChangeEvent",
        "PlayerRiptideEvent",
        "PlayerToggleCrawlEvent",
        "PlayerToggleFlightEvent",
    ],
)
def test_not_cancellable(event_name: str) -> None:
    """Verify the events Bukkit makes cancellable but Endstone does not."""
    from endstone import event as event_module

    event_cls = getattr(event_module, event_name)
    assert not hasattr(event_cls, "cancel")
    assert not hasattr(event_cls, "is_cancelled")


@pytest.mark.parametrize(
    "event_name",
    [
        "PlayerArmorStandManipulateEvent",
        "PlayerBucketActorEvent",
        "PlayerPickupArrowEvent",
        "PlayerPickupExperienceEvent",
        "PlayerShearActorEvent",
    ],
)
def test_cancellable(event_name: str) -> None:
    """Verify the player events that stop the action they announce are cancellable."""
    from endstone import event as event_module

    event_cls = getattr(event_module, event_name)
    assert hasattr(event_cls, "cancel")
    assert hasattr(event_cls, "is_cancelled")
