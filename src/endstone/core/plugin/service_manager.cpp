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

#include "endstone/core/plugin/service_manager.h"

#include <algorithm>

namespace endstone::core {

void EndstoneServiceManager::registerService(std::string name, std::shared_ptr<Service> provider, const Plugin &plugin,
                                             ServicePriority priority)
{
    std::lock_guard lock(mutex_);
    auto &registered = providers_[name];

    RegisteredServiceProvider new_provider(name, provider, priority, const_cast<Plugin &>(plugin));
    const auto position = std::ranges::lower_bound(registered, new_provider, std::greater{});
    registered.insert(position, std::move(new_provider));
}

void EndstoneServiceManager::unregisterAll(const Plugin &plugin)
{
    std::lock_guard lock(mutex_);

    for (auto it = providers_.begin(); it != providers_.end();) {
        auto &services = it->second;

        // Remove entries from the current plugin
        std::erase_if(services, [&plugin](const RegisteredServiceProvider &provider) {
            return &provider.getPlugin() == &plugin;
        });

        if (services.empty()) {
            it = providers_.erase(it);
        }
        else {
            ++it;
        }
    }
}

void EndstoneServiceManager::unregister(std::string name, const Service &provider)
{
    std::lock_guard lock(mutex_);

    if (const auto it = providers_.find(name); it != providers_.end()) {
        auto &services = it->second;

        // Remove entries matching the given provider
        std::erase_if(services, [&provider](const RegisteredServiceProvider &registered) {
            return registered.getProvider().get() == &provider;
        });

        // Remove the entire list if it's empty
        if (services.empty()) {
            providers_.erase(it);
        }
    }
}

void EndstoneServiceManager::unregister(const Service &provider)
{
    std::lock_guard lock(mutex_);
    for (auto it = providers_.begin(); it != providers_.end();) {
        auto &services = it->second;

        // Remove entries matching the given provider
        std::erase_if(services, [&provider](const RegisteredServiceProvider &registered) {
            return registered.getProvider().get() == &provider;
        });

        // Remove the entire list if it's empty
        if (services.empty()) {
            it = providers_.erase(it);
        }
        else {
            ++it;
        }
    }
}

std::shared_ptr<Service> EndstoneServiceManager::get(std::string name) const
{
    std::lock_guard lock(mutex_);
    if (const auto it = providers_.find(name); it != providers_.end() && !it->second.empty()) {
        return it->second.front().getProvider();  // Highest-priority provider is at the front
    }
    return nullptr;
}

}  // namespace endstone::core
