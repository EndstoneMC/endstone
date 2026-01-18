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

#include <boost/uuid/string_generator.hpp>

#include "endstone/ban/player_ban_list.h"
#include "endstone/core/ban/ban_list.h"

namespace nlohmann {
template <>
struct adl_serializer<endstone::PlayerBanEntry> {
    static endstone::PlayerBanEntry from_json(const json &j)  // NOLINT(*-identifier-naming)
    {
        std::string name = j["name"];
        std::optional<endstone::UUID> uuid = std::nullopt;
        std::optional<std::string> xuid = std::nullopt;
        if (j.contains("uuid")) {
            try {
                boost::uuids::string_generator gen;
                boost::uuids::uuid u1 = gen(std::string(j["uuid"]));
                endstone::UUID u2;
                std::memcpy(u2.data, u1.data, u1.size());
                uuid = u2;
            }
            catch (std::exception &) {
            }
        }
        if (j.contains("xuid")) {
            xuid = j["xuid"];
        }

        return endstone::PlayerBanEntry(name, uuid, xuid);
    }

    static void to_json(json &j, const endstone::PlayerBanEntry &entry)  // NOLINT(*-identifier-naming)
    {
        j["name"] = entry.getName();
        if (entry.getUniqueId().has_value()) {
            j["uuid"] = entry.getUniqueId().value().str();
        }
        if (entry.getXuid().has_value()) {
            j["xuid"] = entry.getXuid().value();
        }
    }
};
}  // namespace nlohmann

namespace endstone::core {
struct PlayerBanEntryMatcher {
    bool operator()(const PlayerBanEntry &entry, const std::string &name,
                    const std::optional<UUID> &uuid = std::nullopt,
                    const std::optional<std::string> &xuid = std::nullopt) const;
};

class EndstonePlayerBanList : public EndstoneBanList<PlayerBanList, PlayerBanEntryMatcher> {
public:
    using EndstoneBanList::EndstoneBanList;
    [[nodiscard]] Nullable<PlayerBanEntry> getBanEntry(std::string name) const override;
    [[nodiscard]] Nullable<PlayerBanEntry> getBanEntry(std::string name, std::optional<UUID> uuid,
                                                       std::optional<std::string> xuid) const override;
    NotNull<PlayerBanEntry> addBan(std::string name, std::optional<std::string> reason,
                                   std::optional<BanEntry::Date> expires, std::optional<std::string> source) override;
    NotNull<PlayerBanEntry> addBan(std::string name, std::optional<UUID> uuid, std::optional<std::string> xuid,
                                   std::optional<std::string> reason, std::optional<BanEntry::Date> expires,
                                   std::optional<std::string> source) override;
    NotNull<PlayerBanEntry> addBan(std::string name, std::optional<std::string> reason, std::chrono::seconds duration,
                                   std::optional<std::string> source) override;
    NotNull<PlayerBanEntry> addBan(std::string name, std::optional<UUID> uuid, std::optional<std::string> xuid,
                                   std::optional<std::string> reason, std::chrono::seconds duration,
                                   std::optional<std::string> source) override;
    [[nodiscard]] bool isBanned(std::string name) const override;
    [[nodiscard]] bool isBanned(std::string name, std::optional<UUID> uuid,
                                std::optional<std::string> xuid) const override;
    void removeBan(std::string name) override;
    void removeBan(std::string name, std::optional<UUID> uuid, std::optional<std::string> xuid) override;
};
}  // namespace endstone::core
