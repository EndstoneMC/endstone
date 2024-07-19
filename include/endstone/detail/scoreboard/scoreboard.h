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

#include "bedrock/world/scores/scoreboard.h"
#include "endstone/scoreboard/scoreboard.h"

namespace endstone::detail {

class EndstoneScoreboard : public Scoreboard {
public:
    explicit EndstoneScoreboard(::Scoreboard &board);
    Objective *addObjective(std::string name, Criteria::Type criteria) override;
    Objective *addObjective(std::string name, Criteria::Type criteria, std::string display_name) override;
    Objective *addObjective(std::string name, Criteria::Type criteria, std::string display_name,
                            RenderType render_type) override;
    [[nodiscard]] Objective *getObjective(std::string name) const override;
    [[nodiscard]] Objective *getObjective(DisplaySlot slot) const override;
    [[nodiscard]] std::vector<Objective *> getObjectives() const override;
    [[nodiscard]] std::vector<Objective *> getObjectivesByCriteria(Criteria::Type criteria) const override;
    [[nodiscard]] std::vector<Score *> getScores(ScoreEntry entry) const override;
    void resetScores(ScoreEntry entry) override;
    [[nodiscard]] std::vector<ScoreEntry> getEntries() const override;
    void clearSlot(DisplaySlot slot) override;

private:
    ::Scoreboard &board_;
};

}  // namespace endstone::detail
