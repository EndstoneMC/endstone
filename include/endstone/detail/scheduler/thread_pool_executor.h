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

#pragma once

#include <atomic>
#include <functional>
#include <future>
#include <thread>
#include <vector>

#include <moodycamel/concurrentqueue.h>

namespace endstone::detail {

class ThreadPoolExecutor {
public:
    explicit ThreadPoolExecutor(size_t threadCount = std::thread::hardware_concurrency());
    ~ThreadPoolExecutor();

    template <typename Func, typename... Args>
    auto submit(Func &&func, Args &&...args) -> std::future<std::invoke_result_t<Func, Args...>>
    {
        using ReturnType = std::invoke_result_t<Func, Args...>;

        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<Func>(func), std::forward<Args>(args)...));

        auto result = task->get_future();
        tasks.enqueue([task]() { (*task)(); });

        condition.notify_one();
        return result;
    }

private:
    void worker();

    std::vector<std::thread> threads;
    moodycamel::ConcurrentQueue<std::function<void()>> tasks;
    std::atomic<bool> done;
    std::mutex mutex;
    std::condition_variable condition;
};

}  // namespace endstone::detail
