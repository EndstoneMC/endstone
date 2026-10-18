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
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "bedrock/core/threading/background_worker_perf_info.h"
#include "bedrock/core/utility/enable_non_owner_references.h"
#include "bedrock/forward.h"
#include "bedrock/platform/threading/mutex_details.h"
#include "bedrock/platform/threading/thread_util.h"

class WorkerPool : public Bedrock::EnableNonOwnerReferences {
public:
    WorkerPool(std::string_view name, Scheduler &owner);
    ~WorkerPool() override;

protected:
    const bool async_;
    std::vector<std::unique_ptr<BackgroundWorker>> workers_;
    std::string name_;
    BackgroundWorkerPerfInfo perf_info_;
    std::chrono::steady_clock::time_point last_perf_info_update_;
    Scheduler *owner_scheduler_;
    Bedrock::Threading::Mutex suspend_lock_;
    Bedrock::Threading::OSThreadPriority pool_priority_;

public:  // Endstone: protected -> public
    float min_backlog_duration_seconds_;
};
