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
#include <vector>

#include "endstone/boss/bar_color.h"
#include "endstone/boss/bar_flag.h"
#include "endstone/boss/bar_style.h"
#include "endstone/util/result.h"

namespace endstone {

class Player;

/**
 * @brief Represents a boss bar that is displayed to players.
 **/
class BossBar {
public:
    virtual ~BossBar() = default;

    /**
     * @brief Returns the title of this boss bar
     * @return the title of the bar
     */
    [[nodiscard]] virtual std::string getTitle() const = 0;

    /**
     * @brief Sets the title of this boss bar
     * @param title the title of the bar
     */
    virtual void setTitle(std::string title) = 0;

    /**
     * @brief Returns the color of this boss bar
     * @return the color of the bar
     */
    [[nodiscard]] virtual BarColor getColor() const = 0;

    /**
     * @brief Sets the color of this boss bar
     * @param color the color of the bar
     */
    virtual void setColor(BarColor color) = 0;

    /**
     * @brief Returns the style of this boss bar.
     * @return the style of the bar.
     */
    [[nodiscard]] virtual BarStyle getStyle() const = 0;

    /**
     * @brief Sets the bar style of this boss bar.
     * @param style the style of the bar.
     */
    virtual void setStyle(BarStyle style) = 0;

    /**
     * @brief Returns whether this boss bar has the passed flag set.
     * @param flag the flag to check.
     * @return whether it has the flag.
     */
    [[nodiscard]] virtual bool hasFlag(BarFlag flag) const = 0;

    /**
     * @brief Add an optional flag to this boss bar.
     * @param flag an optional flag to set on the boss bar.
     */
    virtual void addFlag(BarFlag flag) = 0;

    /**
     * @brief Remove an existing flag on this boss bar.
     * @param flag the existing flag to remove.
     */
    virtual void removeFlag(BarFlag flag) = 0;

    /**
     * @brief Returns the progress of the bar between 0.0 and 1.0
     * @return the progress of the bar
     */
    [[nodiscard]] virtual float getProgress() const = 0;

    /**
     * @brief Sets the progress of the bar. Values should be between 0.0 (empty) and 1.0 (full)
     * @param progress the progress of the bar
     */
    virtual Result<void> setProgress(float progress) = 0;

    /**
     * @brief Return if the boss bar is displayed to attached players
     * @return visible status
     */
    [[nodiscard]] virtual bool isVisible() const = 0;

    /**
     * @brief Set if the boss bar is displayed to attached players
     * @param visible visible status
     */
    virtual void setVisible(bool visible) = 0;

    /**
     * @brief Adds the player to this boss bar causing it to display on their screen
     * @param player the player to add
     */
    virtual void addPlayer(Player &player) = 0;

    /**
     * @brief Removes the player from this boss bar causing it to be removed from their screen
     * @param player the player to remove
     */
    virtual void removePlayer(Player &player) = 0;

    /**
     * @brief Removes all players from this boss bar
     */
    virtual void removeAll() = 0;

    /**
     * @brief Returns all players viewing this boss bar
     * @return a list of players
     */
    [[nodiscard]] virtual std::vector<Player *> getPlayers() const = 0;
};

}  // namespace endstone
