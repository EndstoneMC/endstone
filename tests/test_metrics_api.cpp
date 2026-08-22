// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include <gtest/gtest.h>

#include "endstone/metrics/advanced_bar_chart.h"
#include "endstone/metrics/advanced_pie.h"
#include "endstone/metrics/drilldown_pie.h"
#include "endstone/metrics/metrics.h"
#include "endstone/metrics/multi_line_chart.h"
#include "endstone/metrics/simple_bar_chart.h"
#include "endstone/metrics/simple_pie.h"
#include "endstone/metrics/single_line_chart.h"
#include "endstone/plugin/plugin.h"

namespace endstone::metrics {
namespace {

static_assert(!std::is_copy_constructible_v<MetricsRegistration>);
static_assert(!std::is_copy_assignable_v<MetricsRegistration>);
static_assert(std::is_same_v<decltype(std::declval<MetricsService &>().registerPlugin(std::declval<Plugin &>(),
                                                                                      std::declval<int>())),
                             std::shared_ptr<MetricsRegistration>>);

class ChartCollector final : public detail::ChartVisitor {
public:
    static std::optional<ChartData> collect(CustomChart &chart)
    {
        ChartCollector visitor;
        detail::dispatchChart(chart, visitor);
        return std::move(visitor.data_);
    }

    void visitSimplePie([[maybe_unused]] std::string chart_id, SimplePieCallback callback) override
    {
        auto value = callback();
        if (value && !value->empty()) {
            data_ = ChartData{{"value", std::move(*value)}};
        }
    }

    void visitAdvancedPie([[maybe_unused]] std::string chart_id, AdvancedPieCallback callback) override
    {
        auto values = callback();
        if (!values) {
            return;
        }
        StringValues filtered;
        for (auto &[key, value] : *values) {
            if (value != 0) {
                filtered.emplace(key, value);
            }
        }
        if (!filtered.empty()) {
            data_ = ChartData{{"values", std::move(filtered)}};
        }
    }

    void visitDrilldownPie([[maybe_unused]] std::string chart_id, DrilldownPieCallback callback) override
    {
        auto values = callback();
        if (!values) {
            return;
        }
        DrilldownValues filtered;
        for (auto &[key, nested] : *values) {
            if (!nested.empty()) {
                filtered.emplace(key, std::move(nested));
            }
        }
        if (!filtered.empty()) {
            data_ = ChartData{{"values", std::move(filtered)}};
        }
    }

    void visitSimpleBarChart([[maybe_unused]] std::string chart_id, SimpleBarChartCallback callback) override
    {
        auto values = callback();
        if (!values || values->empty()) {
            return;
        }
        BarValues bars;
        for (auto &[key, value] : *values) {
            bars.emplace(key, std::vector<int>{value});
        }
        data_ = ChartData{{"values", std::move(bars)}};
    }

    void visitAdvancedBarChart([[maybe_unused]] std::string chart_id, AdvancedBarChartCallback callback) override
    {
        auto values = callback();
        if (!values) {
            return;
        }
        BarValues filtered;
        for (auto &[key, values_for_bar] : *values) {
            if (!values_for_bar.empty()) {
                filtered.emplace(key, std::move(values_for_bar));
            }
        }
        if (!filtered.empty()) {
            data_ = ChartData{{"values", std::move(filtered)}};
        }
    }

    void visitSingleLineChart([[maybe_unused]] std::string chart_id, SingleLineChartCallback callback) override
    {
        auto value = callback();
        if (value != 0) {
            data_ = ChartData{{"value", value}};
        }
    }

    void visitMultiLineChart([[maybe_unused]] std::string chart_id, MultiLineChartCallback callback) override
    {
        auto values = callback();
        if (!values) {
            return;
        }
        StringValues filtered;
        for (auto &[key, value] : *values) {
            if (value != 0) {
                filtered.emplace(key, value);
            }
        }
        if (!filtered.empty()) {
            data_ = ChartData{{"values", std::move(filtered)}};
        }
    }

