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

#include <chrono>
#include <cstdlib>
#include <exception>

#include <pybind11/embed.h>
#include <spdlog/spdlog.h>

#include "endstone/core/devtools/devtools.h"
#include "endstone/core/logger_factory.h"
#include "endstone/core/server.h"
#include "endstone/runtime/hook.h"
#include "endstone/runtime/runtime.h"

namespace py = pybind11;

int init()
{
    try {
        spdlog::flush_every(std::chrono::seconds(5));
        endstone::hook::install();
        return 0;
    }
    catch (const std::exception &e) {
        const auto &logger = endstone::core::LoggerFactory::getLogger("Endstone");
        logger.error("An exception occurred while installing hooks.");
        logger.error("{}", e.what());
        return 4;
    }
}

#ifdef _WIN32
#include <windows.h>

[[maybe_unused]] BOOL WINAPI DllMain(_In_ HINSTANCE module,     // handle to DLL module
                                     _In_ DWORD reason,         // reason for calling function
                                     _In_ LPVOID /*reserved*/)  // reserved
{
    switch (reason) {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(module);
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        GetConsoleMode(console, &mode);
        SetConsoleMode(console, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
        init();
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
#else
__attribute__((constructor)) int main()
{
    unsetenv("LD_PRELOAD");
    return init();
}
#endif

extern "C" [[maybe_unused]] ENDSTONE_EXPORT void *endstone_get_server()
{
    return &entt::locator<endstone::core::EndstoneServer>::value();
}
