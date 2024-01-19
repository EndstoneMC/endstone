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
#include <spdlog/spdlog.h>
namespace py = pybind11;

#include "endstone_core/endstone_server.h"
#include "endstone_runtime/hook.h"
#include "endstone_runtime/python_plugin_loader.h"

#if defined(__GNUC__) || defined(__clang__)
#define ENDSTONE_RUNTIME_CTOR __attribute__((constructor))
#else
#define ENDSTONE_RUNTIME_CTOR
#endif

std::unique_ptr<py::scoped_interpreter> gInterpreter;
std::unique_ptr<py::gil_scoped_release> gRelease;

ENDSTONE_RUNTIME_CTOR int main()
{
    try {
        gInterpreter = std::make_unique<py::scoped_interpreter>();
        auto &server = EndstoneServer::getInstance();
        server.getLogger().info("Initialising...");
        server.getPluginManager().registerLoader(std::make_unique<PythonPluginLoader>(server));
        gRelease = std::make_unique<py::gil_scoped_release>();

        endstone::hook::install();
        return 0;
    }
    catch (const std::exception &e) {
        spdlog::error("An exception occurred while attaching Endstone runtime to the process.");
        spdlog::error("{}", e.what());
        std::terminate();
    }
}

#ifdef _WIN32
#include <Windows.h>

[[maybe_unused]] BOOL WINAPI DllMain(_In_ HINSTANCE /*module*/,  // handle to DLL module
                                     _In_ DWORD reason,          // reason for calling function
                                     _In_ LPVOID reserved)       // reserved
{
    switch (reason) {
    case DLL_PROCESS_ATTACH: {
        main();
        break;
    }
    case DLL_PROCESS_DETACH: {
        std::exit(0);  // TODO: without this, the server won't exit on Windows for unknown reason.
        break;
    }
    default:
        break;
    }
    return TRUE;
}
#endif
