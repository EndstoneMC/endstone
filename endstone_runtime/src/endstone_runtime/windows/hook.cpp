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
// must be included after Windows.h
#include <Psapi.h>
#include <funchook.h>

#include <unordered_map>

#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "endstone_runtime/symbol_handler.h"

namespace endstone::hook {

namespace {
std::unordered_map<void *, void *> gOriginals;

void *get_module_base(HANDLE process, HMODULE module)
{
    MODULEINFO mi = {nullptr};
    if (!GetModuleInformation(process, module, &mi, sizeof(mi))) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(),
                                "GetModuleInformation failed");
    }

    return mi.lpBaseOfDll;
}

std::string get_module_filename(HMODULE module)
{
    char file_name[MAX_PATH];
    auto len = GetModuleFileNameExA(GetCurrentProcess(), module, file_name, MAX_PATH);
    if (len == 0 || len == MAX_PATH) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "GetModuleFileNameEx failed");
    }
    return file_name;
}

}  // namespace

namespace internals {
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
    auto sym = endstone::sym::create_handler();

    // Find detours
    auto *module = GetModuleHandleA("endstone_runtime");
    if (!module) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "GetModuleHandleA failed");
    }
    auto *module_base = get_module_base(GetCurrentProcess(), module);

    std::unordered_map<std::string, void *> detours;
    sym->enumerate(get_module_filename(module).c_str(), [&](const std::string &name, size_t offset) -> bool {
        auto *detour = static_cast<char *>(module_base) + offset;
        detours.emplace(name, detour);
        return true;
    });

    // Find targets
    const char *executable_name = "bedrock_server.exe";
    auto *executable = GetModuleHandleA(nullptr);
    auto *executable_base = get_module_base(GetCurrentProcess(), executable);
    std::unordered_map<std::string, void *> targets;
    sym->enumerate(get_module_filename(executable).c_str(), [&](const std::string &name, size_t offset) -> bool {
        auto it = detours.find(name);
        if (it != detours.end()) {
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

            spdlog::debug("{}: 0x{:p} -> 0x{:p}", name, target, detour);

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

            spdlog::debug("{}: = 0x{:p}", original);
            gOriginals.emplace(detour, original);
        }
        else {
            throw std::runtime_error(fmt::format("Unable to find target function {} in the executable", name));
        }
    }
}

}  // namespace endstone::hook
#endif
