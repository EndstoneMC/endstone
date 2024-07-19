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

#include "endstone/detail/scoreboard/objective.h"

#include "endstone/detail/scoreboard/scoreboard.h"
#include "endstone/detail/server.h"

namespace endstone::detail {

EndstoneObjective::EndstoneObjective(EndstoneScoreboard &scoreboard, ::Objective &objective)
    : name_(objective.getName()), scoreboard_(scoreboard), objective_(objective), criteria_(*objective.getCriteria())
{
}

std::string EndstoneObjective::getName() const
{
    if (checkState()) {
        return objective_.getName();
    }
    return "";
}

std::string EndstoneObjective::getDisplayName() const
{
    if (checkState()) {
        return objective_.getDisplayName();
    }
    return "";
}

void EndstoneObjective::setDisplayName(std::string display_name)
{
    if (checkState()) {
        objective_.setDisplayName(display_name);
    }
}

const Criteria *EndstoneObjective::getCriteria() const
{
    if (checkState()) {
        return &criteria_;
    }
    return nullptr;
}

bool EndstoneObjective::isModifiable() const
{
    if (checkState()) {
        return !criteria_.isReadOnly();
    }
    return false;
}

Scoreboard &EndstoneObjective::getScoreboard() const
{
    return scoreboard_;
}

DisplaySlot EndstoneObjective::getDisplaySlot() const
{
    throw std::runtime_error("Not implemented.");
}

void EndstoneObjective::setDisplaySlot(DisplaySlot slot)
{
    throw std::runtime_error("Not implemented.");
}

RenderType EndstoneObjective::getRenderType() const
{
    throw std::runtime_error("Not implemented.");
}

void EndstoneObjective::setRenderType(RenderType render_type)
{
    throw std::runtime_error("Not implemented.");
}

ObjectiveSortOrder EndstoneObjective::getSortOrder() const
{
    throw std::runtime_error("Not implemented.");
}

void EndstoneObjective::setSortOrder(ObjectiveSortOrder order)
{
    throw std::runtime_error("Not implemented.");
}

std::shared_ptr<Score> EndstoneObjective::getScore(ScoreEntry entry) const
{
    throw std::runtime_error("Not implemented.");
}

bool EndstoneObjective::checkState() const
{
    if (scoreboard_.board_.getObjective(name_) == nullptr) {
        auto &server = entt::locator<EndstoneServer>::value();
        server.getLogger().error("Objective {} is unregistered from the scoreboard.", name_);
        return false;
    }
    return true;
}

}  // namespace endstone::detail
