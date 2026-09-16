import pytest
from endstone import Server
from endstone.boss import BarColor, BarStyle


def test_create_boss_bar(server: Server) -> None:
    """Boss bar should be created with the exact attributes requested."""
    title = "test boss bar"

    boss_bar = server.create_boss_bar(
        title=title,
        color=BarColor.GREEN,
        style=BarStyle.SOLID,
    )

    assert boss_bar.title == title
    assert boss_bar.color is BarColor.GREEN
    assert boss_bar.style is BarStyle.SOLID
    # Many APIs default to full progress; make that explicit.
    assert boss_bar.progress == 1.0


@pytest.mark.parametrize(
    "value",
    [0.0, 0.25, 0.5, 0.75, 1.0],
)
def test_boss_bar_valid_progress_values(server: Server, value: float) -> None:
    """Progress should accept all in-range values."""
    boss_bar = server.create_boss_bar(
        title="test boss bar",
        color=BarColor.GREEN,
        style=BarStyle.SOLID,
    )

    boss_bar.progress = value
    assert boss_bar.progress == value


@pytest.mark.parametrize(
    "value",
    [-1.0, -0.01, 1.01, 2.0],
)
def test_boss_bar_invalid_progress_values_raise(server: Server, value: float) -> None:
    """Progress outside [0.0, 1.0] must raise ValueError."""
    boss_bar = server.create_boss_bar(
        title="test boss bar",
        color=BarColor.GREEN,
        style=BarStyle.SOLID,
    )

    with pytest.raises(ValueError):
        boss_bar.progress = value
