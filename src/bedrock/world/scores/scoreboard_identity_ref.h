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
#include "bedrock/world/scores/objective.h"
#include "bedrock/world/scores/scoreboard_id.h"
#include "bedrock/world/scores/scoreboard_operation_result.h"

class Scoreboard;

class ScoreboardIdentityRef {
public:
    ScoreboardIdentityRef();
    ScoreboardIdentityRef(const ScoreboardId &);

    [[nodiscard]] const ScoreboardId &getScoreboardId() const;
    [[nodiscard]] const PlayerScoreboardId &getPlayerId() const;
    [[nodiscard]] const ActorUniqueID &getEntityId() const;
    [[nodiscard]] const std::string &getFakePlayerName() const;
    [[nodiscard]] int getNumReferences() const;
    [[nodiscard]] IdentityDefinition::Type getIdentityType() const;
    bool removeFromObjective(Scoreboard &, Objective &);
    ScoreboardOperationResult modifyScoreInObjective(int &result, Objective &objective, int score,
                                                     PlayerScoreSetFunction action);

private:
    std::uint32_t objective_references_;
    ScoreboardId scoreboard_id_;
};
