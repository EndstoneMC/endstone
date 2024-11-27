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

#include <chrono>
#include <optional>
#include <string>
#include <utility>

namespace endstone {
/**
 * @brief A single entry from a ban list.
 */
class BanEntry {

public:
    using Date = std::chrono::system_clock::time_point;

    /**
     * @brief Gets the date this ban entry was created.
     *
     * @return the creation date
     */
    [[nodiscard]] Date getCreated() const
    {
        return created_;
    }

    /**
     * @brief Sets the date this ban entry was created.
     *
     * @param created the new created date
     * @see save() saving changes
     */
    void setCreated(Date created)
    {
        created_ = created;
    }

    /**
     * @brief Gets the source of this ban.
     *
     * @return the source of the ban
     */
    [[nodiscard]] std::string_view getSource()
    {
        return source_;
    }

    /**
     * @brief Sets the source of this ban.
     *
     * @param source the new source
     * @see save() saving changes
     */
    void setSource(std::string_view source)
    {
        source_ = !source.empty() ? source : "(Unknown)";
    }

    /**
     * @brief Gets the date this ban expires on, or std::nullopt for no defined end date.
     *
     * @return the expiration date
     */
    [[nodiscard]] std::optional<Date> getExpiration() const
    {
        return expiration_;
    }

    /**
     * @brief Sets the date this ban expires on. std::nullopt values are considered "infinite" bans.
     *
     * @param expiration the new expiration date, or std::nullopt to indicate an infinite ban
     */
    void setExpiration(std::optional<Date> expiration)
    {
        expiration_ = expiration;
    }

    /**
     * @brief Gets the reason for this ban.
     *
     * @return the ban reason
     */
    [[nodiscard]] std::string_view getReason() const
    {
        return reason_;
    }

    /**
     * @brief Sets the reason for this ban.
     *
     * @param reason the new reason, empty values assume the implementation default
     * @see save() saving changes
     */
    void setReason(std::string_view reason)
    {
        reason_ = !reason.empty() ? reason : "Banned by an operator.";
    }

private:
    Date created_;
    std::string source_;
    std::optional<Date> expiration_;
    std::string reason_;
};

}  // namespace endstone
