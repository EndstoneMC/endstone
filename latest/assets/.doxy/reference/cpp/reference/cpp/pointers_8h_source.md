

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
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

#include "endstone/check.h"

namespace endstone {
template <class T>
class Nullable;

template <class T>
class NotNull {
public:
    using pointer_type = std::shared_ptr<T>;
    using element_type = T;

    NotNull() = delete;
    NotNull(std::shared_ptr<T> ptr) : ptr_(std::move(ptr))
    {
        Preconditions::checkArgument(ptr_ != nullptr, "pointer must not be null.");
    }
    NotNull(const NotNull &other) = default;
    NotNull &operator=(const NotNull &other) = default;

    template <class U>
        requires(!std::is_same_v<U, T>) && std::is_convertible_v<U *, T *>
    NotNull(std::shared_ptr<U> ptr) : NotNull(std::shared_ptr<T>(std::move(ptr)))
    {
    }

    template <class U>
        requires(!std::is_same_v<U, T>) && std::is_convertible_v<U *, T *>
    NotNull(const NotNull<U> &other) : ptr_(other.get())
    {
    }

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
    using pointer_type = std::shared_ptr<T>;
    using element_type = T;

    constexpr Nullable() noexcept = default;
    constexpr Nullable(std::nullptr_t) noexcept {}
    Nullable(std::shared_ptr<T> ptr) : ptr_(std::move(ptr)) {}
    Nullable(const NotNull<T> &other) : ptr_(other.get()) {}

    template <class U>
        requires(!std::is_same_v<U, T>) && std::is_convertible_v<U *, T *>
    Nullable(std::shared_ptr<U> ptr) : ptr_(std::move(ptr))
    {
    }

    template <class U>
        requires(!std::is_same_v<U, T>) && std::is_convertible_v<U *, T *>
    Nullable(const Nullable<U> &other) : ptr_(other.get())
    {
    }

    template <class U>
        requires(!std::is_same_v<U, T>) && std::is_convertible_v<U *, T *>
    Nullable(const NotNull<U> &other) : ptr_(other.get())
    {
    }

    const pointer_type &get() const noexcept { return ptr_; }
    T *operator->() const noexcept { return ptr_.get(); }
    T &operator*() const noexcept { return *get(); }
    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    T &value() const
    {
        Preconditions::checkState(ptr_ != nullptr, "Nullable holds no value.");
        return *ptr_;
    }

    pointer_type value_or(pointer_type default_value) const { return ptr_ ? ptr_ : std::move(default_value); }

    bool operator==(std::nullptr_t) const noexcept { return ptr_ == nullptr; }

private:
    std::shared_ptr<T> ptr_;
};

// Handle-to-handle comparisons (pointer identity), provided for both wrappers so they can be used as
// keys in ordered/unordered containers. These delegate to the underlying shared_ptr comparisons.
#define ENDSTONE_DEFINE_PTR_COMPARISONS(Wrapper)                                                  \
    template <class T, class U>                                                                    \
    bool operator==(const Wrapper<T> &lhs, const Wrapper<U> &rhs) noexcept                         \
    {                                                                                              \
        return lhs.get() == rhs.get();                                                            \
    }                                                                                              \
    template <class T, class U>                                                                    \
    bool operator!=(const Wrapper<T> &lhs, const Wrapper<U> &rhs) noexcept                         \
    {                                                                                              \
        return lhs.get() != rhs.get();                                                            \
    }                                                                                              \
    template <class T, class U>                                                                    \
    bool operator<(const Wrapper<T> &lhs, const Wrapper<U> &rhs) noexcept                          \
    {                                                                                              \
        return lhs.get() < rhs.get();                                                             \
    }                                                                                              \
    template <class T, class U>                                                                    \
    bool operator<=(const Wrapper<T> &lhs, const Wrapper<U> &rhs) noexcept                         \
    {                                                                                              \
        return lhs.get() <= rhs.get();                                                            \
    }                                                                                              \
    template <class T, class U>                                                                    \
    bool operator>(const Wrapper<T> &lhs, const Wrapper<U> &rhs) noexcept                          \
    {                                                                                              \
        return lhs.get() > rhs.get();                                                             \
    }                                                                                              \
    template <class T, class U>                                                                    \
    bool operator>=(const Wrapper<T> &lhs, const Wrapper<U> &rhs) noexcept                         \
    {                                                                                              \
        return lhs.get() >= rhs.get();                                                            \
    }

ENDSTONE_DEFINE_PTR_COMPARISONS(NotNull)
ENDSTONE_DEFINE_PTR_COMPARISONS(Nullable)
#undef ENDSTONE_DEFINE_PTR_COMPARISONS
}  // namespace endstone

template <class T>
struct std::hash<endstone::NotNull<T>> {
    std::size_t operator()(const endstone::NotNull<T> &value) const noexcept
    {
        return std::hash<std::shared_ptr<T>>{}(value.get());
    }
};

template <class T>
struct std::hash<endstone::Nullable<T>> {
    std::size_t operator()(const endstone::Nullable<T> &value) const noexcept
    {
        return std::hash<std::shared_ptr<T>>{}(value.get());
    }
};
```


