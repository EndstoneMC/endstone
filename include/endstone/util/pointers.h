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

/**
 * A wrapper around a std::shared_ptr that is guaranteed never to be null.
 *
 * Construction from a null pointer throws, and the pointer-mutating operators are deleted. This mirrors
 * `gsl::not_null<std::shared_ptr<T>>`, but throws `std::invalid_argument` on a null pointer (instead of a contract
 * violation) so that a misbehaving plugin cannot crash the host.
 */
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

    /** Covariant conversion from a shared_ptr of a convertible (e.g. derived) type. */
    template <class U>
        requires(!std::is_same_v<U, T>) && std::is_convertible_v<U *, T *>
    NotNull(std::shared_ptr<U> ptr) : NotNull(std::shared_ptr<T>(std::move(ptr)))
    {
    }

    /** Covariant conversion from a NotNull of a convertible (e.g. derived) type. */
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

/**
 * A wrapper around a std::shared_ptr that may be null.
 *
 * Convertible from a NotNull, and contextually convertible to bool to test for a value.
 */
template <class T>
class Nullable {
public:
    using pointer_type = std::shared_ptr<T>;
    using element_type = T;

    constexpr Nullable() noexcept = default;
    constexpr Nullable(std::nullptr_t) noexcept {}
    Nullable(std::shared_ptr<T> ptr) : ptr_(std::move(ptr)) {}
    Nullable(const NotNull<T> &other) : ptr_(other.get()) {}

    /** Covariant conversion from a shared_ptr of a convertible (e.g. derived) type. */
    template <class U>
        requires(!std::is_same_v<U, T>) && std::is_convertible_v<U *, T *>
    Nullable(std::shared_ptr<U> ptr) : ptr_(std::move(ptr))
    {
    }

    /** Covariant conversion from a Nullable of a convertible (e.g. derived) type. */
    template <class U>
        requires(!std::is_same_v<U, T>) && std::is_convertible_v<U *, T *>
    Nullable(const Nullable<U> &other) : ptr_(other.get())
    {
    }

    /** Covariant conversion from a NotNull of a convertible (e.g. derived) type. */
    template <class U>
        requires(!std::is_same_v<U, T>) && std::is_convertible_v<U *, T *>
    Nullable(const NotNull<U> &other) : ptr_(other.get())
    {
    }

    const pointer_type &get() const noexcept { return ptr_; }
    T *operator->() const noexcept { return ptr_.get(); }
    T &operator*() const noexcept { return *get(); }
    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    /** Returns a reference to the contained value, throwing `std::runtime_error` if this is null. */
    T &value() const
    {
        Preconditions::checkState(ptr_ != nullptr, "Nullable holds no value.");
        return *ptr_;
    }

    /** Returns the contained pointer if present, otherwise the supplied default. */
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
