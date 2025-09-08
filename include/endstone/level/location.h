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

#include <cmath>
#include <numbers>

#include "endstone/util/vector.h"

namespace endstone {

class Dimension;

/**
 * @brief Represents a 3-dimensional location in a dimension within a level.
 */
class Location : public Vector {
public:
    template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, float>>>
    Location(T x, T y, T z, const float pitch = 0.0, const float yaw = 0.0)
        : Vector(x, y, z), dimension_(nullptr), pitch_(pitch), yaw_(yaw)
    {
    }

    template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, float>>>
    Location(T x, T y, T z, Dimension &dimension) : Location(x, y, z, 0.0, dimension)
    {
    }

    template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, float>>>
    Location(T x, T y, T z, const float pitch, Dimension &dimension) : Location(x, y, z, pitch, 0.0, dimension)
    {
    }

    template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, float>>>
    Location(T x, T y, T z, const float pitch, const float yaw, Dimension &dimension)
        : Vector(x, y, z), dimension_(&dimension), pitch_(pitch), yaw_(yaw)
    {
    }

    /**
     * @brief Gets the pitch of this location, measured in degrees.
     *
     * @return the incline's pitch
     */
    [[nodiscard]] float getPitch() const
    {
        return pitch_;
    }

    /**
     * @brief Sets the pitch of this location, measured in degrees.
     *
     * - A pitch of 0 represents level forward facing.
     * - A pitch of 90 represents downward facing, or negative y
     *     direction.
     * - A pitch of -90 represents upward facing, or positive y direction.
     *
     * Increasing pitch values the equivalent of looking down.
     *
     * @param pitch new incline's pitch
     */
    void setPitch(float pitch)
    {
        pitch_ = pitch;
    }

    /**
     * @brief Gets the yaw of this location, measured in degrees.
     *
     * @return the rotation's yaw
     */
    [[nodiscard]] float getYaw() const
    {
        return yaw_;
    }

    /**
     * @brief Sets the yaw of this location, measured in degrees.
     *
     * - A yaw of 0 or 360 represents the positive z direction.
     * - A yaw of 180 represents the negative z direction.
     * - A yaw of 90 represents the negative x direction.
     * - A yaw of 270 represents the positive x direction.
     *
     * Increasing yaw values are the equivalent of turning to your
     * right-facing, increasing the scale of the next respective axis, and
     * decreasing the scale of the previous axis.
     *
     * @param yaw new rotation's yaw
     */
    void setYaw(float yaw)
    {
        yaw_ = yaw;
    }

    /**
     * @brief Gets the dimension that this position resides in
     *
     * @return Dimension that contains this position, or nullptr if the dimension is not set.
     */
    [[nodiscard]] Dimension *getDimension() const
    {
        return dimension_;
    }

    /**
     * @brief Sets the dimension that this position resides in
     *
     * @param dimension New dimension that this position resides in
     */
    void setDimension(Dimension &dimension)
    {
        dimension_ = &dimension;
    }

    /**
     * @brief Gets the floored value of the X component, indicating the block that this location is contained with.
     *
     * @return block X
     */
    [[nodiscard]] int getBlockX() const
    {
        return static_cast<int>(std::floorf(x_));
    }

    /**
     * @brief Gets the floored value of the Y component, indicating the block that this location is contained with.
     *
     * @return block Y
     */
    [[nodiscard]] int getBlockY() const
    {
        return static_cast<int>(std::floorf(y_));
    }

    /**
     * @brief Gets the floored value of the Z component, indicating the block that this location is contained with.
     *
     * @return block Z
     */
    [[nodiscard]] int getBlockZ() const
    {
        return static_cast<int>(std::floorf(z_));
    }

    /**
     * @brief Gets a unit-vector pointing in the direction that this Location is facing.
     *
     * @return a vector pointing the direction of this location's pitch and yaw
     */
    [[nodiscard]] Vector getDirection() const
    {
        Vector vector;
        const auto rot_x = getYaw() * std::numbers::pi / 180.0F;
        const auto rot_y = getPitch() * std::numbers::pi / 180.0F;
        vector.setY(-std::sin(rot_y));
        const double xz = std::cos(rot_y);
        vector.setX(-xz * std::sin(rot_x));
        vector.setZ(xz * std::cos(rot_x));
        return vector;
    }

private:
    float pitch_;  // Rotation around the right axis (around X axis).
    float yaw_;    // Rotation around the up axis (around Y axis)
    Dimension *dimension_;
};
}  // namespace endstone

template <>
struct fmt::formatter<endstone::Location> : formatter<string_view> {
    template <typename FormatContext>
    auto format(const endstone::Location &self, FormatContext &ctx) const -> format_context::iterator
    {
        auto out = ctx.out();
        if (self.getDimension()) {
            out = fmt::format_to(out, "Location(dimension={},", *self.getDimension());
        }
        else {
            out = fmt::format_to(out, "Location(dimension=null,");
        }
        return fmt::format_to(out, "x={},y={},z={},pitch={},yaw={})", self.getX(), self.getY(), self.getZ(),
                              self.getPitch(), self.getYaw());
    }
};
