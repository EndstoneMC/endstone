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

#include "endstone/detail/scheduler/scheduler.h"

namespace endstone::detail {

EndstoneScheduler::EndstoneScheduler(Server &server) : server_(server) {}

Task *EndstoneScheduler::runTask(Plugin &plugin, std::function<void()> task)
{
    return runTaskLater(plugin, task, 0);
}

Task *EndstoneScheduler::runTaskLater(Plugin &plugin, std::function<void()> task, std::uint64_t delay)
{
    return runTaskTimer(plugin, task, 0, 0);
}

Task *EndstoneScheduler::runTaskTimer(Plugin &plugin, std::function<void()> task, std::uint64_t delay,
                                      std::uint64_t period)
{
    if (!task) {
        server_.getLogger().error("Plugin {} attempted to register an empty task", plugin.getName());
        return nullptr;
    }

    if (!plugin.isEnabled()) {
        server_.getLogger().error("Plugin {} attempted to register task while disabled", plugin.getName());
        return nullptr;
    }

    auto t = std::make_shared<EndstoneTask>(plugin, task, nextId(), period);
    t->setNextRun(current_tick_ + delay);
    if (!pending_.enqueue(t)) {
        return nullptr;
    }
    std::lock_guard lock{tasks_mtx_};
    tasks_[t->getTaskId()] = t;
    return t.get();
}

void EndstoneScheduler::cancelTask(TaskId id)
{
    std::lock_guard lock{tasks_mtx_};
    auto it = tasks_.find(id);
    if (it == tasks_.end()) {
        return;
    }
    it->second->cancel();
    tasks_.erase(it);
}

void EndstoneScheduler::cancelTasks(Plugin &plugin)
{
    std::lock_guard lock{tasks_mtx_};
    for (auto it = tasks_.begin(); it != tasks_.end();) {
        if (&it->second->getOwner() != &plugin) {
            ++it;
        }
        else {
            it->second->cancel();
            it = tasks_.erase(it);
        }
    }
}

bool EndstoneScheduler::isRunning(TaskId id)
{
    if (!isQueued(id)) {
        return false;
    }
    return current_task_ == id;
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
        std::push_heap(queue.begin(), queue.end());
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
                std::lock_guard lock{tasks_mtx_};
                tasks_.erase(task->getTaskId());
                continue;
            }

            try {
                task->run();
            }
            catch (std::exception &e) {
                server_.getLogger().critical("Could not execute task with id {}: {}", task->getTaskId(), e.what());
            }

            if (task->getPeriod() > 0) {  // repeating task
                task->setNextRun(current_tick + task->getPeriod());
                pending_.enqueue(task);
                continue;
            }

            std::lock_guard lock{tasks_mtx_};
            tasks_.erase(task->getTaskId());
            task->cancel();
        }

        it = queue_.erase(it);
    }
    current_tick_ = current_tick;
}

TaskId EndstoneScheduler::nextId()
{
    if (ids_ >= std::numeric_limits<std::uint32_t>::max()) {
        ids_ = 1;
    }
    return ids_++;
}

bool EndstoneScheduler::TaskComparator::operator()(const EndstoneTask *lhs, const EndstoneTask *rhs)
{
    if (lhs->getNextRun() != rhs->getNextRun()) {
        return lhs->getNextRun() > rhs->getNextRun();
    }
    return lhs->getCreatedAt() > rhs->getCreatedAt();
}
}  // namespace endstone::detail
