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

#include "endstone/core/scoreboard/objective.h"

#include <optional>
#include <string>

#include <magic_enum/magic_enum.hpp>

#include "endstone/core/scoreboard/score.h"
#include "endstone/core/scoreboard/scoreboard.h"
#include "endstone/core/server.h"

namespace endstone::core {

EndstoneObjective::EndstoneObjective(EndstoneScoreboard &scoreboard, ::Objective &objective)
    : name_(objective.getName()), scoreboard_(scoreboard), objective_(objective), criteria_(objective.getCriteria())
{
}

Result<std::string> EndstoneObjective::getName() const
{
    ENDSTONE_CHECK_RESULT(checkState());
    return objective_.getName();
}

Result<std::string> EndstoneObjective::getDisplayName() const
{
    ENDSTONE_CHECK_RESULT(checkState());
    return objective_.getDisplayName();
}

Result<void> EndstoneObjective::setDisplayName(std::string display_name)
{
    ENDSTONE_CHECK_RESULT(checkState());
    objective_.setDisplayName(display_name);
    return {};
}

Result<const Criteria *> EndstoneObjective::getCriteria() const
{
    ENDSTONE_CHECK_RESULT(checkState());
    return &criteria_;
}

Result<bool> EndstoneObjective::isModifiable() const
{
    ENDSTONE_CHECK_RESULT(checkState());
    return !criteria_.isReadOnly();
}

Scoreboard &EndstoneObjective::getScoreboard() const
{
    return scoreboard_;
}

Result<void> EndstoneObjective::unregister() const
{
    auto board = checkState();
    ENDSTONE_CHECK_RESULT(board);
    board.value()->board_.removeObjective(&objective_);
    return {};
}

Result<bool> EndstoneObjective::isDisplayed() const
{
    auto scoreboard = checkState();
    ENDSTONE_CHECK_RESULT(scoreboard);
    auto &board = scoreboard.value()->board_;
    for (const auto &i : board.getDisplayObjectiveSlotNames()) {
        if (const auto *display_objective = board.getDisplayObjective(i)) {
            if (display_objective->isDisplaying(objective_)) {
                return true;
            }
        }
    }
    return false;
}

Result<DisplaySlot> EndstoneObjective::getDisplaySlot() const
{

    auto scoreboard = checkState();
    ENDSTONE_CHECK_RESULT(scoreboard);
    auto &board = scoreboard.value()->board_;
    for (const auto &i : board.getDisplayObjectiveSlotNames()) {
        if (const auto *display_objective = board.getDisplayObjective(i)) {
            if (display_objective->isDisplaying(objective_)) {
                return EndstoneScoreboard::fromMinecraftSlot(i);
            }
        }
    }
    return nonstd::make_unexpected("Object is not displayed.");
}

Result<ObjectiveSortOrder> EndstoneObjective::getSortOrder() const
{
    auto scoreboard = checkState();
    ENDSTONE_CHECK_RESULT(scoreboard);
    auto &board = scoreboard.value()->board_;
    for (const auto &i : board.getDisplayObjectiveSlotNames()) {
        if (const auto *display_objective = board.getDisplayObjective(i)) {
            if (display_objective->isDisplaying(objective_)) {
                return static_cast<ObjectiveSortOrder>(display_objective->getSortOrder());
            }
        }
    }
    return nonstd::make_unexpected("Object is not displayed.");
}

Result<void> EndstoneObjective::setDisplaySlot(std::optional<DisplaySlot> slot)
{
    auto scoreboard = checkState();
    ENDSTONE_CHECK_RESULT(scoreboard);
    auto &board = scoreboard.value()->board_;

    std::optional<::ObjectiveSortOrder> order;
    for (const auto &i : board.getDisplayObjectiveSlotNames()) {
        if (const auto *display_objective = board.getDisplayObjective(i)) {
            if (display_objective->isDisplaying(objective_)) {
                if (!order.has_value()) {
                    order = display_objective->getSortOrder();
                }
                board.clearDisplayObjective(i);
            }
        }
    }

    if (slot.has_value()) {
        board.setDisplayObjective(EndstoneScoreboard::toMinecraftSlot(slot.value()), objective_,
                                  order.value_or(::ObjectiveSortOrder::Ascending));
    }
    return {};
}

Result<void> EndstoneObjective::setSortOrder(ObjectiveSortOrder order)
{
    auto scoreboard = checkState();
    ENDSTONE_CHECK_RESULT(scoreboard);
    auto &board = scoreboard.value()->board_;

    std::optional<std::string> slot;
    for (const auto &i : board.getDisplayObjectiveSlotNames()) {
        if (const auto *display_objective = board.getDisplayObjective(i)) {
            if (display_objective->isDisplaying(objective_)) {
                if (!slot.has_value()) {
                    slot = i;
                }
                board.clearDisplayObjective(i);
            }
        }
    }

    if (slot.has_value()) {
        board.setDisplayObjective(slot.value(), objective_, static_cast<::ObjectiveSortOrder>(order));
    }
    return nonstd::make_unexpected("Object is not displayed.");
}

Result<void> EndstoneObjective::setDisplay(const std::optional<DisplaySlot> slot, ObjectiveSortOrder order)
{
    auto scoreboard = checkState();
    ENDSTONE_CHECK_RESULT(scoreboard);
    auto &board = scoreboard.value()->board_;

    for (const auto &i : board.getDisplayObjectiveSlotNames()) {
        if (const auto *display_objective = board.getDisplayObjective(i)) {
            if (display_objective->isDisplaying(objective_)) {
                board.clearDisplayObjective(i);
            }
        }
    }

    if (slot.has_value()) {
        board.setDisplayObjective(EndstoneScoreboard::toMinecraftSlot(slot.value()), objective_,
                                  static_cast<::ObjectiveSortOrder>(order));
    }
    return {};
}

Result<RenderType> EndstoneObjective::getRenderType() const
{
    ENDSTONE_CHECK_RESULT(checkState());
    return static_cast<RenderType>(objective_.getRenderType());
}

Result<std::unique_ptr<Score>> EndstoneObjective::getScore(ScoreEntry entry) const
{
    ENDSTONE_CHECK_RESULT(checkState());
    return std::make_unique<EndstoneScore>(copy(), entry);
}

Result<EndstoneScoreboard *> EndstoneObjective::checkState() const
{
    if (scoreboard_.board_.getObjective(name_) == nullptr) {
        return nonstd::make_unexpected(fmt::format("Objective '{}' is unregistered from the scoreboard.", name_));
    }
    return &scoreboard_;
}

std::unique_ptr<EndstoneObjective> EndstoneObjective::copy() const
{
    return std::make_unique<EndstoneObjective>(scoreboard_, objective_);
}

bool EndstoneObjective::operator==(const Objective &other) const
{
    return &objective_ == &static_cast<const EndstoneObjective &>(other).objective_;
}

bool EndstoneObjective::operator!=(const Objective &other) const
{
    return !(*this == other);
}

}  // namespace endstone::core
