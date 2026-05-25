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
#include "endstone/util/vector.h"

namespace endstone {

/**
 * @brief Represents a debug box (cuboid) shape.
 * Final rendered size = bound * scale.
 */
class DebugBox : public DebugShape<DebugBox> {
public:
    /**
     * @brief Gets the bounding size of the box.
     */
    [[nodiscard]] Vector getBound() const { return bound_; }

    /**
     * @brief Sets the bounding size of the box.
     */
    DebugBox &setBound(Vector bound)
    {
        bound_ = bound;
        return *this;
    }

private:
    Vector bound_{1.0F, 1.0F, 1.0F};
};

}  // namespace endstone
