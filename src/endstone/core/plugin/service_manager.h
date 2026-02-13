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

#include <mutex>
#include <unordered_map>
#include <vector>

#include "endstone/plugin/service_manager.h"

namespace endstone::core {

/**
 * @brief A registered service provider.
 */
class RegisteredServiceProvider {
public:
    RegisteredServiceProvider(const std::string &name, const std::shared_ptr<Service> &provider,
                              ServicePriority priority, Plugin &plugin)
        : name_(name), plugin_(plugin), provider_(provider), priority_(priority)
    {
    }

    [[nodiscard]] std::string getName() const { return name_; }

    [[nodiscard]] Plugin &getPlugin() const { return plugin_; }

    [[nodiscard]] std::shared_ptr<Service> getProvider() const { return provider_; }

    [[nodiscard]] ServicePriority getPriority() const { return priority_; }

    bool operator<(const RegisteredServiceProvider &other) const { return priority_ < other.getPriority(); }

    bool operator>(const RegisteredServiceProvider &other) const { return priority_ > other.getPriority(); }

    bool operator==(const RegisteredServiceProvider &other) const
    {
        return static_cast<int>(priority_) == static_cast<int>(other.getPriority());
    }

    bool operator!=(const RegisteredServiceProvider &other) const { return !(*this == other); }

    bool operator<=(const RegisteredServiceProvider &other) const { return !(*this > other); }

    bool operator>=(const RegisteredServiceProvider &other) const { return !(*this < other); }

private:
    std::string name_;
    std::reference_wrapper<Plugin> plugin_;
    std::shared_ptr<Service> provider_;
    ServicePriority priority_;
};

class EndstoneServiceManager : public ServiceManager {
public:
    void registerService(std::string name, std::shared_ptr<Service> provider, const Plugin &plugin,
                         ServicePriority priority) override;
    void unregisterAll(const Plugin &plugin) override;
    void unregister(std::string name, const Service &provider) override;
    void unregister(const Service &provider) override;
    std::shared_ptr<Service> get(std::string name) const override;

private:
    std::unordered_map<std::string, std::vector<RegisteredServiceProvider>> providers_;
    mutable std::mutex mutex_;
};

}  // namespace endstone::core
