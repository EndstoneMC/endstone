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

#include "endstone/detail/scoreboard/score.h"
#include "endstone/detail/scoreboard/scoreboard.h"
#include "endstone/detail/server.h"

namespace endstone::detail {

EndstoneObjective::EndstoneObjective(EndstoneScoreboard &scoreboard, ::Objective &objective)
    : name_(objective.getName()), scoreboard_(scoreboard), objective_(objective), criteria_(objective.getCriteria())
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

void EndstoneObjective::unregister() const
{
    if (checkState()) {
        scoreboard_.board_.removeObjective(&objective_);
    }
}

std::optional<DisplaySlot> EndstoneObjective::getDisplaySlot() const
{
    std::optional<DisplaySlot> result;
    if (!checkState()) {
        return result;
    }

    forEachDisplayObjective([&](auto slot, const auto &display) -> bool {
        if (&display.getObjective() == &objective_) {
            result = slot;
            return false;
        }
        return true;
    });
    return result;
}

std::optional<ObjectiveSortOrder> EndstoneObjective::getSortOrder() const
{
    std::optional<ObjectiveSortOrder> result;
    if (!checkState()) {
        return result;
    }

    forEachDisplayObjective([&](auto /*slot*/, const auto &display) -> bool {
        if (&display.getObjective() == &objective_) {
            result = static_cast<ObjectiveSortOrder>(display.getSortOrder());
            return false;
        }
        return true;
    });
    return result;
}

void EndstoneObjective::setDisplay(std::optional<DisplaySlot> slot)
{
    setDisplay(slot, ObjectiveSortOrder::Ascending);
}

void EndstoneObjective::setDisplay(std::optional<DisplaySlot> slot, ObjectiveSortOrder order)
{
    if (!checkState()) {
        return;
    }

    forEachDisplayObjective([this](auto i, const auto &display) -> bool {
        if (&display.getObjective() == &objective_) {
            scoreboard_.board_.clearDisplayObjective(EndstoneScoreboard::getDisplaySlotName(i));
        }
        return true;
    });

    if (slot.has_value()) {
        scoreboard_.board_.setDisplayObjective(EndstoneScoreboard::getDisplaySlotName(slot.value()), objective_,
                                               static_cast<::ObjectiveSortOrder>(order));
    }
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
    auto &server = entt::locator<EndstoneServer>::value();
    server.getLogger().error("Objective::setRenderType is not supported.");
}

std::unique_ptr<Score> EndstoneObjective::getScore(ScoreEntry entry) const
{
    if (checkState()) {
        return std::make_unique<EndstoneScore>(copy(), entry);
    }
    return nullptr;
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

void EndstoneObjective::forEachDisplayObjective(
    const std::function<bool(DisplaySlot, const DisplayObjective &)> &callback) const
{
    if (!checkState()) {
        return;
    }

    for (auto const &slot : magic_enum::enum_values<DisplaySlot>()) {
        const auto *display = scoreboard_.board_.getDisplayObjective(EndstoneScoreboard::getDisplaySlotName(slot));
        if (display) {
            if (!callback(slot, *display)) {
                return;
            }
        }
    }
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

}  // namespace endstone::detail
