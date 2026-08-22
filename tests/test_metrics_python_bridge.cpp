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

#include <filesystem>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

#include "endstone/core/logger_factory.h"
#include "endstone/core/metrics.h"
#include "endstone/core/plugin/cpp_plugin_loader.h"
#include "endstone/core/plugin/plugin_manager.h"
#include "endstone/core/plugin/service_manager.h"
#include "endstone/core/scheduler/scheduler.h"
#include "endstone/metrics/metrics.h"
#include "mocks.h"

namespace fs = std::filesystem;
namespace py = pybind11;

using endstone::metrics::AdvancedBarChart;
using endstone::metrics::AdvancedPie;
using endstone::metrics::BarValues;
using endstone::metrics::DrilldownPie;
using endstone::metrics::DrilldownValues;
using endstone::metrics::Metrics;
using endstone::metrics::MultiLineChart;
using endstone::metrics::SimpleBarChart;
using endstone::metrics::SimplePie;
using endstone::metrics::SingleLineChart;
using endstone::metrics::StringValues;

namespace {

void initializePython()
{
    static const bool initialized = [] {
        if (!Py_IsInitialized()) {
            py::initialize_interpreter();
        }

        py::gil_scoped_acquire gil;
        auto sys = py::module_::import("sys");
        sys.attr("path").attr("insert")(0, ENDSTONE_PYTHON_MODULE_DIR);
        auto native_module = py::module_::import("_python");
        auto modules = sys.attr("modules").cast<py::dict>();
        auto package = py::module_::import("types").attr("ModuleType")("endstone");
        package.attr("__path__") = py::make_tuple((fs::path(ENDSTONE_SOURCE_DIR) / "endstone").string());
        modules["endstone"] = package;
        modules["endstone._python"] = native_module;

        auto items = py::list(modules.attr("items")());
        for (auto item : items) {
            auto pair = item.cast<py::tuple>();
            auto name = pair[0].cast<std::string>();
            if (name.starts_with("_python.")) {
                modules[py::str("endstone." + name)] = pair[1];
            }
        }
        sys.attr("path").attr("insert")(0, ENDSTONE_SOURCE_DIR);
        py::module_::import("endstone.metrics");
        return true;
    }();
    (void)initialized;
}

class PythonMetricsCapture {
public:
    PythonMetricsCapture()
    {
        module_ = py::module_::import("endstone.metrics");
        original_ = module_.attr("Metrics");
        charts_ = py::list();
        instance_ = py::module_::import("types").attr("SimpleNamespace")();
        instance_.attr("add_custom_chart") =
            py::cpp_function([charts = charts_](py::object chart) mutable { charts.append(std::move(chart)); });
        instance_.attr("shutdown") = py::cpp_function([charts = charts_]() mutable { charts.attr("clear")(); });
        module_.attr("Metrics") =
            py::cpp_function([instance = instance_](const py::object &, int) { return instance; });
    }

    ~PythonMetricsCapture()
    {
        if (!module_.is_none()) {
            try {
                module_.attr("Metrics") = std::move(original_);
            }
            catch (const py::error_already_set &) {
                PyErr_Clear();
            }
        }
    }

