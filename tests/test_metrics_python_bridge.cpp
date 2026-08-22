// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");

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
#include "endstone/metrics/advanced_bar_chart.h"
#include "endstone/metrics/advanced_pie.h"
#include "endstone/metrics/drilldown_pie.h"
#include "endstone/metrics/multi_line_chart.h"
#include "endstone/metrics/simple_bar_chart.h"
#include "endstone/metrics/simple_pie.h"
#include "endstone/metrics/single_line_chart.h"
#include "mocks.h"

namespace fs = std::filesystem;
namespace py = pybind11;

namespace {

void initializePython()
{
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

class MetricsPythonBridgeTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        initializePython();
        ON_CALL(server_, getLogger())
            .WillByDefault(testing::ReturnRef(endstone::core::LoggerFactory::getLogger("MetricsBridgeTest")));
        ON_CALL(server_, isPrimaryThread()).WillByDefault(testing::Return(true));
        scheduler_ = std::make_unique<endstone::core::EndstoneScheduler>(server_);
        ON_CALL(server_, getScheduler()).WillByDefault(testing::ReturnRef(*scheduler_));
        service_manager_ = std::make_unique<endstone::core::EndstoneServiceManager>();
        ON_CALL(server_, getServiceManager()).WillByDefault(testing::ReturnRef(*service_manager_));
        metrics_ = std::make_unique<endstone::core::EndstoneMetrics>(server_);
        ON_CALL(server_, getMetrics()).WillByDefault(testing::ReturnRef(*metrics_));
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
        metrics_.reset();
        scheduler_.reset();
        service_manager_.reset();
        plugin_ = nullptr;
    }

    testing::NiceMock<MockServer> server_;
    std::unique_ptr<endstone::core::EndstoneScheduler> scheduler_;
    std::unique_ptr<endstone::core::EndstoneServiceManager> service_manager_;
    std::unique_ptr<endstone::core::EndstoneMetrics> metrics_;
    std::unique_ptr<endstone::core::EndstonePluginManager> manager_;
    endstone::Plugin *plugin_{};
};

py::object request(const py::handle &chart)
{
    return chart.attr("_get_request_json_object")();
}

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

TEST_F(MetricsPythonBridgeTest, RegistrationConvertsAllSevenCharts)
{
    py::gil_scoped_acquire gil;
    PythonMetricsCapture capture;
    endstone::core::EndstoneMetrics service(server_);
    auto registration = service.registerPlugin(*plugin_, 12345);
    ASSERT_TRUE(registration);
    EXPECT_THROW(registration->addCustomChart(nullptr), std::invalid_argument);

    registration->addCustomChart(
        std::make_unique<endstone::metrics::SimplePie>("simple", [] { return std::optional<std::string>{"native"}; }));
    registration->addCustomChart(std::make_unique<endstone::metrics::AdvancedPie>(
        "advanced", [] { return std::optional<endstone::metrics::StringValues>{{{"zero", 0}, {"value", 2}}}; }));
    registration->addCustomChart(std::make_unique<endstone::metrics::DrilldownPie>("drilldown", [] {
        return std::optional<endstone::metrics::DrilldownValues>{{{"empty", {}}, {"value", {{"inner", 3}}}}};
    }));
    registration->addCustomChart(std::make_unique<endstone::metrics::SimpleBarChart>(
        "simple_bar", [] { return std::optional<endstone::metrics::StringValues>{{{"first", 1}, {"zero", 0}}}; }));
    registration->addCustomChart(std::make_unique<endstone::metrics::AdvancedBarChart>("advanced_bar", [] {
        return std::optional<endstone::metrics::BarValues>{{{"empty", {}}, {"value", {1, 2}}}};
    }));
    registration->addCustomChart(std::make_unique<endstone::metrics::SingleLineChart>("single_line", [] { return 4; }));
    registration->addCustomChart(std::make_unique<endstone::metrics::MultiLineChart>(
        "multi_line", [] { return std::optional<endstone::metrics::StringValues>{{{"zero", 0}, {"value", -2}}}; }));

    ASSERT_EQ(capture.charts().size(), 7);
    EXPECT_EQ(request(capture.charts()[0])["chartId"].cast<std::string>(), "simple");
    EXPECT_EQ(request(capture.charts()[0])["data"]["value"].cast<std::string>(), "native");
    EXPECT_EQ(request(capture.charts()[1])["data"]["values"].cast<endstone::metrics::StringValues>(),
              (endstone::metrics::StringValues{{"value", 2}}));
    EXPECT_EQ(request(capture.charts()[2])["data"]["values"].cast<endstone::metrics::DrilldownValues>(),
              (endstone::metrics::DrilldownValues{{"value", {{"inner", 3}}}}));
    EXPECT_EQ(request(capture.charts()[3])["data"]["values"].cast<endstone::metrics::BarValues>(),
              (endstone::metrics::BarValues{{"first", {1}}, {"zero", {0}}}));
    EXPECT_EQ(request(capture.charts()[4])["data"]["values"].cast<endstone::metrics::BarValues>(),
              (endstone::metrics::BarValues{{"value", {1, 2}}}));
    EXPECT_EQ(request(capture.charts()[5])["data"]["value"].cast<int>(), 4);
    EXPECT_EQ(request(capture.charts()[6])["data"]["values"].cast<endstone::metrics::StringValues>(),
              (endstone::metrics::StringValues{{"value", -2}}));

    service.shutdown();
}

