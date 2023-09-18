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

#include "versioning.h"

#include <string>

#include "endstone_server.h"
#include "pybind/pybind.h"

[[maybe_unused]] std::string Versioning::getEndstoneVersion()
{
    try {
        py::gil_scoped_acquire lock{};
        auto module = py::module_::import("endstone");
        return py::cast<std::string>(module.attr("__version__"));
    }
    catch (std::exception &e) {
        EndstoneServer::getInstance().getLogger().warning("Could not get Endstone version: {}", e.what());
        return ENDSTONE_VERSION;
    }
}

[[maybe_unused]] std::string Versioning::getMinecraftVersion()
{
    try {
        py::gil_scoped_acquire lock{};
        auto module = py::module_::import("endstone");
        return py::cast<std::string>(module.attr("__minecraft_version__"));
    }
    catch (std::exception &e) {
        EndstoneServer::getInstance().getLogger().warning("Could not get Minecraft version: {}", e.what());
        return MINECRAFT_VERSION;
    }
}
