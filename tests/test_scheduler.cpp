// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include <future>
#include <memory>
#include <string>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "endstone/core/scheduler/scheduler.h"
#include "mocks.h"

class SchedulerTest : public ::testing::Test {
protected:
    // Set Up
    void SetUp() override
    {
        plugin_.setEnabled(true);
        scheduler_ = std::make_unique<endstone::core::EndstoneScheduler>(server_);
        tick_count_ = 0;
    }

    // Tear Down
    void TearDown() override
    {
        tick_count_ = 0;
        scheduler_.reset();
        plugin_.setEnabled(false);
    }

    MockServer server_;
    MockPlugin plugin_;
    std::unique_ptr<endstone::core::EndstoneScheduler> scheduler_;
    std::uint64_t tick_count_;
};

// Test running a task immediately
TEST_F(SchedulerTest, RunTask)
{
    bool executed = false;
    auto task = scheduler_->runTask(plugin_, [&]() { executed = true; });
    ASSERT_TRUE(task != nullptr);
    EXPECT_FALSE(executed);
    scheduler_->mainThreadHeartbeat(++tick_count_);
    EXPECT_TRUE(executed);
}

TEST_F(SchedulerTest, RunTaskAsync)
{
    std::promise<void> executed;
    auto future = executed.get_future();
    auto task = scheduler_->runTaskAsync(plugin_, [&]() { executed.set_value(); });
    ASSERT_TRUE(task != nullptr);
    task.reset();

    scheduler_->mainThreadHeartbeat(++tick_count_);

    EXPECT_EQ(future.wait_for(std::chrono::seconds(5)), std::future_status::ready);
}

// Test running a task later
TEST_F(SchedulerTest, RunTaskLater)
{
    bool executed = false;
    auto task = scheduler_->runTaskLater(plugin_, [&]() { executed = true; }, 5);
    ASSERT_TRUE(task != nullptr);
    for (int i = 0; i < 4; ++i) {
        scheduler_->mainThreadHeartbeat(++tick_count_);
        EXPECT_FALSE(executed);
    }
    scheduler_->mainThreadHeartbeat(++tick_count_);
    EXPECT_TRUE(executed);
}

// Regression test for #317: a delayed task registered before the first heartbeat (e.g. in
// Plugin::onEnable / ServerLoadEvent) must honour its delay even when the world's persisted server
// tick is already large. The scheduler normalises ticks to a zero-based, session-relative clock.
TEST_F(SchedulerTest, RunTaskLaterWhenFirstTickIsLarge)
{
    bool executed = false;
    auto task = scheduler_->runTaskLater(plugin_, [&]() { executed = true; }, 5);
    ASSERT_TRUE(task != nullptr);

    // Simulate a played-in world: the level's first server tick is already far past the delay.
    constexpr std::uint64_t big = 5'000'000;
    scheduler_->mainThreadHeartbeat(big);  // first heartbeat anchors the base
    EXPECT_FALSE(executed);                // must NOT run immediately (the #317 bug)
    for (std::uint64_t i = 1; i < 5; ++i) {
        scheduler_->mainThreadHeartbeat(big + i);
        EXPECT_EQ(executed, i == 4);  // fires on the 5th heartbeat (delay of 5 elapsed), not before
    }
}

// Test for a repeating task
TEST_F(SchedulerTest, RunTaskTimer)
{
    int execution_count = 0;
    auto task = scheduler_->runTaskTimer(plugin_, [&]() { ++execution_count; }, 10, 5);
    for (int i = 0; i < 25; ++i) {
        scheduler_->mainThreadHeartbeat(++tick_count_);
        if (i == 10 || i == 15 || i == 20) {
            EXPECT_EQ(execution_count, (i / 5) - 1);
        }
    }
    scheduler_->cancelTask(task->getTaskId());
}

// Test cancellation of a task
TEST_F(SchedulerTest, CancelTask)
{
    bool executed = false;
    auto task = scheduler_->runTaskLater(plugin_, [&]() { executed = true; }, 5);
    scheduler_->cancelTask(task->getTaskId());
    scheduler_->mainThreadHeartbeat(++tick_count_);
    EXPECT_FALSE(executed);
}

// Test the cancellation of all tasks associated with a specific plugin
TEST_F(SchedulerTest, CancelTasks)
{
    auto task1 = scheduler_->runTask(plugin_, []() {});
    auto task2 = scheduler_->runTaskLater(plugin_, []() {}, 5);
    auto task3 = scheduler_->runTaskTimer(plugin_, []() {}, 10, 5);
    scheduler_->cancelTasks(plugin_);
    EXPECT_FALSE(scheduler_->isQueued(task1->getTaskId()));
    EXPECT_FALSE(scheduler_->isQueued(task2->getTaskId()));
    EXPECT_FALSE(scheduler_->isQueued(task3->getTaskId()));
}

