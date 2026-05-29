"""
Multi and single purpose classes.
"""

import typing

__all__ = [
    "SocketAddress",
    "Vector",
]

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
        The X component of the vector.
        """
        ...
    @x.setter
    def x(self, arg1: float) -> None: ...
    @property
    def y(self) -> float:
        """
        The Y component of the vector.
        """
        ...
    @y.setter
    def y(self, arg1: float) -> None: ...
    @property
    def z(self) -> float:
        """
        The Z component of the vector.
        """
        ...
    @z.setter
    def z(self, arg1: float) -> None: ...
    @property
    def block_x(self) -> int:
        """
        The floored value of the X component, indicating the block that this vector is contained with.
        """
        ...
    @property
    def block_y(self) -> int:
        """
        The floored value of the Y component, indicating the block that this vector is contained with.
        """
        ...
    @property
    def block_z(self) -> int:
        """
        The floored value of the Z component, indicating the block that this vector is contained with.
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
    @typing.overload
    def __iadd__(self, arg0: Vector) -> Vector: ...
    @typing.overload
    def __iadd__(self, arg0: float) -> Vector: ...
    @typing.overload
    def __isub__(self, arg0: Vector) -> Vector: ...
    @typing.overload
    def __isub__(self, arg0: float) -> Vector: ...
    @typing.overload
    def __imul__(self, arg0: Vector) -> Vector: ...
    @typing.overload
    def __imul__(self, arg0: float) -> Vector: ...
    @typing.overload
    def __itruediv__(self, arg0: Vector) -> Vector: ...
    @typing.overload
    def __itruediv__(self, arg0: float) -> Vector: ...
    def __radd__(self, arg0: float) -> Vector: ...
    def __rsub__(self, arg0: float) -> Vector: ...
    def __rmul__(self, arg0: float) -> Vector: ...
    def __rtruediv__(self, arg0: float) -> Vector: ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    @property
    def length(self) -> float:
        """
        The magnitude of the vector, defined as sqrt(x^2 + y^2 + z^2).
        """
        ...
    @property
    def length_squared(self) -> float:
        """
        The magnitude of the vector squared.
        """
        ...
    def distance(self, other: Vector) -> float:
        """
        Get the distance between this vector and another.

        Args:
            other: The other vector.

        Returns:
            The distance.
        """
        ...
    def distance_squared(self, other: Vector) -> float:
        """
        Get the squared distance between this vector and another.

        Args:
            other: The other vector.

        Returns:
            The squared distance.
        """
        ...
    def angle(self, other: Vector) -> float:
        """
        Gets the angle between this vector and another in radians.

        Args:
            other: The other vector.

        Returns:
            The angle in radians.
        """
        ...
    def midpoint(self, other: Vector) -> Vector:
        """
        Gets a new midpoint vector between this vector and another.

        Args:
            other: The other vector.

        Returns:
            A new midpoint vector.
        """
        ...
    def dot(self, other: Vector) -> float:
        """
        Calculates the dot product of this vector with another.

        The dot product is defined as x1*x2 + y1*y2 + z1*z2. The returned value is a scalar.

        Args:
            other: The other vector.

        Returns:
            The dot product.
        """
        ...
    def cross_product(self, other: Vector) -> Vector:
        """
        Calculates the cross-product of this vector with another without mutating the original.

        Args:
            other: The other vector.

        Returns:
            A new vector containing the cross-product.
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
        True if each component of this vector is equal to 0.
        """
        ...
    def normalize_zero(self) -> Vector:
        """
        Converts each component of value ``-0.0`` to ``0.0``.
        """
        ...
    def is_in_aabb(self, min: Vector, max: Vector) -> bool:
        """
        Returns whether this vector is in an axis-aligned bounding box.

        The minimum and maximum vectors given must be truly the minimum and maximum X, Y and Z components.

        Args:
            min: Minimum vector.
            max: Maximum vector.

        Returns:
            Whether this vector is in the AABB.
        """
        ...
    def is_in_sphere(self, origin: Vector, radius: float) -> bool:
        """
        Returns whether this vector is within a sphere.

        Args:
            origin: Sphere origin.
            radius: Sphere radius.

        Returns:
            Whether this vector is in the sphere.
        """
        ...
    @property
    def is_normalized(self) -> bool:
        """
        True if this vector is normalized.
        """
        ...
    def rotate_around_x(self, angle: float) -> Vector:
        """
        Rotates the vector around the x-axis.

        Args:
            angle: The angle to rotate the vector about, in radians.
        """
        ...
    def rotate_around_y(self, angle: float) -> Vector:
        """
        Rotates the vector around the y-axis.

        Args:
            angle: The angle to rotate the vector about, in radians.
        """
        ...
    def rotate_around_z(self, angle: float) -> Vector:
        """
        Rotates the vector around the z-axis.

        Args:
            angle: The angle to rotate the vector about, in radians.
        """
        ...
    def rotate_around_axis(self, axis: Vector, angle: float, normalize: bool = True) -> Vector:
        """
        Rotates the vector around a given arbitrary axis in 3-dimensional space.

        Rotation will follow the general Right-Hand-Rule, which means rotation will be counterclockwise when the axis
        is pointing towards the observer.

        Args:
            axis: The axis to rotate the vector around.
            angle: The angle to rotate the vector around the axis.
            normalize: When True (default), the axis vector is normalized before being used for the rotation, preserving
                the length of this vector.
        """
        ...
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
        The hostname of this socket address.
        """
        ...
    @property
    def port(self) -> int:
        """
        The port number of this socket address.
        """
        ...
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
