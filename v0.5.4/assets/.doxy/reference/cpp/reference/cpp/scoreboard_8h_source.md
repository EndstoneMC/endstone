

# File scoreboard.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**scoreboard**](dir_19c52f9ea81a2cf7449c80dcee80d6f0.md) **>** [**scoreboard.h**](scoreboard_8h.md)

[Go to the documentation of this file](scoreboard_8h.md)


```C++
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

class Scoreboard : public std::enable_shared_from_this<Scoreboard> {
public:
    virtual ~Scoreboard() = default;

    virtual std::unique_ptr<Objective> addObjective(std::string name, Criteria::Type criteria) = 0;

    virtual std::unique_ptr<Objective> addObjective(std::string name, Criteria::Type criteria,
                                                    std::string display_name) = 0;

    virtual std::unique_ptr<Objective> addObjective(std::string name, Criteria::Type criteria, std::string display_name,
                                                    RenderType render_type) = 0;

    [[nodiscard]] virtual std::unique_ptr<Objective> getObjective(std::string name) const = 0;

    [[nodiscard]] virtual std::unique_ptr<Objective> getObjective(DisplaySlot slot) const = 0;

    [[nodiscard]] virtual std::vector<std::unique_ptr<Objective>> getObjectives() const = 0;

    [[nodiscard]] virtual std::vector<std::unique_ptr<Objective>> getObjectivesByCriteria(
        Criteria::Type criteria) const = 0;

    [[nodiscard]] virtual std::vector<std::unique_ptr<Score>> getScores(ScoreEntry entry) const = 0;

    virtual void resetScores(ScoreEntry entry) = 0;

    [[nodiscard]] virtual std::vector<ScoreEntry> getEntries() const = 0;

    virtual void clearSlot(DisplaySlot slot) = 0;
};

}  // namespace endstone
```


