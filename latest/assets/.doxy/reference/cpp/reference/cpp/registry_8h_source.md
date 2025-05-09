

# File registry.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**registry.h**](registry_8h.md)

[Go to the documentation of this file](registry_8h.md)


```C++
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

#include <stdexcept>

#include "endstone/namespaced_key.h"

namespace endstone {

template <typename T>
class Registry {
public:
    virtual ~Registry() = default;

    virtual T *get(NamespacedKey key) noexcept = 0;

    virtual const T *get(NamespacedKey key) const noexcept = 0;

    virtual T &getOrThrow(const NamespacedKey key)
    {
        if (auto *p = get(key)) {
            return *p;
        }
        throw std::invalid_argument(std::string{"No registry entry found for key: "} + key.toString());
    }

    virtual const T &getOrThrow(const NamespacedKey key) const
    {
        if (auto *p = get(key)) {
            return *p;
        }
        throw std::invalid_argument(std::string{"No registry entry found for key: "} + key.toString());
    }

    virtual void forEach(std::function<bool(const T &)> func) const = 0;
};
}  // namespace endstone
```


