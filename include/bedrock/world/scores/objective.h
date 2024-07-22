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
#include <unordered_map>

#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/world/scores/objective_criteria.h"
#include "bedrock/world/scores/player_score_set_function.h"
#include "bedrock/world/scores/score_info.h"
#include "bedrock/world/scores/scoreboard_id.h"

class Objective : public Bedrock::EnableNonOwnerReferences {
public:
    Objective(const std::string &name, const ObjectiveCriteria &criteria);

    [[nodiscard]] const std::unordered_map<ScoreboardId, int> &getScores() const;
    [[nodiscard]] const std::string &getName() const;
    [[nodiscard]] const std::string &getDisplayName() const;
    [[nodiscard]] const ObjectiveCriteria &getCriteria() const;
    [[nodiscard]] ObjectiveRenderType getRenderType() const;
    void setDisplayName(const std::string &display_name);

    [[nodiscard]] bool hasScore(const ScoreboardId &id) const;
    [[nodiscard]] ScoreInfo getPlayerScore(const ScoreboardId &id) const;

private:
    friend class ScoreboardIdentityRef;
    bool _modifyPlayerScore(int &result, const ScoreboardId &id, int value, PlayerScoreSetFunction action);  // NOLINT

    std::unordered_map<ScoreboardId, int> scores_;  // +24
    std::string name_;                              // +88
    std::string display_name_;                      // +120
    const ObjectiveCriteria &criteria_;             // +152
};
