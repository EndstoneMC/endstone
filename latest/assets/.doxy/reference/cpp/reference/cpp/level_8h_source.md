

# File level.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**level**](dir_8e239ca1e5fd0d936d66a30330d3a329.md) **>** [**level.h**](level_8h.md)

[Go to the documentation of this file](level_8h.md)


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

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "endstone/actor/actor.h"
#include "endstone/game_rule.h"
#include "endstone/inventory/recipe.h"
#include "endstone/level/dimension.h"
#include "endstone/level/dimension_creator.h"

namespace endstone {

class Level {
public:
    virtual ~Level() = default;

    [[nodiscard]] virtual std::string getName() const = 0;

    [[nodiscard]] virtual std::vector<NotNull<Actor>> getActors() const = 0;

    [[nodiscard]] virtual int getTime() const = 0;

    virtual void setTime(int time) = 0;

    [[nodiscard]] virtual std::vector<NotNull<Dimension>> getDimensions() const = 0;

    [[nodiscard]] virtual Nullable<Dimension> getDimension(DimensionId id) const = 0;

    [[nodiscard]] virtual Nullable<Dimension> createDimension(const DimensionCreator &creator) = 0;

    [[nodiscard]] virtual std::int64_t getSeed() const = 0;

    [[nodiscard]] virtual bool _hasGameRule(Identifier<GameRule> rule) const = 0;

    template <typename T>
    [[nodiscard]] bool hasGameRule(GameRuleId<T> rule) const
    {
        return _hasGameRule(rule);
    }

    [[nodiscard]] virtual GameRuleValue _getGameRule(Identifier<GameRule> rule) const = 0;

    template <typename T>
    [[nodiscard]] T getGameRule(GameRuleId<T> rule) const
    {
        return std::get<T>(_getGameRule(rule));
    }

    virtual bool _setGameRule(Identifier<GameRule> rule, GameRuleValue value) = 0;

    template <typename T>
    bool setGameRule(GameRuleId<T> rule, T value)
    {
        return _setGameRule(rule, value);
    }

    [[nodiscard]] virtual std::vector<NotNull<Recipe>> getRecipes() const = 0;
};

}  // namespace endstone
```


