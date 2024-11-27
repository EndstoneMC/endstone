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

#include <date/date.h>
#include <fmt/format.h>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "endstone/detail/ban/player_ban_list.h"

namespace endstone::detail {

class PlayerBanListTest : public ::testing::Test {
protected:
    std::string file_ = "test_banned_players.json";
    UUID uuid_{0x0b, 0xd2, 0xc9, 0xc5, 0xc0, 0x18, 0x4f, 0x3c, 0x98, 0x13, 0x82, 0xe1, 0x75, 0x16, 0x2e, 0x37};
    std::string xuid_ = "2535472663854546";

    void SetUp() override {}

    void TearDown() override
    {
        std::remove(file_.c_str());
    }
};

TEST_F(PlayerBanListTest, AddAndRetrieveBanEntry)
{
    EndstonePlayerBanList ban_list{file_};

    // Add a new ban entry
    ban_list.addBan("player11", "Misconduct", std::nullopt, "Moderator");

    // Retrieve and validate the ban entry
    auto *entry = ban_list.getBanEntry("player11");
    ASSERT_NE(entry, nullptr);
    EXPECT_EQ(date::format("%FT%T%Ez", entry->getCreated()), "2024-11-27T12:34:01+00:00");
    EXPECT_EQ(entry->getName(), "player11");
    EXPECT_EQ(entry->getReason(), "Misconduct");
    EXPECT_EQ(entry->getSource(), "Moderator");
    EXPECT_FALSE(entry->getExpiration());
}

TEST_F(PlayerBanListTest, AddBanEntryWithOptionalFields)
{
    EndstonePlayerBanList ban_list{file_};
    ban_list.addBan("player11", uuid_, xuid_, "Misconduct", std::nullopt, "Moderator");

    // Retrieve and validate the ban entry
    auto *entry = ban_list.getBanEntry("player11", uuid_, xuid_);
    ASSERT_NE(entry, nullptr);
    EXPECT_EQ(entry->getName(), "player11");
    EXPECT_EQ(entry->getUniqueId(), uuid_);
    EXPECT_EQ(entry->getXuid(), xuid_);
    EXPECT_EQ(entry->getReason(), "Misconduct");
    EXPECT_EQ(entry->getSource(), "Moderator");
    EXPECT_FALSE(entry->getExpiration());
}

TEST_F(PlayerBanListTest, IsBanned)
{
    EndstonePlayerBanList ban_list{file_};

    // Add a ban entry
    ban_list.addBan("player11", "Misconduct", std::chrono::hours(24), "Moderator");

    // Check if the player is banned
    EXPECT_TRUE(ban_list.isBanned("player11"));

    // Check for a player not in the ban list
    EXPECT_FALSE(ban_list.isBanned("playerNotExist"));
}

TEST_F(PlayerBanListTest, RemoveBanEntry)
{
    EndstonePlayerBanList ban_list{file_};

    // Add a ban entry
    ban_list.addBan("player11", uuid_, xuid_, "Misconduct", std::nullopt, "Moderator");

    // Remove the ban entry
    ban_list.removeBan("player11", uuid_, xuid_);

    // Check if the entry was removed
    EXPECT_FALSE(ban_list.isBanned("player11"));
    auto *entry = ban_list.getBanEntry("player11", uuid_, xuid_);
    EXPECT_EQ(entry, nullptr);
}

TEST_F(PlayerBanListTest, AddBanWithDuration)
{
    EndstonePlayerBanList ban_list{file_};

    // Add a ban entry with duration
    ban_list.addBan("player11", "Misconduct", std::chrono::hours(1), "Moderator");

    // Retrieve and validate the ban entry
    auto *entry = ban_list.getBanEntry("player11");
    ASSERT_NE(entry, nullptr);
    EXPECT_EQ(entry->getName(), "player11");
    EXPECT_EQ(entry->getReason(), "Misconduct");
    EXPECT_EQ(entry->getSource(), "Moderator");
    EXPECT_TRUE(entry->getExpiration().has_value());
}

TEST_F(PlayerBanListTest, SaveBanList)
{
    EndstonePlayerBanList ban_list{file_};
    auto &entry = ban_list.addBan("player11", uuid_, xuid_, "Misconduct", std::nullopt, "Moderator");

    auto result = ban_list.save();
    EXPECT_TRUE(result) << result.error().getMessage() << result.error().getStackTrace();

    // Verify the saved file content
    std::ifstream file(file_);
    nlohmann::json array;
    file >> array;

    ASSERT_EQ(array.size(), 1);
    auto json = array.front();
    EXPECT_EQ(json["created"],
              date::format(BanEntry::DateFormat, date::floor<std::chrono::seconds>(entry.getCreated())));
    EXPECT_EQ(json["name"], "player11");
    EXPECT_EQ(json["uuid"], uuid_.str());
    EXPECT_EQ(json["xuid"], xuid_);
    EXPECT_EQ(json["reason"], "Misconduct");
    EXPECT_EQ(json["source"], "Moderator");
    EXPECT_EQ(json["expires"], "forever");
}

TEST_F(PlayerBanListTest, SaveWithMissingOptionalFields)
{
    EndstonePlayerBanList ban_list{file_};
    auto &entry = ban_list.addBan("player12", "Cheating", std::nullopt, "Admin");

    auto result = ban_list.save();
    EXPECT_TRUE(result) << result.error().getMessage() << result.error().getStackTrace();

    // Verify the saved file content
    std::ifstream file(file_);
    nlohmann::json array;
    file >> array;

    ASSERT_EQ(array.size(), 1);
    auto json = array.front();
    EXPECT_EQ(json["created"],
              date::format(BanEntry::DateFormat, date::floor<std::chrono::seconds>(entry.getCreated())));
    EXPECT_EQ(json["name"], "player12");
    EXPECT_FALSE(json.contains("uuid"));
    EXPECT_FALSE(json.contains("xuid"));
    EXPECT_EQ(json["reason"], "Cheating");
    EXPECT_EQ(json["source"], "Admin");
    EXPECT_EQ(json["expires"], "forever");
}

TEST_F(PlayerBanListTest, SaveWithFileException)
{
    // Mock a scenario where writing to file throws an exception
    std::string invalid_file = "/invalid_path/test_banned_players.json";
    EndstonePlayerBanList ban_list{invalid_file};
    ban_list.addBan("player13", "Abuse", std::nullopt, "AutoModeration");
    auto result = ban_list.save();
    EXPECT_FALSE(result);
    EXPECT_EQ(result.error().getMessage(), "Unable to open file '/invalid_path/test_banned_players.json'.");
}

TEST_F(PlayerBanListTest, LoadBanList)
{
    nlohmann::json json = nlohmann::json::array();
    json.push_back({
        {"created", "2024-11-27T12:34:01+02:00"},
        {"name", "player11"},
        {"uuid", "0bd2c9c5-c018-4f3c-9813-82e175162e37"},
        {"xuid", "2535472663854546"},
        {"reason", "Misconduct"},
        {"source", "Moderator"},
        {"expires", "forever"},
    });

    std::ofstream file(file_);
    file << json;
    file.close();

    EndstonePlayerBanList ban_list{file_};
    auto result = ban_list.load();
    EXPECT_TRUE(result) << result.error().getMessage() << result.error().getStackTrace();

    EXPECT_EQ(ban_list.getEntries().size(), 1);
    auto *entry = ban_list.getBanEntry("player11");
    ASSERT_NE(entry, nullptr);
    EXPECT_EQ(date::format("%FT%T%Ez", date::floor<std::chrono::seconds>(entry->getCreated())),
              "2024-11-27T10:34:01+00:00");
    EXPECT_EQ(entry->getName(), "player11");
    EXPECT_EQ(entry->getUniqueId(), uuid_);
    EXPECT_EQ(entry->getXuid(), "2535472663854546");
    EXPECT_EQ(entry->getReason(), "Misconduct");
    EXPECT_EQ(entry->getSource(), "Moderator");
    EXPECT_FALSE(entry->getExpiration());
}

TEST_F(PlayerBanListTest, LoadNonExistingFile)
{
    EndstonePlayerBanList ban_list{"non_existing_banned_players.json"};
    auto result = ban_list.load();
    EXPECT_TRUE(result);
    EXPECT_EQ(ban_list.getEntries().size(), 0);  // No entries should be loaded
}

}  // namespace endstone::detail
