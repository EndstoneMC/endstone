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
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

namespace endstone::metrics {
using StringValues = std::unordered_map<std::string, int>;
using DrilldownValues = std::unordered_map<std::string, StringValues>;
using BarValues = std::unordered_map<std::string, std::vector<int>>;
using ChartValue = std::variant<std::string, int, StringValues, DrilldownValues, BarValues>;
using ChartData = std::unordered_map<std::string, ChartValue>;

using SimplePieCallback = std::function<std::optional<std::string>()>;
using AdvancedPieCallback = std::function<std::optional<StringValues>()>;
using DrilldownPieCallback = std::function<std::optional<DrilldownValues>()>;
using SimpleBarChartCallback = std::function<std::optional<StringValues>()>;
using AdvancedBarChartCallback = std::function<std::optional<BarValues>()>;
using SingleLineChartCallback = std::function<int()>;
using MultiLineChartCallback = std::function<std::optional<StringValues>()>;
using CustomChartCallback = std::function<std::optional<ChartData>()>;

class CustomChart;

namespace detail {
class ChartVisitor {
public:
    virtual ~ChartVisitor() = default;

    virtual void visitSimplePie(std::string chart_id, SimplePieCallback callback) = 0;
    virtual void visitAdvancedPie(std::string chart_id, AdvancedPieCallback callback) = 0;
    virtual void visitDrilldownPie(std::string chart_id, DrilldownPieCallback callback) = 0;
    virtual void visitSimpleBarChart(std::string chart_id, SimpleBarChartCallback callback) = 0;
    virtual void visitAdvancedBarChart(std::string chart_id, AdvancedBarChartCallback callback) = 0;
    virtual void visitSingleLineChart(std::string chart_id, SingleLineChartCallback callback) = 0;
    virtual void visitMultiLineChart(std::string chart_id, MultiLineChartCallback callback) = 0;
    virtual void visitCustomChart(std::string chart_id, CustomChartCallback callback) = 0;
};

void dispatchChart(CustomChart &chart, ChartVisitor &visitor);
}  // namespace detail

/**
 * Represents a custom metrics chart.
 *
 * Chart callbacks are collected on the primary server thread. Empty callback results omit the chart.
 */
class CustomChart {
public:
    using StringValues = endstone::metrics::StringValues;
    using DrilldownValues = endstone::metrics::DrilldownValues;
    using BarValues = endstone::metrics::BarValues;
    using ChartValue = endstone::metrics::ChartValue;
    using ChartData = endstone::metrics::ChartData;

    /**
     * Creates a chart with the supplied bStats identifier.
     *
     * @throws std::invalid_argument if chart_id is empty.
     */
    explicit CustomChart(std::string chart_id) : chart_id_(std::move(chart_id))
    {
        if (chart_id_.empty()) {
            throw std::invalid_argument("chart_id cannot be empty");
        }
    }

    CustomChart(const CustomChart &) = delete;
    CustomChart &operator=(const CustomChart &) = delete;
    CustomChart(CustomChart &&) noexcept = default;
    CustomChart &operator=(CustomChart &&) noexcept = default;

    virtual ~CustomChart() = default;

    [[nodiscard]] const std::string &getChartId() const noexcept { return chart_id_; }

protected:
    virtual std::optional<ChartData> getChartData() = 0;

private:
    friend void detail::dispatchChart(CustomChart &, detail::ChartVisitor &);

    virtual void dispatch(detail::ChartVisitor &visitor)
    {
        visitor.visitCustomChart(getChartId(), [this] { return getChartData(); });
    }

    std::string chart_id_;
};
}  // namespace endstone::metrics
