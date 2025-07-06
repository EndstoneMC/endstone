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

#include "bedrock/entity/gamerefs_entity/gamerefs_entity.h"
#include "bedrock/entity/utilities/movement_correction_interface.h"
#include "bedrock/forward.h"
#include "bedrock/gamerefs/owner_ptr.h"

struct IReplayableActorInput {
    virtual ~IReplayableActorInput() = 0;
};

class ActorHistory {
public:
    struct Snapshot {
        OwnerPtr<EntityContext> snapshot_state;
        std::vector<std::shared_ptr<IMovementCorrection>> corrections;
        std::unique_ptr<IReplayableActorInput> captured_input;
        bool is_correction;
    };

    void clearFrames()
    {
        oldest_frame_ = 0;
        snapshots_.clear();
        queued_correction_.clear();
    }

private:
    const size_t history_window_;
    std::deque<Snapshot> snapshots_;
    std::uint64_t oldest_frame_ = 0;
    CorrectionTypeBitset pending_corrections_;
    std::vector<std::shared_ptr<IMovementCorrection>> queued_correction_;
    std::unique_ptr<IReplayableActorInput> next_input_;
};
