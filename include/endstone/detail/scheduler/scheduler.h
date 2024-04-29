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
#include <mutex>
#include <queue>

#include <moodycamel/concurrentqueue.h>

#include "endstone/detail/scheduler/task.h"
#include "endstone/scheduler/scheduler.h"

namespace endstone::detail {

class EndstoneScheduler : public Scheduler {
public:
    explicit EndstoneScheduler(Server &server);
    ~EndstoneScheduler() override = default;
    Task *runTask(Plugin &plugin, std::function<void()> task) override;
    Task *runTaskLater(Plugin &plugin, std::function<void()> task, std::uint64_t delay) override;
    Task *runTaskTimer(Plugin &plugin, std::function<void()> task, std::uint64_t delay, std::uint64_t period) override;
    void cancelTask(TaskId id) override;
    void cancelTasks(Plugin &plugin) override;
    bool isRunning(TaskId id) override;
    bool isQueued(TaskId id) override;
    std::vector<Task *> getPendingTasks() override;

    void mainThreadHeartbeat(std::uint64_t current_tick);

private:
    TaskId nextId();

    struct TaskComparator {
        bool operator()(const std::shared_ptr<EndstoneTask> &lhs, const std::shared_ptr<EndstoneTask> &rhs);
    };

    Server &server_;
    std::atomic<TaskId> ids_{1};
    moodycamel::ConcurrentQueue<std::shared_ptr<EndstoneTask>> pending_{};
    std::unordered_map<TaskId, std::shared_ptr<EndstoneTask>> tasks_{};
    std::mutex tasks_mtx_{};
    std::map<std::uint64_t, std::vector<std::shared_ptr<EndstoneTask>>> queue_{};
    std::uint64_t current_tick_{0};
    std::atomic<TaskId> current_task_{0};
    TaskComparator cmp_{};
};

}  // namespace endstone::detail
