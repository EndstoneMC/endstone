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

#include <string>

#include <gsl/gsl>

#include "bedrock/network/packet/player_action_packet.h"

struct PlayerBlockActionData {
    PlayerActionType player_action_type;
    BlockPos pos;
    int facing;
};
static_assert(sizeof(PlayerBlockActionData) == 20);

class PlayerBlockActions {
public:
    void clear();
    void addStartDestroyBlock(const BlockPos &, int);
    void addAbortDestroyBlock(const BlockPos &, int);
    void addStopDestroyBlock();
    void addCrackBlock(const BlockPos &, int);
    void addPredictDestroyBlock(const BlockPos &, int);
    void addContinueDestroyBlock(const BlockPos &, int);
    [[nodiscard]] gsl::span<const PlayerBlockActionData> get() const
    {
        return actions_;
    }
    [[nodiscard]] const PlayerBlockActionData *findFirstOfType(PlayerActionType) const;
    [[nodiscard]] std::string toString() const;
    bool operator==(const PlayerBlockActions &) const;

private:
    friend class endstone::core::EndstonePlayer;
    static constexpr size_t MAX_NUM_ACTIONS = 100;
    std::vector<PlayerBlockActionData> actions_;
};
static_assert(sizeof(PlayerBlockActions) == 24);
