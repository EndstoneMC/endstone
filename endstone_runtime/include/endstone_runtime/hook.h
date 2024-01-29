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
#include <system_error>

namespace endstone::hook {
void install();
const std::error_category &hook_error_category() noexcept;
}  // namespace endstone::hook

namespace endstone::hook::detail {

/**
 * @brief Cast a function pointer to void pointer
 *
 * @tparam Return The return type of the function.
 * @tparam Args The argument types of the function.
 *
 * @param fp The function pointer to be casted.
 *
 * @return A void pointer to the function.
 */
template <typename Return, typename... Args>
void *fp_cast(Return (*fp)(Args...))
{
    union {
        Return (*p)(Args...);
        void *v;
    } temp;
    temp.p = fp;
    return temp.v;
}

/**
 * @brief Cast a member function pointer to void pointer.
 *
 * @tparam Return The return type of the member function.
 * @tparam Class The class type that the member function belongs to.
 * @tparam Args The argument types of the member function.
 *
 * @param fp Pointer to the member function to be casted.
 *
 * @return A void pointer to the member function.
 */
template <typename Return, typename Class, typename... Args>
void *fp_cast(Return (Class::*fp)(Args...))
{
    union {
        Return (Class::*p)(Args...);
        void *v;
    } temp;
    temp.p = fp;
    return temp.v;
}

/**
 * @brief Cast a constant member function pointer to void pointer.
 *
 * @tparam Return The return type of the constant member function.
 * @tparam Class The class type that the constant member function belongs to.
 * @tparam Args The argument types of the constant member function.
 *
 * @param fp Pointer to the constant member function to be casted.
 *
 * @return A void pointer to the constant member function.
 */
template <typename Return, typename Class, typename... Args>
void *fp_cast(Return (Class::*fp)(Args...) const)
{
    union {
        Return (Class::*p)(Args...) const;
        void *v;
    } temp;
    temp.p = fp;
    return temp.v;
}
}  // namespace endstone::hook::detail

namespace endstone::hook::detail {
void *get_original(void *detour);
}

namespace endstone::hook::detail {
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
inline std::function<Return(Class *, Arg...)> get_original(Return (Class::*fp)(Arg...))
{
    auto func = reinterpret_cast<Return (*)(Class *, Arg...)>(get_original(fp_cast(fp)));
    return [func](Class *obj, Arg... args) -> Return {
        return func(obj, std::forward<Arg>(args)...);
    };
}

/**
 * @brief Construct a std::function from a class method (const, no ref-qualifier)
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return(const Class *, Arg...)> get_original(Return (Class::*fp)(Arg...) const)
{
    auto func = reinterpret_cast<Return (*)(const Class *, Arg...)>(get_original(fp_cast(fp)));
    return [func](const Class *obj, Arg... args) -> Return {
        return func(obj, std::forward<Arg>(args)...);
    };
}
}  // namespace endstone::hook::detail
#define ENDSTONE_HOOK_CALL_ORIGINAL(fp, ...) endstone::hook::detail::get_original(fp)(__VA_ARGS__)

namespace endstone::hook::detail {
/**
 * @brief Construct a std::function from a function pointer
 * with Return Value Optimization (RVO).
 */
template <typename Return, typename... Arg>
inline std::function<Return *(Return *, Arg...)> get_original_rvo(Return (*fp)(Arg...))
{
    return reinterpret_cast<Return *(*)(Return *, Arg...)>(get_original(fp_cast(fp)));
}

/**
 * @brief Construct a std::function from a class method (non-const, no ref-qualifier)
 * with Return Value Optimization (RVO).
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return *(Return *, Class *, Arg...)> get_original_rvo(Return (Class::*fp)(Arg...))
{
#ifdef _WIN32
    auto func = reinterpret_cast<Return *(*)(Class *, Return *, Arg...)>(get_original(fp_cast(fp)));
    return [func](Return *ret, Class *obj, Arg... args) -> Return * {
        return func(obj, ret, std::forward<Arg>(args)...);
    };
#elif __linux__
    auto func = reinterpret_cast<Return *(*)(Return *, Class *, Arg...)>(get_original(fp_cast(fp)));
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
inline std::function<Return *(Return *, const Class *, Arg...)> get_original_rvo(Return (Class::*fp)(Arg...) const)
{
#ifdef _WIN32
    auto func = reinterpret_cast<Return *(*)(const Class *, Return *, Arg...)>(get_original(fp_cast(fp)));
    return [func](Return *ret, const Class *obj, Arg... args) -> Return * {
        return func(obj, ret, std::forward<Arg>(args)...);
    };
#elif __linux__
    auto func = reinterpret_cast<Return *(*)(Return *, const Class *, Arg...)>(get_original(fp_cast(fp)));
    return [func](Return *ret, const Class *obj, Arg... args) -> Return * {
        return func(ret, obj, std::forward<Arg>(args)...);
    };
#endif
}
}  // namespace endstone::hook::detail

#define ENDSTONE_HOOK_CALL_ORIGINAL_RVO(fp, ret, ...) ret = *endstone::hook::detail::get_original_rvo(fp)(&ret, __VA_ARGS__)
