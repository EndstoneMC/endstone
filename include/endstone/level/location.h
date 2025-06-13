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

#include "endstone/level/position.h"
#include "endstone/util/vector.h"

namespace endstone {

/**
 * @brief Represents a 3-dimensional location in a dimension within a level.
 */
class Location : public Position {
public:
    Location(Dimension *dimension, int x, int y, int z, float pitch = 0.0, float yaw = 0.0)
        : Location(dimension, static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), pitch, yaw)
    {
    }

    Location(Dimension *dimension, float x, float y, float z, float pitch = 0.0, float yaw = 0.0)
        : Position(dimension, x, y, z), pitch_(pitch), yaw_(yaw)
    {
    }

    /**
     * Gets the pitch of this location, measured in degrees.
     *
     * @return the incline's pitch
     */
    [[nodiscard]] float getPitch() const
    {
        return pitch_;
    }

    /**
     * Sets the pitch of this location, measured in degrees.
     * <ul>
     * <li>A pitch of 0 represents level forward facing.
     * <li>A pitch of 90 represents downward facing, or negative y
     *     direction.
     * <li>A pitch of -90 represents upward facing, or positive y direction.
     * </ul>
     * Increasing pitch values the equivalent of looking down.
     *
     * @param pitch new incline's pitch
     */
    void setPitch(float pitch)
    {
        pitch_ = pitch;
    }

    /**
     * Gets the yaw of this location, measured in degrees.
     *
     * @return the rotation's yaw
     */
    [[nodiscard]] float getYaw() const
    {
        return yaw_;
    }

    /**
     * Sets the yaw of this location, measured in degrees.
     * <ul>
     * <li>A yaw of 0 or 360 represents the positive z direction.
     * <li>A yaw of 180 represents the negative z direction.
     * <li>A yaw of 90 represents the negative x direction.
     * <li>A yaw of 270 represents the positive x direction.
     * </ul>
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

private:
    float pitch_;  // Rotation around the right axis (around X axis).
    float yaw_;    // Rotation around the up axis (around Y axis)
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
