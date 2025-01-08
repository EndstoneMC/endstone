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

    ~SharedPtr() noexcept
    {
        reset();
    }

    T *get() const noexcept
    {
        return pc_ ? pc_->ptr : nullptr;
    }

    operator T *() const
    {
        return get();
    }

    T &operator*() const noexcept
    {
        return *get();
    }

    T *operator->() const noexcept
    {
        return get();
    }

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
    constexpr WeakPtr(nullptr_t) noexcept {}

    explicit WeakPtr(const SharedPtr<T> &shared_ptr) noexcept : pc_(shared_ptr.pc_)
    {
        if (pc_) {
            ++pc_->weak_count;
        }
    }

    WeakPtr(const WeakPtr &other) noexcept
    {
        if (other.pc_) {
            ++other.pc_->weak_count;
        }
        pc_ = other.pc_;
    }

    ~WeakPtr()
    {
        reset();
    }

    WeakPtr &operator=(const WeakPtr &other) noexcept
    {
        if (this != &other) {
            if (other.pc_) {
                ++other.pc_->weak_count;
            }
            reset();
            pc_ = other.pc_;
        }
        return *this;
    }

    [[nodiscard]] bool isNull() const noexcept
    {
        return get() == nullptr;
    }

    T *get() const noexcept
    {
        return pc_ ? pc_->ptr : nullptr;
    }

    operator T *() const
    {
        return get();
    }

    gsl::not_null<T *> operator->() const noexcept
    {
        return get();
    }

    void reset() noexcept
    {
        if (pc_) {
            if (--pc_->weak_count <= 0 && pc_->ptr == nullptr) {
                delete pc_;
            }
            pc_ = nullptr;
        }
    }

    static WeakPtr const &null()
    {
        static WeakPtr weak_null{nullptr};
        return weak_null;
    }

private:
    SharedCounter<T> *pc_{nullptr};
};
