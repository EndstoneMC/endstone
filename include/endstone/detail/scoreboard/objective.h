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

#include <memory>

#include "bedrock/world/scores/objective.h"
#include "criteria.h"
#include "endstone/scoreboard/display_slot.h"
#include "endstone/scoreboard/objective.h"

namespace endstone::detail {

class EndstoneScoreboard;

class EndstoneObjective : public Objective {
public:
    explicit EndstoneObjective(EndstoneScoreboard &scoreboard, ::Objective &objective);

    [[nodiscard]] std::string getName() const override;
    [[nodiscard]] std::string getDisplayName() const override;
    void setDisplayName(std::string display_name) override;
    [[nodiscard]] const Criteria *getCriteria() const override;
    [[nodiscard]] bool isModifiable() const override;
    [[nodiscard]] Scoreboard &getScoreboard() const override;
    [[nodiscard]] DisplaySlot getDisplaySlot() const override;
    void setDisplaySlot(DisplaySlot slot) override;
    [[nodiscard]] RenderType getRenderType() const override;
    void setRenderType(RenderType render_type) override;
    [[nodiscard]] ObjectiveSortOrder getSortOrder() const override;
    void setSortOrder(ObjectiveSortOrder order) override;
    [[nodiscard]] std::shared_ptr<Score> getScore(ScoreEntry entry) const override;

private:
    [[nodiscard]] bool checkState() const;

    std::string name_;
    EndstoneScoreboard &scoreboard_;
    ::Objective &objective_;
    EndstoneCriteria criteria_;
};

}  // namespace endstone::detail
