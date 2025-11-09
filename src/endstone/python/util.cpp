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
        .def_property_readonly("hostname", &SocketAddress::getHostname, "Gets the hostname.")
        .def_property_readonly("port", &SocketAddress::getPort, "Gets the port number.")
        .def("__repr__",
             [](const SocketAddress &self) {
                 return fmt::format("SocketAddress(hostname='{}', port={})", self.getHostname(), self.getPort());
             })
        .def("__str__",
             [](const SocketAddress &self) { return self.getHostname() + ":" + std::to_string(self.getPort()); });

    vector.def(py::init<>())
        .def(py::init<float, float, float>(), py::arg("x"), py::arg("y"), py::arg("z"))
        .def_property(
            "x", &Vector::getX, [](Vector &self, float x) { self.setX(x); }, "The X component of the vector")
        .def_property(
            "y", &Vector::getY, [](Vector &self, float y) { self.setY(y); }, "The Y component of the vector")
        .def_property(
            "z", &Vector::getZ, [](Vector &self, float z) { self.setZ(z); }, "The Z component of the vector")
        .def_property_readonly(
            "block_x", &Vector::getBlockX,
            "Gets the floored value of the X component, indicating the block that this vector is contained with.")
        .def_property_readonly(
            "block_y", &Vector::getBlockY,
            "Gets the floored value of the Y component, indicating the block that this vector is contained with.")
        .def_property_readonly(
            "block_z", &Vector::getBlockZ,
            "Gets the floored value of the Z component, indicating the block that this vector is contained with.")
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
        .def_property_readonly("length", &Vector::length, "The magnitude of the Vector")
        .def_property_readonly("length_squared", &Vector::lengthSquared, "The squared magnitude of the Vector")
        .def("distance", &Vector::distance, py::arg("other"), "The distance between this Vector and another")
        .def("distance_squared", &Vector::distanceSquared, py::arg("other"),
             "The squared distance between this Vector and another")
        .def("angle", &Vector::angle, py::arg("other"), "Gets the angle between this vector and another in radians.")
        .def("midpoint", &Vector::getMidpoint, py::arg("other"),
             "Gets a new midpoint vector between this vector and another.")
        .def("dot", &Vector::dot, py::arg("other"), "Calculates the dot product of this vector with another.")
        .def("cross_product", &Vector::getCrossProduct, py::arg("other"),
             "Calculates the cross-product of this vector with another.")
        .def("normalize", &Vector::normalize, "Converts this vector to a unit vector (a vector with length of 1).")
        .def("zero", &Vector::zero, "Zero this vector's components.")
        .def_property_readonly("is_zero", &Vector::isZero,
                               "Check whether or not each component of this vector is equal to 0.")
        .def("normalize_zero", &Vector::normalizeZeros, "Converts each component of value `-0.0` to `0.0`")
        .def("is_in_aabb", &Vector::isInAABB, py::arg("min"), py::arg("max"),
             "Returns whether this vector is in an axis-aligned bounding box.")
        .def("is_in_sphere", &Vector::isInSphere, py::arg("origin"), py::arg("radius"),
             "Returns whether this vector is within a sphere.")
        .def_property_readonly("is_normalized", &Vector::isNormalized, "Returns if a vector is normalized.")
        .def("rotate_around_x", &Vector::rotateAroundX, py::arg("angle"), "Rotates the vector around the x-axis.")
        .def("rotate_around_y", &Vector::rotateAroundY, py::arg("angle"), "Rotates the vector around the y-axis.")
        .def("rotate_around_z", &Vector::rotateAroundZ, py::arg("angle"), "Rotates the vector around the z-axis.")
        .def(
            "rotate_around_axis",
            [](Vector &self, const Vector &axis, float angle, bool normalize) {
                return self.rotateAroundNonUnitAxis(
                    (!axis.isNormalized() && normalize) ? Vector(axis).normalize() : axis, angle);
            },
            py::arg("axis"), py::arg("angle"), py::arg("normalize") = true,
            "Rotates the vector around a given arbitrary axis in 3-dimensional space.")
        .def("__len__", [](const Vector &) { return 3; })
        .def("__getitem__",
             [](const Vector &v, py::ssize_t i) {
                 if (i < 0) {
                     i += 3;
                 }
                 switch (i) {
                 case 0:
                     return v.getX();
                 case 1:
                     return v.getY();
                 case 2:
                     return v.getZ();
                 default:
                     throw py::index_error();
                 }
             })
        .def("__setitem__",
             [](Vector &v, py::ssize_t i, float val) {
                 if (i < 0) {
                     i += 3;
                 }
                 switch (i) {
                 case 0:
                     v.setX(val);
                     return;
                 case 1:
                     v.setY(val);
                     return;
                 case 2:
                     v.setZ(val);
                     return;
                 default:
                     throw py::index_error();
                 }
             })
        .def("__matmul__", &Vector::dot, py::arg("other"), "Dot product (v @ u).")
        .def("__repr__",
             [](const Vector &v) { return fmt::format("Vector(x={}, y={}, z={})", v.getX(), v.getY(), v.getZ()); })
        .def("__str__", [](const Vector &v) { return fmt::format("{}", v); });
}

}  // namespace endstone::python
