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

#include "endstone/core/scheduler/async_task.h"

#include <format>
#include <sstream>

#include "endstone/core/scheduler/scheduler.h"

namespace endstone::core {

bool EndstoneAsyncTask::isSync() const
{
    return false;
}

void EndstoneAsyncTask::run()
{
    auto thread_id = std::this_thread::get_id();
    {
        std::lock_guard lock{mutex_};
        if (isCancelled()) {
            return;
        }
        workers_.push_back({thread_id, getTaskId(), getOwner()});
    }

    std::optional<std::exception> exception;
    try {
        EndstoneTask::run();
    }
    catch (std::exception &e) {
        exception = e;
        getOwner()->getLogger().warning("Plugin {} generated an exception while executing task {}: {}",
                                        getOwner()->getName(), getTaskId(), e.what());
    }
    catch (...) {
    }

    bool finished = false;
    {
        std::lock_guard lock{mutex_};
        bool removed = false;
        for (auto it = workers_.begin(); it != workers_.end();) {
            if (it->thread_id == thread_id) {
                workers_.erase(it);
                removed = true;
                break;
            }
            ++it;
        }

        if (!removed) {
            std::ostringstream tid;
            tid << thread_id;
            getOwner()->getLogger().error(std::format("Unable to remove worker {} on task {} for {}", tid.str(),
                                                      getTaskId(), getOwner()->getDescription().getFullName()));
            if (exception.has_value()) {
                getOwner()->getLogger().error(exception->what());
            }
        }

        // The last worker is responsible for removing ourselves from the scheduler task list once the
        // task is done (one-shot) or has been cancelled while still running.
        finished = workers_.empty() && (getPeriod() == 0 || isCancelled());
    }
    condition_.notify_all();

    // Remove ourselves outside the lock: removeTask() takes the scheduler's tasks_mtx_, which must
    // never be held together with our own mutex_ (the cancel path takes them in the opposite order).
    if (finished) {
        getScheduler().removeTask(getTaskId());
    }
}

void EndstoneAsyncTask::doCancel()
{
    bool idle;
    {
        std::lock_guard lock{mutex_};
        // Set cancelled flag to true to not accept new runs
        EndstoneTask::doCancel();
        // Do not remove the task unless we are idle; a running worker will remove it when it finishes.
        idle = workers_.empty();
    }
    // Remove ourselves outside the lock: removeTask() takes the scheduler's tasks_mtx_, which must
    // never be held together with our own mutex_ (the cancel path takes them in the opposite order).
    if (idle) {
        getScheduler().removeTask(getTaskId());
    }
}

std::vector<EndstoneAsyncTask::Worker> EndstoneAsyncTask::getWorkers() const
{
    std::lock_guard lock{mutex_};
    return workers_;
}

void EndstoneAsyncTask::wait()
{
    std::unique_lock lock{mutex_};
    condition_.wait(lock, [this]() { return workers_.empty(); });
}

}  // namespace endstone::core
