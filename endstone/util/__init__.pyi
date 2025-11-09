"""
Multi and single purpose classes.
"""

import collections
import typing

__all__ = ["SocketAddress", "Vector"]

class Vector:
    """
    Represents a 3-dimensional vector.
    """
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, x: float, y: float, z: float) -> None: ...
    @property
    def x(self) -> float:
        """
        The X component of the vector
        """
        ...
    @x.setter
    def x(self, arg1: float) -> Vector: ...
    @property
    def y(self) -> float:
        """
        The Y component of the vector
        """
        ...
    @y.setter
    def y(self, arg1: float) -> Vector: ...
    @property
    def z(self) -> float:
        """
        The Z component of the vector
        """
        ...
    @z.setter
    def z(self, arg1: float) -> Vector: ...
    @property
    def block_x(self) -> int:
        """
        Gets the floored value of the X component, indicating the block that this vector is contained with.
        """
        ...
    @property
    def block_y(self) -> int:
        """
        Gets the floored value of the Y component, indicating the block that this vector is contained with.
        """
        ...
    @property
    def block_z(self) -> int:
        """
        Gets the floored value of the Z component, indicating the block that this vector is contained with.
        """
        ...
    @typing.overload
    def __add__(self, arg0: Vector) -> Vector: ...
    @typing.overload
    def __add__(self, arg0: float) -> Vector: ...
    @typing.overload
    def __sub__(self, arg0: Vector) -> Vector: ...
    @typing.overload
    def __sub__(self, arg0: float) -> Vector: ...
    @typing.overload
    def __mul__(self, arg0: Vector) -> Vector: ...
    @typing.overload
    def __mul__(self, arg0: float) -> Vector: ...
    @typing.overload
    def __truediv__(self, arg0: Vector) -> Vector: ...
    @typing.overload
    def __truediv__(self, arg0: float) -> Vector: ...
    def __iadd__(self, arg0: Vector) -> Vector: ...
    def __isub__(self, arg0: Vector) -> Vector: ...
    def __imul__(self, arg0: Vector) -> Vector: ...
    def __itruediv__(self, arg0: Vector) -> Vector: ...
    def __radd__(self, arg0: float) -> Vector: ...
    def __rsub__(self, arg0: float) -> Vector: ...
    def __rmul__(self, arg0: float) -> Vector: ...
    def __rtruediv__(self, arg0: float) -> Vector: ...
    def __eq__(self, arg0: Vector) -> bool: ...
    def __ne__(self, arg0: Vector) -> bool: ...
    @property
    def length(self) -> float:
        """
        The magnitude of the Vector
        """
        ...
    @property
    def length_squared(self) -> float:
        """
        The squared magnitude of the Vector
        """
        ...
    def distance(self, other: Vector) -> float:
        """
        The distance between this Vector and another
        """
        ...
    def distance_squared(self, other: Vector) -> float:
        """
        The squared distance between this Vector and another
        """
        ...
    def angle(self, other: Vector) -> float:
        """
        Gets the angle between this vector and another in radians.
        """
        ...
    def midpoint(self, other: Vector) -> Vector:
        """
        Gets a new midpoint vector between this vector and another.
        """
        ...
    def dot(self, other: Vector) -> float:
        """
        Calculates the dot product of this vector with another.
        """
        ...
    def cross_product(self, other: Vector) -> Vector:
        """
        Calculates the cross-product of this vector with another.
        """
        ...
    def normalize(self) -> Vector:
        """
        Converts this vector to a unit vector (a vector with length of 1).
        """
        ...
    def zero(self) -> Vector:
        """
        Zero this vector's components.
        """
        ...
    @property
    def is_zero(self) -> bool:
        """
        Check whether or not each component of this vector is equal to 0.
        """
        ...
    def normalize_zero(self) -> Vector:
        """
        Converts each component of value `-0.0` to `0.0`
        """
        ...
    def is_in_aabb(self, min: Vector, max: Vector) -> bool:
        """
        Returns whether this vector is in an axis-aligned bounding box.
        """
        ...
    def is_in_sphere(self, origin: Vector, radius: float) -> bool:
        """
        Returns whether this vector is within a sphere.
        """
        ...
    @property
    def is_normalized(self) -> bool:
        """
        Returns if a vector is normalized.
        """
        ...
    def rotate_around_x(self, angle: float) -> Vector:
        """
        Rotates the vector around the x-axis.
        """
        ...
    def rotate_around_y(self, angle: float) -> Vector:
        """
        Rotates the vector around the y-axis.
        """
        ...
    def rotate_around_z(self, angle: float) -> Vector:
        """
        Rotates the vector around the z-axis.
        """
        ...
    def rotate_around_axis(self, axis: Vector, angle: float, normalize: bool = True) -> Vector:
        """
        Rotates the vector around a given arbitrary axis in 3-dimensional space.
        """
        ...
    def __len__(self) -> int: ...
    def __getitem__(self, arg0: int) -> float: ...
    def __setitem__(self, arg0: int, arg1: float) -> None: ...
    def __matmul__(self, other: Vector) -> float:
        """
        Dot product (v @ u).
        """
        ...
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...

class SocketAddress:
    """
    Represents an IP Socket Address (hostname + port number).
    """
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, hostname: str, port: int) -> None: ...
    @property
    def hostname(self) -> str:
        """
        Gets the hostname.
        """
        ...
    @property
    def port(self) -> int:
        """
        Gets the port number.
        """
        ...
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
