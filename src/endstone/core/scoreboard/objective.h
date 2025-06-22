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

#pragma once

#include <functional>
#include <memory>
#include <optional>

#include "bedrock/world/scores/objective.h"
#include "endstone/core/scoreboard/criteria.h"
#include "endstone/scoreboard/display_slot.h"
#include "endstone/scoreboard/objective.h"

namespace endstone::core {

class EndstoneScoreboard;

class EndstoneObjective : public Objective {
public:
    explicit EndstoneObjective(EndstoneScoreboard &scoreboard, ::Objective &objective);

    [[nodiscard]] Result<std::string> getName() const override;
    [[nodiscard]] Result<std::string> getDisplayName() const override;
    Result<void> setDisplayName(std::string display_name) override;
    [[nodiscard]] Result<const Criteria *> getCriteria() const override;
    [[nodiscard]] Result<bool> isModifiable() const override;
    [[nodiscard]] Scoreboard &getScoreboard() const override;
    [[nodiscard]] Result<void> unregister() const override;
    [[nodiscard]] Result<bool> isDisplayed() const override;
    [[nodiscard]] Result<DisplaySlot> getDisplaySlot() const override;
    [[nodiscard]] Result<ObjectiveSortOrder> getSortOrder() const override;
    Result<void> setDisplaySlot(std::optional<DisplaySlot> slot) override;
    Result<void> setSortOrder(ObjectiveSortOrder order) override;
    Result<void> setDisplay(std::optional<DisplaySlot> slot, ObjectiveSortOrder order) override;
    [[nodiscard]] Result<RenderType> getRenderType() const override;
    // Result<void> setRenderType(RenderType render_type) override;
    [[nodiscard]] Result<std::unique_ptr<Score>> getScore(ScoreEntry entry) const override;
    bool operator==(const Objective &other) const override;
    bool operator!=(const Objective &other) const override;

    [[nodiscard]] Result<EndstoneScoreboard *> checkState() const;

    [[nodiscard]] std::unique_ptr<EndstoneObjective> copy() const;

private:
    friend class EndstoneScore;
    std::string name_;
    EndstoneScoreboard &scoreboard_;
    ::Objective &objective_;
    EndstoneCriteria criteria_;
};

}  // namespace endstone::core