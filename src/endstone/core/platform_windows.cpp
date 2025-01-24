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
#include <TlHelp32.h>

#include <string>
#include <string_view>
#include <system_error>

#include "endstone/detail/platform.h"

namespace endstone::detail {

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
    static void *base = []() {
        MODULEINFO mi = {nullptr};
        if (!GetModuleInformation(GetCurrentProcess(), get_module_handle("endstone_runtime.dll"), &mi, sizeof(mi))) {
            throw std::system_error(static_cast<int>(GetLastError()), std::system_category(),
                                    "GetModuleInformation failed");
        }

        return mi.lpBaseOfDll;
    }();
    return base;
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
    static void *base = []() {
        MODULEINFO mi = {nullptr};
        if (!GetModuleInformation(GetCurrentProcess(), get_module_handle(nullptr), &mi, sizeof(mi))) {
            throw std::system_error(static_cast<int>(GetLastError()), std::system_category(),
                                    "GetModuleInformation failed");
        }

        return mi.lpBaseOfDll;
    }();
    return base;
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

std::string_view get_platform()
{
    return "Windows";
}

std::size_t get_thread_count()
{
    DWORD process_id = GetCurrentProcessId();
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(),
                                "CreateToolhelp32Snapshot failed");
    }

    THREADENTRY32 te32;
    te32.dwSize = sizeof(THREADENTRY32);
    if (!Thread32First(snapshot, &te32)) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "Thread32First failed");
    }

    int thread_count = 0;
    do {
        if (te32.th32OwnerProcessID == process_id) {
            ++thread_count;
        }
    } while (Thread32Next(snapshot, &te32));
    CloseHandle(snapshot);
    return thread_count;
}
std::size_t get_used_physical_memory()
{
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize;
    }
    throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "GetProcessMemoryInfo failed");
}

std::size_t get_total_virtual_memory()
{
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.PagefileUsage;
    }
    throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "GetProcessMemoryInfo failed");
}

}  // namespace endstone::detail

#endif
