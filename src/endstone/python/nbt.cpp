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
                       auto name = py::str(py::type::of(self).attr("__name__"));
                       return fmt::format("{}({})", name.cast<std::string>(), self.cast<const T &>().value());
                   });

    if constexpr (std::is_integral_v<value_type>) {
        cls.def("__int__", [](const T &self) { return static_cast<py::int_>(self.value()); });
    }
    if constexpr (std::is_floating_point_v<value_type>) {
        cls.def("__float__", [](const T &self) { return static_cast<py::float_>(self.value()); });
    }
    return cls;
}

void init_nbt(py::module_ &m)
{
    auto tag = py::class_<nbt::Tag>(m, "Tag");
    py::native_enum<nbt::Tag::Type>(tag, "Type", "enum.IntEnum")
        .value("END", nbt::Tag::Type::End)
        .value("BYTE", nbt::Tag::Type::Byte)
        .value("SHORT", nbt::Tag::Type::Short)
        .value("INT", nbt::Tag::Type::Int)
        .value("LONG", nbt::Tag::Type::Long)
        .value("FLOAT", nbt::Tag::Type::Float)
        .value("DOUBLE", nbt::Tag::Type::Double)
        .value("BYTE_ARRAY", nbt::Tag::Type::ByteArray)
        .value("STRING", nbt::Tag::Type::String)
        .value("LIST", nbt::Tag::Type::List)
        .value("COMPOUND", nbt::Tag::Type::Compound)
        .value("INT_ARRAY", nbt::Tag::Type::IntArray)
        .export_values()
        .finalize();

    value_tag<ByteTag>(m, "ByteTag");
    value_tag<ShortTag>(m, "ShortTag");
    value_tag<IntTag>(m, "IntTag");
    value_tag<LongTag>(m, "LongTag");
    value_tag<FloatTag>(m, "FloatTag");
    value_tag<DoubleTag>(m, "DoubleTag");
    value_tag<StringTag>(m, "StringTag");
}
}  // namespace endstone::python
