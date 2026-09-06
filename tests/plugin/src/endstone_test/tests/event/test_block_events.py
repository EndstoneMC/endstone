import pytest

from endstone_test.recorder import EventRecorder


def test_block_break(recorder: EventRecorder) -> None:
    """Verify BlockBreakEvent names the player and the broken block."""
    snapshot = recorder.require("BlockBreakEvent")[0]
    assert snapshot["player"]
    assert ":" in snapshot["block_type"]
    assert len(snapshot["xyz"]) == 3


def test_block_place(recorder: EventRecorder) -> None:
    """Verify BlockPlaceEvent exposes the v0.12 placed/replaced accessors."""
    snapshot = recorder.require("BlockPlaceEvent")[0]
    assert snapshot["player"]
    assert ":" in snapshot["block_placed_type"]
    assert ":" in snapshot["block_replaced_type"]
    assert ":" in snapshot["block_against_type"]


def test_block_place_block_is_the_placed_block(recorder: EventRecorder) -> None:
    """Verify BlockPlaceEvent.block is the placed block, not the replaced one."""
    for snapshot in recorder.require("BlockPlaceEvent"):
        assert snapshot["block_type"] == snapshot["block_placed_type"]


def test_block_from_to(recorder: EventRecorder) -> None:
    """Verify BlockFromToEvent names both ends of the flow."""
    snapshot = recorder.require("BlockFromToEvent")[0]
    assert ":" in snapshot["block_type"]
    assert ":" in snapshot["to_block_type"]


def test_block_cook_is_campfire_only(recorder: EventRecorder) -> None:
    """Verify BlockCookEvent only ever fires for a campfire."""
    for snapshot in recorder.require("BlockCookEvent"):
        assert "campfire" in snapshot["block_type"]
        assert ":" in snapshot["source_type"]
        assert ":" in snapshot["result_type"]
        assert snapshot["recipe_id"]
        assert snapshot["recipe_tag"] in {"campfire", "soul_campfire"}
        assert snapshot["recipe_input_matches"] is True
        assert snapshot["recipe_result_type"] == snapshot["result_type"]


def test_block_explode(recorder: EventRecorder) -> None:
    """Verify BlockExplodeEvent reports the destroyed block list."""
    assert recorder.require("BlockExplodeEvent")[0]["block_count"] >= 0


def test_block_form_is_lava_solidifying(recorder: EventRecorder) -> None:
    """Verify BlockFormEvent reports the lava it forms from, not the block it becomes."""
    for snapshot in recorder.require("BlockFormEvent"):
        assert "lava" in snapshot["block_type"]
        assert snapshot["new_type"] in {
            "minecraft:basalt",
            "minecraft:cobblestone",
            "minecraft:obsidian",
        }
        assert len(snapshot["xyz"]) == 3


def test_leaves_decay(recorder: EventRecorder) -> None:
    """Verify LeavesDecayEvent points at a leaves block."""
    assert "leaves" in recorder.require("LeavesDecayEvent")[0]["block_type"]


@pytest.mark.parametrize(
    "event_name", ["BlockPistonExtendEvent", "BlockPistonRetractEvent"]
)
def test_piston(recorder: EventRecorder, event_name: str) -> None:
    """Verify the piston events report a piston block and a direction."""
    snapshot = recorder.require(event_name)[0]
    assert "piston" in snapshot["block_type"]
    assert snapshot["direction"]


def test_block_grow(recorder: EventRecorder) -> None:
    """Verify BlockGrowEvent reports the sapling and its replacement state."""
    for snapshot in recorder.require("BlockGrowEvent"):
        assert "sapling" in snapshot["block_type"]
        assert ":" in snapshot["new_type"]
        assert snapshot["cancelled_before"] is False
        assert isinstance(snapshot["cancelled"], bool)
