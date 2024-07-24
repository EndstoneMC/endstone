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

#include <gtest/gtest.h>

#include "endstone/detail/scheduler/thread_pool_executor.h"

using endstone::detail::ThreadPoolExecutor;

// Test if tasks are executed
TEST(ThreadPoolExecutorTest, ExecuteTasks)
{
    ThreadPoolExecutor executor(4);
    auto future1 = executor.submit([]() { return 1; });
    auto future2 = executor.submit([](int a, int b) { return a + b; }, 2, 3);
    EXPECT_EQ(future1.get(), 1);
    EXPECT_EQ(future2.get(), 5);
}

// Test if tasks are executed in parallel
TEST(ThreadPoolExecutorTest, ParallelExecution)
{
    ThreadPoolExecutor executor(4);
    std::atomic<int> counter{0};

    auto task = [&counter]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        counter++;
    };

    auto future1 = executor.submit(task);
    auto future2 = executor.submit(task);
    auto future3 = executor.submit(task);
    auto future4 = executor.submit(task);

    auto start = std::chrono::steady_clock::now();
    future1.get();
    future2.get();
    future3.get();
    future4.get();
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    EXPECT_LT(duration.count(), 200);
    EXPECT_EQ(counter.load(), 4);
}

// Test if the destructor waits for tasks to finish
TEST(ThreadPoolExecutorTest, DestructorWaitsForTasks)
{
    auto start = std::chrono::steady_clock::now();
    {
        ThreadPoolExecutor executor(4);
        for (int i = 0; i < 10; ++i) {
            executor.submit([]() { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
        }
    }
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    EXPECT_GE(duration.count(), 300);
}

// Test with no tasks
TEST(ThreadPoolExecutorTest, NoTasks)
{
    ThreadPoolExecutor executor(4);
}

// Test if tasks with exceptions are handled properly
TEST(ThreadPoolExecutorTest, TaskWithException)
{
    ThreadPoolExecutor executor(4);
    auto future = executor.submit([]() { throw std::runtime_error("Task exception"); });
    EXPECT_THROW(future.get(), std::runtime_error);
}

// Test with a large number of tasks
TEST(ThreadPoolExecutorTest, ManyTasks)
{
    ThreadPoolExecutor executor(4);
    std::atomic<int> counter{0};
    const int task_count = 1000;

    auto task = [&counter]() {
        counter++;
    };

    std::vector<std::future<void>> futures;
    futures.reserve(task_count);
    for (int i = 0; i < task_count; ++i) {
        futures.push_back(executor.submit(task));
    }

    for (auto &future : futures) {
        future.get();
    }

    EXPECT_EQ(counter.load(), task_count);
}
