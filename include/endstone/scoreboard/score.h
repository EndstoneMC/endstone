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

#include "endstone/scoreboard/score_entry.h"

namespace endstone {

class Scoreboard;
class Objective;

/**
 * @brief Represents a score for an objective on a scoreboard.
 */
class Score {
public:
    virtual ~Score() = default;

    /**
     * @brief Gets the entry being tracked by this Score
     *
     * @return this Score's tracked entry
     */
    [[nodiscard]] virtual ScoreEntry getEntry() const = 0;

    /**
     * @brief Gets the current score
     *
     * @return the current score
     */
    [[nodiscard]] virtual int getValue() const = 0;

    /**
     * @brief Sets the current score.
     *
     * @param score New score
     */
    virtual void setValue(int score) = 0;

    /**
     * @brief Shows if this score has been set at any point in time.
     *
     * @return if this score has been set before
     */

    [[nodiscard]] virtual bool isScoreSet() const = 0;

    /**
     * @brief Gets the Objective being tracked by this Score.
     *
     * @return the owning objective's scoreboard
     */
    [[nodiscard]] virtual Objective &getObjective() const = 0;

    /**
     * @brief Gets the scoreboard for the associated objective.
     *
     * @return the owning objective's scoreboard
     */
    [[nodiscard]] virtual Scoreboard &getScoreboard() const = 0;
};

}  // namespace endstone
