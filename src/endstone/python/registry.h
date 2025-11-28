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

#include <memory>

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "endstone/registry.h"

namespace py = pybind11;

namespace endstone::python {
using RegistryTypes = type_list<Enchantment, ItemType>;

template <typename List>
struct registry_getter;

template <typename... Ts>
struct registry_getter<type_list<Ts...>> {
    static py::object get(Server &self, const py::type &t)
    {
        py::object result = py::none();
        bool matched = false;

        (
            [&] {
                if (!matched && t.is(py::type::of<Ts>())) {
                    matched = true;
                    result =
                        py::cast(&self.getRegistry<Ts>(), py::return_value_policy::reference_internal, py::cast(self));
                }
            }(),
            ...);

        return result;
    }
};

template <typename... Ts>
py::object get_registry(const Server &self, const py::type &t, type_list<Ts...>)
{
    py::object result = py::none();
    bool matched = false;
    (
        [&] {
            if (!matched && t.is(py::type::of<Ts>())) {
                matched = true;
                result = py::cast(&self.getRegistry<Ts>(), py::return_value_policy::reference);
            }
        }(),
        ...);
    return result;
}

template <typename T, typename... Args>
void bind_registry(py::module &m, Args &&...args)
{
    py::class_<Registry<T>>(m, (std::string(T::RegistryType) + "Registry").c_str(), std::forward<Args>(args)...)
        .def("get", py::overload_cast<Identifier<T>>(&Registry<T>::get, py::const_), py::arg("id"),
             py::return_value_policy::reference, "Get the object by its identifier.")
        .def(
            "get_or_throw",
            [](const Registry<T> &self, const Identifier<T> id) -> const T & {
                if (auto *p = self.get(id)) {
                    return *p;
                }
                throw py::key_error(fmt::format("No registry entry found for identifier: {}", id));
            },
            py::arg("id"), py::return_value_policy::reference, "Get the object by its identifier or throw if missing.")
        .def(
            "__getitem__",
            [](const Registry<T> &self, const Identifier<T> id) -> const T & {
                if (auto *p = self.get(id)) {
                    return *p;
                }
                throw py::key_error(fmt::format("No registry entry found for identifier: {}", id));
            },
            py::arg("id"), py::return_value_policy::reference)
        .def(
            "__iter__",
            [](const Registry<T> &self) {
                py::list items;
                self.forEach([&items](const T &elem) {
                    items.append(elem);
                    return true;  // continue iteration
                });
                return items;
            },
            py::return_value_policy::reference_internal)
        .def(
            "__contains__", [](const Registry<T> &self, const Identifier<T> id) { return self.get(id) != nullptr; },
            py::arg("id"));
}

template <typename... Ts>
void bind_registries(py::module_ &m, type_list<Ts...>)
{
    (bind_registry<Ts>(m), ...);
}
};  // namespace endstone::python
