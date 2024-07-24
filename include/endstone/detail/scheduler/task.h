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

#include <chrono>
#include <functional>

#include "endstone/plugin/plugin.h"
#include "endstone/scheduler/scheduler.h"
#include "endstone/scheduler/task.h"

namespace endstone::detail {

class EndstoneScheduler;

class EndstoneTask : public Task {
public:
    using TaskClock = std::chrono::steady_clock;
    using CreatedAt = std::chrono::time_point<TaskClock>;

    EndstoneTask(EndstoneScheduler &scheduler, std::function<void()> task, TaskId id, std::uint64_t period);
    EndstoneTask(EndstoneScheduler &scheduler, Plugin &plugin, std::function<void()> task, TaskId id,
                 std::uint64_t period);
    ~EndstoneTask() override = default;
    [[nodiscard]] TaskId getTaskId() const override;
    [[nodiscard]] Plugin *getOwner() const override;
    [[nodiscard]] bool isSync() const override;
    [[nodiscard]] bool isCancelled() const override;
    void cancel() override;
    virtual void run();
    virtual void doCancel();

    EndstoneScheduler &getScheduler() const;
    [[nodiscard]] CreatedAt getCreatedAt() const;
    [[nodiscard]] std::uint64_t getPeriod() const;
    void setPeriod(std::uint64_t period);
    [[nodiscard]] std::uint64_t getNextRun() const;
    void setNextRun(std::uint64_t next_run);

private:
    EndstoneScheduler &scheduler_;
    Plugin *plugin_;
    std::function<void()> task_;
    TaskId id_;
    CreatedAt created_at_{TaskClock::now()};
    std::uint64_t period_;
    std::uint64_t next_run_;
    std::atomic<bool> cancelled_{false};
};

}  // namespace endstone::detail
