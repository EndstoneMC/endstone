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

#include "endstone/detail/ban/ip_ban_list.h"

#include <fstream>
#include <utility>

#include <boost/algorithm/string.hpp>
#include <boost/uuid/string_generator.hpp>
#include <date/date.h>
#include <fmt/format.h>
#include <fmt/std.h>
#include <nlohmann/json.hpp>

#include "endstone/detail/util/error.h"
#include "endstone/util/result.h"

namespace endstone::detail {

EndstoneIpBanList::EndstoneIpBanList(fs::path file) : file_(std::move(file)) {}

const IpBanEntry *EndstoneIpBanList::getBanEntry(std::string address) const
{
    const auto it =
        std::find_if(entries_.begin(), entries_.end(), [&](const IpBanEntry &entry) { return match(entry, address); });

    if (it != entries_.end()) {
        return &(*it);
    }
    return nullptr;
}

IpBanEntry *EndstoneIpBanList::getBanEntry(std::string address)
{
    const auto it =
        std::find_if(entries_.begin(), entries_.end(), [&](IpBanEntry &entry) { return match(entry, address); });

    if (it != entries_.end()) {
        return &(*it);
    }
    return nullptr;
}

IpBanEntry &EndstoneIpBanList::addBan(std::string address, std::optional<std::string> reason,
                                      std::optional<BanEntry::Date> expires, std::optional<std::string> source)
{
    entries_.erase(
        std::remove_if(entries_.begin(), entries_.end(), [&](IpBanEntry &entry) { return match(entry, address); }),
        entries_.end());

    IpBanEntry new_entry{address};
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

IpBanEntry &EndstoneIpBanList::addBan(std::string address, std::optional<std::string> reason,
                                      std::chrono::seconds duration, std::optional<std::string> source)
{
    return addBan(address, reason, std::chrono::system_clock::now() + duration, source);
}

std::vector<const IpBanEntry *> EndstoneIpBanList::getEntries() const
{
    std::vector<const IpBanEntry *> entries;
    entries.reserve(entries_.size());
    for (const auto &entry : entries_) {
        entries.push_back(&entry);
    }
    return entries;
}

std::vector<IpBanEntry *> EndstoneIpBanList::getEntries()
{
    std::vector<IpBanEntry *> entries;
    entries.reserve(entries_.size());
    for (auto &entry : entries_) {
        entries.push_back(&entry);
    }
    return entries;
}

bool EndstoneIpBanList::isBanned(std::string address) const
{
    const_cast<EndstoneIpBanList *>(this)->removeExpired();
    return getBanEntry(address) != nullptr;
}

void EndstoneIpBanList::removeBan(std::string address)
{
    const auto it =
        std::find_if(entries_.begin(), entries_.end(), [&](IpBanEntry &entry) { return match(entry, address); });
    if (it != entries_.end()) {
        entries_.erase(it);
        save();
    }
}

bool EndstoneIpBanList::match(const IpBanEntry &entry, const std::string &address)
{
    return entry.getAddress() == address;
}

Result<void> EndstoneIpBanList::save()
{
    nlohmann::json array = nlohmann::json::array();
    for (const auto &entry : entries_) {
        nlohmann::json json;
        json["ip"] = entry.getAddress();
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

Result<void> EndstoneIpBanList::load()
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
            std::string address = json["ip"];
            IpBanEntry entry{address};
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

void EndstoneIpBanList::removeExpired()
{
    auto it = entries_.begin();
    while (it != entries_.end()) {
        if (it->getExpiration().has_value() && it->getExpiration().value() < std::chrono::system_clock::now()) {
            it = entries_.erase(it);
        }
        else {
            ++it;
        }
    }
}

}  // namespace endstone::detail
