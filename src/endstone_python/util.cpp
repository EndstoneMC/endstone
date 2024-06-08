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

#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

#include "endstone/util/socket_address.h"
#include "endstone/util/vector.h"

namespace py = pybind11;

namespace endstone::detail {

void init_util(py::module &m)
{
    py::class_<SocketAddress>(m, "SocketAddress", "Represents an IP Socket Address (hostname + port number).")
        .def(py::init<>())
        .def(py::init<std::string, std::uint32_t>(), py::arg("hostname"), py::arg("port"))
        .def_property_readonly("hostname", &SocketAddress::getHostname, "Gets the hostname.")
        .def_property_readonly("port", &SocketAddress::getPort, "Gets the port number.")
        .def("__repr__",
             [](const SocketAddress &self) {
                 return "<SocketAddress hostname='" + self.getHostname() + "' port=" + std::to_string(self.getPort()) +
                        ">";
             })
        .def("__str__",
             [](const SocketAddress &self) { return self.getHostname() + ":" + std::to_string(self.getPort()); });

    py::class_<Vector<float>>(m, "Vector", "Represents a 3-dimensional vector.")
        .def(py::init<>())
        .def(py::init<float, float, float>(), py::arg("x"), py::arg("y"), py::arg("z"))
        .def_property("x", &Vector<float>::getX, &Vector<float>::setX, "The X component of the vector")
        .def_property("y", &Vector<float>::getY, &Vector<float>::setY, "The Y component of the vector")
        .def_property("z", &Vector<float>::getZ, &Vector<float>::setZ, "The Z component of the vector")
        .def("__repr__",
             [](const Vector<float> &v) {
                 return "<Vector x: " + std::to_string(v.getX()) + " y: " + std::to_string(v.getY()) +
                        " z: " + std::to_string(v.getZ()) + ">";
             })
        .def("__str__", &Vector<float>::toString)
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
        .def_property_readonly("length", &Vector<float>::length, "The magnitude of the Vector")
        .def_property_readonly("length_squared", &Vector<float>::lengthSquared, "The squared magnitude of the Vector")
        .def("distance", &Vector<float>::distance, py::arg("other"), "The distance between this Vector and another")
        .def("distance_squared", &Vector<float>::distanceSquared, py::arg("other"),
             "The squared distance between this Vector and another");
}

}  // namespace endstone::detail
