

# File vector.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**util**](dir_89b85071337bf933dea6c29b4c6a4410.md) **>** [**vector.h**](vector_8h.md)

[Go to the documentation of this file](vector_8h.md)


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

namespace endstone {

class Vector {
public:
    constexpr Vector() = default;

    template <typename T>
    constexpr Vector(T x, T y, T z)
        requires(std::is_convertible_v<T, float>)
        : x_(static_cast<float>(x)), y_(static_cast<float>(y)), z_(static_cast<float>(z))
    {
    }

    [[nodiscard]] constexpr float getX() const
    {
        return x_;
    }

    constexpr void setX(float x)
    {
        x_ = x;
    }

    [[nodiscard]] constexpr float getY() const
    {
        return y_;
    }

    constexpr void setY(float y)
    {
        y_ = y;
    }

    [[nodiscard]] constexpr float getZ() const
    {
        return z_;
    }

    constexpr void setZ(float z)
    {
        z_ = z;
    }

    constexpr Vector operator+(const Vector &other) const
    {
        return {x_ + other.x_, y_ + other.y_, z_ + other.z_};
    }

    constexpr Vector operator-(const Vector &other) const
    {
        return {x_ - other.x_, y_ - other.y_, z_ - other.z_};
    }

    constexpr Vector operator*(const Vector &other) const
    {
        return {x_ * other.x_, y_ * other.y_, z_ * other.z_};
    }

    constexpr Vector operator/(const Vector &other) const
    {
        return {x_ / other.x_, y_ / other.y_, z_ / other.z_};
    }

    Vector &operator+=(const Vector &other)
    {
        x_ += other.x_;
        y_ += other.y_;
        z_ += other.z_;
        return *this;
    }

    Vector &operator-=(const Vector &other)
    {
        x_ -= other.x_;
        y_ -= other.y_;
        z_ -= other.z_;
        return *this;
    }

    Vector &operator*=(const Vector &other)
    {
        x_ *= other.x_;
        y_ *= other.y_;
        z_ *= other.z_;
        return *this;
    }

    Vector &operator/=(const Vector &other)
    {
        x_ /= other.x_;
        y_ /= other.y_;
        z_ /= other.z_;
        return *this;
    }

    Vector operator+(float scalar) const
    {
        return {x_ + scalar, y_ + scalar, z_ + scalar};
    }

    Vector operator-(float scalar) const
    {
        return {x_ - scalar, y_ - scalar, z_ - scalar};
    }

    Vector operator*(float scalar) const
    {
        return {x_ * scalar, y_ * scalar, z_ * scalar};
    }

    Vector operator/(float scalar) const
    {
        return {x_ / scalar, y_ / scalar, z_ / scalar};
    }

    friend Vector operator+(float scalar, const Vector &v)
    {
        return {scalar + v.x_, scalar + v.y_, scalar + v.z_};
    }

    friend Vector operator-(float scalar, const Vector &v)
    {
        return {scalar - v.x_, scalar - v.y_, scalar - v.z_};
    }

    friend Vector operator*(float scalar, const Vector &v)
    {
        return {scalar * v.x_, scalar * v.y_, scalar * v.z_};
    }

    friend Vector operator/(float scalar, const Vector &v)
    {
        return {scalar / v.x_, scalar / v.y_, scalar / v.z_};
    }

    constexpr bool operator==(const Vector &other) const
    {
        return (x_ == other.x_) && (y_ == other.y_) && (z_ == other.z_);
    }

    [[nodiscard]] float length() const
    {
        return std::sqrt(lengthSquared());
    }

    [[nodiscard]] constexpr float lengthSquared() const
    {
        return (x_ * x_) + (y_ * y_) + (z_ * z_);
    }

    [[nodiscard]] float distance(const Vector &other) const
    {
        return std::sqrt(distanceSquared(other));
    }

    [[nodiscard]] constexpr float distanceSquared(const Vector &other) const
    {
        return ((x_ - other.x_) * (x_ - other.x_)) + ((y_ - other.y_) * (y_ - other.y_)) +
               ((z_ - other.z_) * (z_ - other.z_));
    }

    [[nodiscard]] Vector &normalize()
    {
        const auto len = length();
        x_ /= len;
        y_ /= len;
        z_ /= len;
        return *this;
    }

protected:
    float x_ = 0.0;
    float y_ = 0.0;
    float z_ = 0.0;
};

}  // namespace endstone
```


