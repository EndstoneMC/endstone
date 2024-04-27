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

    return handle(std::make_shared<EndstoneTask>(*this, plugin, task, nextId(), period), delay);
}

void EndstoneScheduler::cancelTask(TaskId id)
{
    if (id == 0) {
        return;
    }
    {
        std::scoped_lock lock{runners_mtx_};
        auto it = runners_.find(id);
        if (it != runners_.end()) {
            it->second->cancel0();
        }
    }

    std::shared_ptr<EndstoneTask> cancel_task{new EndstoneTask(*this, [this, id]() { cancelTaskById(id); })};
    handle(cancel_task, 0);
    for (auto task = head_->getNext(); task != nullptr; task = task->getNext()) {
        if (task == cancel_task) {
            return;
        }
        if (task->getTaskId() == id) {
            task->cancel0();
        }
    }
}

void EndstoneScheduler::cancelTaskById(TaskId id)
{
    auto it = std::remove_if(std::begin(temp_), std::end(temp_), [this, id](const auto &task) {
        if (task->getTaskId() == id) {
            task->cancel0();
            if (task->isSync()) {
                std::scoped_lock lock{runners_mtx_};
                runners_.erase(id);
            }
            return true;
        }
        return false;
    });
    if (it != temp_.end()) {
        temp_.erase(it);
        return;
    }

    it = std::remove_if(std::begin(pending_), std::end(pending_), [this, id](const auto &task) {
        if (task->getTaskId() == id) {
            task->cancel0();
            if (task->isSync()) {
                std::scoped_lock lock{runners_mtx_};
                runners_.erase(id);
            }
            return true;
        }
        return false;
    });
    pending_.erase(it);
}

void EndstoneScheduler::cancelTasks(Plugin &plugin)
{
    std::shared_ptr<EndstoneTask> cancel_task{
        new EndstoneTask(*this, [this, &plugin]() { cancelTaskByPlugin(plugin); })};
    handle(cancel_task, 0);
    for (auto task = head_->getNext(); task != nullptr; task = task->getNext()) {
        if (task == cancel_task) {
            break;
        }
        if (task->getTaskId() != 0 && &task->getOwner() == &plugin) {
            task->cancel0();
        }
    }

    std::scoped_lock lock{runners_mtx_};
    for (const auto &[id, task] : runners_) {
        if (&task->getOwner() == &plugin) {
            task->cancel0();
        }
    }
}

void EndstoneScheduler::cancelTaskByPlugin(Plugin &plugin)
{
    auto it = std::remove_if(std::begin(pending_), std::end(pending_), [this, &plugin](const auto &task) {
        if (&task->getOwner() == &plugin) {
            task->cancel0();
            if (task->isSync()) {
                std::scoped_lock lock{runners_mtx_};
                runners_.erase(task->getTaskId());
            }
            return true;
        }
        return false;
    });
    pending_.erase(it);

    it = std::remove_if(std::begin(temp_), std::end(temp_), [this, &plugin](const auto &task) {
        if (&task->getOwner() == &plugin) {
            task->cancel0();
            if (task->isSync()) {
                std::scoped_lock lock{runners_mtx_};
                runners_.erase(task->getTaskId());
            }
            return true;
        }
        return false;
    });
    temp_.erase(it);
}

bool EndstoneScheduler::isRunning(TaskId id)
{
    std::scoped_lock lock{runners_mtx_};
    auto it = runners_.find(id);
    if (it == runners_.end()) {
        return false;
    }
    auto *task = it->second;

    if (task->isSync()) {
        return task == current_task_.get();
    }
    return false;
}

bool EndstoneScheduler::isQueued(TaskId id)
{
    if (id == 0) {
        return false;
    }

    for (auto task = head_->getNext(); task != nullptr; task = task->getNext()) {
        if (task->getTaskId() == id) {
            return task->getStatus() == EndstoneTask::Status::Pending;
        }
    }

    std::scoped_lock lock{runners_mtx_};
    auto it = runners_.find(id);
    return it != runners_.end() && it->second->getStatus() == EndstoneTask::Status::Pending;
}

std::vector<Task *> EndstoneScheduler::getPendingTasks()
{
    std::vector<EndstoneTask *> true_pending;
    for (auto task = head_->getNext(); task != nullptr; task = task->getNext()) {
        if (task->getTaskId() != 0) {
            true_pending.push_back(task.get());
        }
    }

    std::vector<Task *> pending;
    {
        std::scoped_lock lock{runners_mtx_};
        for (const auto &[id, task] : runners_) {
            if (task->getStatus() == EndstoneTask::Status::Pending) {
                pending.push_back(task);
            }
        }
    }

    for (auto *task : true_pending) {
        if (task->getStatus() == EndstoneTask::Status::Pending &&
            std::find(pending.begin(), pending.end(), task) == pending.end()) {
            pending.push_back(task);
        }
    }
    return pending;
}

void EndstoneScheduler::mainThreadHeartbeat(std::uint64_t current_tick)
{
    // TODO(scheduler): remember push_back, push_heap, pop_heap, pop_back
}

void EndstoneScheduler::addTask(std::shared_ptr<EndstoneTask> task)
{
    auto *tail_task = tail_.load();
    while (!tail_.compare_exchange_weak(tail_task, task.get())) {
    }
    tail_task->setNext(std::move(task));
}

Task *EndstoneScheduler::handle(std::shared_ptr<EndstoneTask> task, std::uint64_t delay)
{
    task->setNextRun(current_tick_ + delay);
    addTask(std::move(task));
    return task.get();
}

TaskId EndstoneScheduler::nextId()
{
    if (ids_ >= std::numeric_limits<std::uint32_t>::max()) {
        ids_ = 1;
    }
    return ids_++;
}

bool EndstoneScheduler::PendingTaskComparator::operator()(const EndstoneTask &lhs, const EndstoneTask &rhs)
{
    if (lhs.getNextRun() != rhs.getNextRun()) {
        return lhs.getNextRun() > rhs.getNextRun();
    }
    return lhs.getCreatedAt() > rhs.getCreatedAt();
}

}  // namespace endstone::detail
