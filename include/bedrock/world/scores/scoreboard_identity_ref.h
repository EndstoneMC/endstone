// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy ofo the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "bedrock/world/scores/identity_definition.h"
#include "bedrock/world/scores/scoreboard_id.h"

class ScoreboardIdentityRef {
public:
    [[nodiscard]] int getNumReferences() const
    {
        return num_references_;
    }

    [[nodiscard]] const ScoreboardId &getScoreboardId() const
    {
        return scoreboard_id_;
    }

    [[nodiscard]] const PlayerScoreboardId &getPlayerId() const
    {
        return scoreboard_id_.getIdentityDef().getPlayerId();
    }

    [[nodiscard]] const ActorUniqueID &getEntityId() const
    {
        return scoreboard_id_.getIdentityDef().getEntityId();
    }

    [[nodiscard]] const std::string &getFakePlayerName() const
    {
        return scoreboard_id_.getIdentityDef().getFakePlayerName();
    }

    [[nodiscard]] IdentityDefinition::Type getIdentityType() const
    {
        return scoreboard_id_.getIdentityDef().getIdentityType();
    }

    bool modifyScoreInObjective(int &result, Objective &objective, int score, PlayerScoreSetFunction action)
    {
        bool was_set = objective.getPlayerScore(scoreboard_id_).valid;
        auto success = objective._modifyPlayerScore(result, scoreboard_id_, score, action);
        if (!was_set && success) {
            ++num_references_;
        }
        return success;
    }

private:
    int num_references_;
    ScoreboardId scoreboard_id_;
};
