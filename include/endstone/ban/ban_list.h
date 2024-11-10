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

#include <vector>
#include <string>
#include <optional>
#include <chrono>

namespace endstone {

/**
 * @brief A class that represents a ban list, containing bans of some target type.
 *
 * @tparam T The ban target type
 */
template <typename T>
class BanList {
public:
    virtual ~BanList() = default;

    /**
     * @brief Gets a BanEntry by the target.
     *
     * @param target The entry parameter to search for
     * @return BanEntry<T>* The corresponding entry, or nullptr if none found
     */
    virtual BanEntry<T>* getBanEntry(T target) const = 0;

    /**
     * @brief Adds a ban to this list. If a previous ban exists, this will update the previous entry.
     *
     * @param target The target of the ban
     * @param reason The reason for the ban, std::nullopt indicates implementation default
     * @param expires The date for the ban's expiration (unban), or std::nullopt to imply forever
     * @param source The source of the ban, std::nullopt indicates implementation default
     * @return BanEntry<T>& The entry for the newly created ban, or the entry for the (updated) previous ban
     */
    virtual BanEntry<T>& addBan(T target,
                                std::optional<std::string> reason,
                                std::optional<std::chrono::system_clock::time_point> expires,
                                std::optional<std::string> source) = 0;

    /**
     * @brief Adds a ban to this list. If a previous ban exists, this will update the previous entry.
     *
     * @param target The target of the ban
     * @param reason The reason for the ban, std::nullopt indicates implementation default
     * @param duration The duration of the ban, or std::nullopt to imply forever
     * @param source The source of the ban, std::nullopt indicates implementation default
     * @return BanEntry<T>& The entry for the newly created ban, or the entry for the (updated) previous ban
     */
    virtual BanEntry<T>& addBan(T target,
                                std::optional<std::string> reason,
                                std::optional<std::chrono::seconds> duration,
                                std::optional<std::string> source) = 0;

    /**
     * @brief Gets a vector containing pointers to every BanEntry in this list.
     *
     * @return std::vector<BanEntry<T>*> A vector containing pointers to every entry tracked by this list
     */
    virtual std::vector<BanEntry<T>*> getEntries() const = 0;

    /**
     * @brief Checks if a BanEntry exists for the target, indicating an active ban status.
     *
     * @param target The target to find
     * @return true If a BanEntry exists for the target, indicating an active ban status
     * @return false Otherwise
     */
    virtual bool isBanned(T target) const = 0;

    /**
     * @brief Removes the specified target from this list, therefore indicating a "not banned" status.
     *
     * @param target The target to remove from this list
     */
    virtual void removeBan(T target) = 0;
};

}  // namespace endstone

