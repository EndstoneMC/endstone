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

/** A bStats pie chart with one value. */
class SimplePie : public CustomChart {
public:
    using ValueCallback = SimplePieCallback;

    SimplePie(std::string chart_id, ValueCallback get_value)
        : CustomChart(std::move(chart_id)), get_value_(std::move(get_value))
    {
    }

private:
    std::optional<ChartData> getChartData() override { return std::nullopt; }
    void dispatch(detail::ChartVisitor &visitor) override { visitor.visitSimplePie(getChartId(), get_value_); }

    ValueCallback get_value_;
};
}  // namespace endstone::metrics
