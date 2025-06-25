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
 * @brief Represents a debug sphere.
 */
class DebugSphere : public DebugShape<DebugSphere> {
public:
    /**
     * @brief Gets the scale of the sphere.
     *
     * @return The scale of the sphere.
     */
    [[nodiscard]] std::optional<float> getScale() const
    {
        return scale_;
    }

    /**
     * @brief Sets the scale of the sphere.
     *
     * @param scale The desired scale of the sphere.
     * @return A reference to the current debug shape.
     */
    DebugSphere &setScale(std::optional<float> scale)
    {
        scale_ = scale;
        return *this;
    }

    /**
     * @brief Gets the number of segments used to render the sphere.
     *
     * @return The number of sphere segments.
     */
    [[nodiscard]] std::optional<int> getSegments() const
    {
        return segments_;
    }

    /**
     * @brief Sets the number of segments used to render the sphere.
     *
     * @param segments The desired number of segments.
     * @return A reference to the current debug shape.
     */
    DebugSphere &setSegments(const std::optional<int> segments)
    {
        segments_ = segments;
        return *this;
    }

private:
    std::optional<float> scale_;
    std::optional<int> segments_;
};

}  // namespace endstone