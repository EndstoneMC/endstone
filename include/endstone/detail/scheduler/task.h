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

#include <functional>

#include "endstone/plugin/plugin.h"
#include "endstone/scheduler/task.h"

namespace endstone::detail {

class EndstoneTask : public Task {
public:
    explicit EndstoneTask(Plugin &plugin, std::function<void()> runnable, std::uint32_t task_id);

    ~EndstoneTask() override = default;

    std::uint32_t getTaskId() override;
    Plugin &getOwner() override;
    bool isSync() override;
    bool isCancelled() override;
    void cancel() override;
    void run() const;

private:
    Plugin &plugin_;
    std::function<void()> runnable_;
    std::uint32_t task_id_;
    bool cancelled_{false};
};

}  // namespace endstone::detail
