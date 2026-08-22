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
#include <mutex>
#include <vector>

#include "endstone/metrics/metrics.h"

namespace endstone {
class Plugin;
class Server;
}  // namespace endstone

namespace endstone::core {
class EndstoneMetrics final : public metrics::MetricsService {
public:
    explicit EndstoneMetrics(Server &server);
    ~EndstoneMetrics() override;

    void start();
    [[nodiscard]] std::shared_ptr<metrics::MetricsRegistration> registerPlugin(Plugin &plugin, int service_id) override;
    void unregisterPlugin(Plugin &plugin) noexcept override;
    void shutdown() noexcept;

private:
    class Registration;
    class ServerMetrics;

    Server &server_;
    mutable std::mutex mutex_;
    bool active_{true};
    std::vector<std::weak_ptr<Registration>> registrations_;
    std::unique_ptr<ServerMetrics> server_metrics_;
};
}  // namespace endstone::core
