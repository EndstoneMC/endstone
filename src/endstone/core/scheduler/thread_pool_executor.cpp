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

#include "endstone/core/scheduler/thread_pool_executor.h"

#include <algorithm>

namespace endstone::core {

ThreadPoolExecutor::ThreadPoolExecutor(size_t max_threads) : max_threads_(std::max<std::size_t>(max_threads, 1)) {}

ThreadPoolExecutor::~ThreadPoolExecutor()
{
    {
        std::lock_guard lock{mutex_};
        done_ = true;
    }
    condition_.notify_all();
    for (auto &thread : threads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void ThreadPoolExecutor::enqueue(std::function<void()> task)
{
    std::lock_guard lock{mutex_};
    tasks_.push(std::move(task));
    if (!done_ && tasks_.size() > idle_ && threads_.size() < max_threads_) {
        threads_.emplace_back(&ThreadPoolExecutor::worker, this);
    }
    condition_.notify_one();
}

void ThreadPoolExecutor::worker()
{
    std::unique_lock lock{mutex_};
    while (true) {
        ++idle_;
        condition_.wait(lock, [this] { return done_ || !tasks_.empty(); });
        --idle_;
        if (tasks_.empty()) {
            return;
        }

        auto task = std::move(tasks_.front());
        tasks_.pop();
        lock.unlock();
        task();
        task = {};
        lock.lock();
    }
}
}  // namespace endstone::core
