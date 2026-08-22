// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include <functional>
#include <optional>
#include <string>
#include <utility>

#include "endstone/metrics/custom_chart.h"

namespace endstone::metrics {

/** A bStats pie chart with grouped named values. */
class DrilldownPie : public CustomChart {
public:
    using ValuesCallback = DrilldownPieCallback;

    DrilldownPie(std::string chart_id, ValuesCallback get_values)
        : CustomChart(std::move(chart_id)), get_values_(std::move(get_values))
    {
    }

private:
    std::optional<ChartData> getChartData() override { return std::nullopt; }
    void dispatch(detail::ChartVisitor &visitor) override { visitor.visitDrilldownPie(getChartId(), get_values_); }

    ValuesCallback get_values_;
};
}  // namespace endstone::metrics
