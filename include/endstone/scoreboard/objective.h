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

#include "endstone/scoreboard/objective_sort_order.h"
#include "endstone/scoreboard/score.h"

namespace endstone {

class Scoreboard;

/**
 * @brief Represents an objective on a scoreboard that can show scores specific to entries.
 */
class Objective {
public:
    virtual ~Objective() = default;

    /**
     * @brief Gets the name of this Objective
     *
     * @return this objective's name
     */
    [[nodiscard]] virtual std::optional<std::string> getName() const = 0;

    /**
     * @brief Gets the name displayed to players for this objective
     *
     * @return this objective's display name
     */
    [[nodiscard]] virtual std::optional<std::string> getDisplayName() const = 0;

    /**
     * @brief Sets the name displayed to players for this objective.
     *
     * @param displayName Display name to set
     */
    virtual void setDisplayName(std::string display_name) = 0;

    /**
     * @brief Gets the criteria this objective tracks.
     *
     * @return this objective's criteria, or <code>nullptr</code> if this objective has been unregistered.
     */
    [[nodiscard]] virtual const Criteria *getCriteria() const = 0;

    /**
     * @brief Gets if the objective's scores can be modified directly by a plugin.
     *
     * @return true if scores are modifiable
     */
    [[nodiscard]] virtual bool isModifiable() const = 0;

    /**
     * @brief Gets the scoreboard to which this objective is attached.
     *
     * @return Owning scoreboard
     */
    [[nodiscard]] virtual Scoreboard &getScoreboard() const = 0;

    /**
     * @brief Unregisters this objective from the associated Scoreboard.
     */
    virtual void unregister() const = 0;

    /**
     * @brief Gets the display slot this objective is displayed at.
     *
     * @return the display slot for this objective
     */
    [[nodiscard]] virtual std::optional<DisplaySlot> getDisplaySlot() const = 0;

    /**
     * @brief Gets the sort order for this objective.
     *
     * @return The sort order for this objective.
     */
    [[nodiscard]] virtual std::optional<ObjectiveSortOrder> getSortOrder() const = 0;

    /**
     * @brief Sets the display slot for this objective.
     * This will remove it from any other display slot.
     *
     * @param slot The display slot where this objective should be displayed.
     */
    virtual void setDisplay(std::optional<DisplaySlot> slot) = 0;

    /**
     * @brief Sets the display slot and sort order for this objective.
     * This will remove it from any other display slot.
     *
     * @param slot The display slot where this objective should be displayed.
     * @param order The sort order for this objective in the display slot.
     */
    virtual void setDisplay(std::optional<DisplaySlot> slot, ObjectiveSortOrder order) = 0;

    /**
     * Gets manner in which this objective will be rendered.
     *
     * @return the render type
     */
    [[nodiscard]] virtual std::optional<RenderType> getRenderType() const = 0;

    /**
     * @brief Sets manner in which this objective will be rendered.
     *
     * @param render_type new render type
     */
    virtual void setRenderType(RenderType render_type) = 0;

    /**
     * @brief Gets an entry's Score for this objective.
     *
     * @param entry Entry for the Score
     * @return Score tracking the Objective and entry specified
     */
    [[nodiscard]] virtual std::unique_ptr<Score> getScore(ScoreEntry entry) const = 0;

    virtual bool operator==(const Objective &other) const = 0;
    virtual bool operator!=(const Objective &other) const = 0;
};

}  // namespace endstone
