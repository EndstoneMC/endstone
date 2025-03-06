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

#include "bedrock/world/scores/scoreboard_identity_ref.h"

#include "bedrock/world/scores/scoreboard.h"

const ScoreboardId &ScoreboardIdentityRef::getScoreboardId() const
{
    return scoreboard_id_;
}
const PlayerScoreboardId &ScoreboardIdentityRef::getPlayerId() const
{
    return scoreboard_id_.getIdentityDef().getPlayerId();
}
const ActorUniqueID &ScoreboardIdentityRef::getEntityId() const
{
    return scoreboard_id_.getIdentityDef().getEntityId();
}
const std::string &ScoreboardIdentityRef::getFakePlayerName() const
{
    return scoreboard_id_.getIdentityDef().getFakePlayerName();
}

int ScoreboardIdentityRef::getNumReferences() const
{
    return static_cast<int>(objective_references_);
}

IdentityDefinition::Type ScoreboardIdentityRef::getIdentityType() const
{
    return scoreboard_id_.getIdentityDef().getIdentityType();
}

bool ScoreboardIdentityRef::removeFromObjective(Scoreboard &scoreboard, Objective &objective)
{
    auto score = objective.getPlayerScore(scoreboard_id_);
    if (!score.valid) {
        return false;
    }

    --objective_references_;
    if (objective_references_ <= 0) {
        scoreboard.clearScoreboardIdentity(scoreboard_id_);
    }
    return true;
}

bool ScoreboardIdentityRef::modifyScoreInObjective(int &result, Objective &objective, int score,
                                                   PlayerScoreSetFunction action)
{
    const bool was_set = objective.getPlayerScore(scoreboard_id_).valid;
    const auto success = objective._modifyPlayerScore(result, scoreboard_id_, score, action);
    if (!was_set && success) {
        ++objective_references_;
    }
    return success;
}
