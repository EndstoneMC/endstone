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
#include <queue>

#include "endstone/detail/scheduler/task.h"
#include "endstone/scheduler/scheduler.h"

namespace endstone::detail {

class EndstoneScheduler {
private:
    struct PendingTaskComparator {
        bool operator()(const EndstoneTask &lhs, const EndstoneTask &rhs)
        {
            if (lhs.getNextRun() != rhs.getNextRun()) {
                return lhs.getNextRun() > rhs.getNextRun();
            }
            return lhs.getCreatedAt() > rhs.getCreatedAt();
        }
    };

public:
protected:
    TaskId incrementIds();

private:
    std::atomic<TaskId> ids_{1};
    std::priority_queue<EndstoneTask, std::vector<EndstoneTask>, PendingTaskComparator> pending_;
};

}  // namespace endstone::detail
