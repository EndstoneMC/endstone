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

#include <cfloat>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include <gtest/gtest.h>

#include "bedrock/server/commands/command_selector.h"

TEST(CommandSelectorTest, Size)
{
    EXPECT_EQ(sizeof(CommandSelectorBase), 200);
    EXPECT_EQ(alignof(CommandSelectorBase), 8);
    EXPECT_EQ(sizeof(CommandSelector<Actor>), 200);
    EXPECT_EQ(sizeof(CommandSelector<Player>), 200);
    EXPECT_EQ(sizeof(CommandSelectorResults<Actor>), 16);
    EXPECT_EQ(sizeof(CommandPosition), 16);
    EXPECT_EQ(alignof(CommandPosition), 4);
    EXPECT_EQ(sizeof(CommandPositionFloat), 16);
}

TEST(CommandSelectorTest, DefaultConstructedLayout)
{
    const CommandSelector<Actor> actors;
    const CommandSelector<Player> players;
    const auto *bytes = reinterpret_cast<const unsigned char *>(&actors);
    auto read = [](const void *object, std::size_t offset, auto value) {
        std::memcpy(&value, static_cast<const unsigned char *>(object) + offset, sizeof(value));
        return value;
    };

    EXPECT_EQ(read(bytes, 0, std::int32_t{}), 0);
    EXPECT_EQ(read(bytes, 4, std::int32_t{}), 0);
    EXPECT_EQ(read(bytes, 8, std::int32_t{}), 0);
    for (std::size_t i = 16; i < 136; ++i) {
        EXPECT_EQ(bytes[i], 0) << "filter vectors at " << i;
    }
    EXPECT_EQ(read(bytes, 136, float{}), 0.0F);
    EXPECT_EQ(read(bytes, 140, float{}), 0.0F);
    EXPECT_EQ(read(bytes, 144, float{}), 0.0F);
    EXPECT_EQ(bytes[148], 1);
    EXPECT_EQ(bytes[149], 1);
    EXPECT_EQ(bytes[150], 1);
    EXPECT_EQ(bytes[151], 0);
    EXPECT_EQ(read(bytes, 152, float{}), 0.0F);
    EXPECT_EQ(read(bytes, 156, float{}), 0.0F);
    EXPECT_EQ(read(bytes, 160, float{}), 0.0F);
    EXPECT_EQ(read(bytes, 164, float{}), 0.0F);
    EXPECT_EQ(read(bytes, 168, float{}), FLT_MAX);
    EXPECT_EQ(read(bytes, 176, std::uint64_t{}), 0xFFFFFFFFULL);
    for (std::size_t i = 184; i <= 194; ++i) {
        EXPECT_EQ(bytes[i], 0) << "flag at " << i;
    }
    EXPECT_EQ(read(&players, 190, bool{}), true);
    EXPECT_EQ(read(&players, 194, bool{}), false);
}

TEST(CommandPositionTest, DefaultConstructedLayout)
{
    const CommandPosition position;
    const auto *bytes = reinterpret_cast<const unsigned char *>(&position);
    for (std::size_t i = 0; i < 12; ++i) {
        EXPECT_EQ(bytes[i], 0) << "offset at " << i;
    }
    EXPECT_EQ(bytes[12], 1);
    EXPECT_EQ(bytes[13], 1);
    EXPECT_EQ(bytes[14], 1);
    EXPECT_EQ(bytes[15], 0);
}
