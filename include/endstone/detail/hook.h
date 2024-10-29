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

}  // namespace endstone::detail::hook
#define ENDSTONE_HOOK_CALL_ORIGINAL(fp, ...) std::invoke(endstone::detail::hook::get_original(fp), ##__VA_ARGS__)
#define ENDSTONE_HOOK_CALL_ORIGINAL_NAME(fp, name, ...) \
    std::invoke(endstone::detail::hook::get_original(fp, name), ##__VA_ARGS__)

namespace endstone::detail::hook {
#ifdef _WIN32
template <typename Class, typename... Args>
Class *(*get_ctor(std::unique_ptr<Class> (*)(Args...), const std::string &name))(Class *, Args...)
{
    auto *original = get_original(name);
    return reinterpret_cast<Class *(*)(Class *, Args...)>(original);
}
#elif __linux__
template <typename Class, typename... Args>
void (*get_ctor(std::unique_ptr<Class> (*)(Args...), const std::string &name))(Class *, Args...)
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

#define ENDSTONE_FACTORY_IMPLEMENT(type, ...) ENDSTONE_FACTORY_IMPLEMENT_OVERLOAD(type, &type::create, ##__VA_ARGS__)

#define ENDSTONE_FACTORY_IMPLEMENT_OVERLOAD(type, fp, ...)                             \
    {                                                                                  \
        static std::string func_decorated_name = __FUNCDNAME__;                        \
        static std::string ctor_decorated_name = []() {                                \
            std::string prefix_target = ENDSTONE_FACTORY_PREFIX_TARGET(type);          \
            std::string name = ENDSTONE_FACTORY_PREFIX_REPLACEMENT(type);              \
            std::size_t pos = func_decorated_name.find(prefix_target);                 \
            pos += std::strlen(prefix_target.c_str());                                 \
            name += func_decorated_name.substr(pos);                                   \
            pos = name.find("BV1@@Z");                                                 \
            if (pos != std::string::npos) {                                            \
                name.replace(pos, 6, "BV0@@Z");                                        \
            }                                                                          \
            return name;                                                               \
        }();                                                                           \
        auto *obj = reinterpret_cast<type *>(new char[sizeof(type)]);                  \
        endstone::detail::hook::get_ctor(fp, ctor_decorated_name)(obj, ##__VA_ARGS__); \
        return std::unique_ptr<type>(obj);                                             \
    }
