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

#include "endstone/ban/ip_ban_list.h"
#include "endstone/detail/ban/ban_list.h"

namespace nlohmann {
template <>
struct adl_serializer<endstone::IpBanEntry> {
    static endstone::IpBanEntry from_json(const json &j)  // NOLINT(*-identifier-naming)
    {
        return endstone::IpBanEntry(j.at("ip").get<std::string>());
    }

    static void to_json(json &j, const endstone::IpBanEntry &entry)  // NOLINT(*-identifier-naming)
    {
        j["ip"] = entry.getAddress();
    }
};
}  // namespace nlohmann

namespace endstone::detail {

bool match(const IpBanEntry &entry, const std::string &address);

class EndstoneIpBanList : public IpBanList, public EndstoneBanList<IpBanEntry> {
public:
    using EndstoneBanList::EndstoneBanList;

    [[nodiscard]] const IpBanEntry *getBanEntry(std::string address) const override;
    [[nodiscard]] IpBanEntry *getBanEntry(std::string address) override;
    IpBanEntry &addBan(std::string address, std::optional<std::string> reason, std::optional<BanEntry::Date> expires,
                       std::optional<std::string> source) override;
    IpBanEntry &addBan(std::string address, std::optional<std::string> reason, std::chrono::seconds duration,
                       std::optional<std::string> source) override;
    [[nodiscard]] std::vector<const IpBanEntry *> getEntries() const override;
    [[nodiscard]] std::vector<IpBanEntry *> getEntries() override;
    [[nodiscard]] bool isBanned(std::string address) const override;
    void removeBan(std::string address) override;
};

}  // namespace endstone::detail
