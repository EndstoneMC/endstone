import pytest

from endstone.level import Location
from endstone.util import Vector


def test_construction_without_a_dimension() -> None:
    """Verify a location can be built with no dimension, as v0.12 allows."""
    location = Location(None, 1.0, 2.0, 3.0)
    assert location.dimension is None
    assert abs(location.x - 1.0) <= 0.00001
    assert abs(location.y - 2.0) <= 0.00001
    assert abs(location.z - 3.0) <= 0.00001


def test_rotation_defaults_to_zero() -> None:
    """Verify pitch and yaw default to zero."""
    location = Location(None, 0.0, 0.0, 0.0)
    assert abs(location.pitch) <= 0.00001
    assert abs(location.yaw) <= 0.00001


def test_rotation_is_read_and_written() -> None:
    """Verify pitch and yaw round-trip through the constructor and the setters."""
    location = Location(None, 0.0, 0.0, 0.0, 45.0, 90.0)
    assert abs(location.pitch - 45.0) <= 0.00001
    assert abs(location.yaw - 90.0) <= 0.00001

    location.pitch = -10.0
    location.yaw = 180.0
    assert abs(location.pitch + 10.0) <= 0.00001
    assert abs(location.yaw - 180.0) <= 0.00001


def test_block_coordinates_floor_towards_negative_infinity() -> None:
    """Verify the block coordinates floor rather than truncate."""
    location = Location(None, -0.5, 1.9, -1.1)
    assert location.block_x == -1
    assert location.block_y == 1
    assert location.block_z == -2


def test_is_not_a_vector_subclass() -> None:
    """Verify a location is its own type."""
    location = Location(None, 1.0, 2.0, 3.0)
    assert not isinstance(location, Vector)


def test_vector_arithmetic_returns_a_location() -> None:
    """Verify adding or subtracting a Vector gives a Location that keeps the rotation."""
    location = Location(None, 1.0, 2.0, 3.0, 10.0, 20.0)
    moved = location + Vector(1.0, 1.0, 1.0)
    assert isinstance(moved, Location)
    assert (moved.x, moved.y, moved.z) == (2.0, 3.0, 4.0)
    assert (moved.pitch, moved.yaw) == (10.0, 20.0)
    back = moved - Vector(1.0, 1.0, 1.0)
    assert isinstance(back, Location)
    assert (back.x, back.y, back.z) == (1.0, 2.0, 3.0)


def test_is_unhashable() -> None:
    """Verify a location cannot be used as a dict key, since it defines __eq__."""
    assert Location.__hash__ is None
    with pytest.raises(TypeError):
        {Location(None, 0.0, 0.0, 0.0)}


def test_is_dimension_loaded_without_a_dimension() -> None:
    """Verify a location with no dimension reports it as not loaded."""
    assert Location(None, 0.0, 0.0, 0.0).is_dimension_loaded is False
