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
#include "endstone/core/util/error.h"

namespace endstone::core {

EndstoneObjective::EndstoneObjective(EndstoneScoreboard &scoreboard, ::Objective &objective)
    : name_(objective.getName()), scoreboard_(scoreboard), objective_(objective), criteria_(objective.getCriteria())
{
}

Result<std::string> EndstoneObjective::getName() const
{
    return checkState().and_then([](const auto *self) -> Result<std::string> { return self->objective_.getName(); });
}

Result<std::string> EndstoneObjective::getDisplayName() const
{
    return checkState().and_then(
        [](const auto *self) -> Result<std::string> { return self->objective_.getDisplayName(); });
}

Result<void> EndstoneObjective::setDisplayName(std::string display_name)
{
    return checkState().and_then([&display_name](const auto *self) -> Result<void> {
        self->objective_.setDisplayName(display_name);
        return {};
    });
}

Result<const Criteria *> EndstoneObjective::getCriteria() const
{
    return checkState().and_then([](const auto *self) -> Result<const Criteria *> { return &self->criteria_; });
}

Result<bool> EndstoneObjective::isModifiable() const
{
    return checkState().and_then([](const auto *self) -> Result<bool> { return !self->criteria_.isReadOnly(); });
}

Scoreboard &EndstoneObjective::getScoreboard() const
{
    return scoreboard_;
}

Result<void> EndstoneObjective::unregister() const
{
    return checkState().and_then([](const auto *self) -> Result<void> {
        self->scoreboard_.board_.removeObjective(&self->objective_);
        return {};
    });
}

Result<bool> EndstoneObjective::isDisplayed() const
{
    bool displayed = false;
    return forEachDisplayObjective([&](auto /*slot*/, const auto &display) -> bool {
               if (&display.getObjective() == &objective_) {
                   displayed = true;
                   return false;
               }
               return true;
           })
        .and_then([&displayed]() -> Result<bool> { return displayed; });
}

Result<DisplaySlot> EndstoneObjective::getDisplaySlot() const
{
    std::optional<DisplaySlot> result;
    return forEachDisplayObjective([&](auto slot, const auto &display) -> bool {
               if (&display.getObjective() == &objective_) {
                   result = slot;
                   return false;
               }
               return true;
           })
        .and_then([&result]() -> Result<DisplaySlot> {
            if (result) {
                return result.value();
            }
            return nonstd::make_unexpected(make_error("Object is not displayed."));
        });
}

Result<ObjectiveSortOrder> EndstoneObjective::getSortOrder() const
{
    std::optional<ObjectiveSortOrder> result;
    return forEachDisplayObjective([&](auto /*slot*/, const auto &display) -> bool {
               if (&display.getObjective() == &objective_) {
                   result = static_cast<ObjectiveSortOrder>(display.getSortOrder());
                   return false;
               }
               return true;
           })
        .and_then([&result]() -> Result<ObjectiveSortOrder> {
            if (result) {
                return result.value();
            }
            return nonstd::make_unexpected(make_error("Object is not displayed."));
        });
}

Result<void> EndstoneObjective::setDisplay(std::optional<DisplaySlot> slot)
{
    return setDisplay(slot, ObjectiveSortOrder::Ascending);
}

Result<void> EndstoneObjective::setDisplay(std::optional<DisplaySlot> slot, ObjectiveSortOrder order)
{
    return forEachDisplayObjective([this](auto i, const auto &display) -> bool {
               if (&display.getObjective() == &objective_) {
                   scoreboard_.board_.clearDisplayObjective(EndstoneScoreboard::getDisplaySlotName(i));
               }
               return true;
           })
        .and_then([&]() -> Result<void> {
            if (slot.has_value()) {
                scoreboard_.board_.setDisplayObjective(EndstoneScoreboard::getDisplaySlotName(slot.value()), objective_,
                                                       static_cast<::ObjectiveSortOrder>(order));
            }
            return {};
        });
}

Result<RenderType> EndstoneObjective::getRenderType() const
{
    return checkState().and_then([](const auto *self) -> Result<RenderType> {
        return static_cast<RenderType>(self->objective_.getRenderType());
    });
}

Result<void> EndstoneObjective::setRenderType(RenderType render_type)
{
    return checkState().and_then([](const auto * /*self*/) -> Result<void> {
        return nonstd::make_unexpected(make_error("setRenderType is not supported."));
    });
}

Result<std::unique_ptr<Score>> EndstoneObjective::getScore(ScoreEntry entry) const
{
    return checkState().and_then([entry](const auto *self) -> Result<std::unique_ptr<Score>> {
        return std::make_unique<EndstoneScore>(self->copy(), entry);
    });
}

Result<const EndstoneObjective *> EndstoneObjective::checkState() const
{
    if (scoreboard_.board_.getObjective(name_) == nullptr) {
        return nonstd::make_unexpected(make_error("Objective '{}' is unregistered from the scoreboard.", name_));
    }
    return this;
}

Result<void> EndstoneObjective::forEachDisplayObjective(
    const std::function<bool(DisplaySlot, const DisplayObjective &)> &callback) const
{
    return checkState().and_then([&callback](const EndstoneObjective *self) -> Result<void> {
        for (auto const &slot : magic_enum::enum_values<DisplaySlot>()) {
            const auto slot_name = EndstoneScoreboard::getDisplaySlotName(slot);
            if (const auto *display = self->scoreboard_.board_.getDisplayObjective(slot_name)) {
                if (!callback(slot, *display)) {
                    return {};
                }
            }
        }
        return {};
    });
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
