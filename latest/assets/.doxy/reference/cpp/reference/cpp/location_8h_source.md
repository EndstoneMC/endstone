

# File location.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**level**](dir_8e239ca1e5fd0d936d66a30330d3a329.md) **>** [**location.h**](location_8h.md)

[Go to the documentation of this file](location_8h.md)


```C++
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

    [[nodiscard]] float getPitch() const
    {
        return pitch_;
    }

    void setPitch(float pitch)
    {
        pitch_ = pitch;
    }

    [[nodiscard]] float getYaw() const
    {
        return yaw_;
    }

    void setYaw(float yaw)
    {
        yaw_ = yaw;
    }

    [[nodiscard]] Dimension *getDimension() const
    {
        return dimension_;
    }

    void setDimension(Dimension &dimension)
    {
        dimension_ = &dimension;
    }

    [[nodiscard]] int getBlockX() const
    {
        return static_cast<int>(std::floorf(x_));
    }

    [[nodiscard]] int getBlockY() const
    {
        return static_cast<int>(std::floorf(y_));
    }

    [[nodiscard]] int getBlockZ() const
    {
        return static_cast<int>(std::floorf(z_));
    }

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
```


