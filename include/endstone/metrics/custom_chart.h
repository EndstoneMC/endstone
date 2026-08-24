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

#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

#include "endstone/json.h"

namespace endstone {

/**
 * Represents a custom chart.
 */
class CustomChart {
public:
    /**
     * Creates a chart with the given bStats chart id.
     *
     * @param chart_id the id of the chart
     * @throws std::invalid_argument if chart_id is empty
     */
    explicit CustomChart(std::string chart_id) : chart_id_(std::move(chart_id))
    {
        if (chart_id_.empty()) {
            throw std::invalid_argument("chart_id cannot be empty");
        }
    }

    CustomChart(const CustomChart &) = delete;
    CustomChart &operator=(const CustomChart &) = delete;
    CustomChart(CustomChart &&) = delete;
    CustomChart &operator=(CustomChart &&) = delete;
    virtual ~CustomChart() = default;

    /**
     * Gets the id of this chart.
     *
     * @return the id of this chart
     */
    [[nodiscard]] const std::string &getChartId() const noexcept { return chart_id_; }

    /**
     * Gets the data for this chart.
     *
     * This is called on the primary server thread. Returning no value omits the chart from the submission.
     *
     * @return the chart data, or no value to skip this chart
     */
    [[nodiscard]] virtual std::optional<JsonObject> getChartData() = 0;

private:
    std::string chart_id_;
};
}  // namespace endstone
