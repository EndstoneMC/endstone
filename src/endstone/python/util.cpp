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
        .def_property("x", &Vector::getX, &Vector::setX, "The X component of the vector")
        .def_property("y", &Vector::getY, &Vector::setY, "The Y component of the vector")
        .def_property("z", &Vector::getZ, &Vector::setZ, "The Z component of the vector")
        .def("__repr__",
             [](const Vector &v) { return fmt::format("Vector(x={}, y={}, z={})", v.getX(), v.getY(), v.getZ()); })
        .def("__str__", [](const Vector &v) { return fmt::format("{},{},{}", v.getX(), v.getY(), v.getZ()); })
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * py::self)
        .def(py::self / py::self)
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(py::self *= py::self)
        .def(py::self /= py::self)
        .def(py::self + double())
        .def(py::self - double())
        .def(py::self * double())
        .def(py::self / double())
        .def(double() + py::self)
        .def(double() - py::self)
        .def(double() * py::self)
        .def(double() / py::self)
        .def_property_readonly("length", &Vector::length, "The magnitude of the Vector")
        .def_property_readonly("length_squared", &Vector::lengthSquared, "The squared magnitude of the Vector")
        .def("distance", &Vector::distance, py::arg("other"), "The distance between this Vector and another")
        .def("distance_squared", &Vector::distanceSquared, py::arg("other"),
             "The squared distance between this Vector and another");
}

}  // namespace endstone::python
