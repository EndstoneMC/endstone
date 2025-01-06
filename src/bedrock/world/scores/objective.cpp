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

#include "bedrock/world/scores/objective.h"

#include <stdexcept>

Objective::Objective(const std::string &name, const ObjectiveCriteria &criteria)
    : name_(name), criteria_(criteria), display_name_(name)
{
}

const std::unordered_map<ScoreboardId, int> &Objective::getScores() const
{
    return scores_;
}

const std::string &Objective::getName() const
{
    return name_;
}

const std::string &Objective::getDisplayName() const
{
    return display_name_;
}

const ObjectiveCriteria &Objective::getCriteria() const
{
    return criteria_;
}

ObjectiveRenderType Objective::getRenderType() const
{
    return criteria_.getRenderType();
}

void Objective::setDisplayName(const std::string &display_name)
{
    display_name_ = display_name;
}

bool Objective::hasScore(const ScoreboardId &id) const
{
    return scores_.find(id) != scores_.end();
}

ScoreInfo Objective::getPlayerScore(const ScoreboardId &id) const
{
    ScoreInfo result;
    result.objective = this;
    auto it = scores_.find(id);
    if (it != scores_.end()) {
        result.value = it->second;
        result.valid = true;
    }
    else {
        result.valid = false;
    }
    return result;
}

bool Objective::_modifyPlayerScore(int &result, const ScoreboardId &id, int value, PlayerScoreSetFunction action)
{
    if (criteria_.isReadOnly()) {
        result = 0;
        return false;
    }

    auto &score = scores_[id];
    switch (action) {
    case PlayerScoreSetFunction::Set:
        score = value;
        break;
    case PlayerScoreSetFunction::Add:
        score += value;
        break;
    case PlayerScoreSetFunction::Subtract:
        score -= value;
        break;
    default:
        throw std::runtime_error("Unknown PlayerScoreSetFunction!");
    }
    result = score;
    return true;
}
