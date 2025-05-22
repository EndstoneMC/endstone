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
#include <pybind11/numpy.h>

#include "endstone_python.h"

namespace py = pybind11;

namespace endstone::python {

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

    py::class_<Color>(m, "Color", "Represents a color with red, green, blue, and alpha components.")
        .def(py::init<std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t>(), py::arg("r") = 0, py::arg("g") = 0,
             py::arg("b") = 0, py::arg("a") = 255,
             "Creates a color with the specified red, green, blue, and alpha values in the range (0 - 255).")
        .def_property_readonly("r", &Color::getRed, "Gets the red component of the color.")
        .def_property_readonly("g", &Color::getRed, "Gets the green component of the color.")
        .def_property_readonly("b", &Color::getRed, "Gets the blue component of the color.")
        .def_property_readonly("a", &Color::getRed, "Gets the alpha component of the color.");

    py::class_<Image>(m, "Image",
                      "Represents an RGBA image.\nEach pixel is four bytes: R, G, B, A, in row-major order.")
        .def_static("from_buffer", &Image::fromBuffer, py::arg("buffer"),
                    "Creates an image from the pixel data in a byte buffer.")
        .def_property_readonly("width", &Image::getWidth, "Get the image width.")
        .def_property_readonly("height", &Image::getHeight, "Get the image width.")
        .def("get_color", &Image::getColor, py::arg("x"), py::arg("y"), "Get the color of a pixel.")
        .def("set_color", &Image::setColor, py::arg("x"), py::arg("y"), py::arg("color"), "Set the color of a pixel.")
        .def_property_readonly("data", [](const Image &self) {
            const std::string_view buf = self.getData();
            const py::ssize_t h = self.getHeight();
            const py::ssize_t w = self.getWidth();
            const std::array<py::ssize_t, 3> shape{{h, w, 4}};        // shape: (height, width, 4)
            const std::array<py::ssize_t, 3> strides{{w * 4, 4, 1}};  // strides in bytes: row, column, channel
            return py::array_t<std::uint8_t>(py::buffer_info(
                const_cast<char *>(buf.data()), 1, py::format_descriptor<std::uint8_t>::format(), 3, shape, strides));
        });
}

}  // namespace endstone::python
