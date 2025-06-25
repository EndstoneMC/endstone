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

#include "endstone/debugshape/debug_shape.h"

namespace endstone {

/**
 * @brief Represents a debug arrow.
 */
class DebugArrow : public DebugShape<DebugArrow> {
public:
    /**
     * @brief Gets the end position of the arrow.
     *
     * @return The end position of the arrow.
     */
    [[nodiscard]] std::optional<Vector<float>> getEndPosition() const
    {
        return end_position_;
    }

    /**
     * @brief Sets the end position of the arrow.
     *
     * @param end_position The desired end position of the arrow
     * @return A reference to the current debug shape.
     */
    DebugArrow &setEndPosition(const std::optional<Vector<float>> end_position)
    {
        end_position_ = end_position;
        return *this;
    }

    /**
     * @brief Gets the length of the arrowhead.
     *
     * @return The arrowhead length.
     */
    [[nodiscard]] std::optional<float> getArrowHeadLength() const
    {
        return arrow_head_length_;
    }

    /**
     * @brief Sets the length of the arrowhead.
     *
     * @param arrow_head_length The desired length of the arrowhead.
     * @return A reference to the current debug shape.
     */
    DebugArrow &setArrowHeadLength(const std::optional<float> arrow_head_length)
    {
        arrow_head_length_ = arrow_head_length;
        return *this;
    }

    /**
     * @brief Gets the radius of the arrowhead.
     *
     * @return The arrowhead radius.
     */
    [[nodiscard]] std::optional<float> getArrowHeadRadius() const
    {
        return arrow_head_radius_;
    }

    /**
     * @brief Sets the radius of the arrowhead.
     *
     * @param arrow_head_radius The desired radius of the arrowhead.
     * @return A reference to the current debug shape.
     */
    DebugArrow &setArrowHeadRadius(const std::optional<float> arrow_head_radius)
    {
        arrow_head_radius_ = arrow_head_radius;
        return *this;
    }

    /**
     * @brief Gets the number of segments used to render the arrowhead.
     *
     * @return The number of arrowhead segments.
     */
    [[nodiscard]] std::optional<int> getArrowHeadSegments() const
    {
        return arrow_head_segments_;
    }

    /**
     * @brief Sets the number of segments used to render the arrowhead.
     *
     * @param arrow_head_segments The desired number of segments.
     * @return A reference to the current debug shape.
     */
    DebugArrow &setArrowHeadSegments(const std::optional<int> arrow_head_segments)
    {
        arrow_head_segments_ = arrow_head_segments;
        return *this;
    }

private:
    std::optional<Vector<float>> end_position_;
    std::optional<float> arrow_head_length_;
    std::optional<float> arrow_head_radius_;
    std::optional<int> arrow_head_segments_;
};

}  // namespace endstone