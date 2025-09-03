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
 * @brief Represents a debug circle.
 */
class DebugCircle : public DebugShape<DebugCircle> {
public:
    /**
     * @brief Gets the scale of the circle.
     *
     * @return The scale of the circle.
     */
    [[nodiscard]] std::optional<float> getScale() const
    {
        return scale_;
    }

    /**
     * @brief Sets the scale of the circle.
     *
     * @param scale The desired scale of the circle.
     * @return A reference to the current debug shape.
     */
    DebugCircle &setScale(const std::optional<float> scale)
    {
        scale_ = scale;
        onChange();
        return *this;
    }

    /**
     * @brief Gets the number of segments used to render the circle.
     *
     * @return The number of circle segments.
     */
    [[nodiscard]] std::optional<std::uint8_t> getSegments() const
    {
        return segments_;
    }

    /**
     * @brief Sets the number of segments used to render the circle.
     *
     * @param segments The desired number of segments.
     * @return A reference to the current debug shape.
     */
    DebugCircle &setSegments(const std::optional<std::uint8_t> segments)
    {
        segments_ = segments;
        onChange();
        return *this;
    }

private:
    std::optional<float> scale_;
    std::optional<std::uint8_t> segments_;
};

}  // namespace endstone