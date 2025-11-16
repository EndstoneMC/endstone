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

#include "bedrock/world/level/block/portal_axis.h"
#include "bedrock/world/level/change_dimension_request.h"

struct PlayerChangeDimensionRequestComponent {
    enum class State : uint8_t {
        AwaitingECSTick = 0,
        ECSTransferComplete = 1,
    };
    PlayerChangeDimensionRequestComponent::State mState;
    ChangeDimensionRequest mRequest;
};

struct DimensionTransitionComponent {
    Vec3 portal_entrance_position;
    DimensionType portal_entrance_dimension;
    PortalAxis portal_entrance_axis;
    std::optional<Vec3> existing_target_position;
};
