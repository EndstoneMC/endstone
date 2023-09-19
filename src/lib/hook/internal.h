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

#include <chrono>
#include <future>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <unordered_map>
#include <utility>

namespace Endstone::HookInternal {

struct Internals {
    std::unordered_map<std::string, void *> detours{};
    std::unordered_map<std::string, void *> originals{};
};

inline Internals &get_internals()
{
    static Internals i{};
    return i;
}

}  // namespace Endstone::HookInternal

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
//
#include <DbgHelp.h>
#include <MinHook.h>
#include <Psapi.h>

namespace Endstone::HookInternal {

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

    if (!SymInitialize(handle, search_path, static_cast<BOOL>(invade_process))) {
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
            auto *callback = static_cast<std::function<bool(PSYMBOL_INFO, int)> *>(user_context);
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

    [[nodiscard]] std::string message(int err_val) const override
    {
        auto error_code = static_cast<MH_STATUS>(err_val);
        switch (error_code) {
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
    static const MinHookErrorCategory CATEGORY;
    return CATEGORY;
}

}  // namespace Endstone::HookInternal
#endif

namespace Endstone::HookInternal {
class SymbolHandler {
public:
    explicit SymbolHandler(int options = 0, const char *search_path = nullptr, bool invade_process = false)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dist(1, SIZE_MAX);
        // NOLINTNEXTLINE(performance-no-int-to-ptr)
        handle_ = reinterpret_cast<void *>(dist(gen));
        {
            std::lock_guard lock{mutex_};
            sym_initialize(handle_, options, search_path, invade_process);
        }
    }

    SymbolHandler(const SymbolHandler &) = delete;
    SymbolHandler(SymbolHandler &&other) noexcept
    {
        other.valid_ = false;
    }
    SymbolHandler &operator=(const SymbolHandler &) = delete;
    SymbolHandler &operator=(SymbolHandler &&) = delete;

    ~SymbolHandler()
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
}  // namespace Endstone::HookInternal
