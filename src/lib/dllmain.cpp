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

#include "lib/hook/hook.h"
#include "pybind/pybind.h"

[[maybe_unused]] std::unique_ptr<py::scoped_interpreter> gInterpreter;
[[maybe_unused]] std::unique_ptr<py::gil_scoped_release> gRelease;
[[maybe_unused]] std::unique_ptr<Endstone::Hook::HookManager> gHookManager;

[[maybe_unused]] BOOL WINAPI DllMain(_In_ HINSTANCE module,  // handle to DLL module
                                     _In_ DWORD reason,      // reason for calling function
                                     _In_ LPVOID reserved)   // reserved
{
    // Perform actions based on the reason for calling.
    switch (reason) {
    case DLL_PROCESS_ATTACH: {
        try {
            // Set both input and output code page to be utf-8
            SetConsoleCP(65001);
            SetConsoleOutputCP(65001);

            // Initialize python interpreter and release the GIL
            gInterpreter = std::make_unique<py::scoped_interpreter>();
            py::module_::import("threading");  // https://github.com/pybind/pybind11/issues/2197
            gRelease = std::make_unique<py::gil_scoped_release>();

            // Initialize hook manager
            gHookManager = std::make_unique<Endstone::Hook::HookManager>(module);
            break;
        }
        catch (const std::exception &e) {
            printf("LibEndstone loads failed.\n");
            if (const auto *se = dynamic_cast<const std::system_error *>(&e)) {
                printf("%s, error code: %d.\n", se->what(), se->code().value());
            }
            else {
                printf("%s\n", e.what());
            }
            return FALSE;  // Return FALSE to fail DLL load.
        }
    }
    case DLL_PROCESS_DETACH: {
        if (reserved != nullptr) {
            break;  // do not do cleanup if process termination scenario
        }

        gRelease.reset();  // Ensure the GIL is re-acquired.
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    default:
        // Do thread-specific initialization or cleanup.
        break;
    }

    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

#endif  // _WIN32
