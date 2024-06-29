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

#include <memory>

template <typename T>
class OwnerPtr;

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

    ~SharedPtr() noexcept
    {
        if (pc_) {
            if (--pc_->share_count <= 0) {
                delete pc_->ptr;
                pc_->ptr = nullptr;

                if (pc_->weak_count <= 0) {
                    delete pc_;
                }
            }
        }
    }

    // copy constructor
    SharedPtr(const SharedPtr &other) noexcept
    {
        if (other.pc_) {
            other.pc_->share_count++;
        }
        pc_ = other.pc_;
    }

    // move constructor
    SharedPtr(SharedPtr &&other) noexcept
    {
        pc_ = other.pc_;
        other.pc_ = nullptr;
    }

    // copy assignment operator
    SharedPtr &operator=(const SharedPtr &other) noexcept
    {
        SharedPtr(other).swap(*this);
        return *this;
    }

    // move assignment operator
    SharedPtr &operator=(SharedPtr &&other) noexcept
    {
        SharedPtr(std::move(other)).swap(*this);
        return *this;
    }

    void reset() noexcept
    {
        SharedPtr().swap(*this);
    }

    void reset(T *ptr) noexcept
    {
        SharedPtr(ptr).swap(*this);
    }

    T *operator->() const noexcept
    {
        return get();
    }

    T &operator*() const noexcept
    {
        return *get();
    }

    explicit operator bool() const
    {
        return get() != nullptr;
    }

private:
    void swap(SharedPtr &other) noexcept
    {
        std::swap(pc_, other.pc_);
    }

    T *get() const noexcept
    {
        return pc_ ? pc_->ptr : nullptr;
    }

    SharedCounter<T> *pc_{nullptr};
};

template <typename T>
class WeakPtr {
private:
    SharedCounter<T> *pc_;
};

template <typename T>
class WeakRef {
public:
    T &operator*() const
    {
        if (auto lock = ref_.lock()) {
            return *lock;
        }
        throw std::bad_weak_ptr();
    }

    T *operator->() const noexcept
    {
        return ref_.lock().get();
    }

private:
    std::weak_ptr<T> ref_;
};
