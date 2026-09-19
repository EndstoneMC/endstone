// Copyright (c) 2026, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include "endstone/core/level/chunk_load_queue.h"
#include "endstone/core/logger_factory.h"
#include "mocks.h"

using endstone::Chunk;
using endstone::Nullable;
using endstone::core::ChunkLoadQueue;
using Request = ChunkLoadQueue::Request;

class MockChunk : public Chunk {
public:
    MOCK_METHOD(int, getX, (), (const, override));
    MOCK_METHOD(int, getZ, (), (const, override));
    MOCK_METHOD(endstone::Level &, getLevel, (), (const, override));
    MOCK_METHOD(endstone::NotNull<endstone::Dimension>, getDimension, (), (const, override));
    MOCK_METHOD(endstone::NotNull<endstone::Block>, getBlock, (int, int, int), (const, override));
    MOCK_METHOD(std::vector<endstone::NotNull<endstone::Actor>>, getEntities, (), (const, override));
    MOCK_METHOD(bool, isGenerated, (), (const, override));
    MOCK_METHOD(bool, isSlimeChunk, (), (const, override));
    MOCK_METHOD(bool, isLoaded, (), (const, override));
    MOCK_METHOD(bool, load, (), (override));
    MOCK_METHOD(bool, load, (bool), (override));
    MOCK_METHOD(bool, unload, (), (override));
    MOCK_METHOD(bool, isForceLoaded, (), (const, override));
    MOCK_METHOD(void, setForceLoaded, (bool), (override));
    MOCK_METHOD(bool, addPluginChunkTicket, (endstone::Plugin &), (override));
    MOCK_METHOD(bool, removePluginChunkTicket, (endstone::Plugin &), (override));
    MOCK_METHOD(std::vector<endstone::NotNull<endstone::BlockState>>, getBlockActors, (), (const, override));
    MOCK_METHOD(std::vector<endstone::Plugin *>, getPluginChunkTickets, (), (const, override));
};

class ChunkLoadQueueTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        ON_CALL(plugin_, getDescription()).WillByDefault(testing::ReturnRef(description_));
        ON_CALL(other_, getDescription()).WillByDefault(testing::ReturnRef(description_));
    }

    std::shared_ptr<Request> add(endstone::Plugin &plugin, std::function<void(Nullable<Chunk>)> callback,
                                 std::uint64_t timeout = 3)
    {
        auto request = std::make_shared<Request>(-1, 2, &plugin, std::move(callback), true, timeout);
        queue_.add(request);
        return request;
    }

    void tick()
    {
        queue_.process([&](Request &request) { return poll_(request); },
                       [&](mce::UUID area) { released_.push_back(area); },
                       endstone::core::LoggerFactory::getLogger("ChunkLoadQueueTest"));
    }

    void cancel(endstone::Plugin &plugin)
    {
        queue_.cancel(plugin, [&](mce::UUID area) { released_.push_back(area); });
    }

    endstone::PluginDescription description_{"ChunkTest", "1.0"};
    testing::NiceMock<MockPlugin> plugin_;
    testing::NiceMock<MockPlugin> other_;
    ChunkLoadQueue queue_;
    std::vector<mce::UUID> released_;
    std::function<std::optional<Nullable<Chunk>>(Request &)> poll_ = [](Request &) {
        return std::nullopt;
    };
    Nullable<Chunk> chunk_ = std::make_shared<testing::NiceMock<MockChunk>>();
};

TEST_F(ChunkLoadQueueTest, CompletesOnceAndHoldsThroughCallback)
{
    int called = 0;
    auto request = add(plugin_, [&](Nullable<Chunk> chunk) {
        ++called;
        EXPECT_EQ(chunk, chunk_);
        EXPECT_TRUE(released_.empty());
    });
    request->area_id = {1, 1};
    EXPECT_EQ(called, 0);
    tick();
    EXPECT_EQ(called, 0);
    EXPECT_TRUE(released_.empty());
    poll_ = [&](Request &) {
        return chunk_;
    };
    tick();
    tick();
    EXPECT_EQ(called, 1);
    EXPECT_EQ(released_.size(), 1);
    EXPECT_EQ(request->area_id, mce::UUID::EMPTY);
}

TEST_F(ChunkLoadQueueTest, ReportsMissingChunkWithoutWaitingForTimeout)
{
    int called = 0;
    add(plugin_, [&](Nullable<Chunk> chunk) {
        ++called;
        EXPECT_EQ(chunk, nullptr);
    });
    poll_ = [](Request &) {
        return Nullable<Chunk>{};
    };
    tick();
    EXPECT_EQ(called, 1);
    EXPECT_TRUE(released_.empty());
}

TEST_F(ChunkLoadQueueTest, TimesOutAndReleasesOnlyOnce)
{
    int called = 0;
    auto request = add(plugin_, [&](Nullable<Chunk> chunk) {
        ++called;
        EXPECT_EQ(chunk, nullptr);
    });
    request->area_id = {1, 2};
    tick();
    tick();
    EXPECT_EQ(called, 0);
    tick();
    tick();
    EXPECT_EQ(called, 1);
    EXPECT_EQ(released_.size(), 1);
}

