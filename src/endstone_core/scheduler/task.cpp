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

namespace endstone::detail {

EndstoneTask::EndstoneTask(Plugin &plugin, std::function<void()> runnable, std::uint32_t task_id)
    : plugin_(plugin), runnable_(std::move(runnable)), task_id_(task_id)
{
}

std::uint32_t EndstoneTask::getTaskId()
{
    return task_id_;
}

Plugin &EndstoneTask::getOwner()
{
    return plugin_;
}

bool EndstoneTask::isSync()
{
    return true;
}

bool EndstoneTask::isCancelled()
{
    return cancelled_;
}

void EndstoneTask::run() const
{
    runnable_();
}

void EndstoneTask::cancel()
{
    // TODO(scheduler): call Scheduler:cancelTask(id);
}

}  // namespace endstone::detail
