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

namespace Bedrock {

class EnableNonOwnerReferences {
public:
    struct ControlBlock {
        EnableNonOwnerReferences *ptr;
    };

    EnableNonOwnerReferences()
    {
        control_block_ = std::make_shared<ControlBlock>();
        control_block_->ptr = this;
    }
    virtual ~EnableNonOwnerReferences() = default;

private:
    template <typename T>
    friend class NonOwnerPointer;

    std::shared_ptr<Bedrock::EnableNonOwnerReferences::ControlBlock> control_block_;
};

template <class T>
class NonOwnerPointer {
public:
    using ElementType = std::remove_extent_t<T>;

    constexpr NonOwnerPointer() noexcept = default;
    constexpr NonOwnerPointer(nullptr_t) noexcept {}  // NOLINT(*-explicit-constructor)
    explicit NonOwnerPointer(ElementType &obj)
    {
        reset();
        set(&obj);
    };

    ~NonOwnerPointer()
    {
        reset();
    }

    constexpr T &operator*() const noexcept
    {
        return *get();
    }

    constexpr T *operator->() const noexcept
    {
        return get();
    }

    constexpr explicit operator bool() const noexcept
    {
        return get() != nullptr;
    }

    void reset()
    {
        if (control_block_ && !control_block_->ptr) {
            throw std::runtime_error("Resetting a dangling pointer!");
        }
        control_block_.reset();
    }

private:
    T *get() const
    {
        if (!control_block_) {
            throw std::runtime_error("Accessing a null NonOwnerPointer");
        }
        if (!control_block_->ptr) {
            throw std::runtime_error("Accessing a dangling NonOwnerPointer after the target object has been deleted");
        }
        return static_cast<T *>(control_block_->ptr);
    }

    void set(T *ptr)
    {
        if (control_block_) {
            throw std::runtime_error("Invalid state: control block has been set");
        }

        if (ptr) {
            if (!ptr->control_block_) {
                throw std::runtime_error("set_(): ptr has no control block");
            }
            control_block_ = ptr->control_block_;
        }
    }
    std::shared_ptr<Bedrock::EnableNonOwnerReferences::ControlBlock> control_block_;
};

}  // namespace Bedrock
