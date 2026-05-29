// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <fmt/format.h>

#include "endstone_python.h"

namespace py = pybind11;

namespace endstone::python {
void init_util(py::module &m, py::class_<Vector> &vector)
{
    py::class_<SocketAddress>(m, "SocketAddress", "Represents an IP Socket Address (hostname + port number).")
        .def(py::init<>())
        .def(py::init<std::string, std::uint32_t>(), py::arg("hostname"), py::arg("port"))
        .def_property_readonly("hostname", &SocketAddress::getHostname, "The hostname of this socket address.")
        .def_property_readonly("port", &SocketAddress::getPort, "The port number of this socket address.")
        .def("__repr__",
             [](const SocketAddress &self) {
                 return fmt::format("SocketAddress(hostname='{}', port={})", self.getHostname(), self.getPort());
             })
        .def("__str__",
             [](const SocketAddress &self) { return self.getHostname() + ":" + std::to_string(self.getPort()); });

    vector.def(py::init<>())
        .def(py::init<float, float, float>(), py::arg("x"), py::arg("y"), py::arg("z"))
        .def_property(
            "x", &Vector::getX, [](Vector &self, float x) { self.setX(x); }, "The X component of the vector.")
        .def_property(
            "y", &Vector::getY, [](Vector &self, float y) { self.setY(y); }, "The Y component of the vector.")
        .def_property(
            "z", &Vector::getZ, [](Vector &self, float z) { self.setZ(z); }, "The Z component of the vector.")
        .def_property_readonly(
            "block_x", &Vector::getBlockX,
            "The floored value of the X component, indicating the block that this vector is contained with.")
        .def_property_readonly(
            "block_y", &Vector::getBlockY,
            "The floored value of the Y component, indicating the block that this vector is contained with.")
        .def_property_readonly(
            "block_z", &Vector::getBlockZ,
            "The floored value of the Z component, indicating the block that this vector is contained with.")
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * py::self)
        .def(py::self / py::self)
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(py::self *= py::self)
        .def(py::self /= py::self)
        .def(py::self + float())
        .def(py::self - float())
        .def(py::self * float())
        .def(py::self / float())
        .def(float() + py::self)
        .def(float() - py::self)
        .def(float() * py::self)
        .def(float() / py::self)
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def_property_readonly("length", &Vector::length,
                               "The magnitude of the vector, defined as sqrt(x^2 + y^2 + z^2).")
        .def_property_readonly("length_squared", &Vector::lengthSquared, "The magnitude of the vector squared.")
        .def("distance", &Vector::distance, py::arg("other"), R"doc(
    Get the distance between this vector and another.

    Args:
        other: The other vector.

    Returns:
        The distance.
)doc")
        .def("distance_squared", &Vector::distanceSquared, py::arg("other"), R"doc(
    Get the squared distance between this vector and another.

    Args:
        other: The other vector.

    Returns:
        The squared distance.
)doc")
        .def("angle", &Vector::angle, py::arg("other"), R"doc(
    Gets the angle between this vector and another in radians.

    Args:
        other: The other vector.

    Returns:
        The angle in radians.
)doc")
        .def("midpoint", &Vector::getMidpoint, py::arg("other"), R"doc(
    Gets a new midpoint vector between this vector and another.

    Args:
        other: The other vector.

    Returns:
        A new midpoint vector.
)doc")
        .def("dot", &Vector::dot, py::arg("other"), R"doc(
    Calculates the dot product of this vector with another.

    The dot product is defined as x1*x2 + y1*y2 + z1*z2. The returned value is a scalar.

    Args:
        other: The other vector.

    Returns:
        The dot product.
)doc")
        .def("cross_product", &Vector::getCrossProduct, py::arg("other"), R"doc(
    Calculates the cross-product of this vector with another without mutating the original.

    Args:
        other: The other vector.

    Returns:
        A new vector containing the cross-product.
)doc")
        .def("normalize", &Vector::normalize, "Converts this vector to a unit vector (a vector with length of 1).")
        .def("zero", &Vector::zero, "Zero this vector's components.")
        .def_property_readonly("is_zero", &Vector::isZero,
                               "True if each component of this vector is equal to 0.")
        .def("normalize_zero", &Vector::normalizeZeros, "Converts each component of value ``-0.0`` to ``0.0``.")
        .def("is_in_aabb", &Vector::isInAABB, py::arg("min"), py::arg("max"), R"doc(
    Returns whether this vector is in an axis-aligned bounding box.

    The minimum and maximum vectors given must be truly the minimum and maximum X, Y and Z components.

    Args:
        min: Minimum vector.
        max: Maximum vector.

    Returns:
        Whether this vector is in the AABB.
)doc")
        .def("is_in_sphere", &Vector::isInSphere, py::arg("origin"), py::arg("radius"), R"doc(
    Returns whether this vector is within a sphere.

    Args:
        origin: Sphere origin.
        radius: Sphere radius.

    Returns:
        Whether this vector is in the sphere.
)doc")
        .def_property_readonly("is_normalized", &Vector::isNormalized, "True if this vector is normalized.")
        .def("rotate_around_x", &Vector::rotateAroundX, py::arg("angle"), R"doc(
    Rotates the vector around the x-axis.

    Args:
        angle: The angle to rotate the vector about, in radians.
)doc")
        .def("rotate_around_y", &Vector::rotateAroundY, py::arg("angle"), R"doc(
    Rotates the vector around the y-axis.

    Args:
        angle: The angle to rotate the vector about, in radians.
)doc")
        .def("rotate_around_z", &Vector::rotateAroundZ, py::arg("angle"), R"doc(
    Rotates the vector around the z-axis.

    Args:
        angle: The angle to rotate the vector about, in radians.
)doc")
        .def(
            "rotate_around_axis",
            [](Vector &self, const Vector &axis, float angle, bool normalize) {
                return self.rotateAroundNonUnitAxis(
                    (!axis.isNormalized() && normalize) ? Vector(axis).normalize() : axis, angle);
            },
            py::arg("axis"), py::arg("angle"), py::arg("normalize") = true, R"doc(
    Rotates the vector around a given arbitrary axis in 3-dimensional space.

    Rotation will follow the general Right-Hand-Rule, which means rotation will be counterclockwise when the axis
    is pointing towards the observer.

    Args:
        axis: The axis to rotate the vector around.
        angle: The angle to rotate the vector around the axis.
        normalize: When True (default), the axis vector is normalized before being used for the rotation, preserving
            the length of this vector.
)doc")
        .def("__matmul__", &Vector::dot, py::arg("other"), "Dot product (v @ u).")
        .def("__repr__",
             [](const Vector &v) { return fmt::format("Vector(x={}, y={}, z={})", v.getX(), v.getY(), v.getZ()); })
        .def("__str__", [](const Vector &v) { return fmt::format("{}", v); });
}

}  // namespace endstone::python
