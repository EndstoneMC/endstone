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

namespace Bedrock {

template <class T>
class NonOwnerPointer {
public:
    using ElementType = std::remove_extent_t<T>;

    NonOwnerPointer(){};
    explicit NonOwnerPointer(ElementType &obj) : ptr(&obj){};

    T &operator*() const noexcept
    {
        return *get();
    }

    T *operator->() const noexcept
    {
        return get();
    }

    explicit operator bool() const noexcept
    {
        return get() != nullptr;
    }

    [[nodiscard]] ElementType *get() const noexcept
    {
        return ptr.get();
    }

    std::shared_ptr<ElementType> ptr;
};

class EnableNonOwnerReferences {
public:
    EnableNonOwnerReferences() : ptr(*this) {}
    virtual ~EnableNonOwnerReferences() = default;

    NonOwnerPointer<EnableNonOwnerReferences> ptr;  // +8
};

}  // namespace Bedrock

template <typename T>
class WeakPtr {
    void *rep_;  // reference counter block
};

namespace std {
template <class T1, class T2>
[[nodiscard]] Bedrock::NonOwnerPointer<T1> static_pointer_cast(const Bedrock::NonOwnerPointer<T2> &other) noexcept
{
    auto obj = static_cast<typename Bedrock::NonOwnerPointer<T1>::ElementType *>(other.get());
    auto result = Bedrock::NonOwnerPointer<T1>();
    result.ptr = std::shared_ptr<T1>(other.ptr, obj);
    return result;
}
}  // namespace std
