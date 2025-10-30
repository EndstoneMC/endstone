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

#include <atomic>

namespace Lockless {
template <typename T>
class WeakAtomic {
public:
    WeakAtomic() : value_() {}

    template <typename U>
    WeakAtomic(U &&x) : value_(std::forward<U>(x))
    {
    }

    WeakAtomic(WeakAtomic const &other) : value_(other.load()) {}

    WeakAtomic(WeakAtomic &&other) : value_(std::move(other.load())) {}

    operator T() const
    {
        return load();
    }

    template <typename U>
    WeakAtomic const &operator=(U &&x)
    {
        value_.store(std::forward<U>(x), std::memory_order_relaxed);
        return *this;
    }

    WeakAtomic const &operator=(WeakAtomic const &other)
    {
        value_.store(other.value.load(std::memory_order_relaxed), std::memory_order_relaxed);
        return *this;
    }

    T load() const
    {
        return value_.load(std::memory_order_relaxed);
    }

private:
    std::atomic<T> value_;
};
}  // namespace Lockless
