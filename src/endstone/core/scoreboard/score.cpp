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

#include "endstone/core/scoreboard/score.h"

#include <utility>

#include "endstone/core/player.h"
#include "endstone/core/scoreboard/objective.h"
#include "endstone/core/scoreboard/scoreboard.h"
#include "endstone/core/server.h"

namespace endstone::core {

EndstoneScore::EndstoneScore(std::unique_ptr<EndstoneObjective> objective, ScoreEntry entry)
    : objective_(std::move(objective)), entry_(std::move(entry))
{
}

ScoreEntry EndstoneScore::getEntry() const
{
    return entry_;
}

Result<int> EndstoneScore::getValue() const
{
    auto board = objective_->checkState();
    ENDSTONE_CHECK_RESULT(board);
    const auto &id = board.value()->getScoreboardId(entry_);
    if (id.isValid() && objective_->objective_.hasScore(id)) {
        return objective_->objective_.getPlayerScore(id).value;
    }
    return 0;
}

Result<void> EndstoneScore::setValue(int score)
{
    auto board = objective_->checkState();
    ENDSTONE_CHECK_RESULT(board);
    const auto &id = board.value()->getOrCreateScoreboardId(entry_);
    ENDSTONE_CHECK(id.isValid(), "Unable to create scoreboard id for entry.");

    ScoreboardOperationResult result;
    board.value()->board_.modifyPlayerScore(result, id, objective_->objective_, score, PlayerScoreSetFunction::Set);
    switch (result) {
    case ScoreboardOperationResult::ReadOnlyCriteria:
        return nonstd::make_unexpected("Cannot modify read-only score.");
    case ScoreboardOperationResult::Success:
        return {};
    default:
        return nonstd::make_unexpected("Unable to modify score.");
    }
}

Result<bool> EndstoneScore::isScoreSet() const
{
    auto board = objective_->checkState();
    ENDSTONE_CHECK_RESULT(board);
    const auto &id = board.value()->getScoreboardId(entry_);
    return id.isValid() && objective_->objective_.hasScore(id);
}

Objective &EndstoneScore::getObjective() const
{
    return *objective_;
}

Scoreboard &EndstoneScore::getScoreboard() const
{
    return objective_->getScoreboard();
}

}  // namespace endstone::core
