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

#include <fstream>
#include <utility>

#include <boost/uuid/string_generator.hpp>
#include <date/date.h>
#include <fmt/format.h>
#include <fmt/std.h>
#include <nlohmann/json.hpp>

#include "endstone/detail/util/error.h"
#include "endstone/util/result.h"

namespace endstone::detail {

EndstonePlayerBanList::EndstonePlayerBanList(fs::path file) : file_(std::move(file)) {}

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
                                 [&](const PlayerBanEntry &entry) { return match(entry, name, uuid, xuid); });

    if (it != entries_.end()) {
        return const_cast<PlayerBanEntry *>(&(*it));
    }
    return nullptr;
}

PlayerBanEntry *EndstonePlayerBanList::getBanEntry(std::string name, std::optional<UUID> uuid,
                                                   std::optional<std::string> xuid)
{
    const auto it = std::find_if(entries_.begin(), entries_.end(),
                                 [&](PlayerBanEntry &entry) { return match(entry, name, uuid, xuid); });

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
    auto *entry = getBanEntry(name, uuid, xuid);
    if (!entry) {
        PlayerBanEntry new_entry{name, uuid, xuid};
        if (reason.has_value()) {
            new_entry.setReason(reason.value());
        }
        new_entry.setExpiration(expires);
        if (source.has_value()) {
            new_entry.setSource(source.value());
        }
        entries_.emplace_back(new_entry);
        entry = &entries_.back();
    }
    else {
        // TODO: update entry
    }

    save();
    return *entry;
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
    std::vector<const PlayerBanEntry *> entries;
    entries.reserve(entries_.size());
    for (const auto &entry : entries_) {
        entries.push_back(&entry);
    }
    return entries;
}

std::vector<PlayerBanEntry *> EndstonePlayerBanList::getEntries()
{
    std::vector<PlayerBanEntry *> entries;
    entries.reserve(entries_.size());
    for (auto &entry : entries_) {
        entries.push_back(&entry);
    }
    return entries;
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
                                 [&](PlayerBanEntry &entry) { return match(entry, name, uuid, xuid); });
    if (it != entries_.end()) {
        entries_.erase(it);
        save();
    }
}

bool EndstonePlayerBanList::match(const PlayerBanEntry &entry, const std::string &name, const std::optional<UUID> &uuid,
                                  const std::optional<std::string> &xuid)
{
    const bool name_match = entry.getName() == name;
    const bool uuid_match =
        !uuid.has_value() || (entry.getUniqueId().has_value() && entry.getUniqueId().value() == uuid.value());
    const bool xuid_match =
        !xuid.has_value() || (entry.getXuid().has_value() && entry.getXuid().value() == xuid.value());
    return name_match && uuid_match && xuid_match;
}

Result<void> EndstonePlayerBanList::save()
{
    nlohmann::json array = nlohmann::json::array();
    for (const auto &entry : entries_) {
        nlohmann::json json;
        json["name"] = entry.getName();
        if (entry.getUniqueId().has_value()) {
            json["uuid"] = entry.getUniqueId().value().str();
        }
        if (entry.getXuid().has_value()) {
            json["xuid"] = entry.getXuid().value();
        }
        json["created"] = date::format(BanEntry::DateFormat, date::floor<std::chrono::seconds>(entry.getCreated()));
        json["source"] = entry.getSource();
        if (entry.getExpiration().has_value()) {
            json["expires"] =
                date::format(BanEntry::DateFormat, date::floor<std::chrono::seconds>(entry.getExpiration().value()));
        }
        else {
            json["expires"] = "forever";
        }
        json["reason"] = entry.getReason();
        array.push_back(json);
    }

    std::ofstream file(file_);
    if (!file) {
        return nonstd::make_unexpected(make_error("Unable to open file '{}'.", file_));
    }

    try {
        file << array;
        return {};
    }
    catch (const std::exception &e) {
        return nonstd::make_unexpected(make_error("Unable to write file '{}': {}", file_, e.what()));
    }
}

Result<void> EndstonePlayerBanList::load()
{
    if (!exists(file_)) {
        return {};
    }

    entries_.clear();

    std::ifstream file(file_);
    if (!file) {
        return nonstd::make_unexpected(make_error("Unable to open file '{}'.", file_));
    }

    try {
        auto array = nlohmann::json::parse(file);
        for (const auto &json : array) {
            std::string name = json["name"];
            std::optional<UUID> uuid = std::nullopt;
            std::optional<std::string> xuid = std::nullopt;
            if (json.contains("uuid")) {
                try {
                    boost::uuids::string_generator gen;
                    boost::uuids::uuid u1 = gen(std::string(json["uuid"]));
                    UUID u2;
                    std::memcpy(u2.data, u1.data, u1.size());
                    uuid = u2;
                }
                catch (std::exception &) {
                }
            }
            if (json.contains("xuid")) {
                xuid = json["xuid"];
            }
            PlayerBanEntry entry{name, uuid, xuid};
            if (json.contains("created")) {
                std::string created = json["created"];
                std::istringstream in{created};
                BanEntry::Date date;
                in >> date::parse(BanEntry::DateFormat, date);
                if (!in.fail()) {
                    entry.setCreated(date);
                }
            }
            if (json.contains("source")) {
                entry.setSource(json["source"]);
            }
            if (json.contains("expires")) {
                std::string expires = json["expires"];
                std::istringstream in{expires};
                BanEntry::Date date;
                in >> date::parse(BanEntry::DateFormat, date);
                if (!in.fail()) {
                    entry.setExpiration(date);
                }
            }
            if (json.contains("reason")) {
                entry.setReason(json["reason"]);
            }
            entries_.emplace_back(entry);
        }
        return {};
    }
    catch (const std::exception &e) {
        return nonstd::make_unexpected(make_error("Unable to read file '{}': {}", file_, e.what()));
    }
}

void EndstonePlayerBanList::removeExpired()
{
    auto it = entries_.begin();
    while (it != entries_.end()) {
        if (it->getExpiration() < std::chrono::system_clock::now()) {
            it = entries_.erase(it);
        }
        else {
            ++it;
        }
    }
}

}  // namespace endstone::detail
