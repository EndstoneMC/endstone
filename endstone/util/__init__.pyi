"""
Multi and single purpose classes.
"""

from __future__ import annotations

import typing

import endstone._python.level

__all__: list[str] = ["Location", "Position", "SocketAddress", "Vector"]

class Location(Position):
    """
    Represents a 3-dimensional location in a dimension within a level.
    """
    def __init__(
        self,
        x: typing.SupportsFloat,
        y: typing.SupportsFloat,
        z: typing.SupportsFloat,
        pitch: typing.SupportsFloat = 0.0,
        yaw: typing.SupportsFloat = 0.0,
        dimension: endstone._python.level.Dimension = None,
    ) -> None: ...
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    @property
    def pitch(self) -> float:
        """
        The pitch of this location, measured in degrees.
        """
    @pitch.setter
    def pitch(self, arg1: typing.SupportsFloat) -> None: ...
    @property
    def yaw(self) -> float:
        """
        The yaw of this location, measured in degrees.
        """
    @yaw.setter
    def yaw(self, arg1: typing.SupportsFloat) -> None: ...

class Position(Vector):
    """
    Represents a 3-dimensional position in a dimension within a level.
    """
    def __init__(
        self,
        x: typing.SupportsFloat,
        y: typing.SupportsFloat,
        z: typing.SupportsFloat,
        dimension: endstone._python.level.Dimension = None,
    ) -> None: ...
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    @property
    def block_x(self) -> int:
        """
        Gets the floored value of the X component, indicating the block that this location is contained with.
        """
    @property
    def block_y(self) -> int:
        """
        Gets the floored value of the Y component, indicating the block that this location is contained with.
        """
    @property
    def block_z(self) -> int:
        """
        Gets the floored value of the Z component, indicating the block that this location is contained with.
        """
    @property
    def dimension(self) -> endstone._python.level.Dimension:
        """
        The Dimension that contains this position
        """
    @dimension.setter
    def dimension(self, arg1: endstone._python.level.Dimension) -> None: ...

class SocketAddress:
    """
    Represents an IP Socket Address (hostname + port number).
    """
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, hostname: str, port: typing.SupportsInt) -> None: ...
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    @property
    def hostname(self) -> str:
        """
        Gets the hostname.
        """
    @property
    def port(self) -> int:
        """
        Gets the port number.
        """

class Vector:
    """
    Represents a 3-dimensional vector.
    """
    @typing.overload
    def __add__(self, arg0: Vector) -> Vector: ...
    @typing.overload
    def __add__(self, arg0: typing.SupportsFloat) -> Vector: ...
    def __iadd__(self, arg0: Vector) -> Vector: ...
    def __imul__(self, arg0: Vector) -> Vector: ...
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, x: typing.SupportsFloat, y: typing.SupportsFloat, z: typing.SupportsFloat) -> None: ...
    def __isub__(self, arg0: Vector) -> Vector: ...
    def __itruediv__(self, arg0: Vector) -> Vector: ...
    @typing.overload
    def __mul__(self, arg0: Vector) -> Vector: ...
    @typing.overload
    def __mul__(self, arg0: typing.SupportsFloat) -> Vector: ...
    def __radd__(self, arg0: typing.SupportsFloat) -> Vector: ...
    def __repr__(self) -> str: ...
    def __rmul__(self, arg0: typing.SupportsFloat) -> Vector: ...
    def __rsub__(self, arg0: typing.SupportsFloat) -> Vector: ...
    def __rtruediv__(self, arg0: typing.SupportsFloat) -> Vector: ...
    def __str__(self) -> str: ...
    @typing.overload
    def __sub__(self, arg0: Vector) -> Vector: ...
    @typing.overload
    def __sub__(self, arg0: typing.SupportsFloat) -> Vector: ...
    @typing.overload
    def __truediv__(self, arg0: Vector) -> Vector: ...
    @typing.overload
    def __truediv__(self, arg0: typing.SupportsFloat) -> Vector: ...
    def distance(self, other: Vector) -> float:
        """
        The distance between this Vector and another
        """
    def distance_squared(self, other: Vector) -> float:
        """
        The squared distance between this Vector and another
        """
    @property
    def length(self) -> float:
        """
        The magnitude of the Vector
        """
    @property
    def length_squared(self) -> float:
        """
        The squared magnitude of the Vector
        """
    @property
    def x(self) -> float:
        """
        The X component of the vector
        """
    @x.setter
    def x(self, arg1: typing.SupportsFloat) -> None: ...
    @property
    def y(self) -> float:
        """
        The Y component of the vector
        """
    @y.setter
    def y(self, arg1: typing.SupportsFloat) -> None: ...
    @property
    def z(self) -> float:
        """
        The Z component of the vector
        """
    @z.setter
    def z(self, arg1: typing.SupportsFloat) -> None: ...
