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

#include "bedrock/bedrock.h"

class Vec3 {
public:
    float x;
    float y;
    float z;

    constexpr Vec3 operator+(const Vec3 &other) const
    {
        return {x + other.x, y + other.y, z + other.z};
    }

    constexpr Vec3 operator-(const Vec3 &other) const
    {
        return {x - other.x, y - other.y, z - other.z};
    }

    constexpr Vec3 operator*(const Vec3 &other) const
    {
        return {x * other.x, y * other.y, z * other.z};
    }

    constexpr Vec3 operator/(const Vec3 &other) const
    {
        return {x / other.x, y / other.y, z / other.z};
    }

    constexpr Vec3 &operator+=(const Vec3 &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    [[nodiscard]] float lengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    static const Vec3 ZERO;
};
BEDROCK_STATIC_ASSERT_SIZE(Vec3, 12, 12);

inline const Vec3 Vec3::ZERO{0, 0, 0};
