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

#include "endstone/detail/scheduler/thread_pool_executor.h"

namespace endstone::core {

ThreadPoolExecutor::ThreadPoolExecutor(size_t thread_count) : done(false)
{
    for (size_t i = 0; i < thread_count; ++i) {
        threads.emplace_back(&ThreadPoolExecutor::worker, this);
    }
}

ThreadPoolExecutor::~ThreadPoolExecutor()
{
    done = true;
    condition.notify_all();
    for (auto &thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void ThreadPoolExecutor::worker()
{
    while (!done) {
        std::function<void()> task;
        if (tasks.try_dequeue(task)) {
            task();
        }
        else {
            std::unique_lock<std::mutex> lock(mutex);
            condition.wait_for(lock, std::chrono::milliseconds(10));
        }
    }

    // Process remaining tasks
    std::function<void()> task;
    while (tasks.try_dequeue(task)) {
        task();
    }
}
}  // namespace endstone::core
