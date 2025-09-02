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
namespace {
Position create_position(float x, float y, float z, Dimension *dimension)
{
    if (dimension == nullptr) {
        return Position(x, y, z);
    }
    return Position(x, y, z, *dimension);
}

Location create_location(float x, float y, float z, float pitch, float yaw, Dimension *dimension)
{
    if (dimension == nullptr) {
        return Location(x, y, z, pitch, yaw);
    }
    return Location(x, y, z, pitch, yaw, *dimension);
}
}  // namespace

void init_util(py::module &m)
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

    py::class_<Vector<float>>(m, "Vector", "Represents a 3-dimensional vector.")
        .def(py::init<>())
        .def(py::init<float, float, float>(), py::arg("x"), py::arg("y"), py::arg("z"))
        .def_property("x", &Vector<float>::getX, &Vector<float>::setX, "The X component of the vector")
        .def_property("y", &Vector<float>::getY, &Vector<float>::setY, "The Y component of the vector")
        .def_property("z", &Vector<float>::getZ, &Vector<float>::setZ, "The Z component of the vector")
        .def("__repr__",
             [](const Vector<float> &v) {
                 return fmt::format("Vector(x={}, y={}, z={})", v.getX(), v.getY(), v.getZ());
             })
        .def("__str__", [](const Vector<float> &v) { return fmt::format("{},{},{}", v.getX(), v.getY(), v.getZ()); })
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

    auto position_to_string = [](const Position &p) {
        return fmt::format("Position(dimension={}, x={}, y={}, z={})",
                           p.getDimension() ? p.getDimension()->getName() : "None", p.getX(), p.getY(), p.getZ());
    };
    py::class_<Position, Vector<float>>(m, "Position",
                                        "Represents a 3-dimensional position in a dimension within a level.")
        .def(py::init(&create_position), py::arg("x"), py::arg("y"), py::arg("z"), py::arg("dimension") = nullptr)
        .def_property("dimension", &Position::getDimension, &Position::setDimension, py::return_value_policy::reference,
                      "The Dimension that contains this position")
        .def_property_readonly(
            "block_x", &Position::getBlockX,
            "Gets the floored value of the X component, indicating the block that this location is contained with.")
        .def_property_readonly(
            "block_y", &Position::getBlockY,
            "Gets the floored value of the Y component, indicating the block that this location is contained with.")
        .def_property_readonly(
            "block_z", &Position::getBlockZ,
            "Gets the floored value of the Z component, indicating the block that this location is contained with.")
        .def("__repr__", position_to_string)
        .def("__str__", position_to_string);

    auto location_to_string = [](const Location &l) {
        return fmt::format("Location(dimension={}, x={}, y={}, z={}, pitch={}, yaw={})",
                           l.getDimension() ? l.getDimension()->getName() : "None", l.getX(), l.getY(), l.getZ(),
                           l.getPitch(), l.getYaw());
    };
    py::class_<Location, Position>(m, "Location", "Represents a 3-dimensional location in a dimension within a level.")
        .def(py::init(&create_location), py::arg("x"), py::arg("y"), py::arg("z"), py::arg("pitch") = 0.0,
             py::arg("yaw") = 0.0, py::arg("dimension") = nullptr)
        .def_property("pitch", &Location::getPitch, &Location::setPitch,
                      "The pitch of this location, measured in degrees.")
        .def_property("yaw", &Location::getYaw, &Location::setYaw, "The yaw of this location, measured in degrees.")
        .def("__repr__", location_to_string)
        .def("__str__", location_to_string);
}

}  // namespace endstone::python
