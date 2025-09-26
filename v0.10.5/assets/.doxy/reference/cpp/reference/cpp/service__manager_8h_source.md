

# File service\_manager.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**plugin**](dir_53ee43673b2467e53c4cb8c30a2e7d89.md) **>** [**service\_manager.h**](service__manager_8h.md)

[Go to the documentation of this file](service__manager_8h.md)


```C++
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

#include "endstone/plugin/service.h"
#include "endstone/plugin/service_priority.h"

namespace endstone {

class Plugin;

class ServiceManager {
public:
    virtual ~ServiceManager() = default;

    virtual void registerService(std::string name, std::shared_ptr<Service> provider, const Plugin &plugin,
                                 ServicePriority priority) = 0;

    virtual void unregisterAll(const Plugin &plugin) = 0;

    virtual void unregister(std::string name, const Service &provider) = 0;

    virtual void unregister(const Service &provider) = 0;

    virtual std::shared_ptr<Service> get(std::string name) const = 0;

    template <typename T>
    std::shared_ptr<T> load(std::string name) const
    {
        return std::static_pointer_cast<T>(get(std::move(name)));
    }
};
}  // namespace endstone
```


