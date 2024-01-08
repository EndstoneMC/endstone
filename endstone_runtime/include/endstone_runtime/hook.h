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

namespace endstone::hook::internals {

template <class R, class... Args>
void *fp_cast(R (*fp)(Args...))
{
    std::function<R(Args...)> func = fp;
    void *target = *reinterpret_cast<void **>(func.template target<R (*)(Args...)>());
    return target;
}

template <class R, class T, class... Args>
void *fp_cast(R (T::*fp)(Args...))
{
    std::function<R(T *, Args...)> func = fp;
    void *target = *reinterpret_cast<void **>(func.template target<R (T::*)(Args...)>());
    return target;
}

void *get_original(void *detour);

/**
 * @brief Construct a std::function from a vanilla function pointer
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

/**
 * @brief Construct a std::function from a class method (non-const, no ref-qualifier)
 *  with considerations for Return Value Optimization (RVO).
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return *(Class *, Return *, Arg...)> get_original_rvo(Return (Class::*fp)(Arg...))
{
    auto func = reinterpret_cast<Return *(*)(Class *, Return *, Arg...)>(get_original(fp_cast(fp)));
    return [func](Class *obj, Return *ret, Arg... args) -> Return * {
        return func(obj, ret, std::forward<Arg>(args)...);
    };
}

/**
 * @brief Construct a std::function from a class method (const, no ref-qualifier)
 *  with considerations for Return Value Optimization (RVO).
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return *(const Class *, Return *, Arg...)> get_original_rvo(Return (Class::*fp)(Arg...) const)
{
    auto func = reinterpret_cast<Return *(*)(const Class *, Return *, Arg...)>(get_original(fp_cast(fp)));
    return [func](const Class *obj, Return *ret, Arg... args) -> Return * {
        return func(obj, ret, std::forward<Arg>(args)...);
    };
}
}  // namespace endstone::hook::internals

#define ENDSTONE_HOOK_CALL_ORIGINAL(fp, ...)     endstone::hook::internals::get_original(fp)(__VA_ARGS__);
#define ENDSTONE_HOOK_CALL_ORIGINAL_RVO(fp, ...) endstone::hook::internals::get_original(fp)(__VA_ARGS__);
