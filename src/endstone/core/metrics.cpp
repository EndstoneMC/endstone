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

#include "endstone/core/metrics.h"

#include <atomic>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "endstone/core/scheduler/scheduler.h"
#include "endstone/metrics/custom_chart.h"
#include "endstone/plugin/plugin.h"
#include "endstone/server.h"

namespace py = pybind11;

namespace endstone::core {
namespace {

class ScheduledCallback {
public:
    explicit ScheduledCallback(py::object callback) : callback_(std::move(callback)) {}

    ~ScheduledCallback() { clear(); }

    [[nodiscard]] bool isComplete() const noexcept { return complete_.load(std::memory_order_acquire); }

    void invoke() noexcept
    {
        if (!Py_IsInitialized() || !callback_) {
            complete_.store(true, std::memory_order_release);
            return;
        }
        try {
            py::gil_scoped_acquire gil;
            try {
                callback_();
            }
            catch (const py::error_already_set &) {
            }
            callback_ = py::object();
        }
        catch (...) {
            callback_.release();
        }
        complete_.store(true, std::memory_order_release);
    }

    void clear() noexcept
    {
        if (!callback_) {
            complete_.store(true, std::memory_order_release);
            return;
        }
        if (Py_IsInitialized()) {
            try {
                py::gil_scoped_acquire gil;
                callback_ = py::object();
            }
            catch (...) {
                callback_.release();
            }
        }
        else {
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
    std::vector<std::pair<TaskId, std::shared_ptr<ScheduledCallback>>> pending;
};

void scheduleServerMetricsTask(const std::shared_ptr<ServerMetricsState> &state, py::object callback)
{
    if (state->stopped.load(std::memory_order_acquire)) {
        return;
    }

    auto scheduled = std::make_shared<ScheduledCallback>(std::move(callback));
    auto task_id = std::make_shared<std::atomic<TaskId>>(0);
    const auto task =
        static_cast<EndstoneScheduler &>(state->server.getScheduler()).runTask([state, scheduled, task_id] {
            if (state->stopped.load(std::memory_order_acquire)) {
                scheduled->clear();
            }
            else {
                scheduled->invoke();
            }
            std::scoped_lock lock(state->mutex);
            std::erase_if(state->pending, [task_id](const auto &entry) { return entry.first == task_id->load(); });
        });
    if (!task) {
        scheduled->clear();
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

class PluginMetrics final : public metrics::MetricsBase {
public:
    PluginMetrics(Plugin &plugin, int service_id)
    {
        if (!Py_IsInitialized()) {
            throw std::runtime_error("Python interpreter is not initialized");
        }

        py::gil_scoped_acquire gil;
        try {
            metrics_ = py::module_::import("endstone.metrics")
                           .attr("Metrics")(py::cast(&plugin, py::return_value_policy::reference), service_id);
        }
        catch (py::error_already_set &error) {
            throw std::runtime_error(std::string("Unable to create metrics: ") + error.what());
        }
    }

    ~PluginMetrics() override { shutdown(); }

    void addCustomChart(std::unique_ptr<metrics::CustomChart> chart) override
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
        try {
            metrics_.attr("add_custom_chart")(py::cast(chart.release(), py::return_value_policy::take_ownership));
        }
        catch (py::error_already_set &error) {
            throw std::runtime_error(std::string("Unable to add metrics chart: ") + error.what());
        }
    }

    void shutdown() noexcept override
    {
        if (stopped_.exchange(true, std::memory_order_acq_rel)) {
            return;
        }
        if (!Py_IsInitialized()) {
            metrics_.release();
            return;
        }
        try {
            py::gil_scoped_acquire gil;
            try {
                if (metrics_) {
                    metrics_.attr("shutdown")();
                }
            }
            catch (const py::error_already_set &) {
            }
            metrics_ = py::object();
        }
        catch (...) {
            metrics_.release();
        }
    }

private:
    std::atomic_bool stopped_{false};
    py::object metrics_;
};

}  // namespace

class EndstoneMetrics::Impl {
public:
    explicit Impl(Server &server) : server_(server), state_(std::make_shared<ServerMetricsState>(server))
    {
        if (!Py_IsInitialized()) {
            server_.getLogger().warning("Unable to start metrics: Python interpreter is not initialized");
            return;
        }

        std::string error_message;
        try {
            py::gil_scoped_acquire gil;
            try {
                auto submit_task = py::cpp_function(
                    [state = state_](py::object callback) { scheduleServerMetricsTask(state, std::move(callback)); });
                metrics_ = py::module_::import("endstone._metrics")
                               .attr("EndstoneMetrics")(std::ref(server_), std::move(submit_task));
            }
            catch (const py::error_already_set &error) {
                error_message = error.what();
            }
        }
        catch (const std::exception &error) {
            error_message = error.what();
            metrics_.release();
        }
        if (!error_message.empty()) {
            server_.getLogger().warning("Unable to start metrics: {}", error_message);
        }
    }

    ~Impl()
    {
        if (state_->stopped.exchange(true, std::memory_order_acq_rel)) {
            return;
        }

        std::vector<std::pair<TaskId, std::shared_ptr<ScheduledCallback>>> pending;
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
            catch (const py::error_already_set &error) {
                error_message = error.what();
            }
            metrics_ = py::object();
        }
        catch (const std::exception &error) {
            error_message = error.what();
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

EndstoneMetrics::EndstoneMetrics(Server &server) : impl_(std::make_unique<Impl>(server)) {}

EndstoneMetrics::~EndstoneMetrics() = default;

std::unique_ptr<metrics::MetricsBase> createPluginMetrics(Plugin &plugin, int service_id)
{
    return std::make_unique<PluginMetrics>(plugin, service_id);
}

}  // namespace endstone::core
