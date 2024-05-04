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

template <typename T>
class Vector {
public:
    constexpr Vector(T x, T y, T z) : x_(x), y_(y), z_(z) {}

    constexpr T getX() const
    {
        return x_;
    }

    constexpr void setX(T x)
    {
        x_ = x;
    }

    constexpr T getY() const
    {
        return y_;
    }

    constexpr void setY(T y)
    {
        y_ = y;
    }

    constexpr T getZ() const
    {
        return z_;
    }

    constexpr void setZ(T z)
    {
        z_ = z;
    }

    constexpr Vector<T> operator+(const Vector<T> &other) const
    {
        return Vector(x_ + other.x_, y_ + other.y_, z_ + other.z_);
    }

    constexpr Vector<T> operator-(const Vector<T> &other) const
    {
        return Vector(x_ - other.x_, y_ - other.y_, z_ - other.z_);
    }

    constexpr Vector<T> operator*(const Vector<T> &other) const
    {
        return Vector(x_ * other.x_, y_ * other.y_, z_ * other.z_);
    }

    constexpr Vector<T> operator/(const Vector<T> &other) const
    {
        return Vector(x_ / other.x_, y_ / other.y_, z_ / other.z_);
    }

    [[nodiscard]] constexpr double length() const
    {
        return std::sqrt(lengthSquared());
    }

    [[nodiscard]] constexpr double lengthSquared() const
    {
        return std::pow(x_, 2) + std::pow(y_, 2) + std::pow(z_, 2);
    }

    [[nodiscard]] constexpr double distance(const Vector<T> &other) const
    {
        return std::sqrt(distanceSquared(other));
    }

    [[nodiscard]] constexpr double distanceSquared(const Vector<T> &other) const
    {
        return std::pow(x_ - other.x_, 2) + std::pow(y_ - other.y_, 2) + std::pow(z_ - other.z_, 2);
    }

private:
    T x_;
    T y_;
    T z_;
};
