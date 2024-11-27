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

#include <filesystem>

#include "endstone/ban/player_ban_list.h"
#include "endstone/util/result.h"

namespace fs = std::filesystem;

namespace endstone::detail {

class EndstonePlayerBanList : public PlayerBanList {
public:
    explicit EndstonePlayerBanList(fs::path file);

    [[nodiscard]] const PlayerBanEntry *getBanEntry(std::string name) const override;
    [[nodiscard]] PlayerBanEntry *getBanEntry(std::string name) override;
    [[nodiscard]] const PlayerBanEntry *getBanEntry(std::string name, std::optional<UUID> uuid,
                                                    std::optional<std::string> xuid) const override;
    [[nodiscard]] PlayerBanEntry *getBanEntry(std::string name, std::optional<UUID> uuid,
                                              std::optional<std::string> xuid) override;
    PlayerBanEntry &addBan(std::string name, std::optional<std::string> reason, std::optional<BanEntry::Date> expires,
                           std::optional<std::string> source) override;
    PlayerBanEntry &addBan(std::string name, std::optional<UUID> uuid, std::optional<std::string> xuid,
                           std::optional<std::string> reason, std::optional<BanEntry::Date> expires,
                           std::optional<std::string> source) override;
    PlayerBanEntry &addBan(std::string name, std::optional<std::string> reason, std::chrono::seconds duration,
                           std::optional<std::string> source) override;
    PlayerBanEntry &addBan(std::string name, std::optional<UUID> uuid, std::optional<std::string> xuid,
                           std::optional<std::string> reason, std::chrono::seconds duration,
                           std::optional<std::string> source) override;
    [[nodiscard]] std::vector<const PlayerBanEntry *> getEntries() const override;
    [[nodiscard]] std::vector<PlayerBanEntry *> getEntries() override;
    [[nodiscard]] bool isBanned(std::string name) const override;
    [[nodiscard]] bool isBanned(std::string name, std::optional<UUID> uuid,
                                std::optional<std::string> xuid) const override;
    void removeBan(std::string name) override;
    void removeBan(std::string name, std::optional<UUID> uuid, std::optional<std::string> xuid) override;

    Result<void> save();
    Result<void> load();

private:
    static bool match(const PlayerBanEntry &entry, const std::string &name, const std::optional<UUID> &uuid,
                      const std::optional<std::string> &xuid);

    void removeExpired();

    std::vector<PlayerBanEntry> entries_;
    fs::path file_;
};

}  // namespace endstone::detail