TEST_F(MetricsPythonBridgeTest, RegistrationPreservesEmptyAndExceptionSemantics)
{
    py::gil_scoped_acquire gil;
    PythonMetricsCapture capture;
    endstone::core::EndstoneMetrics service(server_);
    auto registration = service.registerPlugin(*plugin_, 12346);

    registration->addCustomChart(
        std::make_unique<endstone::metrics::SimplePie>("empty", [] { return std::optional<std::string>{}; }));
    registration->addCustomChart(std::make_unique<endstone::metrics::AdvancedPie>(
        "zero", [] { return std::optional<endstone::metrics::StringValues>{{{"zero", 0}}}; }));
    registration->addCustomChart(std::make_unique<endstone::metrics::SimplePie>(
        "throwing", []() -> std::optional<std::string> { throw std::runtime_error("callback failed"); }));

    ASSERT_EQ(capture.charts().size(), 3);
    EXPECT_TRUE(request(capture.charts()[0]).is_none());
    EXPECT_TRUE(request(capture.charts()[1]).is_none());
    EXPECT_THROW(request(capture.charts()[2]), py::error_already_set);
    PyErr_Clear();
    service.shutdown();
}

TEST_F(MetricsPythonBridgeTest, ShutdownIsIdempotentAndReleasesCallbackCaptures)
{
    py::gil_scoped_acquire gil;
    PythonMetricsCapture capture;
    endstone::core::EndstoneMetrics service(server_);
    auto registration = service.registerPlugin(*plugin_, 12347);
    auto state = std::make_shared<int>(1);
    std::weak_ptr<int> weak = state;
    registration->addCustomChart(
        std::make_unique<endstone::metrics::SimplePie>("lifetime", [state] { return std::to_string(*state); }));
    state.reset();
    EXPECT_FALSE(weak.expired());

    registration->shutdown();
    registration->shutdown();
    EXPECT_FALSE(registration->isActive());
    EXPECT_TRUE(weak.expired());
    EXPECT_THROW(registration->addCustomChart(std::make_unique<endstone::metrics::SimplePie>(
                     "late", [] { return std::optional<std::string>{"late"}; })),
                 std::logic_error);
    service.shutdown();
}

TEST_F(MetricsPythonBridgeTest, RetainedHandleIsInertAfterServiceDestruction)
{
    py::gil_scoped_acquire gil;
    PythonMetricsCapture capture;
    std::shared_ptr<endstone::metrics::MetricsRegistration> registration;
    {
        endstone::core::EndstoneMetrics service(server_);
        registration = service.registerPlugin(*plugin_, 12351);
        ASSERT_TRUE(registration->isActive());
    }

    EXPECT_FALSE(registration->isActive());
    EXPECT_THROW(registration->addCustomChart(
                     std::make_unique<endstone::metrics::SimplePie>("late", [] { return std::string{"late"}; })),
                 std::logic_error);
}

TEST_F(MetricsPythonBridgeTest, RegistrationRejectsInvalidOwnersIdsDuplicatesAndOffThreadCalls)
{
    py::gil_scoped_acquire gil;
    PythonMetricsCapture capture;
    endstone::core::EndstoneMetrics service(server_);

    EXPECT_THROW((void)service.registerPlugin(*plugin_, 0), std::invalid_argument);
    MockPlugin disabled_owner;
    EXPECT_THROW((void)service.registerPlugin(disabled_owner, 12348), std::logic_error);

    auto registration = service.registerPlugin(*plugin_, 12348);
    ASSERT_TRUE(registration);
    EXPECT_THROW((void)service.registerPlugin(*plugin_, 12348), std::logic_error);
    EXPECT_THROW((void)service.registerPlugin(*plugin_, 54321), std::logic_error);

    ON_CALL(server_, isPrimaryThread()).WillByDefault(testing::Return(false));
    EXPECT_THROW((void)service.registerPlugin(*plugin_, 12349), std::logic_error);
    service.shutdown();
}

TEST_F(MetricsPythonBridgeTest, ServiceShutdownAndPluginShutdownRetireRetainedHandles)
{
    py::gil_scoped_acquire gil;
    PythonMetricsCapture capture;
    endstone::core::EndstoneMetrics service(server_);
    auto registration = service.registerPlugin(*plugin_, 12350);
    ASSERT_TRUE(registration->isActive());

    service.unregisterPlugin(*plugin_);
    EXPECT_FALSE(registration->isActive());

    EXPECT_THROW(registration->addCustomChart(std::make_unique<endstone::metrics::SimplePie>(
                     "late", [] { return std::optional<std::string>{"late"}; })),
                 std::logic_error);
    service.shutdown();
}

TEST_F(MetricsPythonBridgeTest, LoaderDisableRetiresRegistrationBeforePluginUnload)
{
    PythonMetricsCapture capture;
    auto registration = metrics_->registerPlugin(*plugin_, 12351);
    ASSERT_TRUE(registration->isActive());

    plugin_->getPluginLoader().disablePlugin(*plugin_);

    EXPECT_FALSE(plugin_->isEnabled());
    EXPECT_FALSE(registration->isActive());
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
