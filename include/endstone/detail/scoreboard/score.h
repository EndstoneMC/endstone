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

#include "bedrock/world/scores/scoreboard_id.h"
#include "endstone/scoreboard/score.h"
#include "endstone/scoreboard/score_entry.h"

namespace endstone::detail {

class EndstoneObjective;

class EndstoneScore : public Score {
public:
    EndstoneScore(std::unique_ptr<EndstoneObjective> objective, ScoreEntry entry);
    [[nodiscard]] ScoreEntry getEntry() const override;
    [[nodiscard]] int getValue() const override;
    void setValue(int score) override;
    [[nodiscard]] bool isScoreSet() const override;
    [[nodiscard]] Objective &getObjective() const override;
    [[nodiscard]] Scoreboard &getScoreboard() const override;

private:
    [[nodiscard]] const ScoreboardId &getScoreboardId() const;
    [[nodiscard]] const ScoreboardId &getOrCreateScoreboardId() ;

    std::unique_ptr<EndstoneObjective> objective_;
    ScoreEntry entry_;
};

}  // namespace endstone::detail
