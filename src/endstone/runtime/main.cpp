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
#include "endstone/runtime/hook.h"

namespace py = pybind11;

int init()
{
    spdlog::flush_every(std::chrono::seconds(5));
    const auto &logger = endstone::core::LoggerFactory::getLogger("EndstoneRuntime");
    try {
        logger.info("Initialising...");

        // Initialise an isolated Python environment to avoid installing signal handlers
        // https://docs.python.org/3/c-api/init_config.html#init-isolated-conf
        PyConfig config;
        PyConfig_InitIsolatedConfig(&config);
        config.isolated = 0;
        config.use_environment = 1;
        config.install_signal_handlers = 0;
        py::initialize_interpreter(&config);
        py::module_::import("threading");  // https://github.com/pybind/pybind11/issues/2197
        py::module_::import("numpy");      // https://github.com/numpy/numpy/issues/24833
        py::gil_scoped_release release{};
        release.disarm();

        // Install hooks
        endstone::hook::install();

#ifdef ENDSTONE_WITH_DEVTOOLS
        // Create devtools window
        auto thread = std::thread(&endstone::devtools::render);
        thread.detach();
#endif
        return 0;
    }
    catch (const std::exception &e) {
        logger.error("An exception occurred while initialising Endstone runtime.");
        logger.error("{}", e.what());
        throw e;
    }
}

#ifdef _WIN32
#include <windows.h>

[[maybe_unused]] BOOL WINAPI DllMain(_In_ HINSTANCE /*module*/,  // handle to DLL module
                                     _In_ DWORD reason,          // reason for calling function
                                     _In_ LPVOID /*reserved*/)   // reserved
{
    switch (reason) {
    case DLL_PROCESS_ATTACH: {
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
