

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

#include <algorithm>
#include <cmath>

#include <fmt/format.h>

namespace endstone {

class Vector {
public:
    constexpr Vector() = default;

    template <std::convertible_to<float> T>
    constexpr Vector(T x, T y, T z) : x_(static_cast<float>(x)), y_(static_cast<float>(y)), z_(static_cast<float>(z))
    {
    }

    Vector(const Vector &other) = default;
    Vector(Vector &&other) noexcept = default;
    Vector &operator=(const Vector &other) = default;
    Vector &operator=(Vector &&other) noexcept = default;

    [[nodiscard]] constexpr float getX() const { return x_; }

    template <std::convertible_to<float> T>
    constexpr Vector &setX(T x)
    {
        x_ = static_cast<float>(x);
        return *this;
    }

    [[nodiscard]] constexpr float getY() const { return y_; }

    template <std::convertible_to<float> T>
    constexpr Vector &setY(T y)
    {
        y_ = static_cast<float>(y);
        return *this;
    }

    [[nodiscard]] constexpr float getZ() const { return z_; }

    template <std::convertible_to<float> T>
    constexpr Vector &setZ(T z)
    {
        z_ = static_cast<float>(z);
        return *this;
    }

    [[nodiscard]] int getBlockX() const { return static_cast<int>(std::floor(x_)); }

    [[nodiscard]] int getBlockY() const { return static_cast<int>(std::floor(y_)); }

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

    [[nodiscard]] float length() const { return std::sqrt(lengthSquared()); }

    [[nodiscard]] constexpr float lengthSquared() const { return (x_ * x_) + (y_ * y_) + (z_ * z_); }

    [[nodiscard]] float distance(const Vector &other) const { return std::sqrt(distanceSquared(other)); }

    [[nodiscard]] constexpr float distanceSquared(const Vector &other) const
    {
        return ((x_ - other.x_) * (x_ - other.x_)) + ((y_ - other.y_) * (y_ - other.y_)) +
               ((z_ - other.z_) * (z_ - other.z_));
    }

    [[nodiscard]] float angle(const Vector &other) const
    {
        const auto dot_product = std::clamp(dot(other) / (length() * other.length()), -1.0f, 1.0f);
        return std::acos(dot_product);
    }

    constexpr Vector &midpoint(const Vector &other)
    {
        x_ = (x_ + other.x_) / 2;
        y_ = (y_ + other.y_) / 2;
        z_ = (z_ + other.z_) / 2;
        return *this;
    }

    [[nodiscard]] constexpr Vector getMidpoint(const Vector &other) const
    {
        auto x = (x_ + other.x_) / 2;
        auto y = (y_ + other.y_) / 2;
        auto z = (z_ + other.z_) / 2;
        return {x, y, z};
    }

    [[nodiscard]] constexpr float dot(const Vector &other) const
    {
        return (x_ * other.x_) + (y_ * other.y_) + (z_ * other.z_);
    }

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

    [[nodiscard]] constexpr Vector getCrossProduct(const Vector &other) const
    {
        float x = (y_ * other.z_) - (other.y_ * z_);
        float y = (z_ * other.x_) - (other.z_ * x_);
        float z = (x_ * other.y_) - (other.x_ * y_);
        return {x, y, z};
    }

    Vector &normalize()
    {
        const auto len = length();
        x_ /= len;
        y_ /= len;
        z_ /= len;
        return *this;
    }

    constexpr Vector &zero()
    {
        x_ = 0;
        y_ = 0;
        z_ = 0;
        return *this;
    }

    [[nodiscard]] constexpr bool isZero() const { return x_ == 0 && y_ == 0 && z_ == 0; }

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

    [[nodiscard]] constexpr bool isInAABB(const Vector &min, const Vector &max) const
    {
        return x_ >= min.x_ && x_ <= max.x_ && y_ >= min.y_ && y_ <= max.y_ && z_ >= min.z_ && z_ <= max.z_;
    }

    [[nodiscard]] constexpr bool isInSphere(const Vector &origin, float radius) const
    {
        return ((origin.x_ - x_) * (origin.x_ - x_) + (origin.y_ - y_) * (origin.y_ - y_) +
                (origin.z_ - z_) * (origin.z_ - z_)) <= radius * radius;
    }

    [[nodiscard]] bool isNormalized() const
    {
        constexpr static float eps = 1e-6f;
        return std::abs(lengthSquared() - 1) < eps;
    }

    Vector &rotateAroundX(float angle)
    {
        const auto angle_cos = std::cos(angle);
        const auto angle_sin = std::sin(angle);
        const auto y = (angle_cos * getY()) - (angle_sin * getZ());
        const auto z = (angle_sin * getY()) + (angle_cos * getZ());
        return setY(y).setZ(z);
    }

    Vector &rotateAroundY(float angle)
    {
        const auto angle_cos = std::cos(angle);
        const auto angle_sin = std::sin(angle);
        const auto x = (angle_cos * getX()) + (angle_sin * getZ());
        const auto z = (-angle_sin * getX()) + (angle_cos * getZ());
        return setX(x).setZ(z);
    }

    Vector &rotateAroundZ(float angle)
    {
        const auto angle_cos = std::cos(angle);
        const auto angle_sin = std::sin(angle);
        const auto x = (angle_cos * getX()) - (angle_sin * getY());
        const auto y = (angle_sin * getX()) + (angle_cos * getY());
        return setX(x).setY(y);
    }

    Vector &rotateAroundAxis(const Vector &axis, float angle)
    {
        return rotateAroundNonUnitAxis(axis.isNormalized() ? axis : Vector(axis).normalize(), angle);
    }

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
```


