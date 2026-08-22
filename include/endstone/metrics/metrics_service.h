// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include <memory>

#include "endstone/metrics/metrics_registration.h"

namespace endstone {
class Plugin;
}

namespace endstone::metrics {

/** Server-owned entry point for registering plugin metrics. */
class MetricsService {
public:
    MetricsService() = default;
    MetricsService(const MetricsService &) = delete;
    MetricsService &operator=(const MetricsService &) = delete;
    virtual ~MetricsService() = default;

    /**
     * Registers an enabled plugin with bStats.
     *
     * A plugin can have at most one active registration. This method must be
     * called on the primary server thread.
     *
     * @param plugin plugin that owns the registration
     * @param service_id positive bStats service ID
     * @return registration handle owned by the caller
     * @throws std::invalid_argument if service_id is not positive
     * @throws std::logic_error if the owner is disabled, already registered,
     *         the service is inactive, or the caller is off-thread
     */
    [[nodiscard]] virtual std::shared_ptr<MetricsRegistration> registerPlugin(Plugin &plugin, int service_id) = 0;

    /** Retires all registrations owned by a plugin. */
    virtual void unregisterPlugin(Plugin &plugin) noexcept = 0;
};
}  // namespace endstone::metrics
