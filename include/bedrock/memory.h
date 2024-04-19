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

class EnableNonOwnerReferences {
public:
    virtual ~EnableNonOwnerReferences() = default;

private:
    std::shared_ptr<EnableNonOwnerReferences> ref_;
};

template <class T>
using NonOwnerPointer = std::shared_ptr<T>;

}  // namespace Bedrock
