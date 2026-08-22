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

#include "endstone/metrics/custom_chart.h"

namespace endstone::metrics {

/**
 * Represents one plugin's registration with the server metrics service.
 *
 * A registration owns every chart added to it. It becomes permanently inactive
 * when shut down, destroyed, or retired by plugin or server shutdown.
 */
class MetricsRegistration {
public:
    MetricsRegistration() = default;
    MetricsRegistration(const MetricsRegistration &) = delete;
    MetricsRegistration &operator=(const MetricsRegistration &) = delete;
    MetricsRegistration(MetricsRegistration &&) = delete;
    MetricsRegistration &operator=(MetricsRegistration &&) = delete;
    virtual ~MetricsRegistration() = default;

    /**
     * Transfers ownership of a chart to this registration.
     *
     * This method must be called on the primary server thread.
     *
     * @param chart chart to register
     * @throws std::invalid_argument if chart is null
     * @throws std::logic_error if the registration is inactive or the caller is off-thread
     */
    virtual void addCustomChart(std::unique_ptr<CustomChart> chart) = 0;

    /**
     * Retires this registration and releases all chart callbacks.
     *
     * Repeated calls have no effect.
     */
    virtual void shutdown() noexcept = 0;

    /**
     * Returns whether this registration can accept charts.
     *
     * @return true while the registration is active
     */
    [[nodiscard]] virtual bool isActive() const noexcept = 0;
};
}  // namespace endstone::metrics
