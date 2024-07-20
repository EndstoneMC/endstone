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

#include <string>

#include "endstone/scoreboard/criteria.h"
#include "endstone/scoreboard/display_slot.h"
#include "endstone/scoreboard/objective.h"
#include "endstone/scoreboard/score_entry.h"

namespace endstone {

/**
 * @brief Represents a scoreboard
 */
class Scoreboard {
public:
    virtual ~Scoreboard() = default;

    /**
     * @brief Registers an Objective on this Scoreboard
     *
     * @param name Name of the Objective
     * @param criteria Criteria for the Objective
     * @return A reference to the newly registered Objective,
     * or <code>nullptr</code> if an objective by that name already exists.
     */
    virtual std::unique_ptr<Objective> addObjective(std::string name, Criteria::Type criteria) = 0;

    /**
     * @brief Registers an Objective on this Scoreboard
     *
     * @param name Name of the Objective
     * @param criteria Criteria type for the Objective
     * @param display_name Name displayed to players for the Objective.
     * @return A reference to the newly registered Objective,
     * or <code>nullptr</code> if an objective by that name already exists.
     */
    virtual std::unique_ptr<Objective> addObjective(std::string name, Criteria::Type criteria,
                                                    std::string display_name) = 0;

    /**
     * @brief Registers an Objective on this Scoreboard
     *
     * @param name Name of the Objective
     * @param criteria Criteria type for the Objective
     * @param display_name Name displayed to players for the Objective.
     * @param render_type Manner of rendering the Objective
     * @return A reference to the newly registered Objective,
     * or <code>nullptr</code> if an objective by that name already exists.
     */
    virtual std::unique_ptr<Objective> addObjective(std::string name, Criteria::Type criteria, std::string display_name,
                                                    RenderType render_type) = 0;

    /**
     * @brief Gets an Objective on this Scoreboard by name
     *
     * @param name Name of the Objective
     * @return the Objective or <code>nullptr</code> if it does not exist
     */
    [[nodiscard]] virtual std::unique_ptr<Objective> getObjective(std::string name) const = 0;

    /**
     * @brief Gets the Objective currently displayed in a DisplaySlot on this Scoreboard
     *
     * @param slot The DisplaySlot
     * @return the Objective currently displayed or <code>nullptr</code> if nothing is displayed in that DisplaySlot
     */
    [[nodiscard]] virtual std::unique_ptr<Objective> getObjective(DisplaySlot slot) const = 0;

    /**
     * @brief Gets all Objectives on this Scoreboard
     *
     * @return A list of all Objectives on this Scoreboard
     */
    [[nodiscard]] virtual std::vector<std::unique_ptr<Objective>> getObjectives() const = 0;

    /**
     * @brief Gets all Objectives of a Criteria on the Scoreboard
     *
     * @param criteria Criteria type to search by
     * @return A list of Objectives using the specified Criteria
     */
    [[nodiscard]] virtual std::vector<std::unique_ptr<Objective>> getObjectivesByCriteria(
        Criteria::Type criteria) const = 0;

    /**
     * @brief Gets all scores for an entry on this Scoreboard
     *
     * @param entry the entry whose scores are being retrieved
     * @return a list of all scores tracked for the entry
     */
    [[nodiscard]] virtual std::vector<std::unique_ptr<Score>> getScores(ScoreEntry entry) const = 0;

    /**
     * @brief Removes all scores for an entry on this Scoreboard
     *
     * @param entry the entry to drop all current scores for
     */
    virtual void resetScores(ScoreEntry entry) = 0;

    /**
     * @brief Gets all entries tracked by this Scoreboard
     *
     * @return A list of all tracked entries
     */
    [[nodiscard]] virtual std::vector<ScoreEntry> getEntries() const = 0;

    /**
     * @brief Clears any objective in the specified slot.
     *
     * @param slot the slot to remove objectives
     */
    virtual void clearSlot(DisplaySlot slot) = 0;
};

}  // namespace endstone
