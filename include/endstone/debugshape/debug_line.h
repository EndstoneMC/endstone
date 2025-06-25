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

class DebugLine : public DebugShape<DebugLine> {
public:
    /**
     * @brief Gets the end position of the line.
     *
     * @return The end position of the line.
     */
    [[nodiscard]] std::optional<Vector> getEndPosition() const
    {
        return end_position_;
    }

    /**
     * @brief Sets the end position of the line.
     *
     * @param end_position The desired end position of the line
     * @return A reference to the current debug shape.
     */
    DebugLine &setEndPosition(std::optional<Vector> end_position)
    {
        end_position_ = std::move(end_position);
        return *this;
    }

private:
    std::optional<Vector> end_position_;
};

}  // namespace endstone