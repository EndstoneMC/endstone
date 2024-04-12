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
#include <optional>
#include <system_error>

#include "endstone/detail/cast.h"

namespace endstone::detail::hook {
void install();
const std::error_category &hook_error_category();
}  // namespace endstone::detail::hook

namespace endstone::detail::hook {

void *get_original(void *detour);
void *get_original(const std::string &name);

const std::unordered_map<std::string, void *> &get_targets();
const std::unordered_map<std::string, void *> &get_detours();

}  // namespace endstone::detail::hook

namespace endstone::detail::hook {
/**
 * @brief Construct a std::function from a function pointer
 */
template <typename Return, typename... Arg>
inline std::function<Return(Arg...)> get_original(Return (*fp)(Arg...))
{
    return reinterpret_cast<Return (*)(Arg...)>(get_original(fp_cast(fp)));
}

/**
 * @brief Construct a std::function from a class method (non-const, no ref-qualifier)
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return(Class *, Arg...)> get_original(Return (Class::*fp)(Arg...),
                                                           std::optional<std::string> name = std::nullopt)
{
    auto *original = name.has_value() ? get_original(name.value()) : get_original(fp_cast(fp));
    auto func = reinterpret_cast<Return (*)(Class *, Arg...)>(original);
    return [func](Class *obj, Arg... args) -> Return {
        return func(obj, std::forward<Arg>(args)...);
    };
}

/**
 * @brief Construct a std::function from a class method (const, no ref-qualifier)
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return(const Class *, Arg...)> get_original(Return (Class::*fp)(Arg...) const,
                                                                 std::optional<std::string> name = std::nullopt)
{
    auto *original = name.has_value() ? get_original(name.value()) : get_original(fp_cast(fp));
    auto func = reinterpret_cast<Return (*)(const Class *, Arg...)>(original);
    return [func](const Class *obj, Arg... args) -> Return {
        return func(obj, std::forward<Arg>(args)...);
    };
}
}  // namespace endstone::detail::hook
#define ENDSTONE_HOOK_CALL_ORIGINAL(fp, ...)            endstone::detail::hook::get_original(fp)(__VA_ARGS__)
#define ENDSTONE_HOOK_CALL_ORIGINAL_NAME(fp, name, ...) endstone::detail::hook::get_original(fp, name)(__VA_ARGS__)

namespace endstone::detail::hook {
/**
 * @brief Construct a std::function from a function pointer
 * with Return Value Optimization (RVO).
 */
template <typename Return, typename... Arg>
inline std::function<Return *(Return *, Arg...)> get_original_rvo(Return (*fp)(Arg...),
                                                                  std::optional<std::string> name = std::nullopt)
{
    auto *original = name.has_value() ? get_original(name.value()) : get_original(fp_cast(fp));
    return reinterpret_cast<Return *(*)(Return *, Arg...)>(original);
}

/**
 * @brief Construct a std::function from a class method (non-const, no ref-qualifier)
 * with Return Value Optimization (RVO).
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return *(Return *, Class *, Arg...)> get_original_rvo(
    Return (Class::*fp)(Arg...), std::optional<std::string> name = std::nullopt)
{
    auto *original = name.has_value() ? get_original(name.value()) : get_original(fp_cast(fp));
#ifdef _WIN32
    auto func = reinterpret_cast<Return *(*)(Class *, Return *, Arg...)>(original);
    return [func](Return *ret, Class *obj, Arg... args) -> Return * {
        return func(obj, ret, std::forward<Arg>(args)...);
    };
#elif __linux__
    auto func = reinterpret_cast<Return *(*)(Return *, Class *, Arg...)>(original);
    return [func](Return *ret, Class *obj, Arg... args) -> Return * {
        return func(ret, obj, std::forward<Arg>(args)...);
    };
#endif
}

/**
 * @brief Construct a std::function from a class method (const, no ref-qualifier)
 * with Return Value Optimization (RVO).
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return *(Return *, const Class *, Arg...)> get_original_rvo(
    Return (Class::*fp)(Arg...) const, std::optional<std::string> name = std::nullopt)
{
    auto *original = name.has_value() ? get_original(name.value()) : get_original(fp_cast(fp));
#ifdef _WIN32
    auto func = reinterpret_cast<Return *(*)(const Class *, Return *, Arg...)>(original);
    return [func](Return *ret, const Class *obj, Arg... args) -> Return * {
        return func(obj, ret, std::forward<Arg>(args)...);
    };
#elif __linux__
    auto func = reinterpret_cast<Return *(*)(Return *, const Class *, Arg...)>(original);
    return [func](Return *ret, const Class *obj, Arg... args) -> Return * {
        return func(ret, obj, std::forward<Arg>(args)...);
    };
#endif
}
}  // namespace endstone::detail::hook

#define ENDSTONE_HOOK_CALL_ORIGINAL_RVO(fp, ret, ...) *endstone::detail::hook::get_original_rvo(fp)(&ret, __VA_ARGS__)
#define ENDSTONE_HOOK_CALL_ORIGINAL_RVO_NAME(fp, name, ret, ...) \
    *endstone::detail::hook::get_original_rvo(fp, name)(&ret, __VA_ARGS__)
