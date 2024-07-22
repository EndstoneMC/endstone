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

#include "bedrock/world/scores/player_scoreboard_id.h"
#include "bedrock/world/scores/scoreboard_id.h"

class IdentityDefinition {
public:
    enum class Type : std::uint8_t {
        Invalid,
        Player,
        Entity,
        FakePlayer
    };

    [[nodiscard]] const ScoreboardId &getScoreboardId() const
    {
        return scoreboard_id_;
    }

    [[nodiscard]] bool isHiddenFakePlayer() const
    {
        return is_hidden_fake_player_;
    }

    [[nodiscard]] const PlayerScoreboardId &getPlayerId() const
    {
        return player_id_;
    }

    [[nodiscard]] const ActorUniqueID &getEntityId() const
    {
        return entity_id_;
    }

    [[nodiscard]] const std::string &getFakePlayerName() const
    {
        return player_name_;
    }

    [[nodiscard]] Type getIdentityType() const
    {
        return identity_type_;
    }

    static IdentityDefinition Invalid;

private:
    ScoreboardId scoreboard_id_;
    bool is_hidden_fake_player_;
    PlayerScoreboardId player_id_;
    ActorUniqueID entity_id_;
    std::string player_name_;
    Type identity_type_;  // +72
};

inline IdentityDefinition IdentityDefinition::Invalid;
