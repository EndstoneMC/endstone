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

#include "endstone/detail/scheduler/task.h"

#include <utility>

#include "endstone/detail/scheduler/scheduler.h"

namespace endstone::detail {

EndstoneTask::EndstoneTask(EndstoneScheduler &scheduler, std::function<void()> task, TaskId id, std::uint64_t period)
    : scheduler_(scheduler), task_(std::move(task)), id_(id), period_(period)
{
}

EndstoneTask::EndstoneTask(EndstoneScheduler &scheduler, Plugin &plugin, std::function<void()> task, TaskId id,
                           std::uint64_t period)
    : EndstoneTask(scheduler, std::move(task), id, period)
{
    plugin_ = &plugin;
}

TaskId EndstoneTask::getTaskId() const
{
    return id_;
}

Plugin *EndstoneTask::getOwner() const
{
    return plugin_;
}

bool EndstoneTask::isSync() const
{
    return true;
}

bool EndstoneTask::isCancelled() const
{
    return cancelled_;
}

void EndstoneTask::cancel()
{
    scheduler_.cancelTask(getTaskId());
}

void EndstoneTask::run()
{
    if (task_) {
        task_();
    }
}

void EndstoneTask::doCancel()
{
    cancelled_ = true;
}

EndstoneScheduler &EndstoneTask::getScheduler() const
{
    return scheduler_;
}

EndstoneTask::CreatedAt EndstoneTask::getCreatedAt() const
{
    return created_at_;
}

std::uint64_t EndstoneTask::getPeriod() const
{
    return period_;
}

void EndstoneTask::setPeriod(std::uint64_t period)
{
    period_ = period;
}

std::uint64_t EndstoneTask::getNextRun() const
{
    return next_run_;
}

void EndstoneTask::setNextRun(std::uint64_t next_run)
{
    next_run_ = next_run;
}

}  // namespace endstone::detail
