

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

#include <cstddef>
#include <cstring>
#include <functional>
#include <type_traits>
#include <typeinfo>

namespace endstone {

class ClassInfo {
public:
    explicit ClassInfo(const std::type_info &info) noexcept : info_(&info) {}

    template <typename T>
    static ClassInfo of() noexcept
    {
        return ClassInfo{typeid(T)};
    }

    [[nodiscard]] const char *name() const noexcept { return info_->name(); }

    [[nodiscard]] const std::type_info &info() const noexcept { return *info_; }

    bool operator==(const ClassInfo &other) const noexcept
    {
        return info_ == other.info_ || std::strcmp(name(), other.name()) == 0;
    }

private:
    const std::type_info *info_;
};

class Object {
public:
    virtual ~Object() = default;

    [[nodiscard]] virtual ClassInfo getClassInfo() const = 0;

    [[nodiscard]] virtual bool isInstanceOf(ClassInfo target) const = 0;

    template <typename T>
        requires std::is_base_of_v<Object, T>
    T *as()
    {
        if (isInstanceOf(ClassInfo::of<T>())) {
            return static_cast<T *>(this);
        }
        return nullptr;
    }

    template <typename T>
        requires std::is_base_of_v<Object, T>
    const T *as() const
    {
        if (isInstanceOf(ClassInfo::of<T>())) {
            return static_cast<const T *>(this);
        }
        return nullptr;
    }

    template <typename T>
        requires std::is_base_of_v<Object, T>
    [[nodiscard]] bool is() const
    {
        return isInstanceOf(ClassInfo::of<T>());
    }
};

}  // namespace endstone

template <>
struct std::hash<endstone::ClassInfo> {
    std::size_t operator()(const endstone::ClassInfo &info) const noexcept
    {
        std::size_t hash = 5381;
        for (const auto *ptr = info.name(); *ptr != '\0'; ++ptr) {
            hash = hash * 33 ^ static_cast<unsigned char>(*ptr);
        }
        return hash;
    }
};
```


