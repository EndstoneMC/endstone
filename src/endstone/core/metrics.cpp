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

#include <memory>
#include <stdexcept>
#include <string>

#include <pybind11/embed.h>

#include "endstone/metrics/custom_chart.h"

namespace py = pybind11;

namespace endstone::core {

Metrics::~Metrics()
{
    Metrics::shutdown();
}

void Metrics::addCustomChart(std::unique_ptr<CustomChart> chart)
{
    if (!chart) {
        throw std::invalid_argument("chart cannot be null");
    }
    if (!obj_) {
        return;
    }
    py::gil_scoped_acquire gil{};
    try {
        obj_.attr("add_custom_chart")(py::cast(chart.release(), py::return_value_policy::take_ownership));
    }
    catch (std::exception &e) {
        throw std::runtime_error(std::string("Unable to add metrics chart: ") + e.what());
    }
}

void Metrics::shutdown() noexcept
{
    if (!obj_) {
        return;
    }
    py::gil_scoped_acquire gil{};
    try {
        auto shutdown = obj_.attr("shutdown");
        (void)shutdown();
    }
    catch (std::exception &) {
    }
    obj_ = py::object();
}

}  // namespace endstone::core
