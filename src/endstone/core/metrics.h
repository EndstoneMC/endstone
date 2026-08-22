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

#pragma once

#include <memory>
#include <utility>

#include <pybind11/pybind11.h>

#include "endstone/metrics/base.h"

namespace endstone::core {

/** A metrics reporter backed by a Python class, named by the module it lives in. */
class Metrics final : public MetricsBase {
public:
    template <typename... Args>
    Metrics(const char *module, const char *name, Args &&...args)
    {
        pybind11::gil_scoped_acquire gil{};
        obj_ = pybind11::module_::import(module).attr(name)(std::forward<Args>(args)...);
    }

    ~Metrics() override;
    void addCustomChart(std::unique_ptr<CustomChart> chart) override;
    void shutdown() noexcept override;

private:
    pybind11::object obj_;
};

}  // namespace endstone::core
