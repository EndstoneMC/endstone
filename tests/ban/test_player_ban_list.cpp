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

#include <fstream>
#include <optional>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "endstone/detail/ban/player_ban_list.h"

using namespace endstone::detail;

class PlayerBanListTest : public ::testing::Test {
protected:
    std::string file_ = "test_banned_players.json";
    EndstonePlayerBanList player_ban_{file_};

    void SetUp() override {}

    void TearDown() override
    {
        std::remove(file_.c_str());
    }
};

TEST_F(PlayerBanListTest, LoadBanList)
{
    nlohmann::json json = nlohmann::json::array();
    json.push_back({
        {"created", "2024-11-27 12:34:01 Z"},
        {"name", "player11"},
        {"reason", "Misconduct"},
        {"source", "Moderator"},
        {"expires", "forever"},
    });

    std::ofstream file(file_);
    file << json;
    file.close();

    auto result = player_ban_.load();
    EXPECT_TRUE(result) << result.error().getMessage() << result.error().getStackTrace();

    EXPECT_EQ(player_ban_.getEntries().size(), 1);
    auto *entry = player_ban_.getBanEntry("player11");
    ASSERT_NE(entry, nullptr);
    EXPECT_EQ(entry->getCreated().time_since_epoch().count(), 17327108410000000LL);
    EXPECT_EQ(entry->getName(), "player11");
    EXPECT_EQ(entry->getReason(), "Misconduct");
    EXPECT_EQ(entry->getSource(), "Moderator");
    EXPECT_FALSE(entry->getExpiration());
}
