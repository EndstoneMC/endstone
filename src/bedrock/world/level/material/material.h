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
    enum class Settings : int {
        Solid = 0,
        Liquid = 1,
        NonSolid = 2,
    };

public:
    explicit Material(MaterialType, Material::Settings);
    bool operator==(const Material &rhs) const { return this == &rhs; }
    bool operator!=(const Material &rhs) const { return !(*this == rhs); }
    [[nodiscard]] bool isType(MaterialType type) const { return type == type_; }
    [[nodiscard]] bool isSolidBlocking() const { return !never_buildable_ && blocks_motions_; }
    [[nodiscard]] bool isLiquid() const { return liquid_; }
    [[nodiscard]] bool getBlocksMotion() const { return blocks_motions_; }
    [[nodiscard]] bool getBlocksPrecipitation() const { return blocks_precipitation_; }
    [[nodiscard]] bool isSolid() const { return solid_; }
    [[nodiscard]] bool isSuperHot() const { return super_hot_; }
    [[nodiscard]] bool isTopSolid(bool flag1, bool flag2) const
    {
        if (isType(MaterialType::Leaves)) {
            return flag2;
        }
        if (!flag1 || !isType(MaterialType::Water)) {
            return getBlocksMotion();
        }
        return true;
    }
    [[nodiscard]] MaterialType getType() const { return type_; }

private:
    MaterialType type_;
    bool never_buildable_;
    bool liquid_;
    bool blocks_motions_;
    bool blocks_precipitation_;
    bool solid_;
    bool super_hot_;
};
static_assert(sizeof(Material) == 12);
