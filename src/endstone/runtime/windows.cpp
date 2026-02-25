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

#include "endstone/runtime/hook.h"

#ifdef _WIN32

#include <Windows.h>

// DbgHelp.h must be included after Windows.h
#include <DbgHelp.h>
#include <Psapi.h>
#include <corecrt_io.h>
#include <fcntl.h>

#include <chrono>
#include <iostream>
#include <string>
#include <system_error>
#include <unordered_map>

#include "endstone/core/platform.h"

enum SymTagEnum {
    SymTagPublicSymbol = 10,
};

namespace {
void enumerate_symbols(const char *path, std::function<bool(const std::string &, std::size_t, std::uint32_t)> callback,
                       bool load_symbol)
{
    HANDLE current_process = GetCurrentProcess();
    HANDLE handle;
    if (!DuplicateHandle(current_process, current_process, current_process, &handle, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "DuplicateHandle failed");
    }

    if (!SymInitialize(handle, nullptr, FALSE)) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "SymInitialize failed");
    }

    SymSetOptions(NULL);  // do not undecorate symbols by default

    auto module_base =
        SymLoadModuleEx(handle, nullptr, path, nullptr, 0, 0, nullptr, load_symbol ? 0 : SLMFLAG_NO_SYMBOLS);
    if (!module_base) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "SymLoadModuleEx failed");
    }

    struct UserContext {
        std::size_t module_base;
        std::function<bool(const std::string &, std::size_t, std::uint32_t)> callback;
    };
    auto user_context = UserContext{module_base, std::move(callback)};

    SymSearch(
        handle, module_base, 0, SymTagPublicSymbol, "*", 0,
        [](PSYMBOL_INFO info /*pSymInfo*/, ULONG /*SymbolSize*/, PVOID user_context /*UserContext*/) -> BOOL {
            auto *ctx = static_cast<UserContext *>(user_context);
            if (!(ctx->callback)(info->Name, info->Address - ctx->module_base, info->Flags)) {
                return FALSE;
            }
            return TRUE;
        },
        &user_context, SYMSEARCH_GLOBALSONLY);

    SymCleanup(handle);
}
}  // namespace

namespace endstone::runtime {
namespace hook::details {
const std::unordered_map<std::string, void *> &get_detours()
{
    static std::unordered_map<std::string, void *> detours;
    if (!detours.empty()) {
        return detours;
    }

    auto *module_base = get_module_base();
    const auto module_pathname = get_module_pathname();

    enumerate_symbols(  //
        module_pathname.c_str(),
        [&](const std::string &name, std::size_t offset, std::uint32_t flags) -> bool {
            if (flags & SYMFLAG_EXPORT) {
                auto *detour = static_cast<char *>(module_base) + offset;
                detours.emplace(name, detour);
            }
            return true;
        },
        false);  // set load_symbol to false so symbols are limited to the export table

    detours.erase("endstone_get_server");
    return detours;
}
}  // namespace hook::details

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

std::size_t get_executable_size()
{
    static std::size_t size = []() {
        MODULEINFO mi = {nullptr};
        if (!GetModuleInformation(GetCurrentProcess(), get_module_handle(nullptr), &mi, sizeof(mi))) {
            throw std::system_error(static_cast<int>(GetLastError()), std::system_category(),
                                    "GetModuleInformation failed");
        }
        return static_cast<std::size_t>(mi.SizeOfImage);
    }();
    return size;
}

namespace {
int stdin_fd = -1;
int null_fd = -1;
}  // namespace

void stdin_save()
{
    stdin_fd = _dup(_fileno(stdin));
}

void stdin_close()
{
    null_fd = _open("NUL", _O_RDONLY);
    _dup2(null_fd, _fileno(stdin));
    std::cin.clear();
    std::wcin.clear();
}

void stdin_restore()
{
    if (stdin_fd < 0) {
        return;
    }
    _dup2(stdin_fd, _fileno(stdin));
    _close(stdin_fd);
    stdin_fd = -1;

    if (null_fd >= 0) {
        _close(null_fd);
        null_fd = -1;
    }
    std::cin.clear();
    std::wcin.clear();
    std::clearerr(stdin);
}
}  // namespace endstone::runtime

#endif
