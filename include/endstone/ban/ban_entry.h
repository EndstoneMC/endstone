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

#pragma once

#include <string>
#include <memory>
#include <optional>
#include <chrono>

/**
 * @brief A single entry from a ban list. This may represent either a player ban or an IP ban.
 *
 * Unsaved information is not automatically written to the implementation's ban list, instead, the save() method must
 * be called to write the changes to the ban list. If this ban entry has expired (such as from an unban) and is no
 * longer found in the list, the save() call will re-add it to the list, therefore banning the victim specified.
 *
 * Likewise, changes to the associated BanList or other entries may or may not be reflected in this entry.
 *
 * @tparam T The ban target
 */
template<typename T>
class BanEntry {
public:
    // Ensure derived class has virtual destructor
    virtual ~BanEntry() = default;

    /**
     * @brief Gets the target involved.
     *
     * @return the target, e.g. profile or IP address
     */
    virtual T& getTarget() const = 0;

    /**
     * @brief Gets the date this ban entry was created.
     *
     * @return the creation date
     */
    virtual std::chrono::system_clock::time_point getCreated() const = 0;

    /**
     * @brief Sets the date this ban entry was created.
     *
     * @param created the new created date
     * @see save() saving changes
     */
    virtual void setCreated(std::chrono::system_clock::time_point created) = 0;

    /**
     * @brief Gets the source of this ban.
     *
     * @return the source of the ban
     */
    virtual std::string getSource() const = 0;

    /**
     * @brief Sets the source of this ban.
     *
     * @param source the new source
     * @see save() saving changes
     */
    virtual void setSource(std::string source) = 0;

    /**
     * @brief Gets the date this ban expires on, or std::nullopt for no defined end date.
     *
     * @return the expiration date
     */
    virtual std::optional<std::chrono::system_clock::time_point> getExpiration() const = 0;

    /**
     * @brief Sets the date this ban expires on. std::nullopt values are considered "infinite" bans.
     *
     * @param expiration the new expiration date, or std::nullopt to indicate an eternity
     * @see save() saving changes
     */
    virtual void setExpiration(std::optional<std::chrono::system_clock::time_point> expiration) = 0;

    /**
     * @brief Gets the reason for this ban.
     *
     * @return the ban reason, or std::nullopt if not set
     */
    virtual std::optional<std::string> getReason() const = 0;

    /**
     * @brief Sets the reason for this ban.
     *
     * @param reason the new reason, std::nullopt values assume the implementation default
     * @see save() saving changes
     */
    virtual void setReason(std::optional<std::string> reason) = 0;

    /**
     * @brief Saves the ban entry, overwriting any previous data in the ban list.
     *
     * Saving the ban entry of an unbanned player will cause the player to be banned once again.
     */
    virtual void save() = 0;

    /**
     * @brief Removes this ban entry from the appropriate ban list.
     */
    virtual void remove() = 0;
};