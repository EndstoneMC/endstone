// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#include "endstone/core/metrics.h"

#include <algorithm>
#include <atomic>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "endstone/core/scheduler/scheduler.h"
#include "endstone/plugin/plugin.h"
#include "endstone/server.h"

namespace py = pybind11;

namespace endstone::metrics::detail {

void dispatchChart(CustomChart &chart, ChartVisitor &visitor)
{
    chart.dispatch(visitor);
}

}  // namespace endstone::metrics::detail

namespace endstone::core {
namespace {

[[noreturn]] void throwPythonError(std::string_view context, py::error_already_set &error)
{
    const auto message = std::string(context) + ": " + error.what();
    error.restore();
    PyErr_Clear();
    throw std::runtime_error(message);
}

class PythonScheduledCallback final {
public:
    explicit PythonScheduledCallback(py::object callback) : callback_(std::move(callback)) {}

    ~PythonScheduledCallback() { clear(); }

    [[nodiscard]] bool isComplete() const noexcept { return complete_.load(std::memory_order_acquire); }

    void invoke() noexcept
    {
        if (!Py_IsInitialized() || !callback_) {
            return;
        }
        try {
            py::gil_scoped_acquire gil;
            callback_();
            callback_ = py::object();
        }
        catch (py::error_already_set &error) {
            error.restore();
            PyErr_Clear();
            callback_ = py::object();
        }
        catch (...) {
            clear();
        }
        complete_.store(true, std::memory_order_release);
    }

    void clear() noexcept
    {
        if (!callback_) {
            complete_.store(true, std::memory_order_release);
            return;
        }
        if (!Py_IsInitialized()) {
            callback_.release();
            complete_.store(true, std::memory_order_release);
            return;
        }
        try {
            py::gil_scoped_acquire gil;
            callback_ = py::object();
        }
        catch (...) {
            callback_.release();
        }
        complete_.store(true, std::memory_order_release);
    }

private:
    py::object callback_;
    std::atomic_bool complete_{false};
};

struct ServerMetricsState {
    Server &server;
    std::atomic_bool stopped{false};
    std::mutex mutex;
    std::vector<std::pair<TaskId, std::shared_ptr<PythonScheduledCallback>>> pending;
};

void scheduleServerMetricsTask(const std::shared_ptr<ServerMetricsState> &state, py::object callback)
{
    if (state->stopped.load(std::memory_order_acquire)) {
        return;
    }

    auto scheduled = std::make_shared<PythonScheduledCallback>(std::move(callback));
    auto task_id = std::make_shared<std::atomic<TaskId>>(0);
    auto task = static_cast<EndstoneScheduler &>(state->server.getScheduler()).runTask([state, scheduled, task_id] {
        if (!state->stopped.load(std::memory_order_acquire)) {
            scheduled->invoke();
        }
        else {
            scheduled->clear();
        }
        std::scoped_lock lock(state->mutex);
        std::erase_if(state->pending, [task_id](const auto &entry) { return entry.first == task_id->load(); });
    });
    if (!task) {
        return;
    }
    task_id->store(task->getTaskId(), std::memory_order_release);
    std::scoped_lock lock(state->mutex);
    if (state->stopped.load(std::memory_order_acquire)) {
        task->cancel();
        scheduled->clear();
        return;
    }
    if (!scheduled->isComplete()) {
        state->pending.emplace_back(task_id->load(std::memory_order_acquire), std::move(scheduled));
    }
}

class PythonChartAdapter final : public metrics::detail::ChartVisitor {
public:
    explicit PythonChartAdapter(py::object &module) : module_(module) {}

    [[nodiscard]] py::object adapt(metrics::CustomChart &chart)
    {
        metrics::detail::dispatchChart(chart, *this);
        return std::move(result_);
    }

    void visitSimplePie(std::string chart_id, metrics::SimplePieCallback callback) override
    {
        create("SimplePie", std::move(chart_id), std::move(callback));
    }

    void visitAdvancedPie(std::string chart_id, metrics::AdvancedPieCallback callback) override
    {
        create("AdvancedPie", std::move(chart_id), std::move(callback));
    }

    void visitDrilldownPie(std::string chart_id, metrics::DrilldownPieCallback callback) override
    {
        create("DrilldownPie", std::move(chart_id), std::move(callback));
    }

