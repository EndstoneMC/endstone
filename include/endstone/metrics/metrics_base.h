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

namespace endstone {

class CustomChart;

/**
 * Collects and submits the data behind a Metrics instance.
 *
 * Plugins hold a Metrics rather than implementing this interface.
 */
class MetricsBase {
public:
    MetricsBase() = default;
    MetricsBase(const MetricsBase &) = delete;
    MetricsBase &operator=(const MetricsBase &) = delete;
    virtual ~MetricsBase() = default;

    /**
     * Adds a custom chart.
     *
     * @param chart the chart to add
     */
    virtual void addCustomChart(std::unique_ptr<CustomChart> chart) = 0;

    /**
     * Stops collecting and submitting data.
     */
    virtual void shutdown() noexcept = 0;
};
}  // namespace endstone
