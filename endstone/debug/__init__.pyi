"""
Classes relating to debug shape drawing utilities.
"""

from endstone.util import Vector

__all__ = ["DebugArrow", "DebugBox", "DebugCircle", "DebugLine", "DebugSphere", "DebugText"]

class DebugBox:
    """
    Represents a debug box (cuboid) shape.
    """
    def __init__(self) -> None: ...
    @property
    def color(self) -> tuple[int, ...]:
        """
        The color of this shape.
        """
        ...
    @color.setter
    def color(self, arg1: tuple[int, ...]) -> DebugBox: ...
    @property
    def scale(self) -> float:
        """
        The uniform scale factor.
        """
        ...
    @scale.setter
    def scale(self, arg1: float) -> DebugBox: ...
    @property
    def rotation(self) -> Vector:
        """
        The rotation of this shape as Euler angles (pitch, yaw, roll).
        """
        ...
    @rotation.setter
    def rotation(self, arg1: Vector) -> DebugBox: ...
    @property
    def bound(self) -> Vector:
        """
        The bounding size of the box. Final size = bound * scale.
        """
        ...
    @bound.setter
    def bound(self, arg1: Vector) -> DebugBox: ...

class DebugSphere:
    """
    Represents a debug sphere shape. Radius is controlled via scale.
    """
    def __init__(self) -> None: ...
    @property
    def color(self) -> tuple[int, ...]:
        """
        The color of this shape.
        """
        ...
    @color.setter
    def color(self, arg1: tuple[int, ...]) -> DebugSphere: ...
    @property
    def scale(self) -> float:
        """
        The uniform scale factor.
        """
        ...
    @scale.setter
    def scale(self, arg1: float) -> DebugSphere: ...
    @property
    def rotation(self) -> Vector:
        """
        The rotation of this shape as Euler angles (pitch, yaw, roll).
        """
        ...
    @rotation.setter
    def rotation(self, arg1: Vector) -> DebugSphere: ...

class DebugCircle:
    """
    Represents a debug 2D circle shape. Radius is controlled via scale.
    """
    def __init__(self) -> None: ...
    @property
    def color(self) -> tuple[int, ...]:
        """
        The color of this shape.
        """
        ...
    @color.setter
    def color(self, arg1: tuple[int, ...]) -> DebugCircle: ...
    @property
    def scale(self) -> float:
        """
        The uniform scale factor.
        """
        ...
    @scale.setter
    def scale(self, arg1: float) -> DebugCircle: ...
    @property
    def rotation(self) -> Vector:
        """
        The rotation of this shape as Euler angles (pitch, yaw, roll).
        """
        ...
    @rotation.setter
    def rotation(self, arg1: Vector) -> DebugCircle: ...

class DebugLine:
    """
    Represents a debug line segment shape.
    """
    def __init__(self) -> None: ...
    @property
    def color(self) -> tuple[int, ...]:
        """
        The color of this shape.
        """
        ...
    @color.setter
    def color(self, arg1: tuple[int, ...]) -> DebugLine: ...
    @property
    def scale(self) -> float:
        """
        The uniform scale factor.
        """
        ...
    @scale.setter
    def scale(self, arg1: float) -> DebugLine: ...
    @property
    def rotation(self) -> Vector:
        """
        The rotation of this shape as Euler angles (pitch, yaw, roll).
        """
        ...
    @rotation.setter
    def rotation(self, arg1: Vector) -> DebugLine: ...
    @property
    def length(self) -> float:
        """
        The length of the line segment.
        """
        ...
    @length.setter
    def length(self, arg1: float) -> DebugLine: ...

class DebugArrow:
    """
    Represents a debug arrow shape.
    """
    def __init__(self) -> None: ...
    @property
    def color(self) -> tuple[int, ...]:
        """
        The color of this shape.
        """
        ...
    @color.setter
    def color(self, arg1: tuple[int, ...]) -> DebugArrow: ...
    @property
    def scale(self) -> float:
        """
        The uniform scale factor.
        """
        ...
    @scale.setter
    def scale(self, arg1: float) -> DebugArrow: ...
    @property
    def rotation(self) -> Vector:
        """
        The rotation of this shape as Euler angles (pitch, yaw, roll).
        """
        ...
    @rotation.setter
    def rotation(self, arg1: Vector) -> DebugArrow: ...
    @property
    def length(self) -> float:
        """
        The length of the arrow.
        """
        ...
    @length.setter
    def length(self, arg1: float) -> DebugArrow: ...
    @property
    def head_length(self) -> float:
        """
        The length of the arrow's head.
        """
        ...
    @head_length.setter
    def head_length(self, arg1: float) -> DebugArrow: ...
    @property
    def head_radius(self) -> float:
        """
        The radius of the arrow's head.
        """
        ...
    @head_radius.setter
    def head_radius(self, arg1: float) -> DebugArrow: ...
    @property
    def head_segments(self) -> int:
        """
        The number of segments for the arrow head's base circle.
        """
        ...
    @head_segments.setter
    def head_segments(self, arg1: int) -> DebugArrow: ...

class DebugText:
    """
    Represents a debug billboard text label.
    """
    def __init__(self) -> None: ...
    @property
    def color(self) -> tuple[int, ...]:
        """
        The color of this shape.
        """
        ...
    @color.setter
    def color(self, arg1: tuple[int, ...]) -> DebugText: ...
    @property
    def scale(self) -> float:
        """
        The uniform scale factor.
        """
        ...
    @scale.setter
    def scale(self, arg1: float) -> DebugText: ...
    @property
    def rotation(self) -> Vector:
        """
        The rotation of this shape as Euler angles (pitch, yaw, roll).
        """
        ...
    @rotation.setter
    def rotation(self, arg1: Vector) -> DebugText: ...
    @property
    def text(self) -> str:
        """
        The text displayed by this label.
        """
        ...
    @text.setter
    def text(self, arg1: str) -> DebugText: ...
