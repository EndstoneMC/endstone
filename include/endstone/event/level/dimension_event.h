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

#include "endstone/event/level/level_event.h"
#include "endstone/level/dimension.h"

namespace endstone {

/**
 * @brief Represents events within a level
 */
class DimensionEvent : public LevelEvent {
public:
    explicit DimensionEvent(Dimension &dimension) : LevelEvent(dimension.getLevel()), dimension_(dimension){};

    /**
     * Gets the dimension primarily involved with this event
     *
     * @return Dimension which caused this event
     */
    [[nodiscard]] Dimension &getDimension() const
    {
        return dimension_;
    }

private:
    Dimension &dimension_;
};

}  // namespace endstone
