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

class BlockPos {
public:
    static const BlockPos ZERO;
    static const BlockPos MIN;

    explicit BlockPos(int v) : x(v), y(v), z(v) {}
    BlockPos() = default;
    BlockPos(int x, int y, int z) : x(x), y(y), z(z){};
    BlockPos(const Vec3 &vec)
        : x(static_cast<int>(std::floorf(vec.x))), y(static_cast<int>(std::floorf(vec.y))),
          z(static_cast<int>(std::floorf(vec.z))){};
    BlockPos(double x, double y, double z) : x(std::floor(x)), y(std::floor(y)), z(std::floor(z)){};

    bool operator==(const BlockPos &rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

    bool operator!=(const BlockPos &rhs) const
    {
        return !(*this == rhs);
    }

    int x;
    int y;
    int z;
};

inline const BlockPos BlockPos::ZERO(0);
inline const BlockPos BlockPos::MIN(std::numeric_limits<int>::min());

namespace std {
template <>
struct hash<BlockPos> {
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
}  // namespace std

namespace fmt {
template <>
struct formatter<BlockPos> : formatter<string_view> {
    using Type = BlockPos;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return format_to(ctx.out(), "BlockPos(x={}, y={}, z={})", val.x, val.y, val.z);
    }
};
}  // namespace fmt
