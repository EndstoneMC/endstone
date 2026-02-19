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

#include <cstdint>
#include <string>
#include <utility>

#include "endstone/check.h"

namespace endstone {
/**
 * @brief Represents a cursor on a map.
 */
class MapCursor {
public:
    /**
     * @brief Represents the standard types of map cursors.
     */
    enum class Type {
        Player = 0,
        Frame = 1,
        RedMarker = 2,
        BlueMarker = 3,
        TargetX = 4,
        TargetPoint = 5,
        PlayerOffMap = 6,
        SignMarker = 7,
        PinkMarker = 8,
        OrangeMarker = 9,
        YellowMarker = 10,
        CyanMarker = 11,
        GreenPoint = 12,
        PlayerOffLimits = 13,
        Mansion = 14,
        Monument = 15,
        // PlayerHidden = 16,
        VillageDesert = 17,
        VillagePlains = 18,
        VillageSavanna = 19,
        VillageSnowy = 20,
        VillageTaiga = 21,
        JungleTemple = 22,
        SwampHut = 23,
        TrialChambers = 24,
    };

    /**
     * @brief Initialize the map cursor.
     *
     * @param x The x coordinate, from -128 to 127.
     * @param y The y coordinate, from -128 to 127.
     * @param direction The facing of the cursor, from 0 to 15.
     * @param type The type (color/style) of the map cursor.
     * @param visible Whether the cursor is visible by default.
     * @param caption cursor caption
     */
    MapCursor(std::int8_t x, std::int8_t y, std::int8_t direction, Type type, bool visible, std::string caption = "")
        : x_(x), y_(y), direction_(direction), visible_(visible), caption_(std::move(caption)), type_(type)
    {
    }

    /**
     * @brief Get the X position of this cursor.
     *
     * @return The X coordinate.
     */
    [[nodiscard]] std::int8_t getX() const { return x_; }

    /**
     * @brief Get the Y position of this cursor.
     *
     * @return The Y coordinate.
     */
    [[nodiscard]] std::int8_t getY() const { return y_; }

    /**
     * @brief Get the direction of this cursor.
     *
     * @return The facing of the cursor, from 0 to 15.
     */
    [[nodiscard]] std::int8_t getDirection() const { return direction_; }

    /**
     * @brief Get the type of this cursor.
     *
     * @return The type (color/style) of the map cursor.
     */
    [[nodiscard]] Type getType() const { return type_; }

    /**
     * @brief Get the visibility status of this cursor.
     *
     * @return True if visible, false otherwise.
     */
    [[nodiscard]] bool isVisible() const { return visible_; }

    /**
     * @brief Set the X position of this cursor.
     *
     * @param x The X coordinate.
     */
    void setX(const std::int8_t x) { x_ = x; }

    /**
     * @brief Set the Y position of this cursor.
     *
     * @param y The Y coordinate.
     */
    void setY(const std::int8_t y) { y_ = y; }

    /**
     * @brief Set the direction of this cursor.
     *
     * @param direction The facing of the cursor, from 0 to 15.
     */
    void setDirection(std::int8_t direction)
    {
        Preconditions::checkArgument(direction >= 0 && direction <= 15, "direction must be between 0 and 15 but is {}",
                                     direction);
        direction_ = direction;
    }

    /**
     * @brief Set the type of this cursor.
     *
     * @param type The type (color/style) of the map cursor.
     */
    void setType(Type type) { type_ = type; }

    /**
     * Set the visibility status of this cursor.
     *
     * @param visible True if visible.
     */
    void setVisible(bool visible) { visible_ = visible; }

    /**
     * @brief Gets the caption on this cursor.
     *
     * @return caption
     */

    [[nodiscard]] std::string getCaption() const { return caption_; }

    /**
     * @brief Sets the caption on this cursor.
     *
     * @param caption new caption
     */
    void setCaption(std::string caption) { caption_ = std::move(caption); }

private:
    std::int8_t x_, y_;
    std::int8_t direction_;
    bool visible_;
    std::string caption_;
    Type type_;
};
}  // namespace endstone
