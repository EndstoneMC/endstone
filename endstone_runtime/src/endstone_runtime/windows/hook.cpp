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

#include "endstone_runtime/hook.h"

#include <Windows.h>
// DbgHelp.h must be included after Windows.h
#include <DbgHelp.h>

#include <random>
#include <system_error>
#include <unordered_map>

#include <fmt/format.h>
#include <funchook/funchook.h>
#include <spdlog/spdlog.h>

#include "endstone_runtime/platform.h"

namespace endstone::hook {

namespace {
std::unordered_map<void *, void *> gOriginals;

void enumerate_symbols(const char *path, std::function<bool(const std::string &, size_t)> callback)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(1, static_cast<size_t>(-1));
    auto *handle = reinterpret_cast<HANDLE>(dist(gen));

    if (!SymInitialize(handle, nullptr, FALSE)) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "SymInitialize failed");
    }

    SymSetOptions(NULL);  // do not undecorate symbols by default

    auto module_base = SymLoadModuleEx(handle, nullptr, path, nullptr, 0, 0, nullptr, 0);
    if (!module_base) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "SymLoadModuleEx failed");
    }

    struct UserContext {
        size_t module_base;
        std::function<bool(const std::string &, size_t)> callback;
    };
    auto user_context = UserContext{module_base, std::move(callback)};

    SymEnumSymbols(
        handle, module_base, "*" /*Mask*/,
        [](PSYMBOL_INFO info /*pSymInfo*/, ULONG /*SymbolSize*/, PVOID user_context /*UserContext*/) -> BOOL {
            auto *ctx = static_cast<UserContext *>(user_context);
            if (!(ctx->callback)(info->Name, info->Address - ctx->module_base)) {
                return FALSE;
            }
            return TRUE;
        },
        &user_context);

    SymCleanup(handle);
}

}  // namespace

namespace detail {
void *get_original(void *detour)
{
    auto it = gOriginals.find(detour);
    if (it == gOriginals.end()) {
        return nullptr;
    }
    return it->second;
}
}  // namespace internals

void install()
{
    namespace ep = endstone::platform;

    // Find detours
    auto *module_base = ep::get_module_base();
    auto module_pathname = ep::get_module_pathname();

    std::unordered_map<std::string, void *> detours;
    enumerate_symbols(  //
        module_pathname.c_str(), [&](const std::string &name, size_t offset) -> bool {
            spdlog::debug("{} -> 0x{:x}", name, offset);
            auto *detour = static_cast<char *>(module_base) + offset;
            detours.emplace(name, detour);
            return true;
        });

    // Find targets
    auto *executable_base = ep::get_executable_base();
    std::unordered_map<std::string, void *> targets;

    const auto executable_pathname = ep::get_executable_pathname();
    enumerate_symbols(  //
        executable_pathname.c_str(), [&](const std::string &name, size_t offset) -> bool {
            auto it = detours.find(name);
            if (it != detours.end()) {
                spdlog::debug("{} -> 0x{:x}", name, offset);
                auto *target = static_cast<char *>(executable_base) + offset;
                targets.emplace(name, target);
            }
            return true;
        });

    // Install hooks
    for (const auto &[name, detour] : detours) {
        auto it = targets.find(name);
        if (it != targets.end()) {
            void *target = it->second;
            void *original = target;

            spdlog::debug("{}: T = 0x{:p}", name, target);
            spdlog::debug("{}: D = 0x{:p}", name, detour);

            funchook_t *hook = funchook_create();
            int status;
            status = funchook_prepare(hook, &original, detour);
            if (status != 0) {
                throw std::system_error(status, hook_error_category());
            }

            status = funchook_install(hook, 0);
            if (status != 0) {
                throw std::system_error(status, hook_error_category());
            }

            spdlog::debug("{}: O = 0x{:p}", name, original);
            gOriginals.emplace(detour, original);
        }
        else {
            throw std::runtime_error(fmt::format("Unable to find target function {} in the executable", name));
        }
    }
}

const std::error_category &hook_error_category() noexcept
{
    static const class HookErrorCategory : public std::error_category {
    public:
        [[nodiscard]] const char *name() const noexcept override
        {
            return "HookError";
        }

        [[nodiscard]] std::string message(int err_val) const override
        {
            switch (err_val) {
            case FUNCHOOK_ERROR_INTERNAL_ERROR:
                return "FUNCHOOK_ERROR_INTERNAL_ERROR";
            case FUNCHOOK_ERROR_SUCCESS:
                return "FUNCHOOK_ERROR_SUCCESS";
            case FUNCHOOK_ERROR_OUT_OF_MEMORY:
                return "FUNCHOOK_ERROR_OUT_OF_MEMORY";
            case FUNCHOOK_ERROR_ALREADY_INSTALLED:
                return "FUNCHOOK_ERROR_ALREADY_INSTALLED";
            case FUNCHOOK_ERROR_DISASSEMBLY:
                return "FUNCHOOK_ERROR_DISASSEMBLY";
            case FUNCHOOK_ERROR_IP_RELATIVE_OFFSET:
                return "FUNCHOOK_ERROR_IP_RELATIVE_OFFSET";
            case FUNCHOOK_ERROR_CANNOT_FIX_IP_RELATIVE:
                return "FUNCHOOK_ERROR_CANNOT_FIX_IP_RELATIVE";
            case FUNCHOOK_ERROR_FOUND_BACK_JUMP:
                return "FUNCHOOK_ERROR_FOUND_BACK_JUMP";
            case FUNCHOOK_ERROR_TOO_SHORT_INSTRUCTIONS:
                return "FUNCHOOK_ERROR_TOO_SHORT_INSTRUCTIONS";
            case FUNCHOOK_ERROR_MEMORY_ALLOCATION:
                return "FUNCHOOK_ERROR_MEMORY_ALLOCATION";
            case FUNCHOOK_ERROR_MEMORY_FUNCTION:
                return "FUNCHOOK_ERROR_MEMORY_FUNCTION";
            case FUNCHOOK_ERROR_NOT_INSTALLED:
                return "FUNCHOOK_ERROR_NOT_INSTALLED";
            case FUNCHOOK_ERROR_NO_AVAILABLE_REGISTERS:
                return "FUNCHOOK_ERROR_NO_AVAILABLE_REGISTERS";
            default:
                return "Unknown error.";
            }
        }
    } CATEGORY;
    return CATEGORY;
}
}  // namespace endstone::hook

#endif
