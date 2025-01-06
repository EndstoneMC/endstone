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
#include <fstream>
#include <sstream>
#include <utility>

#include <date/date.h>
#include <fmt/format.h>
#include <fmt/std.h>
#include <nlohmann/json.hpp>

#include "endstone/ban/ip_ban_list.h"
#include "endstone/core/util/error.h"
#include "endstone/util/result.h"

namespace fs = std::filesystem;

namespace endstone::core {

template <typename T, typename Matcher>
class EndstoneBanList : public BanList<T> {
public:
    explicit EndstoneBanList(fs::path file) : file_(std::move(file)){};

    ~EndstoneBanList() override = default;

    [[nodiscard]] const T *getBanEntry(std::string target) const override
    {
        const auto it =
            std::find_if(entries_.begin(), entries_.end(), [&](const T &entry) { return matcher_(entry, target); });

        if (it != entries_.end()) {
            return &(*it);
        }
        return nullptr;
    }

    [[nodiscard]] T *getBanEntry(std::string target) override
    {
        const auto it =
            std::find_if(entries_.begin(), entries_.end(), [&](T &entry) { return matcher_(entry, target); });

        if (it != entries_.end()) {
            return &(*it);
        }
        return nullptr;
    }

    T &addBan(std::string target, std::optional<std::string> reason, std::optional<BanEntry::Date> expires,
              std::optional<std::string> source) override
    {
        entries_.erase(
            std::remove_if(entries_.begin(), entries_.end(), [&](T &entry) { return matcher_(entry, target); }),
            entries_.end());

        T new_entry{target};
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

    T &addBan(std::string target, std::optional<std::string> reason, std::chrono::seconds duration,
              std::optional<std::string> source) override
    {
        return addBan(target, reason, std::chrono::system_clock::now() + duration, source);
    }

    [[nodiscard]] std::vector<const T *> getEntries() const override
    {
        std::vector<const T *> entries;
        entries.reserve(entries_.size());
        for (auto &entry : entries_) {
            entries.push_back(&entry);
        }
        return entries;
    }

    [[nodiscard]] std::vector<T *> getEntries() override
    {
        std::vector<T *> entries;
        entries.reserve(entries_.size());
        for (auto &entry : entries_) {
            entries.push_back(&entry);
        }
        return entries;
    }

    [[nodiscard]] bool isBanned(std::string target) const override
    {
        const_cast<EndstoneBanList *>(this)->removeExpired();
        return getBanEntry(target) != nullptr;
    }

    void removeBan(std::string target) override
    {
        const auto it =
            std::find_if(entries_.begin(), entries_.end(), [&](T &entry) { return matcher_(entry, target); });
        if (it != entries_.end()) {
            entries_.erase(it);
            save();
        }
    }

    Result<void> save()
    {
        nlohmann::json array = nlohmann::json::array();
        for (const auto &entry : entries_) {
            nlohmann::json json = entry;
            json["created"] = date::format(BanEntry::DateFormat, date::floor<std::chrono::seconds>(entry.getCreated()));
            json["source"] = entry.getSource();
            if (entry.getExpiration().has_value()) {
                json["expires"] = date::format(BanEntry::DateFormat,
                                               date::floor<std::chrono::seconds>(entry.getExpiration().value()));
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

    Result<void> load()
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
                auto entry = json.get<T>();
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

protected:
    void removeExpired()
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

    std::vector<T> entries_;
    fs::path file_;
    Matcher matcher_;
};

}  // namespace endstone::core
