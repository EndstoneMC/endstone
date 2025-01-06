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

#include <functional>
#include <memory>
#include <optional>
#include <system_error>

#include "cast.h"

namespace endstone::hook {
void install();
const std::error_category &error_category();
}  // namespace endstone::hook

namespace endstone::hook {

void *get_original(void *detour);
void *get_original(const std::string &name);

const std::unordered_map<std::string, void *> &get_targets();
const std::unordered_map<std::string, void *> &get_detours();

}  // namespace endstone::hook

namespace endstone::hook {
/**
 * @brief Gets the original function pointer from a detour function pointer
 */
template <typename Return, typename... Arg>
Return (*get_original(Return (*fp)(Arg...), std::optional<std::string> name = std::nullopt))(Arg...)
{
    auto *original = name.has_value() ? get_original(name.value()) : get_original(fp_cast(fp));
    return *reinterpret_cast<decltype(&fp)>(&original);
}

/**
 * @brief Gets the original member function pointer from a detour member function pointer (non-const, no ref-qualifier)
 */
template <typename Return, typename Class, typename... Arg>
Return (Class::*get_original(Return (Class::*fp)(Arg...), std::optional<std::string> name = std::nullopt))(Arg...)
{
    void *original = name.has_value() ? get_original(name.value()) : get_original(fp_cast(fp));
    struct {  // https://doi.org/10.1145/3660779
        void *ptr;
        std::size_t adj = 0;
    } temp;
    temp.ptr = original;
    return *reinterpret_cast<decltype(&fp)>(&temp);
}

/**
 * @brief Gets the original member function pointer from a detour member function pointer (const, no ref-qualifier)
 */
template <typename Return, typename Class, typename... Arg>
Return (Class::*get_original(Return (Class::*fp)(Arg...) const,
                             std::optional<std::string> name = std::nullopt))(Arg...) const
{
    void *original = name.has_value() ? get_original(name.value()) : get_original(fp_cast(fp));
    struct {  // https://doi.org/10.1145/3660779
        void *ptr;
        std::size_t adj = 0;
    } temp;
    temp.ptr = original;
    return *reinterpret_cast<decltype(&fp)>(&temp);
}
}  // namespace endstone::hook

#define ENDSTONE_HOOK_CALL_ORIGINAL(fp, ...)                                                                  \
    std::invoke(endstone::runtime::fp_cast(fp, endstone::hook::get_original(endstone::runtime::fp_cast(fp))), \
                ##__VA_ARGS__)
#define ENDSTONE_HOOK_CALL_ORIGINAL_NAME(fp, name, ...) \
    std::invoke(endstone::runtime::fp_cast(fp, endstone::hook::get_original(name)), ##__VA_ARGS__)
