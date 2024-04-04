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

namespace endstone::detail::hook {
void install();
const std::error_category &hook_error_category();
}  // namespace endstone::detail::hook

namespace endstone::detail::hook {

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
}  // namespace endstone::detail::hook

namespace endstone::detail::hook {

void *get_original(void *detour);
void *get_original(const std::string &name);

std::unordered_map<std::string, void *> get_targets();
std::unordered_map<std::string, void *> get_detours();

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

namespace endstone::detail::hook {

template <typename T>
class VmtHook {
public:
    explicit VmtHook(T &target) : target_(target)
    {
        original_ = *(reinterpret_cast<uintptr_t **>(&target_));
        size_ = 0;
        while (original_[size_]) {
            ++size_;
        }

        copy_ = new uintptr_t[size_ + 1];
        std::copy(original_, original_ + size_, copy_);
        copy_[size_] = 0;
        *reinterpret_cast<uintptr_t **>(&target_) = copy_;
    };

    VmtHook(const VmtHook &) = delete;
    VmtHook &operator=(const VmtHook &) = delete;
    VmtHook(VmtHook &&) = delete;
    VmtHook &operator=(VmtHook &&) = delete;

    ~VmtHook()
    {
        *reinterpret_cast<uintptr_t **>(&target_) = original_;
        delete[] copy_;
    }

    template <size_t index>
    void install(void *detour)
    {
        if (index < size_) {
            copy_[index] = reinterpret_cast<uintptr_t>(detour);
        }
    }

    template <size_t index>
    void uninstall()
    {
        copy_[index] = original_[index];
    }

    template <size_t index, typename Return, typename... Arg>
    std::function<Return(T *, Arg...)> getOriginal(Return (T::*)(Arg...))
    {
        auto func = reinterpret_cast<Return (*)(T *, Arg...)>(original_[index]);
        return [func](T *obj, Arg... args) -> Return {
            return func(obj, std::forward<Arg>(args)...);
        };
    }

    template <size_t index, typename Return, typename... Arg>
    std::function<Return(const T *, Arg...)> getOriginal(Return (T::*)(Arg...) const)
    {
        auto func = reinterpret_cast<Return (*)(const T *, Arg...)>(original_[index]);
        return [func](const T *obj, Arg... args) -> Return {
            return func(obj, std::forward<Arg>(args)...);
        };
    }

private:
    T &target_;
    uintptr_t *copy_;
    uintptr_t *original_;
    size_t size_;
};

}  // namespace endstone::detail::hook
