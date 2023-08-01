//
// Created by Vincent on 31/07/2023.
//

#ifndef ENDSTONE_HOOK_MANAGER_H
#define ENDSTONE_HOOK_MANAGER_H

#include "endstone.h"

template <class R, class T, class... Args>
void *fp_cast(R (T::*fp)(Args...))
{
    // Implicit convert from member function pointer to std::function
    std::function<R(T *, Args...)> func = fp;
    // Obtain function pointer from std::function
    void *detour_ptr = *reinterpret_cast<void **>(func.template target<R (T::*)(Args...)>());
    return detour_ptr;
}

template <class R, class T, class... Args>
R call_original(const std::string &symbol, R (T::*)(Args...), T *instance, Args... args)
{
    using fp = R (*)(T *, Args...);
    auto hook = HookManager::getHook(symbol);
    auto func = reinterpret_cast<fp>(hook.p_original);
    auto bound_func = std::bind(func, instance, std::forward<Args>(args)...);
    return bound_func(args...);
}

struct IHook
{
    void *p_target = nullptr;
    void *p_detour = nullptr;
    void *p_original = nullptr;
};

class HookManager
{
  public:
    static void installHooks();

    static void uninstallHooks();

    const static IHook &getHook(const std::string &symbol);

  private:
    inline static bool is_initialised = false;
    inline static std::map<std::string, IHook> hooks;
};

class ServerInstance
{
  public:
    void startServerThread();
};

#define HOOK_FUNCTION(symbol)                                                                                          \
    {                                                                                                                  \
        IHook hook;                                                                                                    \
        hook.p_target = dlsym(RTLD_NEXT, #symbol);                                                                     \
        hook.p_detour = fp_cast(&symbol);                                                                              \
        hooks.insert({#symbol, hook});                                                                                 \
    }

#define CALL_ORIGINAL(symbol, ...) call_original(#symbol, &symbol, this, ##__VA_ARGS__)

#endif // ENDSTONE_HOOK_MANAGER_H