    void visitSimpleBarChart(std::string chart_id, metrics::SimpleBarChartCallback callback) override
    {
        create("SimpleBarChart", std::move(chart_id), std::move(callback));
    }

    void visitAdvancedBarChart(std::string chart_id, metrics::AdvancedBarChartCallback callback) override
    {
        create("AdvancedBarChart", std::move(chart_id), std::move(callback));
    }

    void visitSingleLineChart(std::string chart_id, metrics::SingleLineChartCallback callback) override
    {
        create("SingleLineChart", std::move(chart_id), std::move(callback));
    }

    void visitMultiLineChart(std::string chart_id, metrics::MultiLineChartCallback callback) override
    {
        create("MultiLineChart", std::move(chart_id), std::move(callback));
    }

    void visitCustomChart(std::string chart_id, metrics::CustomChartCallback callback) override
    {
        auto python_callback = py::cpp_function([callback = std::move(callback)]() mutable -> py::object {
            auto data = callback();
            return data ? py::cast(std::move(*data)) : py::none();
        });
        auto chart_module = py::module_::import("endstone.metrics.charts.custom_chart");
        result_ = chart_module.attr("_CallbackChart")(std::move(chart_id), std::move(python_callback));
    }

private:
    template <typename Callback>
    void create(const char *class_name, std::string &&chart_id, Callback callback)
    {
        auto python_callback = py::cpp_function([callback = std::move(callback)]() mutable { return callback(); });
        result_ = module_.attr(class_name)(std::move(chart_id), std::move(python_callback));
    }

    py::object &module_;
    py::object result_;
};

class PythonMetrics final {
public:
    PythonMetrics(Plugin &plugin, int service_id)
    {
        if (!Py_IsInitialized()) {
            throw std::runtime_error("Python interpreter is not initialized");
        }

        py::gil_scoped_acquire gil;
        try {
            module_ = py::module_::import("endstone.metrics");
            metrics_ = module_.attr("Metrics")(py::cast(&plugin, py::return_value_policy::reference), service_id);
        }
        catch (py::error_already_set &error) {
            const auto message = std::string("Unable to create Python-backed metrics: ") + error.what();
            error.restore();
            PyErr_Clear();
            metrics_.release();
            module_.release();
            throw std::runtime_error(message);
        }
        catch (...) {
            metrics_.release();
            module_.release();
            throw;
        }
    }

    ~PythonMetrics() { shutdown(); }

    void addCustomChart(std::unique_ptr<metrics::CustomChart> chart)
    {
        if (!chart) {
            throw std::invalid_argument("chart cannot be null");
        }
        if (stopped_.load(std::memory_order_acquire)) {
            throw std::logic_error("metrics has been shut down");
        }
        if (!Py_IsInitialized()) {
            throw std::runtime_error("Python interpreter is not initialized");
        }

        py::gil_scoped_acquire gil;
        charts_.push_back(std::move(chart));
        try {
            PythonChartAdapter adapter(module_);
            auto python_chart = adapter.adapt(*charts_.back());
            metrics_.attr("add_custom_chart")(std::move(python_chart));
        }
        catch (py::error_already_set &error) {
            charts_.pop_back();
            throwPythonError("Unable to add Python-backed metrics chart", error);
        }
        catch (...) {
            charts_.pop_back();
            throw;
        }
    }