    [[nodiscard]] const py::list &charts() const { return charts_; }

private:
    py::object module_;
    py::object original_;
    py::object instance_;
    py::list charts_;
};

py::object chartData(const py::handle &chart)
{
    return chart.attr("get_chart_data")();
}

std::string chartId(const py::handle &chart)
{
    return chart.attr("chart_id").cast<std::string>();
}

class MetricsPythonBridgeTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        initializePython();
        ON_CALL(server_, getLogger())
            .WillByDefault(testing::ReturnRef(endstone::core::LoggerFactory::getLogger("MetricsBridgeTest")));
        ON_CALL(server_, isPrimaryThread()).WillByDefault(testing::Return(true));
        ON_CALL(server_, createMetrics(testing::_, testing::_))
            .WillByDefault([](endstone::Plugin &plugin, int service_id) {
                return endstone::core::createPluginMetrics(plugin, service_id);
            });
        scheduler_ = std::make_unique<endstone::core::EndstoneScheduler>(server_);
        ON_CALL(server_, getScheduler()).WillByDefault(testing::ReturnRef(*scheduler_));
        service_manager_ = std::make_unique<endstone::core::EndstoneServiceManager>();
        ON_CALL(server_, getServiceManager()).WillByDefault(testing::ReturnRef(*service_manager_));
        manager_ = std::make_unique<endstone::core::EndstonePluginManager>(server_);
        ON_CALL(server_, getPluginManager()).WillByDefault(testing::ReturnRef(*manager_));
        manager_->registerLoader(std::make_unique<endstone::core::CppPluginLoader>(server_));

#ifdef _WIN32
        const auto plugin_file = fs::current_path() / "plugins" / "endstone_test_plugin.dll";
#elif __linux__
        const auto plugin_file = fs::current_path() / "plugins" / "endstone_test_plugin.so";
#endif
        plugin_ = manager_->loadPlugin(plugin_file.string());
        ASSERT_NE(plugin_, nullptr);
        manager_->enablePlugin(*plugin_);
    }

    void TearDown() override
    {
        if (manager_ && plugin_) {
            manager_->disablePlugin(*plugin_);
            {
                py::gil_scoped_acquire gil;
                py::module_::import("gc").attr("collect")();
            }
            manager_->clearPlugins();
        }
        manager_.reset();
        scheduler_.reset();
        service_manager_.reset();
        plugin_ = nullptr;
    }

    testing::NiceMock<MockServer> server_;
    std::unique_ptr<endstone::core::EndstoneScheduler> scheduler_;
    std::unique_ptr<endstone::core::EndstoneServiceManager> service_manager_;
    std::unique_ptr<endstone::core::EndstonePluginManager> manager_;
    endstone::Plugin *plugin_{};
};

TEST_F(MetricsPythonBridgeTest, NativePluginUsesPublicPluginDescription)
{
    py::gil_scoped_acquire gil;
    auto object = py::cast(plugin_, py::return_value_policy::reference);

    EXPECT_EQ(object.attr("name").cast<std::string>(), "test");
    EXPECT_TRUE(object.attr("is_enabled").cast<bool>());
    EXPECT_FALSE(object.attr("server").is_none());
    EXPECT_FALSE(object.attr("logger").is_none());
    EXPECT_EQ(object.attr("plugin_description").attr("version").cast<std::string>(), "1.0.0");
}

TEST_F(MetricsPythonBridgeTest, MetricsConvertsAllSevenCharts)
{
    py::gil_scoped_acquire gil;
    PythonMetricsCapture capture;
    Metrics metrics(*plugin_, 12345);
    EXPECT_THROW(metrics.addCustomChart(nullptr), std::invalid_argument);

    metrics.addCustomChart(std::make_unique<SimplePie>("simple", [] { return std::optional<std::string>{"native"}; }));
    metrics.addCustomChart(
        std::make_unique<AdvancedPie>("advanced", [] { return StringValues{{"zero", 0}, {"value", 2}}; }));
    metrics.addCustomChart(std::make_unique<DrilldownPie>(
        "drilldown", [] { return DrilldownValues{{"empty", {}}, {"value", {{"inner", 3}}}}; }));
    metrics.addCustomChart(
        std::make_unique<SimpleBarChart>("simple_bar", [] { return StringValues{{"first", 1}, {"zero", 0}}; }));
    metrics.addCustomChart(
        std::make_unique<AdvancedBarChart>("advanced_bar", [] { return BarValues{{"empty", {}}, {"value", {1, 2}}}; }));
    metrics.addCustomChart(std::make_unique<SingleLineChart>("single_line", [] { return 4; }));
    metrics.addCustomChart(
        std::make_unique<MultiLineChart>("multi_line", [] { return StringValues{{"zero", 0}, {"value", -2}}; }));

    ASSERT_EQ(capture.charts().size(), 7);
    EXPECT_EQ(chartId(capture.charts()[0]), "simple");
    EXPECT_EQ(chartData(capture.charts()[0])["value"].cast<std::string>(), "native");
    EXPECT_EQ(chartData(capture.charts()[1])["values"].cast<StringValues>(), (StringValues{{"value", 2}}));
    EXPECT_EQ(chartData(capture.charts()[2])["values"].cast<DrilldownValues>(),
              (DrilldownValues{{"value", {{"inner", 3}}}}));
    EXPECT_EQ(chartData(capture.charts()[3])["values"].cast<BarValues>(), (BarValues{{"first", {1}}, {"zero", {0}}}));
    EXPECT_EQ(chartData(capture.charts()[4])["values"].cast<BarValues>(), (BarValues{{"value", {1, 2}}}));
    EXPECT_EQ(chartData(capture.charts()[5])["value"].cast<int>(), 4);
    EXPECT_EQ(chartData(capture.charts()[6])["values"].cast<StringValues>(), (StringValues{{"value", -2}}));
}

