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

#include <cstdint>

#include "endstone/plugin/plugin.h"

namespace endstone {

using TaskId = std::uint32_t;

/**
 * @brief Represents a task being executed by the scheduler.
 */
class Task {
public:
    virtual ~Task() = default;

    /**
     * Returns the taskId for the task.
     *
     * @return Task id number
     */
    [[nodiscard]] virtual TaskId getTaskId() const = 0;

    /**
     * Returns the Plugin that owns this task.
     *
     * @return The Plugin that owns the task
     */
    [[nodiscard]] virtual Plugin *getOwner() const = 0;

    /**
     * Returns true if the Task is a sync task.
     *
     * @return true if the task is run by server thread
     */
    [[nodiscard]] virtual bool isSync() const = 0;

    /**
     * Returns true if this task has been cancelled.
     *
     * @return true if the task has been cancelled
     */
    [[nodiscard]] virtual bool isCancelled() const = 0;

    /**
     * Attempts to cancel this task.
     */
    virtual void cancel() = 0;
};

}  // namespace endstone