    void shutdown() noexcept
    {
        if (stopped_.exchange(true, std::memory_order_acq_rel)) {
            return;
        }

        if (!Py_IsInitialized()) {
            metrics_.release();
            module_.release();
            for (auto &chart : charts_) {
                [[maybe_unused]] auto *leaked_chart = chart.release();
            }
            charts_.clear();
            return;
        }

        try {
            py::gil_scoped_acquire gil;
            try {
                if (metrics_) {
                    metrics_.attr("shutdown")();
                }
            }
            catch (py::error_already_set &) {
                PyErr_Clear();
            }

            metrics_ = py::object();
            module_ = py::object();
            charts_.clear();
        }
        catch (...) {
            metrics_.release();
            module_.release();
            for (auto &chart : charts_) {
                [[maybe_unused]] auto *leaked_chart = chart.release();
            }
            charts_.clear();
        }
    }

private:
    std::atomic_bool stopped_{false};
    py::object module_;
    py::object metrics_;
    std::vector<std::unique_ptr<metrics::CustomChart>> charts_;
};

}  // namespace

class EndstoneMetrics::ServerMetrics final {
public:
    explicit ServerMetrics(Server &server) : server_(server), state_(std::make_shared<ServerMetricsState>(server))
    {
        if (!Py_IsInitialized()) {
            server_.getLogger().warning("Unable to start metrics: Python interpreter is not initialized");
            return;
        }

        std::string error_message;
        try {
            py::gil_scoped_acquire gil;
            try {
                auto module = py::module_::import("endstone._metrics");
                auto submit_task = py::cpp_function(
                    [state = state_](py::object callback) { scheduleServerMetricsTask(state, std::move(callback)); });
                metrics_ = module.attr("EndstoneMetrics")(std::ref(server_), std::move(submit_task));
            }
            catch (py::error_already_set &error) {
                error_message = error.what();
                error.restore();
                PyErr_Clear();
                metrics_ = py::object();
            }
        }
        catch (const std::exception &error) {
            error_message = error.what();
            metrics_.release();
        }
        catch (...) {
            error_message = "unknown error";
            metrics_.release();
        }
        if (!error_message.empty()) {
            server_.getLogger().warning("Unable to start metrics: {}", error_message);
        }
    }

    ~ServerMetrics() { shutdown(); }

    void shutdown() noexcept
    {
        if (state_->stopped.exchange(true, std::memory_order_acq_rel)) {
            return;
        }
        std::vector<std::pair<TaskId, std::shared_ptr<PythonScheduledCallback>>> pending;
        {
            std::scoped_lock lock(state_->mutex);
            pending = std::move(state_->pending);
        }
        for (auto &[task_id, callback] : pending) {
            server_.getScheduler().cancelTask(task_id);
            callback->clear();
        }
        if (!metrics_) {
            return;
        }
        if (!Py_IsInitialized()) {
            metrics_.release();
            return;
        }

        std::string error_message;
        try {
            py::gil_scoped_acquire gil;
            try {
                metrics_.attr("shutdown")();
            }
            catch (py::error_already_set &error) {
                error_message = error.what();
                error.restore();
                PyErr_Clear();
            }
            metrics_ = py::object();
        }
        catch (const std::exception &error) {
            error_message = error.what();
            metrics_.release();
        }
        catch (...) {
            metrics_.release();
        }
        if (!error_message.empty()) {
            server_.getLogger().warning("Unable to shutdown metrics: {}", error_message);
        }
    }

private:
    Server &server_;
    std::shared_ptr<ServerMetricsState> state_;
    py::object metrics_;
};

class EndstoneMetrics::Registration final : public metrics::MetricsRegistration {
public:
    Registration(EndstoneMetrics &service, Plugin &plugin, int service_id)
        : service_(service), plugin_(&plugin), metrics_(std::make_unique<PythonMetrics>(plugin, service_id))
    {
    }

    ~Registration() override { shutdown(); }

    void addCustomChart(std::unique_ptr<metrics::CustomChart> chart) override
    {
        if (!chart) {
            throw std::invalid_argument("chart cannot be null");
        }
        if (!active_.load(std::memory_order_acquire)) {
            throw std::logic_error("metrics registration has been shut down");
        }
        if (!service_.server_.isPrimaryThread()) {
            throw std::logic_error("metrics chart registration must occur on the primary server thread");
        }
        std::scoped_lock lock(mutex_);
        if (!active_.load(std::memory_order_acquire) || !metrics_) {
            throw std::logic_error("metrics registration has been shut down");
        }
        metrics_->addCustomChart(std::move(chart));
    }

    void shutdown() noexcept override
    {
        if (!active_.exchange(false, std::memory_order_acq_rel)) {
            return;
        }
        std::unique_ptr<PythonMetrics> metrics;
        {
            std::scoped_lock lock(mutex_);
            metrics = std::move(metrics_);
        }
        metrics.reset();
    }

    [[nodiscard]] bool isActive() const noexcept override { return active_.load(std::memory_order_acquire); }

private:
    friend class EndstoneMetrics;

    [[nodiscard]] Plugin *owner() const noexcept { return plugin_; }

