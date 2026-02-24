// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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
#include <string>
#include <utility>

#include "endstone/plugin/service.h"
#include "endstone/plugin/service_priority.h"

namespace endstone {

class Plugin;

/**
 * @brief Represent a service manager that manages services and service providers.
 *
 * Services are an interface specifying a list of methods that a provider must implement.
 * Providers are implementations of these services.
 */
class ServiceManager {
public:
    virtual ~ServiceManager() = default;

    /**
     * @brief Register a provider of a service.
     *
     * @param name service name
     * @param provider service provider to register
     * @param plugin plugin associated with the service
     * @param priority priority of the provider
     */
    virtual void registerService(std::string name, std::shared_ptr<Service> provider, const Plugin &plugin,
                                 ServicePriority priority) = 0;

    /**
     * @brief Unregister all the services registered by a particular plugin.
     *
     * @param plugin The plugin
     */
    virtual void unregisterAll(const Plugin &plugin) = 0;

    /**
     * @brief Unregister a particular provider for a particular service.
     *
     * @param name The service name
     * @param provider The service provider implementation
     */
    virtual void unregister(std::string name, const Service &provider) = 0;

    /**
     * @brief Unregister a particular provider.
     *
     * @param provider The service provider implementation
     */
    virtual void unregister(const Service &provider) = 0;

    /**
     * @brief Queries for a provider. This may return null if no provider has been registered for a service. The highest
     * priority provider is returned.
     *
     * @param name The service name
     * @return provider or null
     */
    virtual std::shared_ptr<Service> get(std::string name) const = 0;

    template <typename T>
    std::shared_ptr<T> load(std::string name) const
    {
        return std::static_pointer_cast<T>(get(std::move(name)));
    }
};
}  // namespace endstone
