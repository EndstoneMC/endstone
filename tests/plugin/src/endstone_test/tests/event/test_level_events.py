from endstone.level import Dimension

from endstone_test.recorder import EventRecorder
from endstone_test.tests.test_dimension import CUSTOM_DIMENSION_ID


def test_chunk_load(recorder: EventRecorder) -> None:
    """Verify ChunkLoadEvent reports the chunk coordinates and dimension."""
    snapshot = recorder.require("ChunkLoadEvent")[0]
    assert len(snapshot["chunk_xz"]) == 2
    assert ":" in snapshot["dimension"]


def test_chunk_unload(recorder: EventRecorder) -> None:
    """Verify ChunkUnloadEvent reports the chunk coordinates and dimension."""
    snapshot = recorder.require("ChunkUnloadEvent")[0]
    assert len(snapshot["chunk_xz"]) == 2
    assert ":" in snapshot["dimension"]


def test_chunks_load_before_they_unload(recorder: EventRecorder) -> None:
    """Verify no chunk unloads without a load having been seen first."""
    recorder.require("ChunkUnloadEvent")
    assert recorder.count("ChunkLoadEvent") > 0


def test_dimension_load(recorder: EventRecorder) -> None:
    """Verify DimensionLoadEvent reports the dimension that was loaded."""
    snapshot = recorder.require("DimensionLoadEvent")[0]
    assert ":" in snapshot["dimension"]


def test_dimension_load_reports_a_dimension_created_after_enable(
    recorder: EventRecorder,
) -> None:
    """Verify a dimension created once this plugin was enabled is reported."""
    loaded = {
        snapshot["dimension"] for snapshot in recorder.require("DimensionLoadEvent")
    }
    assert CUSTOM_DIMENSION_ID in loaded


def test_dimension_load_misses_dimensions_loaded_before_enable(
    recorder: EventRecorder,
) -> None:
    """Verify the vanilla dimensions, loaded before this plugin, are not reported."""
    loaded = {
        snapshot["dimension"] for snapshot in recorder.require("DimensionLoadEvent")
    }
    assert not loaded & {
        str(Dimension.OVERWORLD),
        str(Dimension.NETHER),
        str(Dimension.THE_END),
    }
