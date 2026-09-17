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

#include <atomic>
#include <memory>
#include <string>

#include "bedrock/bedrock.h"
#include "bedrock/core/threading/async.h"
#include "bedrock/core/threading/background_task.h"
#include "bedrock/core/threading/task_group_state.h"
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/forward.h"
#include "bedrock/platform/threading/mutex_details.h"

class TaskGroup : IBackgroundTaskOwner {
public:
    ~TaskGroup() override;

protected:
    Bedrock::NonOwnerPointer<Scheduler> scheduler_;
    Bedrock::NonOwnerPointer<WorkerPool> workers_;
    std::string name_;
    bool check_owner_thread_;
    Bedrock::Threading::Mutex lock_;
    std::atomic<TaskGroupState> state_;
    std::shared_ptr<BackgroundTaskBase> tasks_;
    std::size_t task_count_;
    std::shared_ptr<BackgroundTaskBase> enum_curr_;
    std::shared_ptr<BackgroundTaskBase> enum_next_;
    Bedrock::Threading::Async<void> resume_task_handle_;
    Bedrock::Threading::Mutex resume_task_mutex_;
};
BEDROCK_STATIC_ASSERT_SIZE(TaskGroup, 336, 248);
