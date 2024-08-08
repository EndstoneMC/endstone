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

#include "endstone/detail/cast.h"
#include "endstone/endstone.h"

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
std::function<Return(Arg...)> get_original(Return (*fp)(Arg...), std::optional<std::string> name = std::nullopt)
{
    auto *original = name.has_value() ? get_original(name.value()) : get_original(fp_cast(fp));
    return reinterpret_cast<Return (*)(Arg...)>(original);
}

/**
 * @brief Construct a std::function from a class method (non-const, no ref-qualifier)
 */
template <typename Return, typename Class, typename... Arg>
std::function<Return(Class *, Arg...)> get_original(Return (Class::*fp)(Arg...),
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
std::function<Return(const Class *, Arg...)> get_original(Return (Class::*fp)(Arg...) const,
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
std::function<Return *(Return *, Arg...)> get_original_rvo(Return (*fp)(Arg...),
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
std::function<Return *(Return *, Class *, Arg...)> get_original_rvo(Return (Class::*fp)(Arg...),
                                                                    std::optional<std::string> name = std::nullopt)
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
std::function<Return *(Return *, const Class *, Arg...)> get_original_rvo(
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

namespace endstone::detail::hook {
#ifdef _WIN32
template <typename Class, typename... Args>
std::function<Class *(Class *, Args...)> get_ctor(std::unique_ptr<Class> (*)(Args...), const std::string &name)
{
    auto *original = get_original(name);
    return reinterpret_cast<Class *(*)(Class *, Args...)>(original);
}
#elif __linux__
template <typename Class, typename... Args>
std::function<void(Class *, Args...)> get_ctor(std::unique_ptr<Class> (*)(Args...), const std::string &name)
{
    auto *original = get_original(name);
    return reinterpret_cast<void (*)(Class *, Args...)>(original);
}
#endif
}  // namespace endstone::detail::hook

#define ENDSTONE_FACTORY_DECLARE(type, ...) static std::unique_ptr<type> create(__VA_ARGS__);
#ifdef _WIN32
#define ENDSTONE_FACTORY_PREFIX_TARGET(type)      ENDSTONE_TOSTRING(type) "@@@std@@@std@"
#define ENDSTONE_FACTORY_PREFIX_REPLACEMENT(type) "??0" ENDSTONE_TOSTRING(type) "@@QEAA"
#elif __linux__
#define ENDSTONE_FACTORY_PREFIX_TARGET(type) ENDSTONE_TOSTRING(type) "6create"
#define ENDSTONE_FACTORY_PREFIX_REPLACEMENT(type) \
    "_ZN" + std::to_string(std::strlen(ENDSTONE_TOSTRING(type))) + ENDSTONE_TOSTRING(type) "C2"
#endif

#define ENDSTONE_FACTORY_IMPLEMENT(type, ...)                                                           \
    {                                                                                                   \
        static std::string func_decorated_name = __FUNCDNAME__;                                         \
        static std::string name =                                                                       \
            ENDSTONE_FACTORY_PREFIX_REPLACEMENT(type) +                                                 \
            func_decorated_name.substr(func_decorated_name.find(ENDSTONE_FACTORY_PREFIX_TARGET(type)) + \
                                       std::strlen(ENDSTONE_FACTORY_PREFIX_TARGET(type)));              \
        auto *obj = reinterpret_cast<type *>(new char[sizeof(type)]);                                   \
        endstone::detail::hook::get_ctor(&type::create, name)(obj, __VA_ARGS__);                        \
        return std::unique_ptr<type>(obj);                                                              \
    }
