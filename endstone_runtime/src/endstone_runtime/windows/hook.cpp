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

#ifdef _WIN32
#include "endstone_runtime/windows/hook.h"

#include <MinHook.h>
#include <Psapi.h>

#include <system_error>
#include <unordered_map>

#include <spdlog/spdlog.h>

#include "endstone_runtime/internals.h"
#include "endstone_runtime/windows/symbol_manager.h"

namespace bedrock::internals {

namespace {

std::unordered_map<std::string, void *> gSymbols;

void *get_module_base(HANDLE process, HMODULE module)
{
    MODULEINFO mi = {nullptr};
    if (!GetModuleInformation(process, module, &mi, sizeof(mi))) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(),
                                "GetModuleInformation failed");
    }

    return mi.lpBaseOfDll;
}

const std::error_category &minhook_category() noexcept
{
    static const class MinHookErrorCategory : public std::error_category {
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
    } CATEGORY;
    return CATEGORY;
}

}  // namespace

void install_hooks(HINSTANCE module)
{
    MH_STATUS status;
    status = MH_Initialize();
    if (status != MH_OK) {
        throw std::system_error(status, minhook_category());
    }

    auto sym = endstone::sym::SymbolManager();

    std::unordered_map<std::string, void *> detours;
    auto *module_base = get_module_base(GetCurrentProcess(), module);
    sym.enumerate(module, "*", [&](const std::string &name, size_t offset) -> bool {
        auto *detour = static_cast<char *>(module_base) + offset;
        auto *target = bedrock::internals::symbol(name);

        if (detour && target) {
            void *original = nullptr;
            spdlog::debug("{}: 0x{:p} -> 0x{:p}", name, target, detour);
            status = MH_CreateHook(target, detour, &original);
            if (status != MH_OK) {
                throw std::system_error(status, minhook_category());
            }

            spdlog::debug("{}: = 0x{:p}", original);
            bedrock::internals::gSymbols[name] = original;
        }

        return true;
    });

    status = MH_EnableHook(MH_ALL_HOOKS);
    if (status != MH_OK) {
        throw std::system_error(status, minhook_category());
    }
}

void uninstall_hooks()
{
    MH_STATUS status;
    status = MH_DisableHook(MH_ALL_HOOKS);
    if (status != MH_OK) {
        throw std::system_error(status, minhook_category());
    }
    MH_DisableHook(MH_ALL_HOOKS);

    status = MH_Uninitialize();
    if (status != MH_OK) {
        throw std::system_error(status, minhook_category());
    }
}

void *symbol(const std::string &name) noexcept
{
    if (gSymbols.empty()) {
        auto sym = endstone::sym::SymbolManager();

        HMODULE module = GetModuleHandleW(nullptr);
        auto *module_base = get_module_base(GetCurrentProcess(), module);
        sym.enumerate(module, "*", [&](const std::string &name, size_t offset) -> bool {
            gSymbols.insert({name, static_cast<char *>(module_base) + offset});
            return true;
        });
    }

    auto it = gSymbols.find(name);
    if (it == gSymbols.end()) {
        return nullptr;
    }

    return it->second;
}
}  // namespace bedrock::internals

#endif