TEST_F(SchedulerTest, CancelTasksKeepsInternalTasks)
{
    bool internal_executed = false;
    bool plugin_executed = false;
    auto internal_task = scheduler_->runTask([&]() { internal_executed = true; });
    auto plugin_task = scheduler_->runTask(plugin_, [&]() { plugin_executed = true; });
    ASSERT_TRUE(internal_task != nullptr);
    ASSERT_TRUE(plugin_task != nullptr);

    scheduler_->cancelTasks(plugin_);

    EXPECT_TRUE(scheduler_->isQueued(internal_task->getTaskId()));
    EXPECT_FALSE(scheduler_->isQueued(plugin_task->getTaskId()));
    scheduler_->mainThreadHeartbeat(++tick_count_);
    EXPECT_TRUE(internal_executed);
    EXPECT_FALSE(plugin_executed);
}

// Regression test for #351: cancelling an idle async task via cancelTasks() used to re-lock
// tasks_mtx_ recursively (doCancel() -> removeTask()), throwing "resource deadlock would occur".
TEST_F(SchedulerTest, CancelAsyncTasksDoesNotDeadlock)
{
    auto task1 = scheduler_->runTaskLaterAsync(plugin_, []() {}, 100000000);
    auto task2 = scheduler_->runTaskTimerAsync(plugin_, []() {}, 1, 1);
    ASSERT_TRUE(task1 != nullptr);
    ASSERT_TRUE(task2 != nullptr);
    scheduler_->cancelTasks(plugin_);
    EXPECT_FALSE(scheduler_->isQueued(task1->getTaskId()));
    EXPECT_FALSE(scheduler_->isQueued(task2->getTaskId()));
}

// Test to check if a task is running
TEST_F(SchedulerTest, TaskIsRunning)
{
    bool executed = false;
    std::shared_ptr<endstone::Task> task;
    task = scheduler_->runTask(plugin_, [&]() {
        executed = true;
        EXPECT_TRUE(scheduler_->isRunning(task->getTaskId()));
    });
    scheduler_->mainThreadHeartbeat(++tick_count_);
    EXPECT_TRUE(executed);
    EXPECT_FALSE(scheduler_->isRunning(task->getTaskId()));
}

TEST_F(SchedulerTest, AsyncTaskIsRunning)
{
    std::promise<void> started;
    std::promise<void> release;
    std::promise<void> finished;
    auto started_future = started.get_future();
    auto release_future = release.get_future().share();
    auto finished_future = finished.get_future();
    auto task = scheduler_->runTaskAsync(plugin_, [&]() {
        started.set_value();
        release_future.wait();
        finished.set_value();
    });
    ASSERT_TRUE(task != nullptr);

    scheduler_->mainThreadHeartbeat(++tick_count_);

    if (started_future.wait_for(std::chrono::seconds(5)) != std::future_status::ready) {
        release.set_value();
        FAIL() << "Async task did not start";
    }
    EXPECT_TRUE(scheduler_->isRunning(task->getTaskId()));
    release.set_value();
    ASSERT_EQ(finished_future.wait_for(std::chrono::seconds(5)), std::future_status::ready);

    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    while (scheduler_->isRunning(task->getTaskId()) && std::chrono::steady_clock::now() < deadline) {
        std::this_thread::yield();
    }
    EXPECT_FALSE(scheduler_->isRunning(task->getTaskId()));
}

// Test to check if a task is queued
TEST_F(SchedulerTest, TaskIsQueued)
{
    auto task = scheduler_->runTaskLater(plugin_, []() {}, 5);
    EXPECT_TRUE(scheduler_->isQueued(task->getTaskId()));

    for (int i = 0; i < 4; ++i) {
        scheduler_->mainThreadHeartbeat(++tick_count_);  // 1 tick, task still in queue
        EXPECT_TRUE(scheduler_->isQueued(task->getTaskId()));
    }
    scheduler_->mainThreadHeartbeat(++tick_count_);
    EXPECT_FALSE(scheduler_->isQueued(task->getTaskId()));
}

// Test to verify that getPendingTasks returns all currently queued tasks
TEST_F(SchedulerTest, GetPendingTasks)
{
    auto task1 = scheduler_->runTaskLater(plugin_, []() {}, 2);
    auto task2 = scheduler_->runTaskLater(plugin_, []() {}, 3);
    auto task3 = scheduler_->runTaskLater(plugin_, []() {}, 5);

    auto tasks = scheduler_->getPendingTasks();
    std::vector<endstone::TaskId> task_ids;
    task_ids.reserve(tasks.size());
    for (const auto &task : tasks) {
        task_ids.push_back(task->getTaskId());
    }

    EXPECT_EQ(tasks.size(), 3);
    EXPECT_NE(std::find(task_ids.begin(), task_ids.end(), task1->getTaskId()), task_ids.end());
    EXPECT_NE(std::find(task_ids.begin(), task_ids.end(), task2->getTaskId()), task_ids.end());
    EXPECT_NE(std::find(task_ids.begin(), task_ids.end(), task3->getTaskId()), task_ids.end());
}
