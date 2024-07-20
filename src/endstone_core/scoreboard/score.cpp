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

#include <entt/entt.hpp>

#include "endstone/detail/actor/actor.h"
#include "endstone/detail/player.h"
#include "endstone/detail/scoreboard/objective.h"
#include "endstone/detail/scoreboard/scoreboard.h"

namespace endstone::detail {

EndstoneScore::EndstoneScore(std::unique_ptr<EndstoneObjective> objective, ScoreEntry entry)
    : objective_(std::move(objective)), entry_(std::move(entry))
{
}

ScoreEntry EndstoneScore::getEntry() const
{
    return entry_;
}

int EndstoneScore::getScore() const
{
    if (objective_->checkState()) {
        auto id = getScoreboardId();
        if (id.isValid() && objective_->objective_.hasScore(id)) {
            return objective_->objective_.getPlayerScore(id);
        }
    }
    return 0;
}

void EndstoneScore::setScore(int score)
{
    if (objective_->checkState()) {
        auto id = getScoreboardId();
        if (!id.isValid()) {
            return;
        }

        auto &server = entt::locator<EndstoneServer>::value();
        if (!objective_->isModifiable()) {
            server.getLogger().error("Cannot modify read-only score");
            return;
        }

        if (!objective_->objective_.setPlayerScore(id, score)) {
            server.getLogger().error("Cannot modify score");
            return;
        }
    }
}

bool EndstoneScore::isScoreSet() const
{
    if (objective_->checkState()) {
        auto id = getScoreboardId();
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

ScoreboardId EndstoneScore::getScoreboardId() const
{
    if (objective_->checkState()) {
        auto &board = objective_->scoreboard_.board_;
        return std::visit(
            entt::overloaded{[&](Player *player) {
                                 return board.getScoreboardId(static_cast<EndstonePlayer *>(player)->getHandle());
                             },
                             [&](Actor *actor) {  //
                                 return board.getScoreboardId(static_cast<EndstoneActor *>(actor)->getActor());
                             },
                             [&](const std::string &fake) {
                                 return board.getScoreboardId(fake);
                             }},
            entry_);
    }
    auto &server = entt::locator<EndstoneServer>::value();
    server.getLogger().error("Entry is not currently tracked in the scoreboard.");
    return ScoreboardId::INVALID;
}

}  // namespace endstone::detail
