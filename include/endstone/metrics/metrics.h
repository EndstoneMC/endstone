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

#include <memory>
#include <utility>

#include "endstone/metrics/advanced_bar_chart.h"
#include "endstone/metrics/advanced_pie.h"
#include "endstone/metrics/custom_chart.h"
#include "endstone/metrics/drilldown_pie.h"
#include "endstone/metrics/metrics_base.h"
#include "endstone/metrics/multi_line_chart.h"
#include "endstone/metrics/simple_bar_chart.h"
#include "endstone/metrics/simple_pie.h"
#include "endstone/metrics/single_line_chart.h"
#include "endstone/plugin/plugin.h"
#include "endstone/server.h"

namespace endstone {

/**
 * Collects and submits anonymous usage statistics for a plugin to bStats.
 *
 * A plugin owns its Metrics, typically as a member created in Plugin::onEnable. Destroying it stops
 * collection and releases every chart, so a plugin that keeps one needs no further cleanup.
 */
class Metrics {
public:
    /**
     * Creates a new Metrics instance.
     *
     * @param plugin the plugin these metrics belong to
     * @param service_id the id of the service, found at https://bstats.org/what-is-my-plugin-id
     */
    Metrics(Plugin &plugin, int service_id) : impl_(plugin.getServer().createMetrics(plugin, service_id)) {}

    Metrics(const Metrics &) = delete;
    Metrics &operator=(const Metrics &) = delete;
    Metrics(Metrics &&) noexcept = default;
    Metrics &operator=(Metrics &&) noexcept = default;
    ~Metrics() = default;

    /**
     * Adds a custom chart.
     *
     * @param chart the chart to add
     */
    void addCustomChart(std::unique_ptr<CustomChart> chart) { impl_->addCustomChart(std::move(chart)); }

    /**
     * Stops collecting and submitting data.
     *
     * Repeated calls have no effect.
     */
    void shutdown() noexcept { impl_->shutdown(); }

private:
    std::unique_ptr<MetricsBase> impl_;
};
}  // namespace endstone
