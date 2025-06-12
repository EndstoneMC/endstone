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
    enum class Settings : int {
        Solid = 0,
        Liquid = 1,
        NonSolid = 2,
    };

    explicit Material(MaterialType, Settings);
    Material(const Material &) = delete;
    Material &operator=(const Material &) = delete;
    bool operator==(const Material &) const;
    bool operator!=(const Material &) const;
    [[nodiscard]] bool isType(MaterialType) const;
    [[nodiscard]] bool isSolidBlocking() const;
    [[nodiscard]] bool isLiquid() const;
    [[nodiscard]] bool getBlocksMotion() const;
    [[nodiscard]] bool getBlocksPrecipitation() const;
    [[nodiscard]] bool isSolid() const;
    [[nodiscard]] bool isSuperHot() const;
    [[nodiscard]] bool isTopSolid(bool, bool) const;
    [[nodiscard]] MaterialType getType() const;

private:
    MaterialType type_;
    bool never_buildable_;
    bool liquid_;
    bool blocks_motion_;
    bool blocks_precipitation_;
    bool solid_;
    bool super_hot_;
};
static_assert(sizeof(Material) == 12);
