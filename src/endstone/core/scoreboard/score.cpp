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
    return objective_->checkState().and_then([&](const auto *obj) -> Result<int> {
        return getScoreboardId().and_then([&](const auto *id) -> Result<int> {
            if (id->isValid() && obj->objective_.hasScore(*id)) {
                return obj->objective_.getPlayerScore(*id).value;
            }
            return 0;
        });
    });
}

Result<void> EndstoneScore::setValue(int score)
{
    return objective_->checkState().and_then([&](const auto *obj) -> Result<void> {
        return getOrCreateScoreboardId().and_then([&](const auto *id) -> Result<void> {
            return obj->isModifiable().and_then([&](bool modifiable) -> Result<void> {
                ENDSTONE_CHECK(modifiable, "Cannot modify read-only score.");

                bool success = false;
                obj->scoreboard_.board_.modifyPlayerScore(success, *id, obj->objective_, score,
                                                          PlayerScoreSetFunction::Set);

                ENDSTONE_CHECK(success, "Unable to modify score.");
                return {};
            });
        });
    });
}

Result<bool> EndstoneScore::isScoreSet() const
{
    return objective_->checkState().and_then([&](const auto *obj) -> Result<bool> {
        return getScoreboardId().and_then(
            [&](const auto *id) -> Result<bool> { return id->isValid() && obj->objective_.hasScore(*id); });
    });
}

Objective &EndstoneScore::getObjective() const
{
    return *objective_;
}

Scoreboard &EndstoneScore::getScoreboard() const
{
    return objective_->getScoreboard();
}

Result<const ScoreboardId *> EndstoneScore::getScoreboardId() const
{
    return objective_->checkState().and_then(
        [this](const auto *obj) -> Result<const ScoreboardId *> { return &obj->scoreboard_.getScoreboardId(entry_); });
}

Result<const ScoreboardId *> EndstoneScore::getOrCreateScoreboardId()
{
    return objective_->checkState().and_then([this](const auto *obj) -> Result<const ScoreboardId *> {
        return &obj->scoreboard_.getOrCreateScoreboardId(entry_);
    });
}

}  // namespace endstone::core
