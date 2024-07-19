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

#include <optional>
#include <string>

#include <magic_enum/magic_enum.hpp>

#include "endstone/detail/scoreboard/scoreboard.h"
#include "endstone/detail/server.h"

namespace endstone::detail {

EndstoneObjective::EndstoneObjective(EndstoneScoreboard &scoreboard, ::Objective &objective)
    : name_(objective.getName()), scoreboard_(scoreboard), objective_(objective), criteria_(*objective.getCriteria())
{
}

std::optional<std::string> EndstoneObjective::getName() const
{
    if (checkState()) {
        return objective_.getName();
    }
    return std::nullopt;
}

std::optional<std::string> EndstoneObjective::getDisplayName() const
{
    if (checkState()) {
        return objective_.getDisplayName();
    }
    return std::nullopt;
}

void EndstoneObjective::setDisplayName(std::string display_name)
{
    if (checkState()) {
        objective_.setDisplayName(display_name);
        // TODO(scoreboard): probably we need to mark it as dirty, check this later
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

std::optional<DisplaySlot> EndstoneObjective::getDisplaySlot() const
{
    if (!checkState()) {
        return std::nullopt;
    }

    for (auto const &slot : magic_enum::enum_values<DisplaySlot>()) {
        if (const auto *display = scoreboard_.board_.getDisplayObjective(toBedrock(slot)); display) {
            if (display->getObjective() == &objective_) {
                return slot;
            }
        }
    }
    return std::nullopt;
}

void EndstoneObjective::setDisplaySlot(DisplaySlot slot)
{
    throw std::runtime_error("Not implemented.");
}

std::optional<RenderType> EndstoneObjective::getRenderType() const
{
    if (checkState()) {
        return static_cast<RenderType>(objective_.getRenderType());
    }
    return std::nullopt;
}

void EndstoneObjective::setRenderType(RenderType render_type)
{
    throw std::runtime_error("Not implemented.");
}

std::optional<ObjectiveSortOrder> EndstoneObjective::getSortOrder() const
{
    if (!checkState()) {
        return std::nullopt;
    }

    for (auto const &slot : magic_enum::enum_values<DisplaySlot>()) {
        if (const auto *display = scoreboard_.board_.getDisplayObjective(toBedrock(slot)); display) {
            if (display->getObjective() == &objective_) {
                return static_cast<ObjectiveSortOrder>(display->getSortOrder());
            }
        }
    }
    return std::nullopt;
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

std::string EndstoneObjective::toBedrock(DisplaySlot slot)
{
    switch (slot) {
    case DisplaySlot::BelowName:
        return "belowname";
    case DisplaySlot::PlayerList:
        return "list";
    case DisplaySlot::SideBar:
        return "sidebar";
    }
    throw std::runtime_error("You should never be here");
}

}  // namespace endstone::detail
