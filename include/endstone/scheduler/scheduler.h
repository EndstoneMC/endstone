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

/**
 * @brief Represents a scheduler that executes various tasks.
 */
class Scheduler {
public:
    virtual ~Scheduler() = default;

    /**
     * Returns a task that will be executed synchronously on the next server tick.
     *
     * @param plugin the reference to the plugin scheduling task
     * @param task the task to be run
     * @return a Task that contains the id number (nullptr if task is empty)
     */
    virtual std::shared_ptr<Task> runTask(Plugin &plugin, std::function<void()> task) = 0;

    /**
     * Returns a task that will be executed synchronously after the specified number of server ticks.
     *
     * @param plugin the reference to the plugin scheduling task
     * @param task the task to be run
     * @param delay the ticks to wait before running the task
     * @return a Task that contains the id number (nullptr if task is empty)
     */
    virtual std::shared_ptr<Task> runTaskLater(Plugin &plugin, std::function<void()> task, std::uint64_t delay) = 0;

    /**
     * Returns a task that will be executed repeatedly (and synchronously) until cancelled, starting after the
     * specified number of server ticks.
     *
     * @param plugin the reference to the plugin scheduling task
     * @param task the task to be run
     * @param delay the ticks to wait before running the task
     * @param period the ticks to wait between runs
     * @return a Task that contains the id number (nullptr if task is empty)
     */
    virtual std::shared_ptr<Task> runTaskTimer(Plugin &plugin, std::function<void()> task, std::uint64_t delay,
                                               std::uint64_t period) = 0;

    /**
     * @brief Returns a task that will be executed asynchronously on the next server tick.
     * @remark Asynchronous tasks should never access any Endstone API
     *
     * @param plugin the reference to the plugin scheduling task
     * @param task the task to be run
     * @return a Task that contains the id number (nullptr if task is empty)
     */
    virtual std::shared_ptr<Task> runTaskAsync(Plugin &plugin, std::function<void()> task) = 0;

    /**
     * @brief Returns a task that will be executed asynchronously after the specified number of server ticks.
     * @remark Asynchronous tasks should never access any Endstone API
     *
     * @param plugin the reference to the plugin scheduling task
     * @param task the task to be run
     * @param delay the ticks to wait before running the task
     * @return a Task that contains the id number (nullptr if task is empty)
     */
    virtual std::shared_ptr<Task> runTaskLaterAsync(Plugin &plugin, std::function<void()> task, std::uint64_t delay) = 0;

    /**
     * @brief Returns a task that will be executed repeatedly (and asynchronously) until cancelled, starting after the
     * specified number of server ticks.
     * @remark Asynchronous tasks should never access any Endstone API
     *
     * @param plugin the reference to the plugin scheduling task
     * @param task the task to be run
     * @param delay the ticks to wait before running the task
     * @param period the ticks to wait between runs
     * @return a Task that contains the id number (nullptr if task is empty)
     */
    virtual std::shared_ptr<Task> runTaskTimerAsync(Plugin &plugin, std::function<void()> task, std::uint64_t delay,
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

    /**
     * Returns a vector of all pending tasks.
     *
     * The ordering of the tasks is NOT related to their order of execution.
     *
     * @return Pending tasks
     */
    virtual std::vector<Task *> getPendingTasks() = 0;
};

}  // namespace endstone
