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

#include "endstone/detail/scoreboard/score.h"

#include <utility>

#include "endstone/detail/player.h"
#include "endstone/detail/scoreboard/objective.h"
#include "endstone/detail/scoreboard/scoreboard.h"
#include "endstone/detail/server.h"

namespace endstone::detail {

EndstoneScore::EndstoneScore(std::unique_ptr<EndstoneObjective> objective, ScoreEntry entry)
    : objective_(std::move(objective)), entry_(std::move(entry))
{
}

ScoreEntry EndstoneScore::getEntry() const
{
    return entry_;
}

int EndstoneScore::getValue() const
{
    if (objective_->checkState()) {
        const auto &id = getScoreboardId();
        if (id.isValid() && objective_->objective_.hasScore(id)) {
            return objective_->objective_.getPlayerScore(id).value;
        }
    }
    return 0;
}

void EndstoneScore::setValue(int score)
{
    if (objective_->checkState()) {
        const auto &id = getOrCreateScoreboardId();
        if (!id.isValid()) {
            throw std::runtime_error("Invalid scoreboard id");
        }

        auto &server = entt::locator<EndstoneServer>::value();
        if (!objective_->isModifiable()) {
            server.getLogger().error("Cannot modify read-only score");
            return;
        }

        bool success = false;
        objective_->scoreboard_.board_.modifyPlayerScore(success, id, objective_->objective_, score,
                                                         PlayerScoreSetFunction::Set);
        if (!success) {
            server.getLogger().error("Cannot modify score");
        }
    }
}

bool EndstoneScore::isScoreSet() const
{
    if (objective_->checkState()) {
        const auto &id = getScoreboardId();
        return id.isValid() && objective_->objective_.hasScore(id);
    }
    return false;
}

Objective &EndstoneScore::getObjective() const
{
    return *objective_;
}

Scoreboard &EndstoneScore::getScoreboard() const
{
    return objective_->getScoreboard();
}

const ScoreboardId &EndstoneScore::getScoreboardId() const
{
    if (objective_->checkState()) {
        return objective_->scoreboard_.getScoreboardId(entry_);
    }
    return ScoreboardId::INVALID;
}

const ScoreboardId &EndstoneScore::getOrCreateScoreboardId()
{
    if (objective_->checkState()) {
        return objective_->scoreboard_.getOrCreateScoreboardId(entry_);
    }
    return ScoreboardId::INVALID;
}

}  // namespace endstone::detail
