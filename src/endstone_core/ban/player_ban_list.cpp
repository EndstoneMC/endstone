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

#include "endstone/detail/ban/player_ban_list.h"

#include <boost/algorithm/string.hpp>

namespace endstone::detail {

bool PlayerBanEntryMatcher::operator()(const PlayerBanEntry &entry, const std::string &name,
                                       const std::optional<UUID> &uuid, const std::optional<std::string> &xuid) const
{
    const bool name_match = boost::iequals(entry.getName(), name);
    const bool uuid_match =
        uuid.has_value() && entry.getUniqueId().has_value() ? entry.getUniqueId().value() == uuid.value() : true;
    const bool xuid_match =
        xuid.has_value() && !xuid.value().empty() && entry.getXuid().has_value() && !entry.getXuid().value().empty()
            ? entry.getXuid().value() == xuid.value()
            : true;
    return name_match && uuid_match && xuid_match;
}

const PlayerBanEntry *EndstonePlayerBanList::getBanEntry(std::string name) const
{
    return getBanEntry(name, std::nullopt, std::nullopt);
}

PlayerBanEntry *EndstonePlayerBanList::getBanEntry(std::string name)
{
    return getBanEntry(name, std::nullopt, std::nullopt);
}

const PlayerBanEntry *EndstonePlayerBanList::getBanEntry(std::string name, std::optional<UUID> uuid,
                                                         std::optional<std::string> xuid) const
{
    const auto it = std::find_if(entries_.begin(), entries_.end(),
                                 [&](const PlayerBanEntry &entry) { return matcher_(entry, name, uuid, xuid); });

    if (it != entries_.end()) {
        return &(*it);
    }
    return nullptr;
}

PlayerBanEntry *EndstonePlayerBanList::getBanEntry(std::string name, std::optional<UUID> uuid,
                                                   std::optional<std::string> xuid)
{
    const auto it = std::find_if(entries_.begin(), entries_.end(),
                                 [&](PlayerBanEntry &entry) { return matcher_(entry, name, uuid, xuid); });

    if (it != entries_.end()) {
        return &(*it);
    }
    return nullptr;
}

PlayerBanEntry &EndstonePlayerBanList::addBan(std::string name, std::optional<std::string> reason,
                                              std::optional<BanEntry::Date> expires, std::optional<std::string> source)
{
    return addBan(name, std::nullopt, std::nullopt, reason, expires, source);
}

PlayerBanEntry &EndstonePlayerBanList::addBan(std::string name, std::optional<UUID> uuid,
                                              std::optional<std::string> xuid, std::optional<std::string> reason,
                                              std::optional<BanEntry::Date> expires, std::optional<std::string> source)
{
    entries_.erase(std::remove_if(entries_.begin(), entries_.end(),
                                  [&](PlayerBanEntry &entry) { return matcher_(entry, name, uuid, xuid); }),
                   entries_.end());

    PlayerBanEntry new_entry{name, uuid, xuid};
    if (reason.has_value()) {
        new_entry.setReason(reason.value());
    }
    new_entry.setExpiration(expires);
    if (source.has_value()) {
        new_entry.setSource(source.value());
    }
    auto &entry = entries_.emplace_back(new_entry);
    save();

    return entry;
}

PlayerBanEntry &EndstonePlayerBanList::addBan(std::string name, std::optional<std::string> reason,
                                              std::chrono::seconds duration, std::optional<std::string> source)
{
    return addBan(name, std::nullopt, std::nullopt, reason, duration, source);
}

PlayerBanEntry &EndstonePlayerBanList::addBan(std::string name, std::optional<UUID> uuid,
                                              std::optional<std::string> xuid, std::optional<std::string> reason,
                                              std::chrono::seconds duration, std::optional<std::string> source)
{
    return addBan(name, uuid, xuid, reason, std::chrono::system_clock::now() + duration, source);
}

std::vector<const PlayerBanEntry *> EndstonePlayerBanList::getEntries() const
{
    return EndstoneBanList::getEntries();
}

std::vector<PlayerBanEntry *> EndstonePlayerBanList::getEntries()
{
    return EndstoneBanList::getEntries();
}

bool EndstonePlayerBanList::isBanned(std::string name) const
{
    return isBanned(name, std::nullopt, std::nullopt);
}

bool EndstonePlayerBanList::isBanned(std::string name, std::optional<UUID> uuid, std::optional<std::string> xuid) const
{
    const_cast<EndstonePlayerBanList *>(this)->removeExpired();
    return getBanEntry(name, uuid, xuid) != nullptr;
}

void EndstonePlayerBanList::removeBan(std::string name)
{
    removeBan(name, std::nullopt, std::nullopt);
}

void EndstonePlayerBanList::removeBan(std::string name, std::optional<UUID> uuid, std::optional<std::string> xuid)
{
    const auto it = std::find_if(entries_.begin(), entries_.end(),
                                 [&](PlayerBanEntry &entry) { return matcher_(entry, name, uuid, xuid); });
    if (it != entries_.end()) {
        entries_.erase(it);
        save();
    }
}

}  // namespace endstone::detail
