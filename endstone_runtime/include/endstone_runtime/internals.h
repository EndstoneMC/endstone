// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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
#include <string>
#include <string_view>

namespace bedrock::internals {

void *symbol(const std::string &name) noexcept;

template <typename ReturnType = void *>
inline ReturnType variable(const std::string &name)
{
    return reinterpret_cast<ReturnType>(symbol(name));
}

/**
 * @brief Construct a std::function from a vanilla function pointer
 */
template <typename Return, typename... Arg>
inline std::function<Return(Arg...)> function(Return (*)(Arg...), const std::string &name)
{
    return reinterpret_cast<Return (*)(Arg...)>(symbol(name));
}

/**
 * @brief Construct a std::function from a class method (non-const, no ref-qualifier)
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return(Class *, Arg...)> function(Return (Class::*)(Arg...), const std::string &name)
{
    auto func = reinterpret_cast<Return (*)(Class *, Arg...)>(symbol(name));
    return [func](Class *obj, Arg... args) -> Return {
        return func(obj, std::forward<Arg>(args)...);
    };
}

/**
 * @brief Construct a std::function from a class method (const, no ref-qualifier)
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return(const Class *, Arg...)> function(Return (Class::*)(Arg...) const, const std::string &name)
{
    auto func = reinterpret_cast<Return (*)(const Class *, Arg...)>(symbol(name));
    return [func](const Class *obj, Arg... args) -> Return {
        return func(obj, std::forward<Arg>(args)...);
    };
}

/**
 * @brief Construct a std::function from a class method (non-const, no ref-qualifier)
 *  with considerations for Return Value Optimization (RVO).
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return *(Class *, Return *, Arg...)> function_rvo(Return (Class::*)(Arg...),
                                                                       const std::string &name)
{
    auto func = reinterpret_cast<Return *(*)(Class *, Return *, Arg...)>(symbol(name));
    return [func](Class *obj, Return *ret, Arg... args) -> Return * {
        return func(obj, ret, std::forward<Arg>(args)...);
    };
}

/**
 * @brief Construct a std::function from a class method (const, no ref-qualifier)
 *  with considerations for Return Value Optimization (RVO).
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return *(const Class *, Return *, Arg...)> function_rvo(Return (Class::*)(Arg...) const,
                                                                             const std::string &name)
{
    auto func = reinterpret_cast<Return *(*)(const Class *, Return *, Arg...)>(symbol(name));
    return [func](const Class *obj, Return *ret, Arg... args) -> Return * {
        return func(obj, ret, std::forward<Arg>(args)...);
    };
}

}  // namespace bedrock::internals

namespace {
constexpr bool strings_equal(char const *a, char const *b)
{
    return std::string_view(a) == b;
}
}  // namespace

#if _WIN32
#define BEDROCK_API __declspec(dllexport)
#elif __linux__
#define BEDROCK_API __attribute__((visibility("default")))
#endif

#ifdef _MSC_VER  // MSVC
#define BEDROCK_CALL(fp, ...)     bedrock::internals::function(fp, __FUNCDNAME__)(__VA_ARGS__);
#define BEDROCK_CALL_RVO(fp, ...) bedrock::internals::function_rvo(fp, __FUNCDNAME__)(__VA_ARGS__);
#elif __GNUC__  // GUNC
#define BEDROCK_CALL(fp, ...)     bedrock::internals::function(fp, __PRETTY_FUNCTION__)(__VA_ARGS__);
#define BEDROCK_CALL_RVO(fp, ...) bedrock::internals::function_rvo(fp, __PRETTY_FUNCTION__)(__VA_ARGS__);
#endif
