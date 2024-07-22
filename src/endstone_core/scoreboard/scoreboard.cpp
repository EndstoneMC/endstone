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
#include "endstone/detail/server.h"

namespace endstone::detail {

namespace {
std::string getCriteriaName(Criteria::Type type)
{
    switch (type) {
    case Criteria::Type::Dummy:
        return "dummy";
    default:
        throw std::runtime_error("Unknown Criteria::Type!");
    }
}
}  // namespace

EndstoneScoreboard::EndstoneScoreboard(::Scoreboard &board) : board_(board) {}

std::unique_ptr<Objective> EndstoneScoreboard::addObjective(std::string name, Criteria::Type criteria)
{
    return addObjective(name, criteria, name);
}

std::unique_ptr<Objective> EndstoneScoreboard::addObjective(std::string name, Criteria::Type criteria,
                                                            std::string display_name)
{
    return addObjective(name, criteria, name, RenderType::Integer);
}

std::unique_ptr<Objective> EndstoneScoreboard::addObjective(std::string name, Criteria::Type criteria,
                                                            std::string display_name, RenderType render_type)
{
    auto &server = entt::locator<EndstoneServer>::value();
    auto *cr = board_.getCriteria(getCriteriaName(criteria));
    if (!cr) {
        server.getLogger().error("Unknown criteria: {}.", getCriteriaName(criteria));
        return nullptr;
    }

    auto *objective = board_.addObjective(name, display_name, *cr);
    if (!objective) {
        server.getLogger().error("Objective {} already exists.", name);
        return nullptr;
    }

    return std::make_unique<EndstoneObjective>(*this, *objective);
}

std::unique_ptr<Objective> EndstoneScoreboard::getObjective(std::string name) const
{
    if (auto *objective = board_.getObjective(name); objective) {
        return std::make_unique<EndstoneObjective>(const_cast<EndstoneScoreboard &>(*this), *objective);
    }
    return nullptr;
}

std::unique_ptr<Objective> EndstoneScoreboard::getObjective(DisplaySlot slot) const
{
    throw std::runtime_error("Not implemented.");
}

std::vector<std::unique_ptr<Objective>> EndstoneScoreboard::getObjectives() const
{
    throw std::runtime_error("Not implemented.");
}

std::vector<std::unique_ptr<Objective>> EndstoneScoreboard::getObjectivesByCriteria(Criteria::Type criteria) const
{
    throw std::runtime_error("Not implemented.");
}

std::vector<std::unique_ptr<Score>> EndstoneScoreboard::getScores(ScoreEntry entry) const
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
