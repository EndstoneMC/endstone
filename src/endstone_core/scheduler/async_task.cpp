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

#include "endstone/detail/scheduler/async_task.h"

#include "endstone/detail/scheduler/scheduler.h"

namespace endstone::detail {

bool EndstoneAsyncTask::isSync() const
{
    return false;
}

void EndstoneAsyncTask::run()
{
    if (isCancelled()) {
        return;
    }

    auto thread_id = std::this_thread::get_id();
    {
        std::lock_guard lock{mutex_};
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
            getOwner()->getLogger().error("Unable to remove worker {} on task {} for {}", thread_id, getTaskId(),
                                          getOwner()->getDescription().getFullName());
            if (exception.has_value()) {
                getOwner()->getLogger().error("{}", exception);
            }
        }

        if (getPeriod() == 0 && workers_.empty()) {
            // The last worker is responsible for removing ourselves from the scheduler task list
            getScheduler().removeTask(getTaskId());
        }
    }
}

void EndstoneAsyncTask::doCancel()
{
    // Set cancelled flag to true to not accept new runs
    EndstoneTask::doCancel();
    std::lock_guard lock{mutex_};
    // Do not remove the task unless we are idle
    if (workers_.empty()) {
        getScheduler().removeTask(getTaskId());
    }
}

std::vector<EndstoneAsyncTask::Worker> EndstoneAsyncTask::getWorkers() const
{
    std::lock_guard lock{mutex_};
    return workers_;
}

}  // namespace endstone::detail
