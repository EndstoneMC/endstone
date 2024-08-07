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
#include <thread>

#include "bedrock/bedrock.h"
#include "bedrock/forward.h"

namespace Bedrock::Threading {

using Mutex = std::mutex;

class AssignedThread {
public:
    bool isOnThread()
    {
        return std::this_thread::get_id() == assigned_id_;
    }

private:
    std::thread::id assigned_id_;
};

class EnableQueueForThread {
public:
    virtual ~EnableQueueForThread() = 0;

private:
    std::unique_ptr<TaskGroup> task_group_;  // +8
};

class EnableQueueForMainThread : public EnableQueueForThread {};

ENDSTONE_HOOK AssignedThread &getServerThread();

template <typename T>
class IAsyncResult;

}  // namespace Bedrock::Threading
