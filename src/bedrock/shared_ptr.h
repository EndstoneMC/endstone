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
#include <memory>

#include <gsl/gsl>

template <typename T>
struct SharedCounter {
    T *ptr{nullptr};
    std::atomic<int> share_count{1};
    std::atomic<int> weak_count{0};
};

template <typename T>
class SharedPtr {
public:
    constexpr SharedPtr() noexcept = default;
    constexpr SharedPtr(nullptr_t) noexcept {}  // NOLINT(*-explicit-constructor)
    explicit SharedPtr(T *ptr)
    {
        if (ptr) {
            pc_ = new SharedCounter<T>();
            pc_->ptr = ptr;
        }
    }

    SharedPtr(const SharedPtr &other) noexcept
    {
        if (other.pc_) {
            other.pc_->share_count++;
        }
        pc_ = other.pc_;
    }

    ~SharedPtr() noexcept { reset(); }

    T *get() const noexcept { return pc_ ? pc_->ptr : nullptr; }

    operator T *() const { return get(); }

    T &operator*() const noexcept { return *get(); }

    T *operator->() const noexcept { return get(); }

    void reset() noexcept
    {
        if (pc_) {
            if (--pc_->share_count <= 0) {
                delete pc_->ptr;
                pc_->ptr = nullptr;

                if (pc_->weak_count <= 0) {
                    delete pc_;
                }
            }
            pc_ = nullptr;
        }
    }

private:
    template <typename T1>
    friend class WeakPtr;

    SharedCounter<T> *pc_{nullptr};
};

template <typename T>
class WeakPtr {
public:
    constexpr WeakPtr() noexcept = default;
    constexpr WeakPtr(nullptr_t) noexcept {}  // NOLINT(*-explicit-constructor)

    template <typename TDerived>
    friend class WeakPtr;

    // 1.26.51 stores the object directly instead of a SharedCounter, so there is no weak count to keep.
    template <typename TDerived>
    WeakPtr(const WeakPtr<TDerived> &other) noexcept  // NOLINT(*-explicit-constructor)
        : ptr_(reinterpret_cast<T *>(other.ptr_))
    {
    }

    explicit WeakPtr(const SharedPtr<T> &shared_ptr) noexcept : ptr_(shared_ptr.get()) {}

    WeakPtr(const WeakPtr &other) noexcept = default;
    ~WeakPtr() noexcept = default;
    WeakPtr &operator=(const WeakPtr &other) noexcept = default;

    [[nodiscard]] bool isNull() const noexcept { return ptr_ == nullptr; }

    T *get() const noexcept { return ptr_; }

    operator T *() const { return get(); }  // NOLINT(*-explicit-constructor)

    gsl::not_null<T *> operator->() const noexcept { return get(); }

    void reset() noexcept { ptr_ = nullptr; }

    static WeakPtr const &null()
    {
        static WeakPtr weak_null{nullptr};
        return weak_null;
    }

private:
    T *ptr_{nullptr};
};
