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
 * @brief Represents a debug box.
 */
class DebugBox : public DebugShape<DebugBox> {
public:
    /**
     * @brief Gets the scale of the box.
     *
     * @return The scale of the box.
     */
    [[nodiscard]] std::optional<float> getScale() const
    {
        return scale_;
    }

    /**
     * @brief Sets the scale of the box.
     *
     * @param scale The desired scale of the box.
     * @return A reference to the current debug shape.
     */
    DebugBox &setScale(const std::optional<float> scale)
    {
        scale_ = scale;
        onChange();
        return *this;
    }

    /**
     * @brief Gets the bounds of the box.
     *
     * @return The bounds of the box.
     */
    [[nodiscard]] std::optional<Vector<float>> getBoxBounds() const
    {
        return box_bounds_;
    }

    /**
     * @brief Sets the bounds of the box.
     *
     * @param box_bounds The desired bounds of the box.
     * @return A reference to the current debug shape.
     */
    DebugBox &setBoxBounds(const std::optional<Vector<float>> box_bounds)
    {
        box_bounds_ = box_bounds;
        onChange();
        return *this;
    }

private:
    std::optional<float> scale_;
    std::optional<Vector<float>> box_bounds_;
};

}  // namespace endstone