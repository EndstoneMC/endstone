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
#include <stdexcept>

#include <gsl/pointers>

#include "bedrock/core/utility/enable_non_owner_references.h"

namespace Bedrock {

template <class T>
class NonOwnerPointer {
public:
    NonOwnerPointer() noexcept = default;
    NonOwnerPointer(nullptr_t) noexcept {}  // NOLINT(*-explicit-constructor)
    NonOwnerPointer(T *ptr)                 // NOLINT(*-explicit-constructor)
    {
        if (ptr) {
            _setControlBlock(ptr);
        }
        if (control_block_->is_valid) {
            pointer_ = ptr;
        }
    }
    NonOwnerPointer(T &ptr) : NonOwnerPointer(&ptr) {}  // NOLINT(*-explicit-constructor)
    NonOwnerPointer(const NonOwnerPointer &) = default;
    NonOwnerPointer(NonOwnerPointer &&) = default;
    ~NonOwnerPointer()
    {
        reset();
    }
    NonOwnerPointer &operator=(const NonOwnerPointer &) = default;
    NonOwnerPointer &operator=(NonOwnerPointer &&) = default;

    void reset()
    {
        control_block_.reset();
        pointer_ = nullptr;
    }

    T *operator->() noexcept
    {
        return _get();
    }

    T &operator*() noexcept
    {
        return *_get();
    }

    T *operator->() const noexcept
    {
        return _get();
    }

    T &operator*() const noexcept
    {
        return *_get();
    }

    [[nodiscard]] T *access() const
    {
        return _get();
    }

    [[nodiscard]] explicit operator bool() const noexcept
    {
        return isValid();
    }

    [[nodiscard]] bool isValid() const noexcept
    {
        return control_block_ && control_block_->is_valid;
    }

    [[nodiscard]] bool operator==(nullptr_t) const noexcept
    {
        return !isValid();
    }

    [[nodiscard]] bool operator!=(nullptr_t) const noexcept
    {
        return isValid();
    }

private:
    void _setControlBlock(const EnableNonOwnerReferences *ptr)
    {
        if (control_block_) {
            throw std::runtime_error("Invalid state");
        }
        if (ptr) {
            if (!ptr->control_block_) {
                throw std::runtime_error("_setControlBlock(): ptr has no control block");
            }
            control_block_ = ptr->control_block_;
            if (!isValid()) {
                throw std::runtime_error("The newly set NonOwnerPointer was not valid");
            }
        }
    }

    T *_get() const
    {
        if (!control_block_) {
            throw std::runtime_error("Accessing a null NonOwnerPointer");
        }
        if (!control_block_->is_valid) {
            throw std::runtime_error("Accessing a dangling NonOwnerPointer after the target object has been deleted");
        }
        return pointer_;
    }

    std::shared_ptr<EnableNonOwnerReferences::ControlBlock> control_block_{nullptr};  // +0
    T *pointer_{nullptr};                                                             // +24
};

template <typename T>
using NotNullNonOwnerPtr = gsl::not_null<NonOwnerPointer<T>>;

}  // namespace Bedrock
