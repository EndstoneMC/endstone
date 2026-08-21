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

#include "bedrock/server/commands/command_selector.h"

TEST(CommandSelectorTest, SelectorsAreTheSameSizeAsTheirBase)
{
    EXPECT_EQ(sizeof(CommandSelector<Actor>), sizeof(CommandSelectorBase));
    EXPECT_EQ(sizeof(CommandSelector<Player>), sizeof(CommandSelectorBase));
}

TEST(CommandSelectorTest, SelectionTypeMatchesTheGameValues)
{
    EXPECT_EQ(static_cast<int>(CommandSelectionType::Self), 0);
    EXPECT_EQ(static_cast<int>(CommandSelectionType::Entities), 1);
    EXPECT_EQ(static_cast<int>(CommandSelectionType::Players), 2);
    EXPECT_EQ(static_cast<int>(CommandSelectionType::DefaultPlayers), 3);
    EXPECT_EQ(static_cast<int>(CommandSelectionType::OwnedAgent), 4);
    EXPECT_EQ(static_cast<int>(CommandSelectionType::Agents), 5);
    EXPECT_EQ(static_cast<int>(CommandSelectionType::Initiator), 6);
}

TEST(CommandSelectorTest, SelectionOrderMatchesTheGameValues)
{
    EXPECT_EQ(static_cast<int>(CommandSelectionOrder::Sorted), 0);
    EXPECT_EQ(static_cast<int>(CommandSelectionOrder::InverseSorted), 1);
    EXPECT_EQ(static_cast<int>(CommandSelectionOrder::Random), 2);
}
