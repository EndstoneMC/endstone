

# File location.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**level**](dir_8e239ca1e5fd0d936d66a30330d3a329.md) **>** [**location.h**](location_8h.md)

[Go to the documentation of this file](location_8h.md)


```C++
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

#include <memory>

#include "endstone/level/position.h"
#include "endstone/util/vector.h"

namespace endstone {

class Location : public Position {
public:
    Location(Dimension *dimension, int x, int y, int z, float pitch = 0.0, float yaw = 0.0)
        : Location(dimension, static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), pitch, yaw)
    {
    }

    Location(Dimension *dimension, float x, float y, float z, float pitch = 0.0, float yaw = 0.0)
        : Position(dimension, x, y, z), pitch_(pitch), yaw_(yaw)
    {
    }

    [[nodiscard]] float getPitch() const
    {
        return pitch_;
    }

    void setPitch(float pitch)
    {
        pitch_ = pitch;
    }

    [[nodiscard]] float getYaw() const
    {
        return yaw_;
    }

    void setYaw(float yaw)
    {
        yaw_ = yaw;
    }

private:
    float pitch_;  // Rotation around the right axis (around X axis).
    float yaw_;    // Rotation around the up axis (around Y axis)
};

}  // namespace endstone
```