TEST_F(MetricsPythonBridgeTest, MetricsPreservesEmptyAndExceptionSemantics)
{
    py::gil_scoped_acquire gil;
    PythonMetricsCapture capture;
    Metrics metrics(*plugin_, 12346);

    metrics.addCustomChart(std::make_unique<SimplePie>("empty", [] { return std::optional<std::string>{}; }));
    metrics.addCustomChart(std::make_unique<AdvancedPie>("zero", [] { return StringValues{{"zero", 0}}; }));
    metrics.addCustomChart(std::make_unique<SimplePie>(
        "throwing", []() -> std::optional<std::string> { throw std::runtime_error("callback failed"); }));

    ASSERT_EQ(capture.charts().size(), 3);
    EXPECT_TRUE(chartData(capture.charts()[0]).is_none());
    EXPECT_TRUE(chartData(capture.charts()[1]).is_none());
    EXPECT_THROW(chartData(capture.charts()[2]), py::error_already_set);
    PyErr_Clear();
}

TEST_F(MetricsPythonBridgeTest, ShutdownIsIdempotentAndReleasesCallbackCaptures)
{
    py::gil_scoped_acquire gil;
    PythonMetricsCapture capture;
    Metrics metrics(*plugin_, 12347);
    auto state = std::make_shared<int>(1);
    const std::weak_ptr<int> observer = state;
    metrics.addCustomChart(
        std::make_unique<SimplePie>("lifetime", [state = std::move(state)] { return std::to_string(*state); }));
    EXPECT_FALSE(observer.expired());

    metrics.shutdown();
    metrics.shutdown();

    EXPECT_TRUE(observer.expired());
    EXPECT_THROW(
        metrics.addCustomChart(std::make_unique<SimplePie>("late", [] { return std::optional<std::string>{"late"}; })),
        std::logic_error);
}

TEST_F(MetricsPythonBridgeTest, DestroyingMetricsReleasesCallbackCaptures)
{
    py::gil_scoped_acquire gil;
    PythonMetricsCapture capture;
    auto state = std::make_shared<int>(1);
    const std::weak_ptr<int> observer = state;
    {
        Metrics metrics(*plugin_, 12348);
        metrics.addCustomChart(
            std::make_unique<SimplePie>("lifetime", [state = std::move(state)] { return std::to_string(*state); }));
        EXPECT_FALSE(observer.expired());
    }
    EXPECT_TRUE(observer.expired());
}

