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

#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "endstone/metrics/custom_chart.h"

namespace endstone {

/** A bStats bar chart with a single bar per category. */
class SimpleBarChart : public CustomChart {
public:
    using Callback = std::function<std::optional<StringValues>()>;

    SimpleBarChart(std::string chart_id, Callback get_values)
        : CustomChart(std::move(chart_id)), get_values_(std::move(get_values))
    {
    }

    [[nodiscard]] std::optional<JsonObject> getChartData() override
    {
        const auto map_values = get_values_();
        if (!map_values || map_values->empty()) {
            return std::nullopt;
        }
        JsonObject values;
        for (const auto &[key, value] : *map_values) {
            values[key] = JsonArray{value};
        }
        return JsonObject{{"values", std::move(values)}};
    }

private:
    Callback get_values_;
};
}  // namespace endstone
