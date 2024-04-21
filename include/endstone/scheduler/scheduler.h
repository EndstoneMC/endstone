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

    /**
     * Schedules a once off task to be executed synchronously as soon as possible.
     *
     * @param plugin Plugin that owns the task
     * @param task Task to be executed
     * @return Task id number (std::nullopt_t if scheduling failed)
     */
    virtual std::optional<TaskId> scheduleTask(Plugin &plugin, std::function<void()> task) = 0;

    /**
     * Schedules a once off task to be executed synchronously after a delay.
     *
     * @param plugin Plugin that owns the task
     * @param task Task to be executed
     * @param delay Delay in server ticks before executing task
     * @return Task id number (std::nullopt_t if scheduling failed)
     */
    virtual std::optional<TaskId> scheduleDelayedTask(Plugin &plugin, std::function<void()> task,
                                                      std::uint64_t delay) = 0;

    /**
     * Schedules a repeating task to be executed synchronously .
     *
     * @param plugin Plugin that owns the task
     * @param task Task to be executed
     * @param delay Delay in server ticks before executing first repeat
     * @param period Period in server ticks of the task
     * @return Task id number (std::nullopt_t if scheduling failed)
     */
    virtual std::optional<TaskId> scheduleRepeatingTask(Plugin &plugin, std::function<void()> task, std::uint64_t delay,
                                                        std::uint64_t period) = 0;

    /**
     * Removes task from scheduler.
     *
     * @param taskId Id number of task to be removed
     */
    virtual void cancelTask(TaskId id) = 0;

    /**
     * Removes all tasks associated with a particular plugin from the
     * scheduler.
     *
     * @param plugin Owner of tasks to be removed
     */
    virtual void cancelTasks(Plugin &plugin) = 0;

    /**
     * Check if the task currently running.
     *
     * @param taskId The task to check.
     *
     * @return If the task is currently running.
     */
    virtual bool isRunning(TaskId id) = 0;

    /**
     * Check if the task queued to be run later.
     *
     * @param taskId The task to check.
     *
     * @return If the task is queued to be run.
     */
    virtual bool isQueued(TaskId id) = 0;

    // TODO(scheduler): add asynchronous task support
};

}  // namespace endstone
