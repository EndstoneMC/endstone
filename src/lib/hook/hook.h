//
// Created by Vincent on 05/09/2023.
//

#ifndef ENDSTONE_HOOK_H
#define ENDSTONE_HOOK_H

#include "internal.h"

namespace endstone::hook {

/**
 * @brief Construct a std::function from a vanilla function pointer
 */
template <typename Return, typename... Arg>
inline std::function<Return(Arg...)> get_function(Return (*f)(Arg...), const std::string &name)
{
    return reinterpret_cast<decltype(f)>(internal::get_function_raw(name));
}

/**
 * @brief Construct a std::function from a class method (non-const, no ref-qualifier)
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return(Class *, Arg...)> get_function(Return (Class::*f)(Arg...), const std::string &name)
{
    auto func = reinterpret_cast<decltype(f)>(internal::get_function_raw(name));
    return [func](Class *obj, Arg... args) -> Return {
        return (obj->*func)(std::forward<Arg>(args)...);
    };
}

/**
 * @brief Construct a std::function from a class method (non-const, lvalue ref-qualifier)
 *
 * A copy of the overload for non-const functions without explicit ref-qualifier
 * but with an added `&`.
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return(Class *, Arg...)> get_function(Return (Class::*f)(Arg...) &, const std::string &name)
{
    auto func = reinterpret_cast<decltype(f)>(internal::get_function_raw(name));
    return [func](Class *obj, Arg... args) -> Return {
        return (obj->*func)(std::forward<Arg>(args)...);
    };
}

/**
 * @brief Construct a std::function from a class method (const, no ref-qualifier)
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return(Class *, Arg...)> get_function(Return (Class::*f)(Arg...) const, const std::string &name)
{
    auto func = reinterpret_cast<decltype(f)>(internal::get_function_raw(name));
    return [func](const Class *obj, Arg... args) -> Return {
        return (obj->*func)(std::forward<Arg>(args)...);
    };
}

/**
 * @brief Construct a std::function from a class method (const, lvalue ref-qualifier)
 *
 * A copy of the overload for const functions without explicit ref-qualifier
 * but with an added `&`.
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return(Class *, Arg...)> get_function(Return (Class::*f)(Arg...) const &, const std::string &name)
{
    auto func = reinterpret_cast<decltype(f)>(internal::get_function_raw(name));
    return [func](const Class *obj, Arg... args) -> Return {
        return (obj->*func)(std::forward<Arg>(args)...);
    };
}

} // namespace endstone::hook

#ifdef _WIN32
#include <MinHook.h>

namespace endstone::hook {
class manager {
public:
    explicit manager(void *h_library) : h_library_(h_library)
    {
        internal::symbol_handler sym{0, nullptr, false};

        auto base = sym.load_module(h_library);
        sym.enum_symbols(base, "*", [](auto info, auto size) -> bool {
            printf("%s -> 0x%p | %lu\n", info->Name, reinterpret_cast<void *>(info->Address), info->Flags);
            return true;
        });

        MH_STATUS status;
        status = MH_Initialize();
        if (status != MH_OK) {
            throw std::system_error(status, internal::minhook_category());
        }

        initialized_ = true;
    }

    manager(const manager &) = delete;
    manager(manager &&other) noexcept
    {
        other.initialized_ = false;
    }
    manager &operator=(const manager &) = delete;
    manager &operator=(manager &&) = delete;

    ~manager()
    {
        if (initialized_) {
            MH_DisableHook(MH_ALL_HOOKS);
            MH_Uninitialize();
        }
    }

private:
    bool initialized_{false};
    void *h_library_{nullptr};
};
} // namespace endstone::hook

#endif

#endif // ENDSTONE_HOOK_H
