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

#include <memory>
#include <optional>
#include <string>

#include "endstone/util/uuid.h"

namespace endstone {
/**
 * @brief A player profile.
 */
class PlayerProfile : public std::enable_shared_from_this<PlayerProfile> {
public:
    virtual ~PlayerProfile() = default;

    /**
     * @brief Gets the player's unique id.
     *
     * @return the player's unique id, or std::nullopt if not available
     */
    [[nodiscard]] virtual std::optional<UUID> getUniqueId() const = 0;

    /**
     * @brief Gets the player name.
     *
     * @return the player name, or std::nullopt if not available
     */
    [[nodiscard]] virtual std::optional<std::string> getName() const = 0;

    /**
     * @brief Gets the player's xbox user id (xuid).
     *
     * @return the player's xbox user id (xuid), or std::nullopt if not available
     */
    [[nodiscard]] virtual std::optional<std::string> getXuid() const = 0;

    /**
     * @brief Checks whether this profile is complete.
     *
     * A profile is currently considered complete if it has a name and a unique id.
     *
     * @return true if this profile is complete
     */
    [[nodiscard]] virtual bool isComplete() const = 0;
};
}  // namespace endstone
