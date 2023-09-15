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

#include "endstone/endstone.h"

#include "endstone/common.h"
#include "pybind/pybind.h"

std::string Endstone::getVersion()
{
    static std::string version = []() -> std::string {
        py::gil_scoped_acquire lock{};
        auto module = py::module_::import("endstone");
        auto version = module.attr("__version__");
        return py::cast<std::string>(version);
    }();

    return version;
}

[[maybe_unused]] std::string Endstone::getMinecraftVersion()
{
    static std::string version = []() -> std::string {
        py::gil_scoped_acquire lock{};
        auto module = py::module_::import("endstone");
        auto version = module.attr("__minecraft__version__");
        return py::cast<std::string>(version);
    }();

    return version;
}

Server &Endstone::getServer()
{
    return *server_;
}

void Endstone::setServer(std::unique_ptr<Server> server)
{
    if (server_) {
        throw std::runtime_error("Server singleton is already set!");
    }

    server_ = std::move(server);
    server_->getLogger()->info("Endstone Version: {}", Endstone::getVersion().c_str());
}

std::unique_ptr<Server> Endstone::server_ = nullptr;
