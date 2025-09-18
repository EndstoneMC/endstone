"""
Multi and single purpose classes.
"""

from __future__ import annotations

import typing

__all__: list[str] = ["SocketAddress", "Vector"]

class SocketAddress:
    """
    Represents an IP Socket Address (hostname + port number).
    """

    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, hostname: str, port: int) -> None: ...
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
    def __add__(self, arg0: float) -> Vector: ...
    def __iadd__(self, arg0: Vector) -> Vector: ...
    def __imul__(self, arg0: Vector) -> Vector: ...
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, x: float, y: float, z: float) -> None: ...
    def __isub__(self, arg0: Vector) -> Vector: ...
    def __itruediv__(self, arg0: Vector) -> Vector: ...
    @typing.overload
    def __mul__(self, arg0: Vector) -> Vector: ...
    @typing.overload
    def __mul__(self, arg0: float) -> Vector: ...
    def __radd__(self, arg0: float) -> Vector: ...
    def __repr__(self) -> str: ...
    def __rmul__(self, arg0: float) -> Vector: ...
    def __rsub__(self, arg0: float) -> Vector: ...
    def __rtruediv__(self, arg0: float) -> Vector: ...
    def __str__(self) -> str: ...
    @typing.overload
    def __sub__(self, arg0: Vector) -> Vector: ...
    @typing.overload
    def __sub__(self, arg0: float) -> Vector: ...
    @typing.overload
    def __truediv__(self, arg0: Vector) -> Vector: ...
    @typing.overload
    def __truediv__(self, arg0: float) -> Vector: ...
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
    def x(self, arg1: float) -> None: ...
    @property
    def y(self) -> float:
        """
        The Y component of the vector
        """

    @y.setter
    def y(self, arg1: float) -> None: ...
    @property
    def z(self) -> float:
        """
        The Z component of the vector
        """

    @z.setter
    def z(self, arg1: float) -> None: ...
