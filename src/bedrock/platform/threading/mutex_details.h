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

#include <mutex>
#include <shared_mutex>

#include "bedrock/platform/threading/spin_lock.h"

namespace Bedrock::Threading {

#ifdef _WIN32
using Mutex = std::mutex;
using RecursiveMutex = std::recursive_mutex;
using SharedMutex = std::shared_mutex;
#endif

#ifdef __linux__
using Mutex = std::mutex;
using RecursiveMutex = std::recursive_mutex;
using SharedMutex = std::shared_timed_mutex;
#endif

template <typename T>
class UniqueLock : public std::unique_lock<T> {};

template <typename T>
class LockGuard : public std::lock_guard<T> {
    using std::lock_guard<T>::lock_guard;
};

}  // namespace Bedrock::Threading

using SpinLock = SpinLockImpl;
