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

#include "bedrock/platform/threading/mutex_details.h"

namespace Bedrock::Threading {
template <typename ContentT, typename MutexT = Mutex>
class BasicLockbox {
public:
    template <typename CallableT>
    auto lock(CallableT &&callable) const
    {
        std::lock_guard<MutexT> lock(mutex_);
        return callable(content_);
    }

    template <typename CallableT>
    auto lock(CallableT &&callable)
    {
        std::lock_guard<MutexT> lock(mutex_);
        return callable(content_);
    }

protected:
    template <typename CallableT>
    friend void lock(CallableT &&callable);

    MutexT mutex_;
    ContentT content_;
};

template <typename ContentT, typename MutexT = SharedMutex>
class SharedLockbox : BasicLockbox<ContentT, MutexT> {
public:
    template <typename Fn>
    auto lockShared(Fn &&callable) const
    {
        std::shared_lock<MutexT> lock(BasicLockbox<ContentT, MutexT>::mutex_);
        return callable(BasicLockbox<ContentT, MutexT>::content_);
    }

    template <typename Fn>
    auto lockShared(Fn &&callable)
    {
        std::shared_lock<MutexT> lock(BasicLockbox<ContentT, MutexT>::mutex_);
        return callable(BasicLockbox<ContentT, MutexT>::content_);
    }
};
}  // namespace Bedrock::Threading
