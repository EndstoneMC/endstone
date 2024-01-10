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

#include <Windows.h>
// Psapi.h must be included after Windows.h
#include <Psapi.h>

#include <system_error>

#include "endstone_runtime/platform.h"

namespace endstone::platform {

namespace {
HMODULE get_module_handle(const char *module_name)
{
    auto *module = GetModuleHandleA(module_name);
    if (!module) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "GetModuleHandleA failed");
    }
    return module;
}
}  // namespace

void *get_module_base()
{
    MODULEINFO mi = {nullptr};
    if (!GetModuleInformation(GetCurrentProcess(), get_module_handle("endstone_runtime.dll"), &mi, sizeof(mi))) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(),
                                "GetModuleInformation failed");
    }

    return mi.lpBaseOfDll;
}

std::string get_module_pathname()
{
    char file_name[MAX_PATH] = {0};
    auto len =
        GetModuleFileNameExA(GetCurrentProcess(), get_module_handle("endstone_runtime.dll"), file_name, MAX_PATH);
    if (len == 0 || len == MAX_PATH) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "GetModuleFileNameEx failed");
    }
    return file_name;
}

void *get_executable_base()
{
    MODULEINFO mi = {nullptr};
    if (!GetModuleInformation(GetCurrentProcess(), get_module_handle(nullptr), &mi, sizeof(mi))) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(),
                                "GetModuleInformation failed");
    }

    return mi.lpBaseOfDll;
}

std::string get_executable_pathname()
{
    char file_name[MAX_PATH] = {0};
    auto len = GetModuleFileNameExA(GetCurrentProcess(), get_module_handle(nullptr), file_name, MAX_PATH);
    if (len == 0 || len == MAX_PATH) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "GetModuleFileNameEx failed");
    }
    return file_name;
}

}  // namespace endstone::platform

#endif
