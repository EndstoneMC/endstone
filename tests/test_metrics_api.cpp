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

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include <gtest/gtest.h>

#include "endstone/metrics/metrics.h"

using endstone::AdvancedBarChart;
using endstone::AdvancedPie;
using endstone::BarValues;
using endstone::ChartData;
using endstone::CustomChart;
using endstone::DrilldownPie;
using endstone::DrilldownValues;
using endstone::MultiLineChart;
using endstone::SimpleBarChart;
using endstone::SimplePie;
using endstone::SingleLineChart;
using endstone::StringValues;

namespace {
class TestChart : public CustomChart {
public:
    using CustomChart::CustomChart;
    std::optional<ChartData> getChartData() override { return ChartData{{"custom", 7}}; }
};
}  // namespace

TEST(MetricsChartsTest, SimplePieReportsItsValue)
{
    SimplePie chart("id", [] { return std::optional<std::string>("value"); });
    const auto data = chart.getChartData();
    ASSERT_TRUE(data.has_value());
    EXPECT_EQ(std::get<std::string>(data->at("value")), "value");
}

TEST(MetricsChartsTest, SimplePieSkipsMissingAndEmptyValues)
{
    EXPECT_FALSE(SimplePie("id", [] { return std::nullopt; }).getChartData().has_value());
    EXPECT_FALSE(SimplePie("id", [] { return std::optional<std::string>(""); }).getChartData().has_value());
}

TEST(MetricsChartsTest, AdvancedPieDropsZeroEntries)
{
    AdvancedPie chart("id", [] { return StringValues{{"kept", 3}, {"dropped", 0}}; });
    const auto data = chart.getChartData();
    ASSERT_TRUE(data.has_value());
    const auto &values = std::get<StringValues>(data->at("values"));
    ASSERT_EQ(values.size(), 1U);
    EXPECT_EQ(values.at("kept"), 3);
}

TEST(MetricsChartsTest, AdvancedPieSkipsWhenEverythingIsZero)
{
    EXPECT_FALSE(AdvancedPie("id", [] { return StringValues{{"a", 0}}; }).getChartData().has_value());
    EXPECT_FALSE(AdvancedPie("id", [] { return std::nullopt; }).getChartData().has_value());
}

TEST(MetricsChartsTest, DrilldownPieDropsEmptyGroups)
{
    DrilldownPie chart("id", [] { return DrilldownValues{{"kept", {{"inner", 1}}}, {"dropped", {}}}; });
    const auto data = chart.getChartData();
    ASSERT_TRUE(data.has_value());
    const auto &values = std::get<DrilldownValues>(data->at("values"));
    ASSERT_EQ(values.size(), 1U);
    EXPECT_EQ(values.at("kept").at("inner"), 1);
}

TEST(MetricsChartsTest, SimpleBarChartWrapsEachValueInABar)
{
    SimpleBarChart chart("id", [] { return StringValues{{"a", 2}}; });
    const auto data = chart.getChartData();
    ASSERT_TRUE(data.has_value());
    EXPECT_EQ(std::get<BarValues>(data->at("values")).at("a"), std::vector{2});
}

TEST(MetricsChartsTest, AdvancedBarChartDropsEmptyBars)
{
    AdvancedBarChart chart("id", [] { return BarValues{{"kept", {1, 2}}, {"dropped", {}}}; });
    const auto data = chart.getChartData();
    ASSERT_TRUE(data.has_value());
    const auto &values = std::get<BarValues>(data->at("values"));
    ASSERT_EQ(values.size(), 1U);
    EXPECT_EQ(values.at("kept"), (std::vector{1, 2}));
}

TEST(MetricsChartsTest, SingleLineChartSkipsZero)
{
    const auto data = SingleLineChart("id", [] { return 5; }).getChartData();
    ASSERT_TRUE(data.has_value());
    EXPECT_EQ(std::get<int>(data->at("value")), 5);
    EXPECT_FALSE(SingleLineChart("id", [] { return 0; }).getChartData().has_value());
}

TEST(MetricsChartsTest, MultiLineChartDropsZeroLines)
{
    MultiLineChart chart("id", [] { return StringValues{{"kept", 4}, {"dropped", 0}}; });
    const auto data = chart.getChartData();
    ASSERT_TRUE(data.has_value());
    const auto &values = std::get<StringValues>(data->at("values"));
    ASSERT_EQ(values.size(), 1U);
    EXPECT_EQ(values.at("kept"), 4);
}

TEST(MetricsChartsTest, CustomChartProvidesItsOwnData)
{
    TestChart chart("id");
    EXPECT_EQ(chart.getChartId(), "id");
    const auto data = chart.getChartData();
    ASSERT_TRUE(data.has_value());
    EXPECT_EQ(std::get<int>(data->at("custom")), 7);
}

TEST(MetricsChartsTest, EmptyChartIdIsRejected)
{
    EXPECT_THROW((TestChart{""}), std::invalid_argument);
}

TEST(MetricsChartsTest, CallbackExceptionsPropagate)
{
    SimplePie chart("id", []() -> std::optional<std::string> { throw std::runtime_error("boom"); });
    EXPECT_THROW((void)chart.getChartData(), std::runtime_error);
}

TEST(MetricsChartsTest, CallbackCaptureIsReleasedWithTheChart)
{
    auto captured = std::make_shared<int>(1);
    const std::weak_ptr<int> observer = captured;
    {
        SimplePie chart("id", [captured = std::move(captured)] { return std::optional<std::string>("v"); });
        EXPECT_FALSE(observer.expired());
    }
    EXPECT_TRUE(observer.expired());
}
