//
// Created by Vincent on 31/07/2023.
//

#ifndef ENDSTONE_HOOK_MANAGER_H
#define ENDSTONE_HOOK_MANAGER_H

#include "endstone/common.h"

/**
 * @struct Hook
 * @brief Structure to represent a hook.
 */
struct Hook
{
    void *p_target;   ///< Pointer to the target function.
    void *p_detour;   ///< Pointer to the detour function.
    void *p_original; ///< Pointer to the original function.
};

/**
 * @class HookManager
 * @brief Class to manage hooks.
 */
class HookManager
{
  public:
    /**
     * @brief Initialize the hook manager.
     */
    static void initialize();

    /**
     * @brief Finalize the hook manager.
     */
    static void finalize();

    /**
     * @brief Register all hooks.
     */
    static void registerHooks();

    /**
     * @brief Retrieve hook by symbol name.
     * @param symbol Symbol name of the hook.
     * @return Reference to the Hook object.
     */
    const static Hook &getHook(const std::string &symbol);

  private:
    inline static bool initialized_ = false;          ///< Flag indicating whether the hook manager is initialized.
    inline static std::map<std::string, Hook> hooks_; ///< Map to store hooks by symbol name.
};

/**
 * @brief Look up a symbol within the base module.
 * @param symbol The name of the symbol to look up.
 * @return Pointer to the symbol if found, otherwise nullptr.
 */
void *lookupSymbol(const char *symbol);

/**
 * @brief Cast a member function pointer to a void pointer.
 * @tparam R Return type of the member function.
 * @tparam T Type of the class containing the member function.
 * @tparam Args Types of the arguments to the member function.
 * @param fp Pointer to the member function.
 * @return Void pointer representing the member function.
 */
template <class R, class T, class... Args>
void *fp_cast(R (T::*fp)(Args...))
{
    // Implicit convert from member function pointer to std::function
    std::function<R(T *, Args...)> func = fp;
    // Obtain function pointer from std::function
    void *detour_ptr = *reinterpret_cast<void **>(func.template target<R (T::*)(Args...)>());
    return detour_ptr;
}

/**
 * @brief Cast a free function pointer to a void pointer.
 * @tparam R Return type of the function.
 * @tparam Args Types of the arguments to the function.
 * @param fp Pointer to the function.
 * @return Void pointer representing the function.
 */
template <class R, class... Args>
void *fp_cast(R (*fp)(Args...))
{
    void *detour_ptr = reinterpret_cast<void *>(fp);
    return detour_ptr;
}

/**
 * @brief Call the original member function represented by a symbol.
 * @tparam R Return type of the member function.
 * @tparam T Type of the class containing the member function.
 * @tparam Args Types of the arguments to the member function.
 * @param symbol Symbol name representing the member function.
 * @param instance Pointer to the instance of the class containing the member function.
 * @param args Arguments to pass to the member function.
 * @return The return value of the member function.
 */
template <class R, class T, class... Args>
R call_original(const std::string &symbol, R (T::*)(Args...), T *instance, Args... args)
{
    using fp = R (*)(T *, Args...);
    auto hook = HookManager::getHook(symbol);
    auto func = reinterpret_cast<fp>(hook.p_original);
    auto bound_func = std::bind(func, instance, std::forward<Args>(args)...);
    return bound_func(args...);
}

/**
 * @brief Call the original free function represented by a symbol.
 * @tparam R Return type of the function.
 * @tparam Args Types of the arguments to the function.
 * @param symbol Symbol name representing the function.
 * @param args Arguments to pass to the function.
 * @return The return value of the function.
 */
template <class R, class... Args>
R call_original(const std::string &symbol, R (*)(Args...), Args... args)
{
    using fp = R (*)(Args...);
    auto hook = HookManager::getHook(symbol);
    auto func = reinterpret_cast<fp>(hook.p_original);
    return func(args...);
}

#define HOOK_FUNCTION(symbol)                                                                                          \
    {                                                                                                                  \
        Hook hook{};                                                                                                   \
        hook.p_detour = fp_cast(&symbol);                                                                              \
        hooks_.insert({#symbol, hook});                                                                                \
    }

#define CALL_ORIGINAL(symbol, ...) call_original(#symbol, &symbol, this, ##__VA_ARGS__);
#define CALL_ORIGINAL_STATIC(symbol, ...) call_original(#symbol, &symbol, ##__VA_ARGS__);

#endif // ENDSTONE_HOOK_MANAGER_H
