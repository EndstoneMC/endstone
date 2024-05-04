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

#include "endstone/level/dimension.h"
#include "endstone/level/level.h"
#include "endstone/level/position.h"
#include "endstone/util/vector.h"

namespace endstone {

class Location : Position {
public:
    float getPitch() const
    {
        return pitch_;
    }

    void setPitch(float pitch)
    {
        pitch_ = pitch;
    }

    float getYaw() const
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
