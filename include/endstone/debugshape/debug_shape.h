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

#include <optional>
#include <atomic>

#include "endstone/util/color.h"
#include "endstone/util/vector.h"

namespace endstone {

namespace debugshape_internal {
    inline std::atomic<int64_t> id_counter{0};
}  // namespace debugshape_internal

/**
 * @brief Represents a generic debug shape.
 *
 * The DebugShape class provides a base abstraction for different types of debug shapes.
 * It supports six types of debug shapes: Arrow, Box, Circle, Line, Sphere and Text.
 * Each debug shape has a id, position and color.
 */
template <typename T>
class DebugShape {
public:
    using DebugShapeId = int64_t;
    explicit DebugShape()
    {
        id_ = --debugshape_internal::id_counter;
    }

    /**
     * @brief Gets the id of the debug shape.
     *
     * @return The id of the debug shape.
     */
    [[nodiscard]] DebugShapeId getId() const
    {
        return id_;
    }

    /**
     * @brief Gets the position of the debug shape.
     *
     * @return The position of the debug shape.
     */
    [[nodiscard]] std::optional<Vector<float>> getPosition() const
    {
        return position_;
    }

    /**
     * @brief Sets the position of the debug shape.
     *
     * @param position The position to set for the debug shape.
     * @return A reference to the current debug shape.
     */
    T &setPosition(const std::optional<Vector<float>> position)
    {
        position_ = position;
        return *static_cast<T *>(this);
    }

    /**
     * @brief Gets the color of the debug shape.
     *
     * @return The color of the debug shape.
     */
    [[nodiscard]] std::optional<Color> getColor() const
    {
        return color_;
    }

    /**
     * @brief Sets the color of the debug shape.
     *
     * @param color The color to set for the debug shape.
     * @return A reference to the current debug shape.
     */
    T &setColor(const std::optional<Color> color)
    {
        color_ = color;
        return *static_cast<T *>(this);
    }

protected:
    DebugShapeId id_;
    std::optional<Vector<float>> position_;
    std::optional<Color> color_;
};

}  // namespace endstone