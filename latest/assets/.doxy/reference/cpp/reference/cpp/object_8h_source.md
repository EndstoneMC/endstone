

# File object.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**object.h**](object_8h.md)

[Go to the documentation of this file](object_8h.md)


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

#include <type_traits>
#include <typeinfo>

namespace endstone {

class Object {
public:
    virtual ~Object() = default;

    [[nodiscard]] virtual const std::type_info &getClassTypeId() const = 0;

    [[nodiscard]] virtual bool isInstanceOf(const std::type_info &target) const = 0;

    template <typename T>
        requires std::is_base_of_v<Object, T>
    T *as()
    {
        if (isInstanceOf(typeid(T))) {
            return static_cast<T *>(this);
        }
        return nullptr;
    }

    template <typename T>
        requires std::is_base_of_v<Object, T>
    const T *as() const
    {
        if (isInstanceOf(typeid(T))) {
            return static_cast<const T *>(this);
        }
        return nullptr;
    }

    template <typename T>
        requires std::is_base_of_v<Object, T>
    [[nodiscard]] bool is() const
    {
        return isInstanceOf(typeid(T));
    }
};

}  // namespace endstone
```


