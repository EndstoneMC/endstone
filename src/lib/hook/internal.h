//
// Created by Vincent on 05/09/2023.
//

#ifndef ENDSTONE_INTERNAL_H
#define ENDSTONE_INTERNAL_H

namespace endstone::hook::internal {

struct internals {
    std::unordered_map<std::string, void *> symbol_map{};
};

inline internals &get_internals()
{
    static internals i{};
    return i;
}

inline void *get_function_raw(const std::string &name)
{
    auto it = get_internals().symbol_map.find(name);
    if (it == get_internals().symbol_map.end()) {
        throw std::runtime_error("Function " + name + " not found.");
    }

    return it->second;
}

} // namespace endstone::hook::internal

#ifdef _WIN32

#include <Windows.h>
//
#include <DbgHelp.h>
#include <MinHook.h>
#include <Psapi.h>

namespace endstone::hook::internal {
inline void sym_initialize(void *handle, int options = 0, const char *search_path = nullptr,
                           bool invade_process = false)
{
    SymSetOptions(options);

    if (!SymInitialize(handle, search_path, invade_process)) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "SymInitialize failed");
    }

    auto &internals = get_internals();

    wchar_t image_name[MAX_PATH];
    auto len = GetModuleFileNameEx(handle, nullptr, image_name, MAX_PATH);
    if (len == 0 || len == MAX_PATH) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "GetModuleFileNameEx failed");
    }

    auto module_base = SymLoadModuleExW(handle, nullptr, image_name, nullptr, 0, 0, nullptr, 0);
    if (!module_base) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "SymLoadModuleExW failed");
    }

    SymEnumSymbols(
        handle, 0, "*!*",
        [](auto info, auto size, auto user_context) -> auto {
            auto internals = static_cast<internal::internals *>(user_context);
            internals->symbol_map.insert({std::string(info->Name), static_cast<char *>(nullptr) + info->Address});
            return 1;
        },
        &internals);

    MODULEINFO mi = {nullptr};
    if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &mi, sizeof(mi))) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(),
                                "GetModuleInformation failed");
    }

    for (auto &item : internals.symbol_map) {
        item.second = static_cast<char *>(mi.lpBaseOfDll) + (reinterpret_cast<size_t>(item.second) - module_base);
    }
}

inline void sym_cleanup(void *handle)
{
    SymCleanup(handle);
}

/**
 * @brief This class provides a custom error category for MinHook errors.
 */
class MinHookErrorCategory : public std::error_category {
public:
    [[nodiscard]] const char *name() const noexcept override
    {
        return "MinHookError";
    }

    [[nodiscard]] std::string message(int errVal) const override
    {
        auto errorCode = static_cast<MH_STATUS>(errVal);
        switch (errorCode) {
        case MH_ERROR_ALREADY_INITIALIZED:
            return "MinHook is already initialized.";
        case MH_ERROR_NOT_INITIALIZED:
            return "MinHook is not initialized yet, or already uninitialized.";
        case MH_ERROR_ALREADY_CREATED:
            return "The hook for the specified target function is already created.";
        case MH_ERROR_NOT_CREATED:
            return "The hook for the specified target function is not created yet.";
        case MH_ERROR_ENABLED:
            return "The hook for the specified target function is already enabled.";
        case MH_ERROR_DISABLED:
            return "The hook for the specified target function is not enabled yet, or already disabled.";
        case MH_ERROR_NOT_EXECUTABLE:
            return "The specified pointer is invalid. It points the address of non-allocated and/or non-executable "
                   "region.";
        case MH_ERROR_UNSUPPORTED_FUNCTION:
            return "The specified target function cannot be hooked.";
        case MH_ERROR_MEMORY_ALLOC:
            return "Failed to allocate memory.";
        case MH_ERROR_MEMORY_PROTECT:
            return "Failed to change the memory protection.";
        case MH_ERROR_MODULE_NOT_FOUND:
            return "The specified module is not loaded.";
        case MH_ERROR_FUNCTION_NOT_FOUND:
            return "The specified function is not found.";
        default:
            return "Unknown error.";
        }
    }
};

inline const std::error_category &minhook_category() noexcept
{
    static const MinHookErrorCategory category;
    return category;
}

} // namespace endstone::hook::internal
#endif

namespace endstone::hook::internal {
class symbol_handler {
public:
    explicit symbol_handler(void *handle = ((void *)-1l), int options = 0, const char *search_path = nullptr,
                            bool invade_process = false)
        : handle_(handle)
    {
        sym_initialize(handle, options, search_path, invade_process);
    }

    symbol_handler(const symbol_handler &) = delete;
    symbol_handler(symbol_handler &&other) noexcept
    {
        other.valid_ = false;
    }
    symbol_handler &operator=(const symbol_handler &) = delete;
    symbol_handler &operator=(symbol_handler &&) = delete;

    ~symbol_handler()
    {
        if (valid_) {
            sym_cleanup(handle_);
        }
    }

private:
    bool valid_ = true;
    void *handle_ = nullptr;
};
} // namespace endstone::hook::internal

#endif // ENDSTONE_INTERNAL_H
