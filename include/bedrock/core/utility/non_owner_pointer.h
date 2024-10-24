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
    using ElementType = std::remove_extent_t<T>;

    constexpr NonOwnerPointer() noexcept = default;
    constexpr NonOwnerPointer(nullptr_t) noexcept {}  // NOLINT(*-explicit-constructor)
    explicit NonOwnerPointer(ElementType &ptr) : NonOwnerPointer(&ptr) {}
    explicit NonOwnerPointer(ElementType *ptr)
    {
        if (ptr) {
            _setControlBlock(ptr);
        }
        if (control_block_->is_valid) {
            pointer_ = ptr;
        }
    }

    ~NonOwnerPointer()
    {
        reset();
    }

    constexpr T &operator*() const noexcept
    {
        return *_get();
    }

    constexpr T *operator->() const noexcept
    {
        return _get();
    }

    [[nodiscard]] bool isValid() const noexcept
    {
        return control_block_ && control_block_->is_valid;
    }

    void reset()
    {
        if (control_block_ && !control_block_->is_valid) {
            throw std::runtime_error("Resetting a dangling pointer!");
        }
        control_block_.reset();
        pointer_ = nullptr;
    }

    [[nodiscard]] T *access() const
    {
        return _get();
    }

    [[nodiscard]] operator bool() const noexcept
    {
        return isValid();
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
    std::shared_ptr<EnableNonOwnerReferences::ControlBlock> control_block_{nullptr};  // +0
    T *pointer_{nullptr};                                                             // +24
};

template <typename T>
using NotNullNonOwnerPtr = gsl::not_null<NonOwnerPointer<T>>;

}  // namespace Bedrock
