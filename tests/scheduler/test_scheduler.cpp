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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "endstone/detail/scheduler/scheduler.h"
#include "endstone/scheduler/scheduler.h"

class MockServer : public endstone::Server {
public:
    MOCK_METHOD(std::string, getName, (), (const, override));
    MOCK_METHOD(std::string, getVersion, (), (const, override));
    MOCK_METHOD(std::string, getMinecraftVersion, (), (const, override));
    MOCK_METHOD(endstone::Logger &, getLogger, (), (const, override));
    MOCK_METHOD(endstone::PluginManager &, getPluginManager, (), (const, override));
    MOCK_METHOD(endstone::PluginCommand *, getPluginCommand, (std::string), (const, override));
    MOCK_METHOD(endstone::ConsoleCommandSender &, getCommandSender, (), (const, override));
    MOCK_METHOD(bool, dispatchCommand, (endstone::CommandSender &, std::string), (const, override));
    MOCK_METHOD(endstone::Scheduler &, getScheduler, (), (const, override));
    MOCK_METHOD(endstone::Level *, getLevel, (), (const, override));
    MOCK_METHOD(std::vector<endstone::Player *>, getOnlinePlayers, (), (const, override));
    MOCK_METHOD(int, getMaxPlayers, (), (const, override));
    MOCK_METHOD(void, setMaxPlayers, (int), (override));
    MOCK_METHOD(endstone::Player *, getPlayer, (endstone::UUID), (const, override));
    MOCK_METHOD(endstone::Player *, getPlayer, (std::string), (const, override));
    MOCK_METHOD(void, shutdown, (), (override));
    MOCK_METHOD(void, reload, (), (override));
    MOCK_METHOD(void, reloadData, (), (override));
    MOCK_METHOD(void, broadcast, (const std::string &, const std::string &), (const, override));
    MOCK_METHOD(void, broadcastMessage, (const std::string &), (const, override));
    MOCK_METHOD(bool, isPrimaryThread, (), (const, override));
    MOCK_METHOD(endstone::Scoreboard *, getScoreboard, (), (const, override));
    MOCK_METHOD(std::shared_ptr<endstone::Scoreboard>, getNewScoreboard, (), (override));
    MOCK_METHOD(float, getCurrentMillisecondsPerTick, (), (override));
    MOCK_METHOD(float, getAverageMillisecondsPerTick, (), (override));
    MOCK_METHOD(float, getCurrentTicksPerSecond, (), (override));
    MOCK_METHOD(float, getAverageTicksPerSecond, (), (override));
    MOCK_METHOD(float, getCurrentTickUsage, (), (override));
    MOCK_METHOD(float, getAverageTickUsage, (), (override));
    MOCK_METHOD(std::chrono::system_clock::time_point, getStartTime, (), (override));
};

class MockPlugin : public endstone::Plugin {
public:
    MOCK_METHOD(const endstone::PluginDescription &, getDescription, (), (const, override));
    MockPlugin()
    {
        setEnabled(true);
    }
};

class SchedulerTest : public ::testing::Test {
protected:
    // Set Up
    void SetUp() override
    {
        server_ = std::make_unique<MockServer>();
        plugin_ = std::make_unique<MockPlugin>();
        scheduler_ = std::make_unique<endstone::detail::EndstoneScheduler>(*server_);
        tick_count_ = 0;
    }

    // Tear Down
    void TearDown() override
    {
        tick_count_ = 0;
        scheduler_.reset();
        plugin_.reset();
        server_.reset();
    }

    std::unique_ptr<MockServer> server_;
    std::unique_ptr<MockPlugin> plugin_;
    std::unique_ptr<endstone::detail::EndstoneScheduler> scheduler_;
    std::uint64_t tick_count_;
};

// Test running a task immediately
TEST_F(SchedulerTest, RunTask)
{
    bool executed = false;
    auto task = scheduler_->runTask(*plugin_, [&]() { executed = true; });
    ASSERT_TRUE(task != nullptr);
    EXPECT_FALSE(executed);
    scheduler_->mainThreadHeartbeat(++tick_count_);
    EXPECT_TRUE(executed);
}

// Test running a task later
TEST_F(SchedulerTest, RunTaskLater)
{
    bool executed = false;
    auto task = scheduler_->runTaskLater(*plugin_, [&]() { executed = true; }, 5);
    ASSERT_TRUE(task != nullptr);
    for (int i = 0; i < 4; ++i) {
        scheduler_->mainThreadHeartbeat(++tick_count_);
        EXPECT_FALSE(executed);
    }
    scheduler_->mainThreadHeartbeat(++tick_count_);
    EXPECT_TRUE(executed);
}

// Test for a repeating task
TEST_F(SchedulerTest, RunTaskTimer)
{
    int execution_count = 0;
    auto task = scheduler_->runTaskTimer(*plugin_, [&]() { ++execution_count; }, 10, 5);
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
    auto task = scheduler_->runTaskLater(*plugin_, [&]() { executed = true; }, 5);
    scheduler_->cancelTask(task->getTaskId());
    scheduler_->mainThreadHeartbeat(++tick_count_);
    EXPECT_FALSE(executed);
}

// Test the cancellation of all tasks associated with a specific plugin
TEST_F(SchedulerTest, CancelTasks)
{
    auto task1 = scheduler_->runTask(*plugin_, []() {});
    auto task2 = scheduler_->runTaskLater(*plugin_, []() {}, 5);
    auto task3 = scheduler_->runTaskTimer(*plugin_, []() {}, 10, 5);
    scheduler_->cancelTasks(*plugin_);
    EXPECT_FALSE(scheduler_->isQueued(task1->getTaskId()));
    EXPECT_FALSE(scheduler_->isQueued(task2->getTaskId()));
    EXPECT_FALSE(scheduler_->isQueued(task3->getTaskId()));
}

// Test to check if a task is running
TEST_F(SchedulerTest, TaskIsRunning)
{
    bool executed = false;
    std::shared_ptr<endstone::Task> task;
    task = scheduler_->runTask(*plugin_, [&]() {
        executed = true;
        EXPECT_TRUE(scheduler_->isRunning(task->getTaskId()));
    });
    scheduler_->mainThreadHeartbeat(++tick_count_);
    EXPECT_TRUE(executed);
    EXPECT_FALSE(scheduler_->isRunning(task->getTaskId()));
}

// Test to check if a task is queued
TEST_F(SchedulerTest, TaskIsQueued)
{
    auto task = scheduler_->runTaskLater(*plugin_, []() {}, 5);
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
    auto task1 = scheduler_->runTaskLater(*plugin_, []() {}, 2);
    auto task2 = scheduler_->runTaskLater(*plugin_, []() {}, 3);
    auto task3 = scheduler_->runTaskLater(*plugin_, []() {}, 5);

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
