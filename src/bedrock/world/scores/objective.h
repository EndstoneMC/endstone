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
#include "bedrock/world/scores/score.h"
#include "bedrock/world/scores/scoreboard_id.h"
#include "bedrock/world/scores/scoreboard_operation_result.h"

enum class ObjectiveSortOrder : std::uint8_t {
    Ascending = 0,
    Descending = 1
};

enum class PlayerScoreSetFunction : std::uint8_t {
    Set = 0,
    Add = 1,
    Subtract = 2
};

struct PlayerScore {
    PlayerScore() = default;
    PlayerScore(const ScoreboardId &scoreboard_id, int score);
    [[nodiscard]] const ScoreboardId &getId() const;
    [[nodiscard]] int getScore() const;
    ScoreboardId scoreboard_id;
    int score;
};
static_assert(sizeof(PlayerScore) == 24);

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
    ScoreboardOperationResult _modifyPlayerScore(int &result, const ScoreboardId &id, int value,
                                                 PlayerScoreSetFunction action);  // NOLINT

    std::unordered_map<ScoreboardId, int> scores_;  // +24
    std::string name_;                              // +88
    std::string display_name_;                      // +120
    const ObjectiveCriteria &criteria_;             // +152
};

class DisplayObjective {
public:
    [[nodiscard]] bool isDisplaying(const Objective &objective) const
    {
        return objective_ == &objective;
    }

    [[nodiscard]] bool isValid() const
    {
        return objective_ != nullptr;
    }

    [[nodiscard]] const Objective &getObjective() const
    {
        return *objective_;
    }

    [[nodiscard]] ObjectiveSortOrder getSortOrder() const
    {
        return sort_order_;
    }

private:
    const Objective *objective_;
    ObjectiveSortOrder sort_order_;
};
