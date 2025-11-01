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

#include "bedrock/server/dedicated_server.h"

#include <pybind11/embed.h>

#include "endstone/core/logger_factory.h"
#include "endstone/runtime/hook.h"

#if defined(_WIN32)
#include <io.h>
#define DUP    _dup
#define DUP2   _dup2
#define CLOSE  _close
#define FILENO _fileno
#else
#include <unistd.h>
#define DUP    dup
#define DUP2   dup2
#define CLOSE  close
#define FILENO fileno
#endif

namespace py = pybind11;

DedicatedServer::StartResult DedicatedServer::start(const std::string &sessionID,
                                                    const Bedrock::ActivationArguments &args)
{
    // Save the current stdin, as it will be altered after the initialisation of python interpreter
    const auto old_stdin = DUP(FILENO(stdin));

    // Initialise an isolated Python environment to avoid installing signal handlers
    // https://docs.python.org/3/c-api/init_config.html#init-isolated-conf
    PyConfig config;
    PyConfig_InitIsolatedConfig(&config);
    config.isolated = 0;
    config.use_environment = 1;
    py::scoped_interpreter interpreter(&config);
    py::module_::import("threading");  // https://github.com/pybind/pybind11/issues/2197
    py::module_::import("numpy");      // https://github.com/numpy/numpy/issues/24833

    // Release the GIL
    py::gil_scoped_release release{};

    // Restore the stdin
    std::fflush(stdin);
    DUP2(old_stdin, FILENO(stdin));
    CLOSE(old_stdin);

    auto result = ENDSTONE_HOOK_CALL_ORIGINAL(&DedicatedServer::start, this, sessionID, args);

    // Clean up
    entt::locator<endstone::core::EndstoneServer>::reset();
    return result;
}
