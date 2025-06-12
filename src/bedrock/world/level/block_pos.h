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

#include <fmt/format.h>

#include "bedrock/core/math/vec3.h"
#include "bedrock/util/mirror.h"
#include "bedrock/util/rotation.h"

class BlockPos {
public:
    static const BlockPos MIN;
    static const BlockPos ZERO;

    explicit constexpr BlockPos(int v) : x(v), y(v), z(v) {}
    constexpr BlockPos() = default;
    constexpr BlockPos(int x, int y, int z) : x(x), y(y), z(z){};
    BlockPos(const Vec3 &vec)
        : x(static_cast<int>(std::floorf(vec.x))), y(static_cast<int>(std::floorf(vec.y))),
          z(static_cast<int>(std::floorf(vec.z))){};
    BlockPos(double x, double y, double z) : x(std::floor(x)), y(std::floor(y)), z(std::floor(z)){};

    BlockPos operator*(int scalar) const
    {
        return {x * scalar, y * scalar, z * scalar};
    }

    const BlockPos &operator*=(int scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    BlockPos operator/(int divisor) const
    {
        return {x / divisor, y / divisor, z / divisor};
    }

    BlockPos operator+(const BlockPos &rhs) const
    {
        return {x + rhs.x, y + rhs.y, z + rhs.z};
    }

    BlockPos operator-(const BlockPos &rhs) const
    {
        return {x - rhs.x, y - rhs.y, z - rhs.z};
    }

    BlockPos operator+(int delta) const
    {
        return {x + delta, y + delta, z + delta};
    }

    BlockPos operator-(int delta) const
    {
        return {x - delta, y - delta, z - delta};
    }

    const BlockPos &operator+=(const Vec3 &vec)
    {
        x += static_cast<int>(std::floor(vec.x));
        y += static_cast<int>(std::floor(vec.y));
        z += static_cast<int>(std::floor(vec.z));
        return *this;
    }

    const BlockPos &operator+=(const BlockPos &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    const BlockPos &operator-=(const BlockPos &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    BlockPos operator-() const
    {
        return {-x, -y, -z};
    }

    bool operator==(const BlockPos &rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

    bool operator!=(const BlockPos &rhs) const
    {
        return !(*this == rhs);
    }

    [[nodiscard]] BlockPos offset(int dx, int dy, int dz) const
    {
        return {x + dx, y + dy, z + dz};
    }

    [[nodiscard]] BlockPos offset(const BlockPos &other) const
    {
        return {x + other.x, y + other.y, z + other.z};
    }

    [[nodiscard]] BlockPos above() const
    {
        return offset(0, 1, 0);
    }

    [[nodiscard]] BlockPos above(int n) const
    {
        return offset(0, n, 0);
    }

    [[nodiscard]] BlockPos below() const
    {
        return offset(0, -1, 0);
    }

    [[nodiscard]] BlockPos below(int n) const
    {
        return offset(0, -n, 0);
    }

    [[nodiscard]] BlockPos transform(Rotation rotation, Mirror mirror, const Vec3 &pivot) const
    {
        BlockPos out = *this;

        switch (mirror) {
        case Mirror::None:
            break;
        case Mirror::X:
            out.z = 2.0F * pivot.z - out.z;
            break;
        case Mirror::Z:
            out.x = 2.0F * pivot.x - out.x;
            break;
        case Mirror::XZ:
            out.z = 2.0F * pivot.z - out.z;
            out.x = 2.0F * pivot.x - out.x;
            break;
        }

        // 2) Rotation step about pivot in the X–Z plane
        float dx = out.x - pivot.x;
        float dz = out.z - pivot.z;
        float rx, rz;

        switch (rotation) {
        case Rotation::Rotate90:
            rx = dz;
            rz = -dx;
            break;
        case Rotation::Rotate180:
            rx = -dx;
            rz = -dz;
            break;
        case Rotation::Rotate270:
            rx = -dz;
            rz = dx;
            break;
        case Rotation::None:
        default:
            rx = dx;
            rz = dz;
            break;
        }

        out.x = pivot.x + rx;
        out.z = pivot.z + rz;

        return out;
    }

    int x;
    int y;
    int z;
};

inline const BlockPos BlockPos::ZERO(0);
inline const BlockPos BlockPos::MIN(std::numeric_limits<int>::min());

template <>
struct std::hash<BlockPos> {
    std::size_t operator()(const BlockPos &pos) const noexcept
    {
        static std::hash<int> hasher;
        std::size_t seed = 0;
        seed ^= hasher(pos.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hasher(pos.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hasher(pos.z) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

template <>
struct fmt::formatter<BlockPos> : formatter<string_view> {
    using Type = BlockPos;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return format_to(ctx.out(), "BlockPos(x={}, y={}, z={})", val.x, val.y, val.z);
    }
};