    void visitCustomChart([[maybe_unused]] std::string chart_id, CustomChartCallback callback) override
    {
        auto values = callback();
        if (values && !values->empty()) {
            data_ = std::move(*values);
        }
    }

private:
    std::optional<ChartData> data_;
};

class TestChart final : public CustomChart {
public:
    TestChart(std::string chart_id, ChartData data) : CustomChart(std::move(chart_id)), data_(std::move(data)) {}

protected:
    std::optional<ChartData> getChartData() override { return data_; }

private:
    ChartData data_;
};

TEST(MetricsChartsTest, BuiltInChartsReturnExpectedData)
{
    auto collect = &ChartCollector::collect;

    SimplePie simple_pie("simple_pie", []() -> std::optional<std::string> { return "vanilla"; });
    EXPECT_EQ(collect(simple_pie), (ChartData{{"value", std::string("vanilla")}}));

    AdvancedPie advanced_pie(
        "advanced_pie", []() -> std::optional<StringValues> { return StringValues{{"survival", 3}, {"creative", 2}}; });
    EXPECT_EQ(collect(advanced_pie), (ChartData{{"values", StringValues{{"survival", 3}, {"creative", 2}}}}));

    DrilldownPie drilldown_pie("drilldown_pie", []() -> std::optional<DrilldownValues> {
        return DrilldownValues{{"overworld", StringValues{{"stone", 4}, {"dirt", 2}}},
                               {"nether", StringValues{{"netherrack", 5}}}};
    });
    EXPECT_EQ(collect(drilldown_pie),
              (ChartData{{"values", DrilldownValues{{"overworld", StringValues{{"stone", 4}, {"dirt", 2}}},
                                                    {"nether", StringValues{{"netherrack", 5}}}}}}));

    SimpleBarChart simple_bar_chart(
        "simple_bar_chart", []() -> std::optional<StringValues> { return StringValues{{"first", 4}, {"second", 0}}; });
    EXPECT_EQ(collect(simple_bar_chart), (ChartData{{"values", BarValues{{"first", {4}}, {"second", {0}}}}}));

    AdvancedBarChart advanced_bar_chart("advanced_bar_chart", []() -> std::optional<BarValues> {
        return BarValues{{"first", {1, 2}}, {"second", {0, -1}}};
    });
    EXPECT_EQ(collect(advanced_bar_chart), (ChartData{{"values", BarValues{{"first", {1, 2}}, {"second", {0, -1}}}}}));

    SingleLineChart single_line_chart("single_line_chart", [] { return 7; });
    EXPECT_EQ(collect(single_line_chart), (ChartData{{"value", 7}}));

    MultiLineChart multi_line_chart(
        "multi_line_chart", []() -> std::optional<StringValues> { return StringValues{{"first", 4}, {"second", 2}}; });
    EXPECT_EQ(collect(multi_line_chart), (ChartData{{"values", StringValues{{"first", 4}, {"second", 2}}}}));
}

TEST(MetricsChartsTest, BuiltInChartsFilterEmptyAndZeroValues)
{
    SimplePie simple_pie_null("simple_pie_null", []() -> std::optional<std::string> { return std::nullopt; });
    EXPECT_FALSE(ChartCollector::collect(simple_pie_null).has_value());
    SimplePie simple_pie_empty("simple_pie_empty", []() -> std::optional<std::string> { return std::string{}; });
    EXPECT_FALSE(ChartCollector::collect(simple_pie_empty).has_value());

    AdvancedPie advanced_pie_null("advanced_pie_null", []() -> std::optional<StringValues> { return std::nullopt; });
    EXPECT_FALSE(ChartCollector::collect(advanced_pie_null).has_value());
    AdvancedPie advanced_pie_empty("advanced_pie_empty",
                                   []() -> std::optional<StringValues> { return StringValues{}; });
    EXPECT_FALSE(ChartCollector::collect(advanced_pie_empty).has_value());
    AdvancedPie advanced_pie_zero("advanced_pie_zero", []() -> std::optional<StringValues> {
        return StringValues{{"zero", 0}, {"positive", 2}};
    });
    EXPECT_EQ(ChartCollector::collect(advanced_pie_zero), (ChartData{{"values", StringValues{{"positive", 2}}}}));
    AdvancedPie advanced_pie_all_zero("advanced_pie_all_zero", []() -> std::optional<StringValues> {
        return StringValues{{"first", 0}, {"second", 0}};
    });
    EXPECT_FALSE(ChartCollector::collect(advanced_pie_all_zero).has_value());

    DrilldownPie drilldown_pie_null("drilldown_pie_null",
                                    []() -> std::optional<DrilldownValues> { return std::nullopt; });
    EXPECT_FALSE(ChartCollector::collect(drilldown_pie_null).has_value());
    DrilldownPie drilldown_pie_empty("drilldown_pie_empty", []() -> std::optional<DrilldownValues> {
        return DrilldownValues{{"empty", StringValues{}}};
    });
    EXPECT_FALSE(ChartCollector::collect(drilldown_pie_empty).has_value());
    DrilldownPie drilldown_pie_filtered("drilldown_pie_filtered", []() -> std::optional<DrilldownValues> {
        return DrilldownValues{{"empty", StringValues{}}, {"values", StringValues{{"zero", 0}}}};
    });
    EXPECT_EQ(ChartCollector::collect(drilldown_pie_filtered),
              (ChartData{{"values", DrilldownValues{{"values", StringValues{{"zero", 0}}}}}}));

    SimpleBarChart simple_bar_chart_null("simple_bar_chart_null",
                                         []() -> std::optional<StringValues> { return std::nullopt; });
    EXPECT_FALSE(ChartCollector::collect(simple_bar_chart_null).has_value());
    SimpleBarChart simple_bar_chart_empty("simple_bar_chart_empty",
                                          []() -> std::optional<StringValues> { return StringValues{}; });
    EXPECT_FALSE(ChartCollector::collect(simple_bar_chart_empty).has_value());

    AdvancedBarChart advanced_bar_chart_null("advanced_bar_chart_null",
                                             []() -> std::optional<BarValues> { return std::nullopt; });
    EXPECT_FALSE(ChartCollector::collect(advanced_bar_chart_null).has_value());
    AdvancedBarChart advanced_bar_chart_empty("advanced_bar_chart_empty",
                                              []() -> std::optional<BarValues> { return BarValues{{"empty", {}}}; });
    EXPECT_FALSE(ChartCollector::collect(advanced_bar_chart_empty).has_value());
    AdvancedBarChart advanced_bar_chart_filtered("advanced_bar_chart_filtered", []() -> std::optional<BarValues> {
        return BarValues{{"empty", {}}, {"values", {0, -1}}};
    });
    EXPECT_EQ(ChartCollector::collect(advanced_bar_chart_filtered),
              (ChartData{{"values", BarValues{{"values", {0, -1}}}}}));

    SingleLineChart single_line_chart("single_line_chart", [] { return 0; });
    EXPECT_FALSE(ChartCollector::collect(single_line_chart).has_value());

    MultiLineChart multi_line_chart_null("multi_line_chart_null",
                                         []() -> std::optional<StringValues> { return std::nullopt; });
    EXPECT_FALSE(ChartCollector::collect(multi_line_chart_null).has_value());
    MultiLineChart multi_line_chart_empty("multi_line_chart_empty",
                                          []() -> std::optional<StringValues> { return StringValues{}; });
    EXPECT_FALSE(ChartCollector::collect(multi_line_chart_empty).has_value());
    MultiLineChart multi_line_chart_filtered("multi_line_chart_filtered", []() -> std::optional<StringValues> {
        return StringValues{{"zero", 0}, {"values", -2}};
    });
    EXPECT_EQ(ChartCollector::collect(multi_line_chart_filtered),
              (ChartData{{"values", StringValues{{"values", -2}}}}));
}

TEST(MetricsChartsTest, CustomChartCanProvideCustomData)
{
    const ChartData expected{{"custom", std::string("value")}, {"count", 3}};
    TestChart chart("custom_chart", expected);

    EXPECT_EQ(chart.getChartId(), "custom_chart");
    EXPECT_EQ(ChartCollector::collect(chart), expected);

    TestChart empty("empty_custom_chart", {});
    EXPECT_FALSE(ChartCollector::collect(empty).has_value());
}

TEST(MetricsChartsTest, EmptyChartIdIsRejected)
{
    EXPECT_THROW(SimplePie("", []() -> std::optional<std::string> { return "value"; }), std::invalid_argument);
}

TEST(MetricsChartsTest, CallbackExceptionsPropagateAtTheChartBoundary)
{
    SimplePie chart("throwing", []() -> std::optional<std::string> { throw std::runtime_error("callback failed"); });

    EXPECT_THROW(ChartCollector::collect(chart), std::runtime_error);
}

TEST(MetricsChartsTest, CallbackCaptureIsReleasedWithTheChart)
{
    auto state = std::make_shared<int>(1);
    std::weak_ptr<int> weak = state;
    {
        auto chart = std::make_unique<SimplePie>("lifetime", [state] { return std::to_string(*state); });
        state.reset();
        EXPECT_FALSE(weak.expired());
    }
    EXPECT_TRUE(weak.expired());
}

}  // namespace
}  // namespace endstone::metrics
