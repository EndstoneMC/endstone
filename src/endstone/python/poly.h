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

namespace detail {

/**
 * @brief Hierarchical polymorphic dispatch for pybind11.
 *
 * Each hook lists only its direct children. If a child type has its own
 * poly_dispatch hook, dispatch is delegated to it automatically.
 * Children should be listed most-derived first within the same level.
 */
template <typename Base, typename... Children>
struct poly_dispatch {
    // Tag so we can detect our own hooks vs pybind11 defaults
    using is_poly_dispatch = std::true_type;

    static const void *get(const Base *src, const std::type_info *&type)
    {
        if (!src) {
            return src;
        }
        const void *result = nullptr;
        (try_dispatch<Children>(src, type, result) || ...);
        if (!result) {
            type = &typeid(Base);
            result = src;
        }
        return result;
    }

private:
    template <typename T>
    static bool try_dispatch(const Base *src, const std::type_info *&type, const void *&result)
    {
        auto *child = src->template as<T>();
        if (!child) {
            return false;
        }
        if constexpr (requires { typename polymorphic_type_hook<T>::is_poly_dispatch; }) {
            result = polymorphic_type_hook<T>::get(child, type);
        }
        else {
            type = &typeid(T);
            result = child;
        }
        return true;
    }
};

}  // namespace detail

// Each hook lists only DIRECT children — delegation handles deeper dispatch.

// ---- Permissible / CommandSender / Actor hierarchy ----

template <>
struct polymorphic_type_hook<endstone::Mob> : detail::poly_dispatch<endstone::Mob, endstone::Player> {};

template <>
struct polymorphic_type_hook<endstone::Actor> : detail::poly_dispatch<endstone::Actor, endstone::Mob, endstone::Item> {};

template <>
struct polymorphic_type_hook<endstone::CommandSender>
    : detail::poly_dispatch<endstone::CommandSender, endstone::Actor, endstone::ConsoleCommandSender,
                            endstone::BlockCommandSender> {};

template <>
struct polymorphic_type_hook<endstone::Permissible>
    : detail::poly_dispatch<endstone::Permissible, endstone::CommandSender> {};

// ---- ItemMeta hierarchy ----

template <>
struct polymorphic_type_hook<endstone::WritableBookMeta>
    : detail::poly_dispatch<endstone::WritableBookMeta, endstone::BookMeta> {};

template <>
struct polymorphic_type_hook<endstone::ItemMeta>
    : detail::poly_dispatch<endstone::ItemMeta, endstone::WritableBookMeta, endstone::MapMeta,
                            endstone::CrossbowMeta> {};

}  // namespace PYBIND11_NAMESPACE
