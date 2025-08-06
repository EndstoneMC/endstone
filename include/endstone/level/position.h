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

#include <memory>
#include <optional>

#include "endstone/util/vector.h"

namespace endstone {

class Level;
class Dimension;

/**
 * @brief Represents a 3-dimensional position in a dimension within a level.
 */
class Position : public Vector<float> {
public:
    template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, float>>>
    Position(T x, T y, T z)
        : Vector(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)), dimension_(nullptr)
    {
    }

    template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, float>>>
    Position(T x, T y, T z, Dimension &dimension)
        : Vector(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)), dimension_(&dimension)
    {
    }

    /**
     * Gets the dimension that this position resides in
     *
     * @return Dimension that contains this position, or nullptr if the dimension is not set.
     */
    [[nodiscard]] Dimension *getDimension() const
    {
        return dimension_;
    }

    /**
     * Sets the dimension that this position resides in
     *
     * @param dimension New dimension that this position resides in
     */
    void setDimension(Dimension &dimension)
    {
        dimension_ = &dimension;
    }

    /**
     * @brief Gets the floored value of the X component, indicating the block that this location is contained with.
     *
     * @return block X
     */
    [[nodiscard]] int getBlockX() const
    {
        return static_cast<int>(std::floorf(x_));
    }

    /**
     * @brief Gets the floored value of the Y component, indicating the block that this location is contained with.
     *
     * @return block Y
     */
    [[nodiscard]] int getBlockY() const
    {
        return static_cast<int>(std::floorf(y_));
    }

    /**
     * @brief Gets the floored value of the Z component, indicating the block that this location is contained with.
     *
     * @return block Z
     */
    [[nodiscard]] int getBlockZ() const
    {
        return static_cast<int>(std::floorf(z_));
    }

private:
    Dimension *dimension_;
};

}  // namespace endstone
