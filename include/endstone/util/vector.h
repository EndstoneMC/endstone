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

/**
 * @brief Represents a 3-dimensional vector.
 */
class Vector {
public:
    /**
     * @brief Construct the vector with all components as 0.
     */
    constexpr Vector() = default;

    /**
     * @brief Construct the vector with provided components.
     *
     * @param x X component
     * @param y Y component
     * @param z Z component
     */
    template <typename T>
    constexpr Vector(T x, T y, T z)
        requires(std::is_convertible_v<T, float>)
        : x_(static_cast<float>(x)), y_(static_cast<float>(y)), z_(static_cast<float>(z))
    {
    }

    /**
     * @brief Gets the X component.
     *
     * @return The X component.
     */
    [[nodiscard]] constexpr float getX() const
    {
        return x_;
    }

    /**
     * @brief Set the X component.
     *
     * @param x The new X component.
     * @return This vector.
     */
    constexpr void setX(float x)
    {
        x_ = x;
    }

    /**
     * @brief Gets the Y component.
     *
     * @return The Y component.
     */
    [[nodiscard]] constexpr float getY() const
    {
        return y_;
    }

    /**
     * @brief Set the Y component.
     *
     * @param y The new Y component.
     * @return This vector.
     */
    constexpr void setY(float y)
    {
        y_ = y;
    }

    /**
     * @brief Gets the Z component.
     *
     * @return The Z component.
     */
    [[nodiscard]] constexpr float getZ() const
    {
        return z_;
    }

    /**
     * @brief Set the Z component.
     *
     * @param z The new Z component.
     * @return This vector.
     */
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

    /**
     * @brief Gets the magnitude of the vector, defined as sqrt(x^2+y^2+z^2).
     *
     * @return the magnitude
     */
    [[nodiscard]] float length() const
    {
        return std::sqrt(lengthSquared());
    }

    /**
     * @brief Gets the magnitude of the vector squared.
     *
     * @return the magnitude
     */
    [[nodiscard]] constexpr float lengthSquared() const
    {
        return (x_ * x_) + (y_ * y_) + (z_ * z_);
    }

    /**
     * Get the distance between this vector and another.
     *
     * @param other The other vector
     * @return the distance
     */
    [[nodiscard]] float distance(const Vector &other) const
    {
        return std::sqrt(distanceSquared(other));
    }

    /**
     * Get the squared distance between this vector and another.
     *
     * @param other The other vector
     * @return the distance
     */
    [[nodiscard]] constexpr float distanceSquared(const Vector &other) const
    {
        return ((x_ - other.x_) * (x_ - other.x_)) + ((y_ - other.y_) * (y_ - other.y_)) +
               ((z_ - other.z_) * (z_ - other.z_));
    }

    /**
     * Converts this vector to a unit vector (a vector with length of 1).
     *
     * @return the same vector
     */
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
