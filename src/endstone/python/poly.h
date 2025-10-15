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

#include <pybind11/pybind11.h>

namespace PYBIND11_NAMESPACE {

// This specialization is used to allow a class to specify a holder for its base class.
// By default, py::class_<Derived, Base, std::unique_ptr<Base>> is not permitted in pybind11.
// In scenarios involving virtual inheritance, when a Base pointer is down-cast to a Derived pointer
// using dynamic_cast, pybind11 will incorrectly attempt to deallocate the pointer using std::unique_ptr<Derived>.
// This behavior is invalid and can cause runtime crashes.
// To handle this properly, we explicitly specify std::unique_ptr<Base> as the holder for a Derived class
// that inherits virtually from Base, e.g., class Derived : public virtual Base {}.
namespace detail {
template <typename type, typename base, typename deleter>
struct is_holder_type<type, std::unique_ptr<base, deleter>>
    : std::integral_constant<bool, std::is_base_of_v<base, type>> {};
}  // namespace detail

template <>
struct polymorphic_type_hook<endstone::Mob> {
    static const void *get(const endstone::Mob *src, const std::type_info *&type)
    {
        if (!src) {
            return src;
        }
        if (const auto *player = src->asPlayer()) {
            type = &typeid(endstone::Player);
            return player;
        }
        type = &typeid(endstone::Mob);
        return src;
    }
};

template <>
struct polymorphic_type_hook<endstone::Actor> {
    static const void *get(const endstone::Actor *src, const std::type_info *&type)
    {
        if (!src) {
            return src;
        }
        if (const auto *mob = src->asMob()) {
            return polymorphic_type_hook<endstone::Mob>::get(mob, type);
        }
        if (const auto *item = src->asItem()) {
            type = &typeid(endstone::Item);
            return item;
        }
        type = &typeid(endstone::Actor);
        return src;
    }
};

template <>
struct polymorphic_type_hook<endstone::CommandSender> {
    static const void *get(const endstone::CommandSender *src, const std::type_info *&type)
    {
        if (!src) {
            return src;
        }
        if (const auto *actor = src->asActor()) {
            return polymorphic_type_hook<endstone::Actor>::get(actor, type);
        }
        if (const auto *console = src->asConsole()) {
            type = &typeid(endstone::ConsoleCommandSender);
            return console;
        }
        if (const auto *block = src->asBlock()) {
            type = &typeid(endstone::BlockCommandSender);
            return block;
        }
        type = &typeid(endstone::CommandSender);
        return src;
    }
};

template <>
struct polymorphic_type_hook<endstone::Permissible> {
    static const void *get(const endstone::Permissible *src, const std::type_info *&type)
    {
        if (!src) {
            return src;
        }
        if (const auto *command_sender = src->asCommandSender()) {
            return polymorphic_type_hook<endstone::CommandSender>::get(command_sender, type);
        }
        type = &typeid(endstone::Permissible);
        return src;
    }
};

template <>
struct polymorphic_type_hook<endstone::ItemMeta> {
    static const void *get(const endstone::ItemMeta *src, const std::type_info *&type)
    {
        if (!src) {
            return src;
        }

        switch (src->getType()) {
        case endstone::ItemMeta::Type::Map:
            type = &typeid(endstone::MapMeta);
            return static_cast<const endstone::MapMeta *>(src);
        default:
            type = &typeid(endstone::ItemMeta);
            return src;
        }
    }
};
}  // namespace PYBIND11_NAMESPACE
