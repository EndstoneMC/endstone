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

#include "endstone/detail/cast.h"
#include "endstone/detail/platform.h"

namespace endstone::detail {
#include "bedrock_symbols.generated.h"

constexpr bool has_symbol(const std::string_view name)
{
    return std::ranges::find_if(symbols, [&name](const auto &pair) { return pair.first == name; }) != symbols.end();
}

inline void *get_symbol_addr(std::string_view name)
{
    static auto *executable_base = endstone::detail::get_executable_base();
    auto it = std::ranges::find_if(symbols, [&name](const auto &pair) { return pair.first == name; });
    if (it == symbols.end()) {
        throw std::out_of_range("symbol not found");
    }
    return static_cast<char *>(executable_base) + it->second;
}

template <typename Func>
constexpr void foreach_symbol(Func &&func)
{
    for (const auto &[key, value] : symbols) {
        std::invoke(std::forward<Func>(func), key, value);
    }
}
}  // namespace endstone::detail

#define ENDSTONE_SYMCALL(fp, ...)                                                                                     \
    static_assert(endstone::detail::has_symbol(__FUNCDNAME__), "undefined symbol " __FUNCDNAME__ " in " __FUNCSIG__); \
    return std::invoke(endstone::detail::fp_cast(fp, endstone::detail::get_symbol_addr(__FUNCDNAME__)), ##__VA_ARGS__)

namespace endstone::detail {
#ifdef _WIN32
template <typename Class, typename... Args>
Class *(*get_ctor(std::string_view name))(Class *, Args...)
{
    auto *original = get_symbol_addr(name);
    return reinterpret_cast<Class *(*)(Class *, Args...)>(original);
}
#elif __linux__
template <typename Class, typename... Args>
void (*get_ctor(std::string_view name))(Class *, Args...)
{
    auto *original = get_symbol_addr(name);
    return reinterpret_cast<void (*)(Class *, Args...)>(original);
}
#endif
}  // namespace endstone::detail

#define BEDROCK_CTOR(type, ...)                                                                                       \
    static_assert(endstone::detail::has_symbol(__FUNCDNAME__), "undefined symbol " __FUNCDNAME__ " in " __FUNCSIG__); \
    endstone::detail::get_ctor<type, ##__VA_ARGS__>(__FUNCDNAME__)
