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

#include "endstone/debug/shape/shape.h"

namespace endstone {

/**
 * @brief Represents a debug arrow shape.
 * An arrow is a line segment with a cone head at the end point.
 */
class DebugArrow : public DebugShape<DebugArrow> {
public:
    /**
     * @brief Gets the length of the arrow.
     */
    [[nodiscard]] float getLength() const { return length_; }

    /**
     * @brief Sets the length of the arrow.
     */
    DebugArrow &setLength(float length)
    {
        length_ = length;
        return *this;
    }

    /**
     * @brief Gets the length of the arrow's head.
     */
    [[nodiscard]] float getHeadLength() const { return head_length_; }

    /**
     * @brief Sets the length of the arrow's head.
     */
    DebugArrow &setHeadLength(float length)
    {
        head_length_ = length;
        return *this;
    }

    /**
     * @brief Gets the radius of the arrow's head.
     */
    [[nodiscard]] float getHeadRadius() const { return head_radius_; }

    /**
     * @brief Sets the radius of the arrow's head.
     */
    DebugArrow &setHeadRadius(float radius)
    {
        head_radius_ = radius;
        return *this;
    }

    /**
     * @brief Gets the number of segments for the arrow head's base circle.
     */
    [[nodiscard]] int getHeadSegments() const { return head_segments_; }

    /**
     * @brief Sets the number of segments for the arrow head's base circle.
     * @param segments value between 3 and 128, default is 4
     */
    DebugArrow &setHeadSegments(int segments)
    {
        head_segments_ = segments;
        return *this;
    }

private:
    float length_{1.0F};
    float head_length_{0.5F};
    float head_radius_{0.25F};
    int head_segments_{4};
};

}  // namespace endstone
