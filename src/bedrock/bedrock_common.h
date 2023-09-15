//
// Created by Vincent on 28/08/2023.
//

#pragma once

#include "endstone/common.h"

namespace Core {
class FilePathManager;
}

#if defined(_WIN32)
#ifdef BEDROCK_EXPORTS
#define BEDROCK_API __declspec(dllexport)
#elif defined(BEDROCK_IMPORTS)
#define BEDROCK_API __declspec(dllimport)
#endif
#else
#if defined(BEDROCK_EXPORTS) || defined(BEDROCK_IMPORTS)
#if defined(__GNUC__) || defined(__clang__)
#define BEDROCK_API __attribute__((visibility("default")))
#endif
#endif
#endif

#ifndef BEDROCK_API
#define BEDROCK_API
#endif

namespace {
/**
 * @brief Construct a std::function from a class virtual method (non-const, no ref-qualifier)
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return(Arg...)> get_virtual_function(Return (Class::*)(Arg...), Class *obj, int index)
{
    auto vf_table = *reinterpret_cast<char **>(obj);
    auto func = *reinterpret_cast<Return (**)(Class *, Arg...)>(vf_table + index * sizeof(size_t));
    return [func, obj](Arg... args) -> Return {
        return func(obj, std::forward<Arg>(args)...);
    };
}

/**
 * @brief Construct a std::function from a class virtual method (const, no ref-qualifier)
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return(Arg...)> get_virtual_function(Return (Class::*)(Arg...) const, const Class *obj, int index)
{
    auto vf_table = *reinterpret_cast<char **>(const_cast<Class *>(obj));
    auto func = *reinterpret_cast<Return (**)(const Class *, Arg...)>(vf_table + index * sizeof(size_t));
    return [func, obj](Arg... args) -> Return {
        return func(obj, std::forward<Arg>(args)...);
    };
}

/**
 * @brief Construct a std::function from a class virtual method (non-const, no ref-qualifier)
    with considerations for Return Value Optimization (RVO).
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return *(Return *, Arg...)> get_virtual_function_rvo(Return (Class::*)(Arg...), Class *obj,
                                                                          int index)
{
    auto vf_table = *reinterpret_cast<char **>(obj);
    auto func = *reinterpret_cast<Return *(**)(Class *, Arg...)>(vf_table + index * sizeof(size_t));
    return [func, obj](Return *ret, Arg... args) -> Return * {
        return func(obj, ret, std::forward<Arg>(args)...);
    };
}

/**
 * @brief Construct a std::function from a class virtual method (const, no ref-qualifier)
 *  with considerations for Return Value Optimization (RVO).
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return *(Return *, Arg...)> get_virtual_function_rvo(Return (Class::*)(Arg...) const,
                                                                          const Class *obj, int index)
{
    auto vf_table = *reinterpret_cast<char **>(const_cast<Class *>(obj));
    auto func = *reinterpret_cast<Return *(**)(const Class *, Return *, Arg...)>(vf_table + index * sizeof(size_t));
    return [func, obj](Return *ret, Arg... args) -> Return * {
        return func(obj, ret, std::forward<Arg>(args)...);
    };
}

#define BEDROCK_VIRTUAL_CALL(index, fp, obj, ...)     get_virtual_function(fp, obj, index)(__VA_ARGS__);
#define BEDROCK_VIRTUAL_CALL_RVO(index, fp, obj, ...) get_virtual_function_rvo(fp, obj, index)(__VA_ARGS__);
} // namespace

