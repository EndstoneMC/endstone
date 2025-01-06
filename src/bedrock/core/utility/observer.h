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

#include <atomic>
#include <cstdint>
#include <vector>

#include <gsl/gsl>

namespace Core {

template <typename ObserverType, typename LockType>
class Subject {
public:
private:
    LockType lock_;
    std::vector<gsl::not_null<ObserverType *>> observers_;
};

template <typename T, typename LockType>
class Observer {
public:
    virtual ~Observer() = 0;

private:
    virtual void _onSubjectDestroyed() = 0;

    using SubjectType = Subject<T, LockType>;
    SubjectType *subject_;
};

class SingleThreadedLock {
public:
private:
    std::atomic<bool> locked_;
};

}  // namespace Core
