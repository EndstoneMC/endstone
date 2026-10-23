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

#include <cstddef>
#include <iterator>
#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

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
PYBIND11_MAKE_OPAQUE(std::unordered_set<endstone::NotNull<endstone::Player>>)
PYBIND11_MAKE_OPAQUE(std::unordered_set<endstone::NotNull<endstone::CommandSender>>)

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
template <>
struct py_class<ProxiedCommandSender> {
    using type = py::classh<ProxiedCommandSender, CommandSender>;
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

template <typename Set>
class SetIterator {
public:
    using value_type = typename Set::value_type;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type *;
    using reference = const value_type &;
    using iterator_category = std::forward_iterator_tag;

    SetIterator(const Set &set, std::shared_ptr<const std::vector<value_type>> items, std::size_t index)
        : set_(&set), items_(std::move(items)), size_(set.size()), index_(index)
    {
    }

    reference operator*() const
    {
        check();
        return (*items_)[index_];
    }

    SetIterator &operator++()
    {
        check();
        ++index_;
        return *this;
    }

    bool operator==(const SetIterator &other) const { return index_ == other.index_; }

private:
    void check() const
    {
        if (set_->size() != size_) {
            throw std::runtime_error("Set changed size during iteration");
        }
    }

    const Set *set_;
    std::shared_ptr<const std::vector<value_type>> items_;
    std::size_t size_;
    std::size_t index_;
};

template <typename T>
std::optional<T> try_load(const py::handle &value)
{
    if (value.is_none()) {
        return std::nullopt;
    }
    py::detail::make_caster<T> caster;
    if (!caster.load(value, true)) {
        return std::nullopt;
    }
    return py::detail::cast_op<T>(std::move(caster));
}

template <typename Set>
py::class_<Set> bind_set(py::handle scope, const char *name, const char *doc)
{
    using T = typename Set::value_type;
    const auto snapshot = [](const py::handle &value) {
        return py::set(py::reinterpret_borrow<py::object>(value));
    };
    auto cls = py::class_<Set>(scope, name, doc);
    cls.def("__len__", [](const Set &self) { return self.size(); })
        .def(
            "__iter__",
            [](const Set &self) {
                auto items = std::make_shared<const std::vector<T>>(self.begin(), self.end());
                const auto size = items->size();
                return py::make_iterator(SetIterator<Set>(self, items, 0), SetIterator<Set>(self, items, size));
            },
            py::keep_alive<0, 1>())
        .def("__contains__",
             [](const Set &self, const py::object &value) {
                 const auto item = try_load<T>(value);
                 return item.has_value() && self.contains(*item);
             })
        .def(
            "add", [](Set &self, const T &value) { self.insert(value); }, py::arg("value"))
        .def(
            "discard",
            [](Set &self, const py::object &value) {
                if (const auto item = try_load<T>(value)) {
                    self.erase(*item);
                }
            },
            py::arg("value"))
        .def(
            "remove",
            [](Set &self, const py::object &value) {
                const auto item = try_load<T>(value);
                if (!item.has_value() || self.erase(*item) == 0) {
                    throw py::key_error(py::repr(value));
                }
            },
            py::arg("value"))
        .def("clear", [](Set &self) { self.clear(); })
        .def("copy", [snapshot](const py::object &self) { return snapshot(self); })
        .def("union", [snapshot](const py::object &self,
                                 const py::args &others) { return snapshot(self).attr("union")(*others); })
        .def("intersection",
             [snapshot](const py::object &self, const py::args &others) {
                 return snapshot(self).attr("intersection")(*others);
             })
        .def("difference", [snapshot](const py::object &self,
                                      const py::args &others) { return snapshot(self).attr("difference")(*others); })
        .def(
            "symmetric_difference",
            [snapshot](const py::object &self, const py::object &other) {
                return snapshot(self).attr("symmetric_difference")(other);
            },
            py::arg("other"))
        .def(
            "issubset",
            [snapshot](const py::object &self, const py::object &other) {
                return snapshot(self).attr("issubset")(other);
            },
            py::arg("other"))
        .def(
            "issuperset",
            [snapshot](const py::object &self, const py::object &other) {
                return snapshot(self).attr("issuperset")(other);
            },
            py::arg("other"))
        .def("update",
             [snapshot](Set &self, const py::args &others) {
                 for (const auto &other : others) {
                     for (const auto &value : snapshot(other)) {
                         self.insert(py::cast<T>(value));
                     }
                 }
             })
        .def("difference_update",
             [snapshot](Set &self, const py::args &others) {
                 for (const auto &other : others) {
                     for (const auto &value : snapshot(other)) {
                         if (const auto item = try_load<T>(value)) {
                             self.erase(*item);
                         }
                     }
                 }
             })
        .def("intersection_update",
             [snapshot](const py::object &self, const py::args &others) {
                 const py::set keep = snapshot(self).attr("intersection")(*others);
                 auto &set = py::cast<Set &>(self);
                 for (const auto &value : snapshot(self)) {
                     if (!keep.contains(value)) {
                         set.erase(py::cast<T>(value));
                     }
                 }
             })
        .def(
            "symmetric_difference_update",
            [snapshot](Set &self, const py::object &other) {
                for (const auto &value : snapshot(other)) {
                    const auto item = py::cast<T>(value);
                    if (self.erase(item) == 0) {
                        self.insert(item);
                    }
                }
            },
            py::arg("other"));
    const auto mutable_set = py::module_::import("collections.abc").attr("MutableSet");
    for (const auto *mixin :
         {"__eq__", "__lt__", "__le__", "__gt__", "__ge__", "__and__", "__rand__", "__or__", "__ror__", "__sub__",
          "__rsub__", "__xor__", "__rxor__", "__iand__", "__ior__", "__isub__", "__ixor__", "isdisjoint", "pop"}) {
        cls.attr(mixin) = mutable_set.attr(mixin);
    }
    const auto builtins = py::module_::import("builtins");
    cls.attr("__hash__") = py::none();
    cls.attr("_from_iterable") = builtins.attr("staticmethod")(builtins.attr("set"));
    mutable_set.attr("register")(cls);
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
