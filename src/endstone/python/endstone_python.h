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
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>
#include <pybind11/stl_bind.h>

#include "endstone/endstone.hpp"
#include "poly.h"
#include "type_caster.h"

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
}  // namespace endstone::python

template <typename... Ts>
struct type_list {};
