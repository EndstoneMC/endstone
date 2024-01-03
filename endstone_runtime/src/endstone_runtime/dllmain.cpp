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

#ifdef _WIN32

#include <Windows.h>

#include <cstdio>
#include <system_error>

#include <spdlog/spdlog.h>

[[maybe_unused]] BOOL WINAPI DllMain(_In_ HINSTANCE module,  // handle to DLL module
                                     _In_ DWORD reason,      // reason for calling function
                                     _In_ LPVOID reserved)   // reserved
{
    // Perform actions based on the reason for calling.
    switch (reason) {
    case DLL_PROCESS_ATTACH: {
        try {
            SetConsoleCP(65001);
            SetConsoleOutputCP(65001);
            spdlog::info("Hello World!!");
            // TODO: hook methods
            break;
        }
        catch (const std::exception &e) {
            spdlog::error("Endstone runtime loads failed.");
            if (const auto *se = dynamic_cast<const std::system_error *>(&e)) {
                spdlog::error("{}, error code: {}", se->what(), se->code().value());
            }
            else {
                spdlog::error("{}", e.what());
            }
            return FALSE;  // Return FALSE to fail DLL load.
        }
    }
    case DLL_PROCESS_DETACH:
        if (reserved != nullptr) {
            break;  // do not do cleanup if process termination scenario
        }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    default:
        break;
    }

    return TRUE;
}

#elif __GNUC__

#include <cstdio>
#include <system_error>

#include <spdlog/spdlog.h>

__attribute__((constructor)) int main()
{
    spdlog::info("Hello World!!");
    return 0;
}

#endif  // _WIN32
