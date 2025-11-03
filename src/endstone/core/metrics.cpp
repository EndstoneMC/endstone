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

#include "endstone/core/metrics.h"

#include <pybind11/embed.h>

namespace py = pybind11;

namespace endstone::core {
EndstoneMetrics::EndstoneMetrics(Server &server) : server_(server)
{
    try {
        py::gil_scoped_acquire gil{};
        const auto module = py::module_::import("endstone._metrics");
        const auto cls = module.attr("EndstoneMetrics");
        obj_ = cls(std::ref(server));
    }
    catch (std::exception &e) {
        server_.getLogger().warning("Unable to start metrics: {}", e.what());
    }
}

EndstoneMetrics::~EndstoneMetrics()
{
    if (!obj_) {
        return;
    }
    py::gil_scoped_acquire gil{};
    try {
        auto shutdown = obj_.attr("shutdown");
        (void)shutdown();
    }
    catch (std::exception &e) {
        server_.getLogger().warning("Unable to shutdown metrics: {}", e.what());
    }
    obj_ = py::object();
}
}  // namespace endstone::core
