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

#include <atomic>
#include <optional>
#include <unordered_set>

#include "endstone/detail/endstone.h"
#include "endstone/player.h"
#include "endstone/util/color.h"
#include "endstone/util/uuid.h"
#include "endstone/util/vector.h"

namespace endstone {

namespace debugshape_internal {
// Decreases from the maximum value to avoid conflicts with sapi's id allocation
inline std::atomic id_counter{UINT64_MAX};
}  // namespace debugshape_internal

/**
 * @brief Represents a generic debug shape.
 *
 * The DebugShape class provides a base abstraction for different types of debug shapes.
 * It supports six types of debug shapes: Arrow, Box, Circle, Line, Sphere and Text.
 * Each debug shape has an id, position and color.
 */
template <typename T>
class DebugShape {
public:
    using DebugShapeId = uint64_t;
    explicit DebugShape()
    {
        id_ = --debugshape_internal::id_counter;
    }

    /**
     * @brief Gets the id of the debug shape.
     *
     * @return The id of the debug shape.
     */
    [[nodiscard]] DebugShapeId getId() const
    {
        return id_;
    }

    /**
     * @brief Gets the position of the debug shape.
     *
     * @return The position of the debug shape.
     */
    [[nodiscard]] std::optional<Vector<float>> getPosition() const
    {
        return position_;
    }

    /**
     * @brief Sets the position of the debug shape.
     *
     * @param position The position to set for the debug shape.
     * @return A reference to the current debug shape.
     */
    T &setPosition(const std::optional<Vector<float>> position)
    {
        position_ = position;
        onChange();
        return *static_cast<T *>(this);
    }

    /**
     * @brief Gets the color of the debug shape.
     *
     * @return The color of the debug shape.
     */
    [[nodiscard]] std::optional<Color> getColor() const
    {
        return color_;
    }

    /**
     * @brief Sets the color of the debug shape.
     *
     * @param color The color to set for the debug shape.
     * @return A reference to the current debug shape.
     */
    T &setColor(const std::optional<Color> color)
    {
        color_ = color;
        onChange();
        return *static_cast<T *>(this);
    }

    /**
     * @brief Adds the player to this debug shape causing it to display on the player's screen.
     * @param player the player to add.
     */
    void addPlayer(Player &player)
    {
        players_.emplace(player.getUniqueId());
        player.sendDebugShape(static_cast<const T &>(*this));
    }

    /**
     * @brief Removes the player from this debug shape causing it to be removed from the player's screen.
     * @param player the player to remove.
     */
    void removePlayer(Player &player)
    {
        players_.erase(player.getUniqueId());
        player.removeDebugShape(static_cast<const T &>(*this));
    }

    /**
     * @brief Removes all players from this debug shape.
     */
    void removeAll()
    {
        for (const auto &player : getPlayers()) {
            removePlayer(*player);
        }
    }

    /**
     * @brief Returns all players viewing this debug shape.
     * @return a list of players.
     */
    std::vector<Player *> getPlayers() const
    {
        std::vector<Player *> players;
        const auto &server = Endstone::getServer();
        for (auto it = players_.begin(); it != players_.end();) {
            if (auto *player = server.getPlayer(*it); player) {
                players.emplace_back(player);
                ++it;
            }
            else {
                it = players_.erase(it);
            }
        }
        return players;
    }

protected:
    DebugShapeId id_;
    std::optional<Vector<float>> position_;
    std::optional<Color> color_;
    mutable std::unordered_set<UUID> players_;

    void onChange() const
    {
        for (const auto &player : getPlayers()) {
            player->sendDebugShape(static_cast<const T &>(*this));
        }
    }
};

}  // namespace endstone
