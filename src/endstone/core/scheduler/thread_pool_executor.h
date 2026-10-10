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

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace endstone::core {

class ThreadPoolExecutor {
public:
    explicit ThreadPoolExecutor(size_t max_threads = 32);
    ~ThreadPoolExecutor();

    template <typename Func, typename... Args>
    auto submit(Func &&func, Args &&...args) -> std::future<std::invoke_result_t<Func, Args...>>
    {
        using ReturnType = std::invoke_result_t<Func, Args...>;

        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<Func>(func), std::forward<Args>(args)...));

        auto result = task->get_future();
        enqueue([task]() { (*task)(); });
        return result;
    }

private:
    void enqueue(std::function<void()> task);
    void worker();

    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    size_t max_threads_;
    size_t idle_ = 0;
    bool done_ = false;
    std::mutex mutex_;
    std::condition_variable condition_;
};

}  // namespace endstone::core
