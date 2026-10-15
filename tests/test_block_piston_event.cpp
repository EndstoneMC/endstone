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

#include <memory>
#include <type_traits>
#include <utility>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "endstone/event/block/block_piston_extend_event.h"
#include "endstone/event/block/block_piston_retract_event.h"

namespace {

class MockBlock : public endstone::Block {
public:
    MOCK_METHOD(const endstone::BlockType &, getType, (), (const, override));
    MOCK_METHOD(void, setType, (endstone::BlockTypeId), (override));
    MOCK_METHOD(void, setType, (endstone::BlockTypeId, bool), (override));
    MOCK_METHOD(endstone::NotNull<endstone::BlockData>, getData, (), (const, override));
    MOCK_METHOD(void, setData, (const endstone::BlockData &), (override));
    MOCK_METHOD(void, setData, (const endstone::BlockData &, bool), (override));
    MOCK_METHOD(endstone::NotNull<endstone::Block>, getRelative, (int, int, int), (override));
    MOCK_METHOD(endstone::NotNull<endstone::Block>, getRelative, (endstone::BlockFace), (override));
    MOCK_METHOD(endstone::NotNull<endstone::Block>, getRelative, (endstone::BlockFace, int), (override));
    MOCK_METHOD(endstone::NotNull<endstone::Dimension>, getDimension, (), (const, override));
    MOCK_METHOD(const endstone::Biome &, getBiome, (), (const, override));
    MOCK_METHOD(int, getX, (), (const, override));
    MOCK_METHOD(int, getY, (), (const, override));
    MOCK_METHOD(int, getZ, (), (const, override));
    MOCK_METHOD(endstone::Location, getLocation, (), (const, override));
    MOCK_METHOD(endstone::NotNull<endstone::BlockState>, captureState, (bool), (const, override));
};

template <typename T>
class BlockPistonEventTest : public testing::Test {};

using PistonEvents = testing::Types<endstone::BlockPistonExtendEvent, endstone::BlockPistonRetractEvent>;
TYPED_TEST_SUITE(BlockPistonEventTest, PistonEvents);

TYPED_TEST(BlockPistonEventTest, PreservesTheTwoArgumentConstructor)
{
    endstone::NotNull<endstone::Block> piston = std::make_shared<MockBlock>();
    TypeParam event(piston, endstone::BlockFace::East);

    EXPECT_EQ(event.getBlock().get(), piston.get());
    EXPECT_EQ(event.getDirection(), endstone::BlockFace::East);
    EXPECT_TRUE(event.getBlocks().empty());
    EXPECT_FALSE(event.isCancelled());
}

TYPED_TEST(BlockPistonEventTest, OwnsAnOrderedSnapshotOfTheAffectedBlocks)
{
    endstone::NotNull<endstone::Block> piston = std::make_shared<MockBlock>();
    endstone::NotNull<endstone::Block> moved = std::make_shared<MockBlock>();
    endstone::NotNull<endstone::Block> broken = std::make_shared<MockBlock>();
    endstone::BlockPistonEvent::BlockList blocks{moved, broken};
    TypeParam event(piston, endstone::BlockFace::West, blocks);
    blocks.clear();

    ASSERT_EQ(event.getBlocks().size(), 2);
    EXPECT_EQ(event.getBlocks()[0].get(), moved.get());
    EXPECT_EQ(event.getBlocks()[1].get(), broken.get());
    EXPECT_EQ(event.getDirection(), endstone::BlockFace::West);
}

TYPED_TEST(BlockPistonEventTest, ExposesTheBlockListAsReadOnly)
{
    static_assert(std::is_same_v<decltype(std::declval<TypeParam &>().getBlocks()),
                                 const endstone::BlockPistonEvent::BlockList &>);
    static_assert(std::is_same_v<decltype(std::declval<const TypeParam &>().getBlocks()),
                                 const endstone::BlockPistonEvent::BlockList &>);
}

TYPED_TEST(BlockPistonEventTest, CancellationDoesNotChangeTheAffectedBlocks)
{
    endstone::NotNull<endstone::Block> piston = std::make_shared<MockBlock>();
    endstone::NotNull<endstone::Block> moved = std::make_shared<MockBlock>();
    TypeParam event(piston, endstone::BlockFace::Up, {moved});

    event.cancel();
    EXPECT_TRUE(event.isCancelled());
    event.setCancelled(false);
    EXPECT_FALSE(event.isCancelled());
    ASSERT_EQ(event.getBlocks().size(), 1);
    EXPECT_EQ(event.getBlocks()[0].get(), moved.get());
}

}  // namespace
