

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

#include <unordered_set>

namespace endstone {

class Scoreboard {
public:
    virtual ~Scoreboard() = default;

    virtual Objective &addObjective(std::string name, Criteria criteria) = 0;

    virtual Objective &addObjective(std::string name, Criteria criteria, std::string display_name) = 0;

    [[nodiscard]] virtual Objective *getObjective(std::string name) const = 0;

    [[nodiscard]] virtual Objective *getObjective(DisplaySlot slot) const = 0;

    [[nodiscard]] virtual std::unordered_set<Objective *> getObjectives() const = 0;

    [[nodiscard]] virtual std::unordered_set<Objective *> getObjectivesByCriteria(Criteria criteria) const = 0;

    [[nodiscard]] virtual std::unordered_set<std::string> getEntries() const = 0;

    virtual void clearSlot(DisplaySlot slot) = 0;
};

}  // namespace endstone
```


