

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

#include "endstone/util/result.h"
#include "endstone/util/vector.h"

namespace endstone {
class Block;
class Chunk;
class Dimension;

class Location {
public:
    template <std::convertible_to<float> T>
    Location(Dimension &dimension, T x, T y, T z, const float pitch = 0.0, const float yaw = 0.0)
        : dimension_(&dimension), x_(static_cast<float>(x)), y_(static_cast<float>(y)), z_(static_cast<float>(z)),
          pitch_(pitch), yaw_(yaw)
    {
    }

    void setDimension(Dimension &dimension) { dimension_ = &dimension; }

    [[nodiscard]] Dimension &getDimension() const { return *dimension_; }

    [[nodiscard]] std::unique_ptr<Block> getBlock() const;

    template <std::convertible_to<float> T>
    constexpr void setX(T x)
    {
        x_ = static_cast<float>(x);
    }

    [[nodiscard]] constexpr float getX() const { return x_; }

    [[nodiscard]] int getBlockX() const { return static_cast<int>(std::floorf(x_)); }

    template <std::convertible_to<float> T>
    constexpr void setY(T y)
    {
        y_ = static_cast<float>(y);
    }

    [[nodiscard]] constexpr float getY() const { return y_; }

    [[nodiscard]] int getBlockY() const { return static_cast<int>(std::floorf(y_)); }

    template <std::convertible_to<float> T>
    constexpr void setZ(T z)
    {
        z_ = static_cast<float>(z);
    }

    [[nodiscard]] constexpr float getZ() const { return z_; }

    [[nodiscard]] int getBlockZ() const { return static_cast<int>(std::floorf(z_)); }

    [[nodiscard]] float getPitch() const { return pitch_; }

    void setPitch(float pitch) { pitch_ = pitch; }

    [[nodiscard]] float getYaw() const { return yaw_; }

    void setYaw(float yaw) { yaw_ = yaw; }

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

    Location &setDirection(const Vector &vector)
    {
        const auto x = vector.getX();
        const auto z = vector.getZ();
        if (x == 0 && z == 0) {
            pitch_ = vector.getY() > 0 ? -90 : 90;
            return *this;
        }

        const auto theta = std::atan2(-x, z);
        yaw_ = std::fmod(theta + 2 * std::numbers::pi, 2 * std::numbers::pi) * 180.0F / std::numbers::pi;
        const auto xz = std::sqrt((x * x) + (z * z));
        pitch_ = std::atan(-vector.getY() / xz) * 180.0F / std::numbers::pi;

        return *this;
    }

    [[nodiscard]] float length() const { return std::sqrt(lengthSquared()); }

    [[nodiscard]] constexpr float lengthSquared() const { return (x_ * x_) + (y_ * y_) + (z_ * z_); }

    [[nodiscard]] float distance(const Location &other) const { return std::sqrt(distanceSquared(other)); }

    [[nodiscard]] float distanceSquared(const Location &other) const;

    Location &operator+=(const Location &other)
    {
        x_ += other.x_;
        y_ += other.y_;
        z_ += other.z_;
        return *this;
    }

    Location &operator+=(const Vector &other)
    {
        x_ += other.getX();
        y_ += other.getY();
        z_ += other.getZ();
        return *this;
    }

    Location &operator-=(const Location &other)
    {
        x_ -= other.x_;
        y_ -= other.y_;
        z_ -= other.z_;
        return *this;
    }

    Location &operator-=(const Vector &other)
    {
        x_ -= other.getX();
        y_ -= other.getY();
        z_ -= other.getZ();
        return *this;
    }

    template <std::convertible_to<float> T>
    Location &operator*=(T scalar)
    {
        const auto s = static_cast<float>(scalar);
        x_ *= s;
        y_ *= s;
        z_ *= s;
        return *this;
    }

    constexpr Location &zero()
    {
        x_ = 0;
        y_ = 0;
        z_ = 0;
        return *this;
    }

    bool operator==(const Location &other) const noexcept
    {
        constexpr static float eps = 1e-6F;
        return dimension_ == other.dimension_ && (std::fabs(x_ - other.x_) <= eps) &&
               (std::fabs(y_ - other.y_) <= eps) && (std::fabs(z_ - other.z_) <= eps) &&
               (std::fabs(pitch_ - other.pitch_) <= eps) && (std::fabs(yaw_ - other.yaw_) <= eps);
    }

    bool operator!=(const Location &other) const noexcept { return !(*this == other); }

    operator Vector() const noexcept { return {x_, y_, z_}; }

    static float normalizeYaw(float yaw)
    {
        yaw = std::fmod(yaw, 360.0F);
        if (yaw >= 180.0F) {
            yaw -= 360.0F;
        }
        else if (yaw < -180.0F) {
            yaw += 360.0F;
        }
        return yaw;
    }

    static float normalizePitch(float pitch)
    {
        if (pitch > 90.0F) {
            pitch = 90.0F;
        }
        else if (pitch < -90.0F) {
            pitch = -90.0F;
        }
        return pitch;
    }

private:
    Dimension *dimension_;
    float x_;
    float y_;
    float z_;
    float pitch_;  // Rotation around the right axis (around X axis).
    float yaw_;    // Rotation around the up axis (around Y axis)
};
}  // namespace endstone

template <>
struct fmt::formatter<endstone::Location> : formatter<string_view> {
    template <typename FormatContext>
    auto format(const endstone::Location &self, FormatContext &ctx) const -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "Location(dimension={},x={},y={},z={},pitch={},yaw={})", self.getDimension(),
                              self.getX(), self.getY(), self.getZ(), self.getPitch(), self.getYaw());
    }
};
```