TEST(MetricsPythonLifecycleTest, PythonMetricsShutdownCancelsAndDetachesWithoutWaiting)
{
    initializePython();
    py::gil_scoped_acquire gil;
    auto module = py::module_::import("endstone.metrics");
    auto metrics_class = module.attr("Metrics");
    auto metrics = metrics_class.attr("__new__")(metrics_class);

    auto periodic_cancellations = std::make_shared<int>(0);
    auto send_cancellations = std::make_shared<int>(0);
    auto periodic = py::module_::import("types").attr("SimpleNamespace")();
    periodic.attr("cancel") = py::cpp_function([periodic_cancellations] { ++*periodic_cancellations; });
    auto builtins = py::module_::import("builtins");
    auto send_type = builtins.attr("type")("Cancelable", py::make_tuple(builtins.attr("object")), py::dict());
    auto send = send_type();
    send.attr("cancel") = py::cpp_function([send_cancellations] { ++*send_cancellations; });

    py::set charts;
    charts.add("chart");
    py::set sends;
    sends.add(send);
    metrics.attr("_shutdown") = false;
    metrics.attr("_plugin") = "plugin";
    metrics.attr("_future") = periodic;
    metrics.attr("_send_futures") = sends;
    metrics.attr("_custom_charts") = charts;

    metrics.attr("shutdown")();
    metrics.attr("shutdown")();

    EXPECT_EQ(*periodic_cancellations, 1);
    EXPECT_EQ(*send_cancellations, 1);
    EXPECT_TRUE(metrics.attr("_future").is_none());
    EXPECT_TRUE(metrics.attr("_plugin").is_none());
    EXPECT_EQ(py::len(metrics.attr("_custom_charts")), 0);
}

TEST(MetricsPythonLifecycleTest, PythonMetricsReportsUnexpectedSubmissionFailures)
{
    initializePython();
    py::gil_scoped_acquire gil;
    auto module = py::module_::import("endstone.metrics");
    auto metrics_class = module.attr("Metrics");
    auto metrics = metrics_class.attr("__new__")(metrics_class);
    auto logged_errors = std::make_shared<int>(0);
    metrics.attr("_shutdown") = false;
    metrics.attr("_log_errors") = true;
    metrics.attr("log_error") =
        py::cpp_function([logged_errors](const std::string &, const py::object &) { ++*logged_errors; });

    auto future = py::module_::import("concurrent.futures").attr("Future")();
    future.attr("set_exception")(py::module_::import("builtins").attr("RuntimeError")("boom"));
    metrics.attr("_submission_done")(future);
    EXPECT_EQ(*logged_errors, 1);

    auto cancelled = py::module_::import("concurrent.futures").attr("Future")();
    cancelled.attr("cancel")();
    metrics.attr("_submission_done")(cancelled);
    EXPECT_EQ(*logged_errors, 1);

    metrics.attr("_shutdown") = true;
    metrics.attr("_submission_done")(future);
    EXPECT_EQ(*logged_errors, 1);
}

TEST(MetricsPythonLifecycleTest, BuiltInMetricsDelegatesCollectionToPrimaryThreadSubmitter)
{
    initializePython();
    py::gil_scoped_acquire gil;
    auto endstone = py::module_::import("endstone");
    endstone.attr("Server") = py::module_::import("builtins").attr("object");
    endstone.attr("__minecraft_version__") = "test";
    auto module = py::module_::import("endstone._metrics");
    auto metrics_class = module.attr("EndstoneMetrics");
    auto metrics = metrics_class.attr("__new__")(metrics_class);
    auto submissions = std::make_shared<int>(0);
    auto executions = std::make_shared<int>(0);
    metrics.attr("_shutdown") = false;
    metrics.attr("_submit_task") = py::cpp_function([submissions](const py::function &) { ++*submissions; });

    metrics.attr("submit_task")(py::cpp_function([executions] { ++*executions; }));

    EXPECT_EQ(*submissions, 1);
    EXPECT_EQ(*executions, 0);
    metrics.attr("_shutdown") = true;
    metrics.attr("submit_task")(py::cpp_function([executions] { ++*executions; }));
    EXPECT_EQ(*submissions, 1);
}

}  // namespace
