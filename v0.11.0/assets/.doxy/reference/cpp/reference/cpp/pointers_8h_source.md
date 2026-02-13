

# File pointers.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**util**](dir_89b85071337bf933dea6c29b4c6a4410.md) **>** [**pointers.h**](pointers_8h.md)

[Go to the documentation of this file](pointers_8h.md)


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
#include <memory>
#include <utility>

#include "endstone/check.h"

namespace endstone {
template <class T>
class NotNull {
public:
    using pointer_type = std::shared_ptr<T>;
    NotNull() = delete;
    NotNull(std::shared_ptr<T> ptr) : ptr_(std::move(ptr))
    {
        Preconditions::checkArgument(ptr_ != nullptr, "pointer must not be null.");
    }
    NotNull(const NotNull &other) = default;
    NotNull &operator=(const NotNull &other) = default;
    const pointer_type &get() const noexcept { return ptr_; }
    T *operator->() const noexcept { return ptr_.get(); }
    T &operator*() const noexcept { return *get(); }

    NotNull(std::nullptr_t) = delete;
    NotNull &operator=(std::nullptr_t) = delete;
    NotNull &operator++() = delete;
    NotNull &operator--() = delete;
    NotNull operator++(int) = delete;
    NotNull operator--(int) = delete;
    NotNull &operator+=(std::ptrdiff_t) = delete;
    NotNull &operator-=(std::ptrdiff_t) = delete;
    void operator[](std::ptrdiff_t) const = delete;
    void swap(NotNull &other) noexcept { std::swap(ptr_, other.ptr_); }

private:
    pointer_type ptr_;
};

template <class T>
class Nullable {
public:
    constexpr Nullable() noexcept = default;
    constexpr Nullable(std::nullptr_t) noexcept {}
    Nullable(std::shared_ptr<T> ptr) : ptr_(std::move(ptr)) {}
    Nullable(const NotNull<T> &other) : ptr_(other.get()) {}
    const std::shared_ptr<T> &get() const noexcept { return ptr_; }
    T *operator->() const noexcept { return ptr_.get(); }
    T &operator*() const noexcept { return *get(); }
    explicit operator bool() const noexcept { return ptr_ != nullptr; }
    bool operator==(const Nullable &) const = default;
    bool operator!=(const Nullable &) const = default;
    bool operator==(std::nullptr_t) const noexcept { return ptr_ == nullptr; }
    bool operator!=(std::nullptr_t) const noexcept { return ptr_ != nullptr; }

private:
    std::shared_ptr<T> ptr_;
};
}  // namespace endstone
```


