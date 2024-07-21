// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the license at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifdef ENDSTONE_RUNTIME_LOADER

#include <windows.h>

#include <cstdio>
#include <cstring>

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID /*reserved*/)
{
    switch (reason) {
    case DLL_PROCESS_ATTACH: {
        CHAR filename[MAX_PATH] = {0};
        GetModuleFileNameA(module, filename, MAX_PATH);

        CHAR dll_path[MAX_PATH] = {0};
        strcpy(dll_path, filename);
        char *last_slash = strrchr(dll_path, '\\');
        if (last_slash) {
            *(last_slash + 1) = '\0';  // Null-terminate the string at the slash
        }
        strcat(dll_path, "endstone_runtime.dll");

        if (LoadLibraryA(dll_path) == nullptr) {
            DWORD error_code = GetLastError();
            LPVOID buffer;
            DWORD len = FormatMessageA(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr,
                error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPTSTR>(&buffer), 0, nullptr);

            if (len) {
                printf("Failed to load DLL: %s, Error code: %lu, Message: %s\n", dll_path, error_code,
                       static_cast<LPCSTR>(buffer));
                LocalFree(buffer);
            }
            ExitProcess(error_code);
        }
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
    default:
        break;
    }
    return TRUE;
}

#endif
