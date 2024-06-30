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

#include "bedrock/world/level/material/material_type.h"

class Material {
public:
    [[nodiscard]] MaterialType getType() const
    {
        return type_;
    }
    [[nodiscard]] bool isNeverBuildable() const
    {
        return never_buildable_;
    }

    [[nodiscard]] bool isAlwaysDestroyable() const
    {
        return always_destroyable_;
    }
    [[nodiscard]] bool isReplaceable() const
    {
        return replaceable_;
    }
    [[nodiscard]] bool isLiquid() const
    {
        return is_liquid_;
    }
    [[nodiscard]] float getTranslucency() const
    {
        return translucency_;
    }
    [[nodiscard]] bool getBlocksMotion() const
    {
        return blocks_motion_;
    }
    [[nodiscard]] bool getBlocksPrecipitation() const
    {
        return blocks_precipitation_;
    }
    [[nodiscard]] bool isSolid() const
    {
        return solid_;
    }
    [[nodiscard]] bool isSuperHot() const
    {
        return super_hot_;
    }
    [[nodiscard]] bool isSolidBlocking() const
    {
        return !never_buildable_ && blocks_motion_;
    }

private:
    MaterialType type_;          // +0
    bool never_buildable_;       // +4
    bool always_destroyable_;    // +5
    bool replaceable_;           // +6
    bool is_liquid_;             // +7
    float translucency_;         // +8
    bool blocks_motion_;         // +12
    bool blocks_precipitation_;  // +13
    bool solid_;                 // +14
    bool super_hot_;             // +15
};
