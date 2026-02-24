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

#include <algorithm>
#include <cmath>
#include <concepts>

#include <fmt/format.h>

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
    template <std::convertible_to<float> T>
    constexpr Vector(T x, T y, T z) : x_(static_cast<float>(x)), y_(static_cast<float>(y)), z_(static_cast<float>(z))
    {
    }

    Vector(const Vector &other) = default;
    Vector(Vector &&other) noexcept = default;
    Vector &operator=(const Vector &other) = default;
    Vector &operator=(Vector &&other) noexcept = default;

    /**
     * @brief Gets the X component.
     *
     * @return The X component.
     */
    [[nodiscard]] constexpr float getX() const { return x_; }

    /**
     * @brief Set the X component.
     *
     * @param x The new X component.
     * @return This vector.
     */
    template <std::convertible_to<float> T>
    constexpr Vector &setX(T x)
    {
        x_ = static_cast<float>(x);
        return *this;
    }

    /**
     * @brief Gets the Y component.
     *
     * @return The Y component.
     */
    [[nodiscard]] constexpr float getY() const { return y_; }

    /**
     * @brief Set the Y component.
     *
     * @param y The new Y component.
     * @return This vector.
     */
    template <std::convertible_to<float> T>
    constexpr Vector &setY(T y)
    {
        y_ = static_cast<float>(y);
        return *this;
    }

    /**
     * @brief Gets the Z component.
     *
     * @return The Z component.
     */
    [[nodiscard]] constexpr float getZ() const { return z_; }

    /**
     * @brief Set the Z component.
     *
     * @param z The new Z component.
     * @return This vector.
     */
    template <std::convertible_to<float> T>
    constexpr Vector &setZ(T z)
    {
        z_ = static_cast<float>(z);
        return *this;
    }

    /**
     * @brief Gets the floored value of the X component, indicating the block that this vector is contained with.
     *
     * @return block X
     */
    [[nodiscard]] int getBlockX() const { return static_cast<int>(std::floor(x_)); }

    /**
     * @brief Gets the floored value of the Y component, indicating the block that this vector is contained with.
     *
     * @return block y
     */
    [[nodiscard]] int getBlockY() const { return static_cast<int>(std::floor(y_)); }

    /**
     * @brief Gets the floored value of the Z component, indicating the block that this vector is contained with.
     *
     * @return block z
     */
    [[nodiscard]] int getBlockZ() const { return static_cast<int>(std::floor(z_)); }

    constexpr Vector operator+(const Vector &other) const { return {x_ + other.x_, y_ + other.y_, z_ + other.z_}; }

    constexpr Vector operator-(const Vector &other) const { return {x_ - other.x_, y_ - other.y_, z_ - other.z_}; }

    constexpr Vector operator*(const Vector &other) const { return {x_ * other.x_, y_ * other.y_, z_ * other.z_}; }

    constexpr Vector operator/(const Vector &other) const { return {x_ / other.x_, y_ / other.y_, z_ / other.z_}; }

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

    template <std::convertible_to<float> T>
    constexpr Vector operator+(T scalar) const
    {
        const auto s = static_cast<float>(scalar);
        return {x_ + s, y_ + s, z_ + s};
    }

    template <std::convertible_to<float> T>
    constexpr Vector operator-(T scalar) const
    {
        const auto s = static_cast<float>(scalar);
        return {x_ - s, y_ - s, z_ - s};
    }

    template <std::convertible_to<float> T>
    constexpr Vector operator*(T scalar) const
    {
        const auto s = static_cast<float>(scalar);
        return {x_ * s, y_ * s, z_ * s};
    }

    template <std::convertible_to<float> T>
    constexpr Vector operator/(T scalar) const
    {
        const auto s = static_cast<float>(scalar);
        return {x_ / s, y_ / s, z_ / s};
    }

    template <typename T>
        requires std::convertible_to<T, float>
    friend Vector operator+(T scalar, const Vector &v)
    {
        const auto s = static_cast<float>(scalar);
        return {s + v.x_, s + v.y_, s + v.z_};
    }

    template <typename T>
        requires std::convertible_to<T, float>
    friend Vector operator-(T scalar, const Vector &v)
    {
        const auto s = static_cast<float>(scalar);
        return {s - v.x_, s - v.y_, s - v.z_};
    }

    template <typename T>
        requires std::convertible_to<T, float>
    friend Vector operator*(T scalar, const Vector &v)
    {
        float s = static_cast<float>(scalar);
        return {s * v.x_, s * v.y_, s * v.z_};
    }

    template <typename T>
        requires std::convertible_to<T, float>
    friend Vector operator/(T scalar, const Vector &v)
    {
        float s = static_cast<float>(scalar);
        return {s / v.x_, s / v.y_, s / v.z_};
    }

    bool operator==(const Vector &other) const noexcept
    {
        constexpr static float eps = 1e-6f;
        return (std::fabs(x_ - other.x_) <= eps) && (std::fabs(y_ - other.y_) <= eps) &&
               (std::fabs(z_ - other.z_) <= eps);
    }

    bool operator!=(const Vector &other) const noexcept { return !(*this == other); }

    /**
     * @brief Gets the magnitude of the vector, defined as sqrt(x^2+y^2+z^2).
     *
     * @return the magnitude
     */
    [[nodiscard]] float length() const { return std::sqrt(lengthSquared()); }

    /**
     * @brief Gets the magnitude of the vector squared.
     *
     * @return the magnitude
     */
    [[nodiscard]] constexpr float lengthSquared() const { return (x_ * x_) + (y_ * y_) + (z_ * z_); }

    /**
     * @brief Get the distance between this vector and another.
     *
     * @param other The other vector
     * @return the distance
     */
    [[nodiscard]] float distance(const Vector &other) const { return std::sqrt(distanceSquared(other)); }

    /**
     * @brief Get the squared distance between this vector and another.
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
     * @brief Gets the angle between this vector and another in radians.
     *
     * @param other The other vector
     * @return angle in radians
     */
    [[nodiscard]] float angle(const Vector &other) const
    {
        const auto dot_product = std::clamp(dot(other) / (length() * other.length()), -1.0f, 1.0f);
        return std::acos(dot_product);
    }

    /**
     * @brief Sets this vector to the midpoint between this vector and another.
     *
     * @param other The other vector
     * @return this same vector (now a midpoint)
     */
    constexpr Vector &midpoint(const Vector &other)
    {
        x_ = (x_ + other.x_) / 2;
        y_ = (y_ + other.y_) / 2;
        z_ = (z_ + other.z_) / 2;
        return *this;
    }

    /**
     * @brief Gets a new midpoint vector between this vector and another.
     *
     * @param other The other vector
     * @return a new midpoint vector
     */
    [[nodiscard]] constexpr Vector getMidpoint(const Vector &other) const
    {
        auto x = (x_ + other.x_) / 2;
        auto y = (y_ + other.y_) / 2;
        auto z = (z_ + other.z_) / 2;
        return {x, y, z};
    }

    /**
     * @brief Calculates the dot product of this vector with another. The dot product
     * is defined as x1*x2+y1*y2+z1*z2. The returned value is a scalar.
     *
     * @param other The other vector
     * @return dot product
     */
    [[nodiscard]] constexpr float dot(const Vector &other) const
    {
        return (x_ * other.x_) + (y_ * other.y_) + (z_ * other.z_);
    }

    /**
     * @brief Calculates the cross-product of this vector with another.
     *
     * The cross-product is defined as:
     * - x = y1 * z2 - y2 * z1
     * - y = z1 * x2 - z2 * x1
     * - z = x1 * y2 - x2 * y1
     *
     * @param other The other vector
     * @return the same vector
     */
    constexpr Vector &crossProduct(const Vector &other)
    {
        const auto new_x = (y_ * other.z_) - (other.y_ * z_);
        const auto new_y = (z_ * other.x_) - (other.z_ * x_);
        const auto new_z = (x_ * other.y_) - (other.x_ * y_);
        x_ = new_x;
        y_ = new_y;
        z_ = new_z;
        return *this;
    }

    /**
     * @brief Calculates the cross-product of this vector with another without mutating the original.
     *
     * The cross-product is defined as:
     * - x = y1 * z2 - y2 * z1
     * - y = z1 * x2 - z2 * x1
     * - z = x1 * y2 - x2 * y1
     *
     * @param other The other vector
     * @return a new vector
     */
    [[nodiscard]] constexpr Vector getCrossProduct(const Vector &other) const
    {
        float x = (y_ * other.z_) - (other.y_ * z_);
        float y = (z_ * other.x_) - (other.z_ * x_);
        float z = (x_ * other.y_) - (other.x_ * y_);
        return {x, y, z};
    }

    /**
     * @brief Converts this vector to a unit vector (a vector with length of 1).
     *
     * @return the same vector
     */
    Vector &normalize()
    {
        const auto len = length();
        x_ /= len;
        y_ /= len;
        z_ /= len;
        return *this;
    }

    /**
     * @brief Zero this vector's components.
     *
     * @return the same vector
     */
    constexpr Vector &zero()
    {
        x_ = 0;
        y_ = 0;
        z_ = 0;
        return *this;
    }

    /**
     * @brief Check whether or not each component of this vector is equal to 0.
     *
     * @return true if equal to zero, false if at least one component is non-zero
     */
    [[nodiscard]] constexpr bool isZero() const { return x_ == 0 && y_ == 0 && z_ == 0; }

    /**
     * @brief Converts each component of value <code>-0.0</code> to <code>0.0</code>.
     *
     * @return This vector.
     */
    constexpr Vector &normalizeZeros()
    {
        if (x_ == -0.0F) {
            x_ = 0.0F;
        }
        if (y_ == -0.0F) {
            y_ = 0.0F;
        }
        if (z_ == -0.0F) {
            z_ = 0.0F;
        }
        return *this;
    }

    /**
     * @brief Returns whether this vector is in an axis-aligned bounding box.
     *
     * The minimum and maximum vectors given must be truly the minimum and maximum X, Y and Z components.
     *
     * @param min Minimum vector
     * @param max Maximum vector
     *
     * @return whether this vector is in the AABB
     */
    [[nodiscard]] constexpr bool isInAABB(const Vector &min, const Vector &max) const
    {
        return x_ >= min.x_ && x_ <= max.x_ && y_ >= min.y_ && y_ <= max.y_ && z_ >= min.z_ && z_ <= max.z_;
    }

    /**
     * @brief Returns whether this vector is within a sphere.
     *
     * @param origin Sphere origin.
     * @param radius Sphere radius
     *
     * @return whether this vector is in the sphere
     */
    [[nodiscard]] constexpr bool isInSphere(const Vector &origin, float radius) const
    {
        return ((origin.x_ - x_) * (origin.x_ - x_) + (origin.y_ - y_) * (origin.y_ - y_) +
                (origin.z_ - z_) * (origin.z_ - z_)) <= radius * radius;
    }

    /**
     * @brief Returns if a vector is normalized
     *
     * @return whether the vector is normalized
     */
    [[nodiscard]] bool isNormalized() const
    {
        constexpr static float eps = 1e-6f;
        return std::abs(lengthSquared() - 1) < eps;
    }

    /**
     * @brief Rotates the vector around the x-axis.
     *
     * @param angle the angle to rotate the vector about. This angle is passed in radians
     *
     * @return the same vector
     */
    Vector &rotateAroundX(float angle)
    {
        const auto angle_cos = std::cos(angle);
        const auto angle_sin = std::sin(angle);
        const auto y = (angle_cos * getY()) - (angle_sin * getZ());
        const auto z = (angle_sin * getY()) + (angle_cos * getZ());
        return setY(y).setZ(z);
    }

    /**
     * @brief Rotates the vector around the y-axis.
     *
     * @param angle the angle to rotate the vector about. This angle is passed in radians
     *
     * @return the same vector
     */
    Vector &rotateAroundY(float angle)
    {
        const auto angle_cos = std::cos(angle);
        const auto angle_sin = std::sin(angle);
        const auto x = (angle_cos * getX()) + (angle_sin * getZ());
        const auto z = (-angle_sin * getX()) + (angle_cos * getZ());
        return setX(x).setZ(z);
    }

    /**
     * @brief Rotates the vector around the z-axis
     *
     * @param angle the angle to rotate the vector about. This angle is passed in radians
     *
     * @return the same vector
     */
    Vector &rotateAroundZ(float angle)
    {
        const auto angle_cos = std::cos(angle);
        const auto angle_sin = std::sin(angle);
        const auto x = (angle_cos * getX()) - (angle_sin * getY());
        const auto y = (angle_sin * getX()) + (angle_cos * getY());
        return setX(x).setY(y);
    }

    /**
     * @brief Rotates the vector around a given arbitrary axis in 3-dimensional space.
     *
     * Rotation will follow the general Right-Hand-Rule, which means rotation
     * will be counterclockwise when the axis is pointing towards the observer.
     *
     * This method will always make sure the provided axis is a unit vector, to
     * not modify the length of the vector when rotating. If you are experienced
     * with the scaling of a non-unit axis vector, you can use
     * <code>rotateAroundNonUnitAxis(Vector, float)</code>.
     *
     * @param axis the axis to rotate the vector around. If the passed vector is
     * not of length 1, it gets normalized before using it for the rotation.
     *
     * @param angle the angle to rotate the vector around the axis
     *
     * @return the same vector
     */
    Vector &rotateAroundAxis(const Vector &axis, float angle)
    {
        return rotateAroundNonUnitAxis(axis.isNormalized() ? axis : Vector(axis).normalize(), angle);
    }

    /**
     * @brief Rotates the vector around a given arbitrary axis in 3-dimensional space.
     *
     * Rotation will follow the general Right-Hand-Rule, which means rotation
     * will be counterclockwise when the axis is pointing towards the observer.
     *
     * Note that the vector length will change accordingly to the axis vector
     * length. If the provided axis is not a unit vector, the rotated vector
     * will not have its previous length. The scaled length of the resulting
     * vector will be related to the axis vector. If you are not sure about
     * the scaling of the vector, use <code>rotateAroundAxis(Vector, float)</code>.
     *
     * @param axis the axis to rotate the vector around.
     * @param angle the angle to rotate the vector around the axis
     *
     * @return the same vector
     */
    Vector &rotateAroundNonUnitAxis(const Vector &axis, float angle)
    {
        const auto x = getX(), y = getY(), z = getZ();
        const auto x2 = axis.getX(), y2 = axis.getY(), z2 = axis.getZ();

        const auto cos_theta = std::cos(angle);
        const auto sin_theta = std::sin(angle);

        const auto dot_product = dot(axis);
        const auto x_prime =
            (x2 * dot_product * (1.0F - cos_theta)) + (x * cos_theta) + ((-z2 * y + y2 * z) * sin_theta);
        const auto y_prime =
            (y2 * dot_product * (1.0F - cos_theta)) + (y * cos_theta) + ((z2 * x - x2 * z) * sin_theta);
        const auto z_prime =
            (z2 * dot_product * (1.0F - cos_theta)) + (z * cos_theta) + ((-y2 * x + x2 * y) * sin_theta);

        return setX(x_prime).setY(y_prime).setZ(z_prime);
    }

protected:
    float x_ = 0.0;
    float y_ = 0.0;
    float z_ = 0.0;
};
}  // namespace endstone

template <>
struct fmt::formatter<endstone::Vector> : formatter<string_view> {
    template <typename FormatContext>
    auto format(const endstone::Vector &self, FormatContext &ctx) const -> format_context::iterator
    {
        auto out = ctx.out();
        return fmt::format_to(out, "{},{},{},", self.getX(), self.getY(), self.getZ());
    }
};