TEST_F(ChunkLoadQueueTest, ReadyChunkWinsOnLastAllowedTick)
{
    int called = 0;
    add(
        plugin_,
        [&](Nullable<Chunk> chunk) {
            ++called;
            EXPECT_EQ(chunk, chunk_);
        },
        1);
    poll_ = [&](Request &) {
        return chunk_;
    };
    tick();
    EXPECT_EQ(called, 1);
}

TEST_F(ChunkLoadQueueTest, CancelsOnlyTheOwningPlugin)
{
    int called = 0;
    auto first = add(plugin_, [](Nullable<Chunk>) { FAIL() << "Cancelled callback ran"; });
    auto second = add(other_, [&](Nullable<Chunk>) { ++called; });
    first->area_id = {1, 3};
    second->area_id = {1, 4};
    tick();
    cancel(plugin_);
    cancel(plugin_);
    ASSERT_EQ(released_.size(), 1);
    EXPECT_EQ(released_.front(), (mce::UUID{1, 3}));
    poll_ = [&](Request &) {
        return chunk_;
    };
    tick();
    EXPECT_EQ(called, 1);
    EXPECT_EQ(released_.size(), 2);
}

TEST_F(ChunkLoadQueueTest, CancelsBeforeLoadingWithoutReleasingAnArea)
{
    add(plugin_, [](Nullable<Chunk>) { FAIL() << "Cancelled callback ran"; });
    cancel(plugin_);
    poll_ = [](Request &) -> std::optional<Nullable<Chunk>> {
        ADD_FAILURE();
        return std::nullopt;
    };
    tick();
    EXPECT_TRUE(released_.empty());
}

TEST_F(ChunkLoadQueueTest, CallbackCanCancelLaterRequestsInTheSameBatch)
{
    auto first = add(plugin_, [&](Nullable<Chunk>) { cancel(plugin_); });
    auto second = add(plugin_, [](Nullable<Chunk>) { FAIL() << "Cancelled callback ran"; });
    first->area_id = {1, 5};
    second->area_id = {1, 6};
    poll_ = [&](Request &) {
        return chunk_;
    };
    tick();
    EXPECT_EQ(released_.size(), 2);
    EXPECT_FALSE(second->callback);
}

TEST_F(ChunkLoadQueueTest, RequestCreatedInCallbackWaitsForNextTick)
{
    int called = 0;
    add(plugin_, [&](Nullable<Chunk>) { add(plugin_, [&](Nullable<Chunk>) { ++called; }); });
    poll_ = [&](Request &) {
        return chunk_;
    };
    tick();
    EXPECT_EQ(called, 0);
    tick();
    EXPECT_EQ(called, 1);
}

TEST_F(ChunkLoadQueueTest, CallbackExceptionDoesNotLeakOrBlockOtherRequests)
{
    int called = 0;
    auto first = add(plugin_, [](Nullable<Chunk>) { throw std::runtime_error("callback failed"); });
    auto second = add(plugin_, [&](Nullable<Chunk>) { ++called; });
    first->area_id = {1, 7};
    second->area_id = {1, 8};
    poll_ = [&](Request &) {
        return chunk_;
    };
    EXPECT_NO_THROW(tick());
    EXPECT_EQ(called, 1);
    EXPECT_EQ(released_.size(), 2);
}

TEST_F(ChunkLoadQueueTest, NonStandardCallbackExceptionStillReleasesTheArea)
{
    auto request = add(plugin_, [](Nullable<Chunk>) { throw 1; });
    request->area_id = {1, 9};
    poll_ = [&](Request &) {
        return chunk_;
    };
    EXPECT_NO_THROW(tick());
    EXPECT_EQ(released_.size(), 1);
}

TEST_F(ChunkLoadQueueTest, LoadExceptionCompletesWithNullAndReleasesTheArea)
{
    int called = 0;
    auto request = add(plugin_, [&](Nullable<Chunk> chunk) {
        ++called;
        EXPECT_EQ(chunk, nullptr);
    });
    request->area_id = {1, 10};
    poll_ = [](Request &) -> std::optional<Nullable<Chunk>> {
        throw std::runtime_error("load failed");
    };
    EXPECT_NO_THROW(tick());
    EXPECT_EQ(called, 1);
    EXPECT_EQ(released_.size(), 1);
}

TEST_F(ChunkLoadQueueTest, RequestsForSameCoordinatesHaveIndependentHolds)
{
    int called = 0;
    auto first = add(plugin_, [&](Nullable<Chunk>) { ++called; });
    auto second = add(plugin_, [&](Nullable<Chunk>) { ++called; });
    first->area_id = {1, 11};
    second->area_id = {1, 12};
    poll_ = [&](Request &request) -> std::optional<Nullable<Chunk>> {
        return &request == first.get() ? std::optional{chunk_} : std::nullopt;
    };
    tick();
    EXPECT_EQ(called, 1);
    ASSERT_EQ(released_.size(), 1);
    EXPECT_EQ(second->area_id, (mce::UUID{1, 12}));
    poll_ = [&](Request &) {
        return chunk_;
    };
    tick();
    EXPECT_EQ(called, 2);
    EXPECT_EQ(released_.size(), 2);
}
