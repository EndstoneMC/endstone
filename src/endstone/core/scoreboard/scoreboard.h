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

#include "bedrock/world/scores/scoreboard.h"
#include "endstone/core/scoreboard/scoreboard_packet_sender.h"
#include "endstone/scoreboard/scoreboard.h"

namespace endstone::core {

class EndstoneScoreboard : public Scoreboard {
public:
    explicit EndstoneScoreboard(::Scoreboard &board);
    explicit EndstoneScoreboard(std::unique_ptr<::Scoreboard> board);
    void init();

    Result<std::unique_ptr<Objective>> addObjective(std::string name, Criteria::Type criteria) override;
    Result<std::unique_ptr<Objective>> addObjective(std::string name, Criteria::Type criteria,
                                                    std::string display_name) override;
    Result<std::unique_ptr<Objective>> addObjective(std::string name, Criteria::Type criteria, std::string display_name,
                                                    RenderType render_type) override;
    [[nodiscard]] std::unique_ptr<Objective> getObjective(std::string name) const override;
    [[nodiscard]] std::unique_ptr<Objective> getObjective(DisplaySlot slot) const override;
    [[nodiscard]] std::vector<std::unique_ptr<Objective>> getObjectives() const override;
    [[nodiscard]] std::vector<std::unique_ptr<Objective>> getObjectivesByCriteria(
        Criteria::Type criteria) const override;
    [[nodiscard]] std::vector<std::unique_ptr<Score>> getScores(ScoreEntry entry) const override;
    void resetScores(ScoreEntry entry) override;
    [[nodiscard]] std::vector<ScoreEntry> getEntries() const override;
    void clearSlot(DisplaySlot slot) override;
    [[nodiscard]] const ::ScoreboardId &getScoreboardId(ScoreEntry entry) const;
    const ::ScoreboardId &getOrCreateScoreboardId(ScoreEntry entry);
    [[nodiscard]] ::Scoreboard &getHandle() const;

    static std::string getCriteriaName(Criteria::Type type);
    static std::string toMinecraftSlot(DisplaySlot slot);
    static DisplaySlot fromMinecraftSlot(std::string slot);

private:
    friend class EndstoneObjective;
    friend class EndstoneScore;

    ::Scoreboard &board_;
    std::unique_ptr<::Scoreboard> holder_;
    std::unique_ptr<ScoreboardPacketSender> packet_sender_;
};

}  // namespace endstone::core
