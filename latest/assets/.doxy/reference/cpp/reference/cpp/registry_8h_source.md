

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

#include <concepts>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

#include "endstone/namespaced_key.h"

namespace endstone {

template <typename T>
concept Keyed = requires(const T &t) {
    {
        t.getKey()
    } -> std::convertible_to<std::string_view>;
};

template <Keyed T>
class Registry {
public:
    using key_type = std::string_view;
    using value_type = std::unique_ptr<T>;
    using reference = T &;
    using const_reference = const T &;

protected:
    using storage_type = std::unordered_map<key_type, value_type>;

public:
    using iterator = typename storage_type::iterator;
    using const_iterator = typename storage_type::const_iterator;

    virtual ~Registry() = default;

    virtual T *get(key_type key) noexcept = 0;

    virtual const T *get(key_type key) const noexcept = 0;

    virtual T &getOrThrow(key_type key)
    {
        if (auto *p = get(key)) {
            return *p;
        }
        throw std::invalid_argument(std::string{"No object with key: "} + std::string{key});
    }

    virtual const T &getOrThrow(key_type key) const
    {
        if (auto *p = get(key)) {
            return *p;
        }
        throw std::invalid_argument(std::string{"No object with key: "} + std::string{key});
    }

    virtual iterator begin() = 0;

    virtual iterator end() = 0;

    virtual const_iterator begin() const = 0;

    virtual const_iterator end() const = 0;
};
}  // namespace endstone
```


