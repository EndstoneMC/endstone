//
// Created by Vincent on 05/09/2023.
//

#ifndef ENDSTONE_INTERNAL_H
#define ENDSTONE_INTERNAL_H

#include <chrono>
#include <future>
#include <iostream>
#include <limits>
#include <random>
#include <utility>

namespace endstone::hook::internal {

struct internals {
    std::unordered_map<std::string, void *> detours{};
    std::unordered_map<std::string, void *> originals{};
};

inline internals &get_internals()
{
    static internals i{};
    return i;
}

} // namespace endstone::hook::internal

#ifdef _WIN32

#define NOMINMAX

#include <Windows.h>
//
#include <DbgHelp.h>
#include <MinHook.h>
#include <Psapi.h>

namespace endstone::hook::internal {

inline void *get_module_base(void *h_process, void *h_module)
{
    MODULEINFO mi = {nullptr};
    if (!GetModuleInformation(h_process, reinterpret_cast<HMODULE>(h_module), &mi, sizeof(mi))) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(),
                                "GetModuleInformation failed");
    }

    return mi.lpBaseOfDll;
}

inline std::wstring get_module_file_name(void *h_process, void *h_module)
{
    wchar_t file_name[MAX_PATH];
    auto len = GetModuleFileNameExW(h_process, reinterpret_cast<HMODULE>(h_module), file_name, MAX_PATH);
    if (len == 0 || len == MAX_PATH) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "GetModuleFileNameEx failed");
    }
    return file_name;
}

inline void sym_initialize(void *handle, int options = 0, const char *search_path = nullptr,
                           bool invade_process = false)
{
    SymSetOptions(options);

    if (!SymInitialize(handle, search_path, invade_process)) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "SymInitialize failed");
    }
}

inline size_t sym_load_module(void *handle, void *h_module = nullptr)
{
    auto file_name = get_module_file_name(GetCurrentProcess(), h_module);
    auto module_base = SymLoadModuleExW(handle, nullptr, file_name.c_str(), nullptr, 0, 0, nullptr, 0);
    if (!module_base) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "SymLoadModuleExW failed");
    }

    return module_base;
}

inline void sym_enum_symbols(void *handle, size_t module_base, const char *mask,
                             std::function<bool(PSYMBOL_INFO, int)> callback)
{
    SymEnumSymbols(
        handle, module_base, mask,
        [](auto info, auto value, auto user_context) -> int {
            auto callback = static_cast<std::function<bool(PSYMBOL_INFO, int)> *>(user_context);
            auto result = (*callback)(info, value);
            return result ? TRUE : FALSE;
        },
        &callback);
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
    explicit symbol_handler(int options = 0, const char *search_path = nullptr, bool invade_process = false)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dist(1, std::numeric_limits<size_t>::max());
        handle_ = reinterpret_cast<void *>(dist(gen));
        {
            std::lock_guard lock{mutex_};
            sym_initialize(handle_, options, search_path, invade_process);
        }
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
            std::lock_guard lock{mutex_};
            sym_cleanup(handle_);
        }
    }

    size_t load_module(void *h_module)
    {
        std::lock_guard lock{mutex_};
        return sym_load_module(handle_, h_module);
    }

    void enum_symbols(size_t module_base, const char *mask, std::function<bool(PSYMBOL_INFO, int)> callback)
    {
        std::lock_guard lock{mutex_};
        sym_enum_symbols(handle_, module_base, mask, std::move(callback));
    }

private:
    bool valid_{true};
    void *handle_{nullptr};
    std::mutex mutex_{};
};
} // namespace endstone::hook::internal

#endif // ENDSTONE_INTERNAL_H
