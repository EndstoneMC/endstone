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

#include "endstone/core/scheduler/scheduler.h"

#include "endstone/core/scheduler/async_task.h"
#include "endstone/core/util/error.h"

namespace endstone::core {

namespace {
Result<void> validate(const Plugin &plugin, const std::function<void()> &task)
{
    if (!task) {
        return nonstd::make_unexpected(make_error("Plugin {} attempted to register an empty task", plugin.getName()));
    }

    if (!plugin.isEnabled()) {
        return nonstd::make_unexpected(
            make_error("Plugin {} attempted to register task while disabled", plugin.getName()));
    }
    return {};
}
}  // namespace

EndstoneScheduler::EndstoneScheduler(Server &server) : server_(server) {}

std::shared_ptr<Task> EndstoneScheduler::runTask(Plugin &plugin, std::function<void()> task)
{
    return runTaskLater(plugin, task, 0);
}

std::shared_ptr<Task> EndstoneScheduler::runTaskLater(Plugin &plugin, std::function<void()> task, std::uint64_t delay)
{
    return runTaskTimer(plugin, task, delay, 0);
}

std::shared_ptr<Task> EndstoneScheduler::runTaskTimer(Plugin &plugin, std::function<void()> task, std::uint64_t delay,
                                                      std::uint64_t period)
{
    if (!validate(plugin, task)) {
        return nullptr;
    }

    auto t = std::make_shared<EndstoneTask>(*this, plugin, task, nextId(), period);
    t->setNextRun(current_tick_ + delay);
    addTask(t);
    return t;
}

std::shared_ptr<Task> EndstoneScheduler::runTaskAsync(Plugin &plugin, std::function<void()> task)
{
    return runTaskLaterAsync(plugin, task, 0);
}

std::shared_ptr<Task> EndstoneScheduler::runTaskLaterAsync(Plugin &plugin, std::function<void()> task,
                                                           std::uint64_t delay)
{
    return runTaskTimerAsync(plugin, task, delay, 0);
}

std::shared_ptr<Task> EndstoneScheduler::runTaskTimerAsync(Plugin &plugin, std::function<void()> task,
                                                           std::uint64_t delay, std::uint64_t period)
{
    if (!validate(plugin, task)) {
        return nullptr;
    }

    auto t = std::make_shared<EndstoneAsyncTask>(*this, plugin, task, nextId(), period);
    t->setNextRun(current_tick_ + delay);
    addTask(t);
    return t;
}

void EndstoneScheduler::cancelTask(TaskId id)
{
    std::lock_guard lock{tasks_mtx_};
    auto it = tasks_.find(id);
    if (it == tasks_.end()) {
        return;
    }
    auto task = it->second;
    task->doCancel();
    if (task->isSync()) {
        tasks_.erase(it);
    }
}

void EndstoneScheduler::cancelTasks(Plugin &plugin)
{
    std::lock_guard lock{tasks_mtx_};
    for (auto it = tasks_.begin(); it != tasks_.end();) {
        if (it->second->getOwner() != &plugin) {
            ++it;
        }
        else {
            auto task = it->second;
            task->doCancel();
            if (task->isSync()) {
                it = tasks_.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}

bool EndstoneScheduler::isRunning(TaskId id)
{
    std::lock_guard lock{tasks_mtx_};
    auto it = tasks_.find(id);
    if (it == tasks_.end()) {
        return false;
    }
    auto &task = it->second;
    if (task->isSync()) {
        return current_task_ == id;
    }
    return std::static_pointer_cast<EndstoneAsyncTask>(task)->getWorkers().empty();
}

bool EndstoneScheduler::isQueued(TaskId id)
{
    std::lock_guard lock{tasks_mtx_};
    return tasks_.find(id) != tasks_.end();
}

std::vector<Task *> EndstoneScheduler::getPendingTasks()
{
    std::vector<Task *> pending;
    std::lock_guard lock{tasks_mtx_};
    for (const auto &[id, task] : tasks_) {
        if (task->isCancelled()) {
            continue;
        }
        pending.push_back(task.get());
    }
    return pending;
}

std::shared_ptr<Task> EndstoneScheduler::runTask(std::function<void()> task)
{
    if (!task) {
        return nullptr;
    }
    auto t = std::make_shared<EndstoneTask>(*this, task, nextId(), 0);
    t->setNextRun(current_tick_);
    addTask(t);
    return t;
}

void EndstoneScheduler::addTask(std::shared_ptr<EndstoneTask> task)
{
    pending_.enqueue(task);
    std::lock_guard lock{tasks_mtx_};
    tasks_[task->getTaskId()] = task;
}

void EndstoneScheduler::mainThreadHeartbeat(std::uint64_t current_tick)
{
    // Consume the tasks in the pending queue
    std::shared_ptr<EndstoneTask> pending_task;
    while (pending_.try_dequeue(pending_task)) {
        if (pending_task->isCancelled()) {
            continue;
        }

        auto tick = std::max(current_tick, pending_task->getNextRun());
        auto &queue = queue_.emplace(tick, std::vector<std::shared_ptr<EndstoneTask>>{}).first->second;
        queue.push_back(pending_task);
        std::push_heap(queue.begin(), queue.end(), cmp_);
    }

    auto it = queue_.begin();
    while (it != queue_.end()) {
        auto tick = it->first;
        auto &queue = it->second;

        if (tick > current_tick) {
            break;
        }

        for (const auto &task : queue) {
            if (task->isCancelled()) {
                if (task->isSync()) {
                    removeTask(task->getTaskId());
                }
                continue;
            }

            if (task->isSync()) {
                current_task_ = task->getTaskId();
                try {
                    task->run();
                }
                catch (std::exception &e) {
                    server_.getLogger().error("Could not execute task with id {}: {}", task->getTaskId(), e.what());
                }
                current_task_ = 0;
            }
            else {
                executor_.submit([&task]() { task->run(); });
            }

            if (task->getPeriod() > 0) {  // repeating task
                task->setNextRun(current_tick + task->getPeriod());
                pending_.enqueue(task);
                continue;
            }

            if (task->isSync()) {
                removeTask(task->getTaskId());
            }
        }

        it = queue_.erase(it);
    }
    current_tick_ = current_tick;
}

void EndstoneScheduler::removeTask(TaskId id)
{
    std::lock_guard lock{tasks_mtx_};
    auto it = tasks_.find(id);
    if (it == tasks_.end()) {
        return;
    }
    tasks_.erase(it);
}

TaskId EndstoneScheduler::nextId()
{
    TaskId id;
    std::lock_guard lock{tasks_mtx_};
    do {
        if (ids_ >= std::numeric_limits<std::uint32_t>::max()) {
            ids_ = 1;
        }
        id = ids_++;
    } while (tasks_.find(id) != tasks_.end());
    return id;
}

bool EndstoneScheduler::TaskComparator::operator()(const std::shared_ptr<EndstoneTask> &lhs,
                                                   const std::shared_ptr<EndstoneTask> &rhs)
{
    if (lhs->getNextRun() != rhs->getNextRun()) {
        return lhs->getNextRun() > rhs->getNextRun();
    }
    return lhs->getCreatedAt() > rhs->getCreatedAt();
}
}  // namespace endstone::core
