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
template <typename T>
class Vector {
public:
    constexpr Vector() : x_(0), y_(0), z_(0) {}
    constexpr Vector(T x, T y, T z) : x_(x), y_(y), z_(z) {}

    /**
     * Gets the X component.
     *
     * @return The X component.
     */
    constexpr T getX() const
    {
        return x_;
    }

    /**
     * Set the X component.
     *
     * @param x The new X component.
     * @return This vector.
     */
    constexpr void setX(T x)
    {
        x_ = x;
    }

    /**
     * Gets the Y component.
     *
     * @return The Y component.
     */
    constexpr T getY() const
    {
        return y_;
    }

    /**
     * Set the Y component.
     *
     * @param y The new Y component.
     * @return This vector.
     */
    constexpr void setY(T y)
    {
        y_ = y;
    }

    /**
     * Gets the Z component.
     *
     * @return The Z component.
     */
    constexpr T getZ() const
    {
        return z_;
    }

    /**
     * Set the Z component.
     *
     * @param z The new Z component.
     * @return This vector.
     */
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

    Vector<T> &operator+=(const Vector<T> &other)
    {
        x_ += other.x_;
        y_ += other.y_;
        z_ += other.z_;
        return *this;
    }

    Vector<T> &operator-=(const Vector<T> &other)
    {
        x_ -= other.x_;
        y_ -= other.y_;
        z_ -= other.z_;
        return *this;
    }

    Vector<T> &operator*=(const Vector<T> &other)
    {
        x_ *= other.x_;
        y_ *= other.y_;
        z_ *= other.z_;
        return *this;
    }

    Vector<T> &operator/=(const Vector<T> &other)
    {
        x_ /= other.x_;
        y_ /= other.y_;
        z_ /= other.z_;
        return *this;
    }

    Vector<T> operator+(T scalar) const
    {
        return Vector<T>(x_ + scalar, y_ + scalar, z_ + scalar);
    }

    Vector<T> operator-(T scalar) const
    {
        return Vector<T>(x_ - scalar, y_ - scalar, z_ - scalar);
    }

    Vector<T> operator*(T scalar) const
    {
        return Vector<T>(x_ * scalar, y_ * scalar, z_ * scalar);
    }

    Vector<T> operator/(T scalar) const
    {
        return Vector<T>(x_ / scalar, y_ / scalar, z_ / scalar);
    }

    friend Vector<T> operator+(T scalar, const Vector<T> &v)
    {
        return Vector<T>(scalar + v.x_, scalar + v.y_, scalar + v.z_);
    }

    friend Vector<T> operator-(T scalar, const Vector<T> &v)
    {
        return Vector<T>(scalar - v.x_, scalar - v.y_, scalar - v.z_);
    }

    friend Vector<T> operator*(T scalar, const Vector<T> &v)
    {
        return Vector<T>(scalar * v.x_, scalar * v.y_, scalar * v.z_);
    }

    friend Vector<T> operator/(T scalar, const Vector<T> &v)
    {
        return Vector<T>(scalar / v.x_, scalar / v.y_, scalar / v.z_);
    }

    /**
     * Gets the magnitude of the vector, defined as sqrt(x^2+y^2+z^2).
     *
     * @return the magnitude
     */
    [[nodiscard]] constexpr T length() const
    {
        return std::sqrt(lengthSquared());
    }

    /**
     * Gets the magnitude of the vector squared.
     *
     * @return the magnitude
     */
    [[nodiscard]] constexpr T lengthSquared() const
    {
        return x_ * x_ + y_ * y_ + z_ * z_;
    }

    /**
     * Get the distance between this vector and another.
     *
     * @param other The other vector
     * @return the distance
     */
    [[nodiscard]] constexpr T distance(const Vector<T> &other) const
    {
        return std::sqrt(distanceSquared(other));
    }

    /**
     * Get the squared distance between this vector and another.
     *
     * @param other The other vector
     * @return the distance
     */
    [[nodiscard]] constexpr T distanceSquared(const Vector<T> &other) const
    {
        return (x_ - other.x_) * (x_ - other.x_) + (y_ - other.y_) * (y_ - other.y_) +
               (z_ - other.z_) * (z_ - other.z_);
    }

private:
    T x_;
    T y_;
    T z_;
};

}  // namespace endstone
