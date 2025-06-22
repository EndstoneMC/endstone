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

#include "bedrock/entity/gamerefs_entity/entity_context.h"
#include "bedrock/forward.h"
#include "bedrock/network/packet/player_auth_input_packet.h"

enum class CorrectionMethod : uint8_t {
    None = 0,
    AcceptClient = 1,
    CorrectClient = 2,
};

struct MovementCorrection {
    MovementCorrection(CorrectionMethod method, const Vec3 &accept_position)
        : method(method), accept_position(accept_position){};
    CorrectionMethod method = CorrectionMethod::None;
    Vec3 accept_position = Vec3::ZERO;
    uint8_t new_divergence_counter = 0;
};
static_assert(sizeof(MovementCorrection) == 20);

struct IReplayStatePolicy {
    virtual ~IReplayStatePolicy() = 0;
    [[nodiscard]] virtual bool isReplayNeeded(AdvanceFrameResult) const = 0;
    [[nodiscard]] virtual MovementCorrection shouldCorrectMovement(EntityContext &, const PlayerAuthInputPacket &,
                                                                   uint64_t, uint8_t, bool) = 0;
    virtual void notifyOfExternalCorrection(uint64_t) = 0;
};
