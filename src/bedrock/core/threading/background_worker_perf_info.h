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
#include <chrono>
#include <cstdint>

#include "bedrock/bedrock.h"
#include "bedrock/forward.h"

class BackgroundWorkerPerfInfo {
public:
    const BackgroundWorker *updater_worker;
    std::atomic<std::uint64_t> total_run_tasks;
    std::atomic<std::uint64_t> total_run_tasks_ticks;
    std::atomic<std::uint32_t> total_wake_ups;
    std::atomic<double> average_task_duration;
    std::atomic<std::uint32_t> wake_ups_per_second;

protected:
    std::chrono::steady_clock::time_point last_perf_info_update_;
    std::chrono::steady_clock::time_point next_perf_info_update_;
};
BEDROCK_STATIC_ASSERT_SIZE(BackgroundWorkerPerfInfo, 64, 64);
