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

#pragma once

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/native_enum.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>
#include <pybind11/stl_bind.h>

#include "endstone/endstone.hpp"
#include "endstone/identifier.h"
#include "poly.h"
#include "type_caster.h"

PYBIND11_MAKE_OPAQUE(std::vector<endstone::NotNull<endstone::Block>>)

namespace py = pybind11;

namespace endstone::python {
namespace detail {
template <typename T>
struct py_class;
template <>
struct py_class<Permissible> {
    using type = py::classh<Permissible>;
};
template <>
struct py_class<CommandSender> {
    using type = py::classh<CommandSender, Permissible>;
};
template <>
struct py_class<Actor> {
    using type = py::classh<Actor, CommandSender>;
};
template <>
struct py_class<Mob> {
    using type = py::classh<Mob, Actor>;
};
template <>
struct py_class<Player> {
    using type = py::classh<Player, Mob>;
};
template <>
struct py_class<Item> {
    using type = py::classh<Item, Actor>;
};
template <>
struct py_class<BlockCommandSender> {
    using type = py::classh<BlockCommandSender, CommandSender>;
};
template <>
struct py_class<ConsoleCommandSender> {
    using type = py::classh<ConsoleCommandSender, CommandSender>;
};
template <>
struct py_class<CommandSenderWrapper> {
    using type = py::classh<CommandSenderWrapper, CommandSender>;
};

}  // namespace detail

template <typename T>
using py_class = typename detail::py_class<T>::type;

// def_property_readonly_static getters that ignore the class and yield a fixed value.
template <typename T>
auto constant(T value)
{
    return [value = std::move(value)](const py::object &) {
        return value;
    };
}

// Every Registry<T>::Type carries the same identity surface. Bind it in one place so a new registry
// type cannot quietly ship without it: comparing and hashing against the plain id string is what lets
// a plugin write `actor.type == "minecraft:zombie"` or key a dict on either form.
template <typename Class>
Class def_registry_type(Class cls)
{
    using T = typename Class::type;
    auto name = cls.attr("__name__").template cast<std::string>();
    cls.def("__str__", [](const T &self) { return std::string(self.getId()); })
        .def("__repr__", [name = std::move(name)](const T &self) { return std::format("{}({})", name, self.getId()); })
        .def("__hash__", [](const T &self) { return py::hash(py::str(std::string(self.getId()))); })
        .def(py::self == py::self)  // NOLINT(misc-redundant-expression)
        .def(py::self != py::self)  // NOLINT(misc-redundant-expression)
        .def(py::self == std::string_view())
        .def(py::self != std::string_view());
    return cls;
}

// The base is taken by value, so a derived id such as GameRuleId<T> slices to the type the caster binds.
template <typename T>
auto id(Identifier<T> value)
{
    return [value = std::move(value)](const py::object &) {
        return value;
    };
}
}  // namespace endstone::python

template <typename... Ts>
struct type_list {};
