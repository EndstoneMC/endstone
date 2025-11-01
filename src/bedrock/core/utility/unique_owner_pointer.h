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
template <typename T>
class UniqueOwnerPointer {
public:
    UniqueOwnerPointer();
    UniqueOwnerPointer(nullptr_t);
    ~UniqueOwnerPointer();
    UniqueOwnerPointer(const UniqueOwnerPointer &) = delete;
    UniqueOwnerPointer(UniqueOwnerPointer &&);
    UniqueOwnerPointer(std::unique_ptr<T>);
    UniqueOwnerPointer &operator=(const UniqueOwnerPointer &) = delete;
    UniqueOwnerPointer &operator=(UniqueOwnerPointer &&);
    UniqueOwnerPointer &operator=(nullptr_t);
    template <typename U>
    UniqueOwnerPointer &operator=(std::unique_ptr<T> &&);
    T *operator->() const;
    T &operator*() const;
    T *get() const;
    explicit operator bool() const;
    bool isValid() const;
    bool operator==(const UniqueOwnerPointer &) const;
    bool operator==(nullptr_t) const;
    void reset();

private:
    std::unique_ptr<EnableNonOwnerReferences> mEnableNonOwnerReferences;
    std::unique_ptr<T> mValue;
};
}  // namespace Bedrock
