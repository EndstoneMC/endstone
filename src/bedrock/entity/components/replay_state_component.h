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

#include "bedrock/entity/components/replay_state_policy.h"
#include "bedrock/world/actor/actor_history.h"

class ReplayStateComponent {
public:
    ReplayStateComponent(std::unique_ptr<ActorHistory>, std::unique_ptr<IReplayStatePolicy>);

    MovementCorrection shouldSendCorrectionToClient(EntityContext &entity, const PlayerAuthInputPacket &packet,
                                                    bool is_strict_movement) const
    {
        return policy->shouldCorrectMovement(entity, packet, current_tick, divergence_counter, is_strict_movement);
    }

    [[nodiscard]] std::uint64_t getCurrentTick() const
    {
        return current_tick;
    }

    [[nodiscard]] std::uint8_t getDivergenceCounter() const
    {
        return divergence_counter;
    }

    void clearHistory()
    {
        if (history) {
            history->clearFrames();
        }
    }

    void setDivergenceCounter(std::uint8_t counter)
    {
        divergence_counter = counter;
    }

    bool force_correction;
    bool should_report_next_correction_telemetry;
    bool is_correction_telemetry_enabled;
    std::uint8_t divergence_counter;
    std::unique_ptr<ActorHistory> history;
    std::unique_ptr<IReplayStatePolicy> policy;
    std::uint64_t current_tick;
};
