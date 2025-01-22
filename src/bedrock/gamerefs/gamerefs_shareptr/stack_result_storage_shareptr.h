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

#include "bedrock/gamerefs/gamerefs_shareptr/weak_storage_shareptr.h"

template <typename T>
class StackResultStorageSharePtr {
protected:
    StackResultStorageSharePtr() = default;
    explicit StackResultStorageSharePtr(nullptr_t) : value_(nullptr) {}
    template <typename U>
    explicit StackResultStorageSharePtr(const WeakStorageSharePtr<U> &weak_storage)
    {
        if (!weak_storage.handle_.expired()) {
            value_ = weak_storage.handle_.lock();
        }
    }
    StackResultStorageSharePtr(const StackResultStorageSharePtr &) = delete;
    StackResultStorageSharePtr(StackResultStorageSharePtr &&) noexcept = default;
    StackResultStorageSharePtr &operator=(const StackResultStorageSharePtr &) = delete;
    StackResultStorageSharePtr &operator=(StackResultStorageSharePtr &&) = delete;

    bool _hasValue() const
    {
        return value_ != nullptr;
    }

    T &_getStackRef() const
    {
        return *value_;
    }

private:
    std::shared_ptr<T> value_;
};
