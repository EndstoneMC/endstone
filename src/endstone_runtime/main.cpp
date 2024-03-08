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

#include <exception>

#include <pybind11/embed.h>

#include "endstone/detail/hook.h"
#include "endstone/detail/logger_factory.h"

#if __GNUC__
#define ENDSTONE_RUNTIME_CTOR __attribute__((constructor))
#else
#define ENDSTONE_RUNTIME_CTOR
#endif

namespace py = pybind11;

ENDSTONE_RUNTIME_CTOR int main()
{
    auto &logger = endstone::detail::LoggerFactory::getLogger("EndstoneRuntime");
    try {
        logger.info("Initialising...");
        py::initialize_interpreter();
        py::module_::import("threading");  // https://github.com/pybind/pybind11/issues/2197
        py::gil_scoped_release release{};
        release.disarm();

        // Install hooks
        endstone::detail::hook::install();
        return 0;
    }
    catch (const std::exception &e) {
        logger.error("An exception occurred while initialising Endstone runtime.");
        logger.error("{}", e.what());
        throw e;
    }
}

#ifdef _WIN32
#include <Windows.h>

[[maybe_unused]] BOOL WINAPI DllMain(_In_ HINSTANCE /*module*/,  // handle to DLL module
                                     _In_ DWORD reason,          // reason for calling function
                                     _In_ LPVOID /*reserved*/)   // reserved
{
    switch (reason) {
    case DLL_PROCESS_ATTACH: {
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
        main();
        break;
    }
    case DLL_PROCESS_DETACH: {
        break;
    }
    default:
        break;
    }
    return TRUE;
}
#endif
