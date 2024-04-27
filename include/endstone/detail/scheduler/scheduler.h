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
    void addTask(std::shared_ptr<EndstoneTask> task);
    Task *handle(std::shared_ptr<EndstoneTask> task, std::uint64_t delay);
    TaskId nextId();
    void cancelTaskById(TaskId id);
    void cancelTaskByPlugin(Plugin &plugin);

    struct PendingTaskComparator {
        bool operator()(const EndstoneTask &lhs, const EndstoneTask &rhs);
    };

    Server &server_;
    std::atomic<TaskId> ids_{1};
    std::shared_ptr<EndstoneTask> head_{new EndstoneTask(*this, {})};
    std::atomic<EndstoneTask *> tail_{head_.get()};
    std::vector<std::shared_ptr<EndstoneTask>> pending_;
    std::vector<std::shared_ptr<EndstoneTask>> temp_;
    std::unordered_map<TaskId, EndstoneTask *> runners_;
    std::mutex runners_mtx_;
    std::shared_ptr<EndstoneTask> current_task_;
    std::uint64_t current_tick_;
};

}  // namespace endstone::detail
