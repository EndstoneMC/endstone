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

#include "endstone_python.h"

namespace py = pybind11;

namespace endstone::python {
template <typename T, typename... Args>
auto value_tag(py::module &m, const char *name, Args &&...args)
{
    using value_type = T::value_type;
    auto cls = py::class_<T>(m, name, std::forward<Args>(args)...)
                   .def(py::init<>())
                   .def(py::init<const value_type &>(), py::arg("value"))
                   .def_property_readonly("value", &T::value)
                   .def(py::self == py::self)
                   .def(py::self != py::self)
                   .def(py::self == value_type())
                   .def(py::self != value_type())
                   .def(value_type() == py::self)
                   .def(value_type() != py::self)
                   .def("__str__", [](const T &self) { return fmt::format("{}", self); })
                   .def("__repr__", [](const py::object &self) {
                       auto name = py::type::of(self).attr("__name__").cast<std::string>();
                       return fmt::format("{}({})", name, self.cast<const T &>().value());
                   });

    if constexpr (std::is_integral_v<value_type>) {
        cls.def("__int__", [](const T &self) { return static_cast<py::int_>(self.value()); });
    }
    if constexpr (std::is_floating_point_v<value_type>) {
        cls.def("__float__", [](const T &self) { return static_cast<py::float_>(self.value()); });
    }
    return cls;
}

template <typename T, typename... Args>
static void array_tag(py::module_ &m, const char *name, Args &&...args)
{
    auto cls = py::class_<T>(m, name, std::forward<Args>(args)...);
    using value_type = T::value_type;
    using storage_type = T::storage_type;
    auto norm_index = [](py::ssize_t i, py::ssize_t n) -> py::ssize_t {
        if (i < 0) {
            i += n;
        }
        if (i < 0 || i >= n) {
            throw py::index_error("index out of range");
        }
        return i;
    };

    // --- Constructors ---
    cls.def(py::init<>())
        // From any iterable: ArrayTag([1,2,3])
        .def(py::init([](const py::iterable &it) {
                 storage_type v;
                 for (py::handle h : it) {
                     v.push_back(py::cast<value_type>(h));
                 }
                 return T(std::move(v));
             }),
             py::arg("iterable"));

    // Byte-array friendly ctor from Python buffer (bytes/bytearray/memoryview/numpy)
    if constexpr (std::is_same_v<value_type, std::int8_t> || std::is_same_v<value_type, std::uint8_t>) {
        cls.def(py::init([](py::buffer b) {
                    py::buffer_info info = b.request();
                    if (info.ndim != 1) {
                        throw py::value_error("buffer must be 1-D");
                    }
                    // Accept any 1-byte itemsize buffer; reinterpret as bytes
                    if (info.itemsize != 1) {
                        throw py::value_error("buffer itemsize must be 1");
                    }
                    storage_type v(static_cast<size_t>(info.size));
                    std::memcpy(v.data(), info.ptr, v.size());
                    return T(std::move(v));
                }),
                py::arg("buffer"));
    }

    // --- Basic container methods ---
    cls.def("clear", &T::clear)
        .def("append", &T::push_back, py::arg("value"))
        .def(
            "extend",
            [](T &self, const py::iterable &it) {
                for (py::handle h : it) {
                    self.push_back(py::cast<value_type>(h));
                }
            },
            py::arg("iterable"));

    // --- Python sequence protocol ---
    cls.def("__len__", [](const T &self) { return self.size(); })
        .def("__getitem__",
             [=](const T &self, py::ssize_t i) -> value_type {
                 auto n = static_cast<py::ssize_t>(self.size());
                 return self[static_cast<size_t>(norm_index(i, n))];
             })
        .def("__setitem__",
             [=](T &self, py::ssize_t i, const value_type &val) {
                 auto n = static_cast<py::ssize_t>(self.size());
                 self[static_cast<size_t>(norm_index(i, n))] = val;
             })
        .def("__iter__", [](T &self) { return py::make_iterator(self.begin(), self.end()); }, py::keep_alive<0, 1>());

    // --- Comparisons via operators.h ---
    cls.def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self == storage_type())
        .def(py::self != storage_type())
        .def(storage_type() == py::self)
        .def(storage_type() != py::self);

    // --- __repr__ ---
    cls.def("__str__", [](const T &self) { return fmt::format("{}", self); });
    cls.def("__repr__", [](const py::object &self) {
        auto name = py::type::of(self).attr("__name__").cast<std::string>();
        return fmt::format("{}({})", name, self.cast<const T &>());
    });

    // --- Byte-array niceties: bytes() and buffer protocol ---
    if constexpr (std::is_same_v<value_type, std::int8_t> || std::is_same_v<value_type, std::uint8_t>) {
        cls.def("__bytes__", [](const T &self) {
            const auto *ptr = reinterpret_cast<const char *>(self.data());
            return py::bytes(ptr, self.size());
        });
        cls.def_buffer([](T &self) -> py::buffer_info {
            return py::buffer_info(self.data(),                                    // ptr
                                   sizeof(value_type),                             // itemsize
                                   py::format_descriptor<value_type>::format(),    // format
                                   1,                                              // ndim
                                   {static_cast<py::ssize_t>(self.size())},        // shape
                                   {static_cast<py::ssize_t>(sizeof(value_type))}  // strides
            );
        });
    }
}

void init_nbt(py::module_ &m)
{
    value_tag<ByteTag>(m, "ByteTag");
    value_tag<ShortTag>(m, "ShortTag");
    value_tag<IntTag>(m, "IntTag");
    value_tag<LongTag>(m, "LongTag");
    value_tag<FloatTag>(m, "FloatTag");
    value_tag<DoubleTag>(m, "DoubleTag");
    array_tag<ByteArrayTag>(m, "ByteArrayTag", py::buffer_protocol());
    value_tag<StringTag>(m, "StringTag");
    array_tag<IntArrayTag>(m, "IntArrayTag");
}
}  // namespace endstone::python
