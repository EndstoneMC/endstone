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

#include <algorithm>
#include <array>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <utility>

#include <entt/entt.hpp>
#include <fmt/format.h>

#include "endstone/detail/cast.h"
#include "endstone/detail/platform.h"

namespace endstone::detail {
#include "bedrock_symbols.generated.h"

consteval std::size_t get_symbol(const std::string_view symbol)
{
    for (const auto &[key, value] : symbols) {
        if (key == symbol) {
            return value;
        }
    }
    throw "symbol not found";
}

template <typename Func>
constexpr void foreach_symbol(Func &&func)
{
    for (const auto &[key, value] : symbols) {
        std::invoke(std::forward<Func>(func), key, value);
    }
}

template <typename Func, typename... Args>
constexpr decltype(auto) invoke(const char *function, Func &&func, Args &&...args)
{
    (void)function;
    return std::invoke(func, std::forward<Args>(args)...);
}
}  // namespace endstone::detail

#define BEDROCK_CALL(fp, ...)                                                                        \
    endstone::detail::invoke(                                                                        \
        __FUNCDNAME__,                                                                               \
        endstone::detail::fp_cast(fp, static_cast<char *>(endstone::detail::get_executable_base()) + \
                                          endstone::detail::get_symbol(__FUNCDNAME__)),              \
        ##__VA_ARGS__)

#define BEDROCK_VAR(type, name)                                                           \
    reinterpret_cast<type>(static_cast<char *>(endstone::detail::get_executable_base()) + \
                           endstone::detail::get_symbol(name));

namespace endstone::detail {
#ifdef _WIN32
template <std::size_t Offset, typename Class, typename... Args>
Class *(*get_ctor())(Class *, Args...)
{
    auto *addr = static_cast<char *>(get_executable_base()) + Offset;
    return reinterpret_cast<Class *(*)(Class *, Args...)>(addr);
}
#elif __linux__
template <std::size_t Offset, typename Class, typename... Args>
void (*get_ctor())(Class *, Args...)
{
    auto *addr = static_cast<char *>(get_executable_base()) + Offset;
    return reinterpret_cast<void (*)(Class *, Args...)>(addr);
}
#endif
}  // namespace endstone::detail

#define BEDROCK_CTOR(type, ...) \
    endstone::detail::get_ctor<endstone::detail::get_symbol(__FUNCDNAME__), type, ##__VA_ARGS__>()
