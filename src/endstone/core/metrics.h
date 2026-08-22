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
#include <unordered_map>
#include <vector>

#include <pybind11/pybind11.h>

#include "endstone/metrics/base.h"
#include "endstone/server.h"

namespace endstone::core {
class EndstoneMetrics {
public:
    EndstoneMetrics(Server &server);
    ~EndstoneMetrics();

private:
    Server &server_;
    pybind11::object obj_;
};

/** Holds every metrics the server has created for a plugin, so none of them outlives it. */
class PluginMetricsRegistry {
public:
    [[nodiscard]] NotNull<MetricsBase> create(Plugin &plugin, int service_id);
    void retire(const Plugin &plugin);

private:
    std::unordered_map<const Plugin *, std::vector<std::shared_ptr<MetricsBase>>> metrics_;
};

}  // namespace endstone::core
