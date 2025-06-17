

# File objective.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**scoreboard**](dir_19c52f9ea81a2cf7449c80dcee80d6f0.md) **>** [**objective.h**](objective_8h.md)

[Go to the documentation of this file](objective_8h.md)


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

#include "endstone/scoreboard/objective_sort_order.h"
#include "endstone/scoreboard/score.h"
#include "endstone/util/result.h"

namespace endstone {

class Scoreboard;

class Objective {
public:
    virtual ~Objective() = default;

    [[nodiscard]] virtual Result<std::string> getName() const = 0;

    [[nodiscard]] virtual Result<std::string> getDisplayName() const = 0;

    virtual Result<void> setDisplayName(std::string display_name) = 0;

    [[nodiscard]] virtual Result<const Criteria *> getCriteria() const = 0;

    [[nodiscard]] virtual Result<bool> isModifiable() const = 0;

    [[nodiscard]] virtual Scoreboard &getScoreboard() const = 0;

    [[nodiscard]] virtual Result<void> unregister() const = 0;

    [[nodiscard]] virtual Result<bool> isDisplayed() const = 0;

    [[nodiscard]] virtual Result<DisplaySlot> getDisplaySlot() const = 0;

    [[nodiscard]] virtual Result<ObjectiveSortOrder> getSortOrder() const = 0;

    virtual Result<void> setDisplay(std::optional<DisplaySlot> slot) = 0;

    virtual Result<void> setDisplay(std::optional<DisplaySlot> slot, ObjectiveSortOrder order) = 0;

    [[nodiscard]] virtual Result<RenderType> getRenderType() const = 0;

    virtual Result<void> setRenderType(RenderType render_type) = 0;

    [[nodiscard]] virtual Result<std::unique_ptr<Score>> getScore(ScoreEntry entry) const = 0;

    virtual bool operator==(const Objective &other) const = 0;
    virtual bool operator!=(const Objective &other) const = 0;
};

}  // namespace endstone
```


