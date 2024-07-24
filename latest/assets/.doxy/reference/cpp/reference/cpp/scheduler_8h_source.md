

# File scheduler.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**scheduler**](dir_3a50fadda3c7cdc27c965a478a3f7d27.md) **>** [**scheduler.h**](scheduler_8h.md)

[Go to the documentation of this file](scheduler_8h.md)


```C++
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

#include "endstone/scheduler/task.h"

namespace endstone {

class Scheduler {
public:
    virtual ~Scheduler() = default;

    virtual std::shared_ptr<Task> runTask(Plugin &plugin, std::function<void()> task) = 0;

    virtual std::shared_ptr<Task> runTaskLater(Plugin &plugin, std::function<void()> task, std::uint64_t delay) = 0;

    virtual std::shared_ptr<Task> runTaskTimer(Plugin &plugin, std::function<void()> task, std::uint64_t delay,
                                               std::uint64_t period) = 0;

    virtual std::shared_ptr<Task> runTaskAsync(Plugin &plugin, std::function<void()> task) = 0;

    virtual std::shared_ptr<Task> runTaskLaterAsync(Plugin &plugin, std::function<void()> task, std::uint64_t delay) = 0;

    virtual std::shared_ptr<Task> runTaskTimerAsync(Plugin &plugin, std::function<void()> task, std::uint64_t delay,
                                               std::uint64_t period) = 0;

    virtual void cancelTask(TaskId id) = 0;

    virtual void cancelTasks(Plugin &plugin) = 0;

    virtual bool isRunning(TaskId id) = 0;

    virtual bool isQueued(TaskId id) = 0;

    virtual std::vector<Task *> getPendingTasks() = 0;
};

}  // namespace endstone
```


