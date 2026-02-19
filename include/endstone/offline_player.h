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

#include <string>

#include "endstone/util/uuid.h"

namespace endstone {

/**
 * @brief Represents a reference to a player identity and the data belonging to a player that is stored on the disk and
 * can, thus, be retrieved without the player needing to be online.
 */
class OfflinePlayer {
public:
    virtual ~OfflinePlayer() = default;

    /**
     * @brief Returns the name of this player
     *
     * @return Player name or null if we have not seen a name for this player yet
     */
    [[nodiscard]] virtual std::string getName() const = 0;

    /**
     * @brief Returns the UUID of this player
     *
     * @return Player UUID
     */
    [[nodiscard]] virtual UUID getUniqueId() const = 0;
};

}  // namespace endstone
