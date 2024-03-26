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

#include "endstone/detail/hook.h"

#include <Windows.h>
// DbgHelp.h must be included after Windows.h
#include <DbgHelp.h>

#include <random>
#include <string>
#include <system_error>
#include <unordered_map>

#include <spdlog/spdlog.h>

#include "endstone/detail/os.h"

namespace endstone::detail::hook {

namespace {
void enumerate_symbols(const char *path, std::function<bool(const std::string &, size_t)> callback)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(1, static_cast<size_t>(-1));
    HANDLE handle = reinterpret_cast<HANDLE>(dist(gen));  // NOLINT

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

std::unordered_map<std::string, void *> get_detours()
{
    auto *module_base = os::get_module_base();
    const auto module_pathname = os::get_module_pathname();
    std::unordered_map<std::string, void *> detours;

    enumerate_symbols(  //
        module_pathname.c_str(), [&](const std::string &name, size_t offset) -> bool {
            spdlog::debug("{} -> 0x{:x}", name, offset);
            auto *detour = static_cast<char *>(module_base) + offset;
            detours.emplace(name, detour);
            return true;
        });
    return detours;
}

std::unordered_map<std::string, void *> get_targets()
{
    auto *executable_base = os::get_executable_base();
    const auto executable_pathname = os::get_executable_pathname();

    std::unordered_map<std::string, void *> targets;
    enumerate_symbols(  //
        executable_pathname.c_str(), [&](const std::string &name, size_t offset) -> bool {
            spdlog::debug("{} -> 0x{:x}", name, offset);
            auto *target = static_cast<char *>(executable_base) + offset;
            targets.emplace(name, target);
            return true;
        });
    return targets;
}

}  // namespace endstone::detail::hook

#endif
