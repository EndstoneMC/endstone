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

#include "bedrock/gamerefs/gamerefs_shareptr/gamerefs_shareptr.h"

template <typename Type>
class StackRefResult : public GameRefs<Type>::StackResultStorage {
public:
    using StackRef = typename GameRefs<Type>::StackRef;
    using GameRefs<Type>::StackResultStorage::StackResultStorage;

    operator bool() const  // NOLINT(*-explicit-constructor)
    {
        return hasValue();
    }

    bool hasValue() const  // NOLINT(*-use-nodiscard)
    {
        return GameRefs<Type>::StackResultStorage::_hasValue();
    }

    StackRef &operator*() const
    {
        return value();
    }

    StackRef &operator*()
    {
        return value();
    }

    StackRef *operator->() const
    {
        return &value();
    }

    StackRef *operator->()
    {
        return &value();
    }

    StackRef &value() const
    {
        return GameRefs<Type>::StackResultStorage::_getStackRef();
    }

    StackRef &value()
    {
        return GameRefs<Type>::StackResultStorage::_getStackRef();
    }

    WeakRef<Type> getWeakRef() const;
    bool operator==(nullptr_t) const;
    bool operator!=(nullptr_t) const;

private:
    template <typename Type2>
    friend class WeakRef;
    template <typename Type2>
    explicit StackRefResult(const WeakRef<Type2> &weak_ref) : GameRefs<Type>::StackResultStorage(weak_ref)
    {
    }
};
