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

#include "endstone/util/color.h"

namespace endstone {

/**
 * @brief Base class for debug shapes rendered in world space.
 */
template <typename T>
class DebugShape {
public:
    /**
     * @brief Gets the color of this shape.
     */
    [[nodiscard]] Color getColor() const { return color_; }

    /**
     * @brief Sets the color of this shape.
     */
    T &setColor(Color color)
    {
        color_ = color;
        return *static_cast<T *>(this);
    }

    /**
     * @brief Gets the uniform scale factor.
     */
    [[nodiscard]] float getScale() const { return scale_; }

    /**
     * @brief Sets the uniform scale factor.
     */
    T &setScale(float scale)
    {
        scale_ = scale;
        return *static_cast<T *>(this);
    }

protected:
    Color color_{255, 255, 255};
    float scale_{1.0F};

};

}  // namespace endstone
