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

#include <optional>
#include <string>
#include <utility>

#include "endstone/ban/ban_entry.h"
#include "endstone/util/uuid.h"

namespace endstone {

/**
 * @brief Represents an entry for a banned player.
 */
class PlayerBanEntry : public BanEntry {
public:
    explicit PlayerBanEntry(std::string name, std::optional<UUID> uuid = std::nullopt,
                            std::optional<std::string> xuid = std::nullopt)
        : name_(std::move(name)), uuid_(uuid), xuid_(std::move(xuid))
    {
    }

    /**
     * @brief Gets the banned player's name.
     *
     * @return the player name
     */
    [[nodiscard]] std::string getName() const { return name_; }

    /**
     * @brief Gets the banned player's unique id.
     *
     * @return the player's unique id, or std::nullopt if not available
     */
    [[nodiscard]] std::optional<UUID> getUniqueId() const { return uuid_; }

    /**
     * @brief Gets the banned player's xbox user id (xuid).
     *
     * @return the player's xbox user id (xuid), or std::nullopt if not available
     */
    [[nodiscard]] std::optional<std::string> getXuid() const { return xuid_; }

private:
    std::string name_;
    std::optional<UUID> uuid_;
    std::optional<std::string> xuid_;
};

}  // namespace endstone
