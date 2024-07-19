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

#include "endstone/detail/scoreboard/scoreboard.h"

#include <stdexcept>

#include "bedrock/world/scores/objective_criteria.h"
#include "bedrock/world/scores/scoreboard.h"
#include "endstone/detail/scoreboard/objective.h"

namespace endstone::detail {

EndstoneScoreboard::EndstoneScoreboard(::Scoreboard &board) : board_(board) {}

std::shared_ptr<Objective> EndstoneScoreboard::addObjective(std::string name, Criteria::Type criteria)
{
    return addObjective(name, criteria, name);
}

std::shared_ptr<Objective> EndstoneScoreboard::addObjective(std::string name, Criteria::Type criteria,
                                                            std::string display_name)
{
    return addObjective(name, criteria, name, RenderType::Integer);
}

std::shared_ptr<Objective> EndstoneScoreboard::addObjective(std::string name, Criteria::Type criteria,
                                                            std::string display_name, RenderType render_type)
{
    throw std::runtime_error("Not implemented.");
}

std::shared_ptr<Objective> EndstoneScoreboard::getObjective(std::string name) const
{
    if (auto *objective = board_.getObjective(name); objective) {
        return std::make_shared<EndstoneObjective>(const_cast<EndstoneScoreboard &>(*this), *objective);
    }
    return nullptr;
}

std::shared_ptr<Objective> EndstoneScoreboard::getObjective(DisplaySlot slot) const
{
    throw std::runtime_error("Not implemented.");
}

std::vector<std::shared_ptr<Objective>> EndstoneScoreboard::getObjectives() const
{
    throw std::runtime_error("Not implemented.");
}

std::vector<std::shared_ptr<Objective>> EndstoneScoreboard::getObjectivesByCriteria(Criteria::Type criteria) const
{
    throw std::runtime_error("Not implemented.");
}

std::vector<std::shared_ptr<Score>> EndstoneScoreboard::getScores(ScoreEntry entry) const
{
    throw std::runtime_error("Not implemented.");
}

void EndstoneScoreboard::resetScores(ScoreEntry entry)
{
    throw std::runtime_error("Not implemented.");
}

std::vector<ScoreEntry> EndstoneScoreboard::getEntries() const
{
    throw std::runtime_error("Not implemented.");
}

void EndstoneScoreboard::clearSlot(DisplaySlot slot)
{
    throw std::runtime_error("Not implemented.");
}

}  // namespace endstone::detail