    EndstoneMetrics &service_;
    Plugin *plugin_;
    std::atomic_bool active_{true};
    mutable std::mutex mutex_;
    std::unique_ptr<PythonMetrics> metrics_;
};

EndstoneMetrics::EndstoneMetrics(Server &server) : server_(server) {}

EndstoneMetrics::~EndstoneMetrics()
{
    shutdown();
}

void EndstoneMetrics::start()
{
    {
        std::scoped_lock lock(mutex_);
        if (!active_) {
            throw std::logic_error("metrics service has been shut down");
        }
        if (server_metrics_) {
            return;
        }
    }

    auto server_metrics = std::make_unique<ServerMetrics>(server_);
    std::unique_ptr<ServerMetrics> discarded_metrics;
    {
        std::scoped_lock lock(mutex_);
        if (active_ && !server_metrics_) {
            server_metrics_ = std::move(server_metrics);
        }
        else {
            discarded_metrics = std::move(server_metrics);
        }
    }
    if (discarded_metrics) {
        discarded_metrics->shutdown();
    }
}

std::shared_ptr<metrics::MetricsRegistration> EndstoneMetrics::registerPlugin(Plugin &plugin, int service_id)
{
    if (!server_.isPrimaryThread()) {
        throw std::logic_error("metrics registration must occur on the primary server thread");
    }
    if (service_id <= 0) {
        throw std::invalid_argument("service_id must be positive");
    }
    if (!plugin.isEnabled()) {
        throw std::logic_error("metrics registration owner must be enabled");
    }

    {
        std::scoped_lock lock(mutex_);
        if (!active_) {
            throw std::logic_error("metrics service has been shut down");
        }
        for (auto it = registrations_.begin(); it != registrations_.end();) {
            if (auto registration = it->lock()) {
                if (registration->isActive() && registration->owner() == &plugin) {
                    throw std::logic_error("plugin already has an active metrics registration");
                }
                ++it;
            }
            else {
                it = registrations_.erase(it);
            }
        }
    }

    auto registration = std::make_shared<Registration>(*this, plugin, service_id);
    bool service_inactive = false;
    bool duplicate = false;
    {
        std::scoped_lock lock(mutex_);
        if (!active_) {
            service_inactive = true;
        }
        for (auto it = registrations_.begin(); !service_inactive && it != registrations_.end();) {
            if (auto existing = it->lock()) {
                if (existing->isActive() && existing->owner() == &plugin) {
                    duplicate = true;
                    break;
                }
                ++it;
            }
            else {
                it = registrations_.erase(it);
            }
        }
        if (!service_inactive && !duplicate) {
            registrations_.emplace_back(registration);
        }
    }
    if (service_inactive || duplicate) {
        registration->shutdown();
        if (service_inactive) {
            throw std::logic_error("metrics service has been shut down");
        }
        throw std::logic_error("plugin already has an active metrics registration");
    }
    return registration;
}

void EndstoneMetrics::unregisterPlugin(Plugin &plugin) noexcept
{
    std::vector<std::shared_ptr<Registration>> retiring;
    {
        std::scoped_lock lock(mutex_);
        for (auto it = registrations_.begin(); it != registrations_.end();) {
            if (auto registration = it->lock()) {
                if (registration->owner() == &plugin) {
                    retiring.push_back(std::move(registration));
                    it = registrations_.erase(it);
                    continue;
                }
                ++it;
            }
            else {
                it = registrations_.erase(it);
            }
        }
    }
    for (auto &registration : retiring) {
        registration->shutdown();
    }
}

void EndstoneMetrics::shutdown() noexcept
{
    std::vector<std::shared_ptr<Registration>> retiring;
    std::unique_ptr<ServerMetrics> server_metrics;
    {
        std::scoped_lock lock(mutex_);
        if (!active_) {
            return;
        }
        active_ = false;
        for (auto &weak_registration : registrations_) {
            if (auto registration = weak_registration.lock()) {
                retiring.push_back(std::move(registration));
            }
        }
        registrations_.clear();
        server_metrics = std::move(server_metrics_);
    }
    for (auto &registration : retiring) {
        registration->shutdown();
    }
    if (server_metrics) {
        server_metrics->shutdown();
    }
}

}  // namespace endstone::core
